#ifndef WIFI5_H
#define WIFI5_H

#include "wifi.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Custom User Class for WiFi 5
class WiFi5User:public User<int,double>{
public:
    WiFi5User(int id):User(id){}
    bool retrytransmission(Freq_channel& ch,double& latency,double congestionFactor) override{
        if (ch.isAvailable()){
            ch.setState(Freq_channel::OCCUPIED);
            latency+=backoffTime;
            return true;
        }
        return false;
    }
};

class Wifi5_AP {
private:
    const double comm_time=0.015;
    const int data_size_of_CSI=200;        
    const int max_parallel_user=5;    
    double BW_total; 
    double bps;  
    double coding_rate;      
    vector<User<int,double>*>accessUser; 
public:
    Wifi5_AP(double bw,double bps,double cr):BW_total(bw),bps(bps),coding_rate(cr){}
    void addUser(User<int, double>*user){
        accessUser.push_back(user);
    }
    vector<double> Modeling_MU_MIMO(){
        double csi_overhead_time=CSI_cal_overhead();
        double Total_TP=0.0;
        vector<double>Latency_user(accessUser.size(),0.0);
        int numUser=accessUser.size();
        if(numUser==1){
            vector<double>result=handleSingleUser(csi_overhead_time,Total_TP,Latency_user);
            return result;
        }
        int curr_stream=min(max_parallel_user,numUser);
        double time_left=comm_time-csi_overhead_time;
        for(int cycle=0;cycle<3;cycle++){
            RR_Cycle(curr_stream,numUser,time_left,csi_overhead_time,Total_TP,Latency_user);
        }
        return Show_output(Total_TP, Latency_user);
    }

private:
    vector<double> handleSingleUser(double csi_overhead_time,double& Total_TP,vector<double>&Latency_user){
        cout<<"Single user scenario: No need for cycles."<<endl;
        double available_time=comm_time-csi_overhead_time;
        NetworkPacket<>packet(1024,BW_total,bps,"DATA");
        double transmission_time=packet.getTransmissionTime();
        Latency_user[0]+=csi_overhead_time+transmission_time;
        Total_TP=(packet.getSize()*8.0)/(transmission_time+csi_overhead_time);
        return Show_output(Total_TP,Latency_user);
    }

 
    void RR_Cycle(int curr_stream,int numUser,double time_left,double csi_overhead_time,double& Total_TP,vector<double>& Latency_user){
        int user_id=0;
        double time_used=0.0;

        while(time_used < time_left){
            vector<int>curr_group_users;

            for(int i=0;i<curr_stream;i++){
                curr_group_users.push_back(user_id);
                user_id=(user_id+1)%numUser;
            }

            double BW_per_user=(BW_total/curr_stream);
            for(int idx : curr_group_users){
                NetworkPacket<> packet(1024,BW_per_user,bps,"DATA");
                double transmission_time=packet.getTransmissionTime();

                Latency_user[idx]+=csi_overhead_time+transmission_time;
                double throughput=(packet.getSize()*8.0)/(transmission_time+csi_overhead_time);
                Total_TP+=throughput;
            }
            time_used+=(time_left / numUser * curr_group_users.size());
        }
    }


    double CSI_cal_overhead()const{
        double broad_cast_time=(data_size_of_CSI*8.0)/(BW_total* bps* coding_rate);
        double seq_CSI_time=accessUser.size()*((data_size_of_CSI*8.0)/(BW_total* bps* coding_rate));
        return broad_cast_time + seq_CSI_time;
    }

   
    vector<double> Show_output(double Total_TP,const vector<double>& latencies) const{
        double avg_latency=accumulate(latencies.begin(),latencies.end(),0.0)/latencies.size();
        double max_latency=*max_element(latencies.begin(),latencies.end());
        double totaltp=(Total_TP/1e6);
        double median_latency=(avg_latency*1000);
        double maximum_latency=(max_latency*1000);
        vector<double> result;
        result.push_back(totaltp);
        result.push_back(median_latency);
        result.push_back(maximum_latency);
        return result;
       
    }
};

#endif
