#ifndef WIFI4_H
#define WIFI4_H

#include "wifi.h"
#include <iostream>
#include <vector>
#include <algorithm>

class Wifi4_AP {
private:
    Freq_channel ch;
    int all_user;

public:
    Wifi4_AP(int users):all_user(users),ch(){}

    Freq_channel& getChannel(){ 
        return ch; 
    }
    int getall_user() const 
    { 
        return all_user; 
    }
};

class wifi4User:public User<>{
public:
    wifi4User(int id):User(id){}

    bool retrytransmission(Freq_channel& ch,double& latency,double congestionFactor)override{
        if(!isStatus()){
            return false;
        }

        try {
            if(ch.getState()!=Freq_channel::FREE){
                resetbackofftime();
                return false;
            }

            bool collision_data=(rand()%100<congestionFactor*100);

            if(!collision_data){
                ch.setState(Freq_channel::OCCUPIED);
                latency+=backoffTime;
                collisiondataCount=0;
                ch.setState(Freq_channel::FREE);
                return true;
            }else{
                collisiondataCount++;
                resetbackofftime();
                return false;
            }
        }catch(const NW_resource_Exception& e){
            cerr<<"Transmission Error for User through Network "<<userID<<": "<<e.what()<<std::endl;
            return false;
        }
    }
};

vector<double> Modelwifi4(int numUsers,int numPackets){
    try {
        Wifi4_AP accessPoint(numUsers);
        vector<wifi4User>users;

        for(int i=0;i<numUsers;i++){
            users.emplace_back(i);
        }
        CommunicationParameters<double>commParams(20e6,8,(5.0/6.0));
        const double dR=commParams.calDataRate();
        const double packetSizeBits=(1024*8);
        const double idealTTime=(packetSizeBits/dR);

        vector<double>latencyies;
        int successfullydataPackets=0;
        double totalTime=0;
        vector<double> resultofwifi4;
        if(numUsers==1){
          
            double idealThroughput=(dR/1e6); 
            double idealLatency=(idealTTime*1000); 

            latencyies=vector<double>(numPackets,idealLatency);
            successfullydataPackets=numPackets;
            totalTime=(numPackets*idealTTime);
            resultofwifi4.push_back(idealThroughput);
            resultofwifi4.push_back(idealThroughput);
            resultofwifi4.push_back(idealLatency);
            resultofwifi4.push_back(idealLatency);
            return resultofwifi4;
            
        } else {
        
            const int contentionWindowSize=32;
            vector<int>contentionSlots(contentionWindowSize,0);

            for(int packetCount=0;packetCount<numPackets;packetCount++){
                fill(contentionSlots.begin(),contentionSlots.end(),0);

                for(auto &user : users){
                    int selectedSlot=(rand() % contentionWindowSize);
                    contentionSlots[selectedSlot]++;
                }

                for(int slot=0;slot<contentionWindowSize; slot++){
                    if (contentionSlots[slot]==1){
                        double latency=(slot*0.001);
                        latency +=(idealTTime*1000);
                        latencyies.push_back(latency);
                        successfullydataPackets++;
                        totalTime+=(latency / 1000);
                    }else if (contentionSlots[slot]>1){
                        for(auto &user : users){
                            user.resetbackofftime();
                        }
                    }
                }
            }
        }

     
        if (numUsers>1) {
            double idealThroughput=(dR/1e6);
            double rawThroughput=(successfullydataPackets*packetSizeBits)/totalTime;
            double effectiveThroughput=min(rawThroughput/1e6,idealThroughput);

            double averageLatency=latencyies.empty()?0:accumulate(latencyies.begin(),latencyies.end(),0.0)/latencyies.size();
            double maxLatency=latencyies.empty()?0:*max_element(latencyies.begin(),latencyies.end());
             resultofwifi4.push_back(idealThroughput);
            resultofwifi4.push_back(effectiveThroughput);
            resultofwifi4.push_back(averageLatency);
            resultofwifi4.push_back(maxLatency);
            return resultofwifi4;
        }
        return vector<double>{0.0, 0.0};

    } catch(const exception& e){
        cerr<<"modeling Error: "<<e.what()<<endl;
    }
    return vector<double>{0.0, 0.0,0.0};
   
}
#endif
