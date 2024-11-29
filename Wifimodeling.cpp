#include "wifi5.h"
#include "wifi4.h"
#include "wifi6.h"
#include <iostream>

using namespace std;

int main() {
   
    cout<<"------------------------Modeling for WIFI 4---------------------------"<<endl;
    
    
    vector<double>resultfor1user=Modelwifi4(1,100);   
    vector<double>resultfor10user=Modelwifi4(10,100);  
    vector<double>resultfor100user=Modelwifi4(100,100); 
    
    cout<<"Modeling Results for 1 Users:"<<endl;
    cout<< "Effective Throughput: " << resultfor1user[0] << " Mbps"<<endl;
    cout<< "Average Latency: " << resultfor1user[2]<<" ms"<<endl;
    cout<< "Maximum Latency: " << resultfor1user[3]<<" ms"<<endl;
    cout<<endl;
    cout<<"Modeling Results for 10 Users:"<<endl;
    cout<< "Effective Throughput: " << resultfor10user[1] << " Mbps"<<endl;
    cout<< "Average Latency: " << resultfor10user[2]<<" ms"<<endl;
    cout<< "Maximum Latency: " << resultfor10user[3]<<" ms"<<endl;
    cout<<endl;
    cout<<"Modeling Results for 100 Users:"<<endl;
    cout<< "Effective Throughput: " << resultfor100user[1] << " Mbps"<<endl;
    cout<< "Average Latency: " << resultfor100user[2]<<" ms"<<endl;
    cout<< "Maximum Latency: " << resultfor100user[3]<<" ms"<<endl;


    cout<<"------------------------Modeling for WIFI 5---------------------------"<<endl;

    double bandwidth=20e6;        
    double bits_per_symbol=8.0;    
    double coding_rate=(5.0/6.0); 


    vector<int>user_counts={1,10,100};

    for (int num_users:user_counts) {
        cout<<endl;
        cout<<"Modeling for "<<num_users<<" users..."<<endl;

        Wifi5_AP accessUser(bandwidth,bits_per_symbol,coding_rate);

     
        for (int i=0;i<num_users;i++) {
            accessUser.addUser(new WiFi5User(i));
        }
        vector<double> resultwifi5user=accessUser.Modeling_MU_MIMO();
        cout<< "Effective Throughput: " << resultwifi5user[0] << " Mbps"<<endl;
        cout<< "Average Latency: " << resultwifi5user[1]<<" ms"<<endl;
        cout<< "Maximum Latency: " << resultwifi5user[2]<<" ms"<<endl;
    }

    cout<<"------------------------Modeling for WIFI 6---------------------------"<<endl;

    vector<int> userCounts = {1, 10, 100};

    for (int userCount : userCounts) {
        cout << "==== Simulation for " << userCount << " Users ====" << endl;
        

        vector<OFDMAUser<>*> users;
        for (int i = 0; i < userCount; ++i) {
         
            users.push_back(new OFDMAUser<>(i,(rand()% 10+ 1) * 256 * 256));
        }

    
        OFDMAChannelSimulator simulator;
        simulator.simulateOFDMATransmission(users);

     
        for (auto* user : users) {
            delete user;
        }
    }

  
    return 0;
}


