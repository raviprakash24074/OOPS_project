#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <memory>
#include <algorithm>
#include <numeric>
#include <string>
using namespace std;

class wifi_exception_code:public exception{
protected:
    string message;
public:
    explicit wifi_exception_code(const string& msg):message(msg){}
    const char* what() const noexcept override{ 
        return message.c_str(); 
    }
};

class NW_resource_Exception:public wifi_exception_code{
public:
    explicit NW_resource_Exception(const string& msg):wifi_exception_code("Network Resource Error: "+msg){}
};

class transmission_Exception_code:public wifi_exception_code{
public:
    explicit transmission_Exception_code(const string& msg):wifi_exception_code("Transmission Error: "+msg){}
};

template<typename T>
class CommunicationParameters{
protected:
    T bandwidth;
    T modulationEfficiency;
    T codingRate;

public:
    CommunicationParameters(T bw,T modEff,T codeRate) {
        bandwidth=bw;
        modulationEfficiency=modEff;
        codingRate=codeRate;
    }
    virtual T calDataRate()const{
        return bandwidth*modulationEfficiency*codingRate;
    }
};


template<typename SizeType=int,typename TimeType=double>
class NetworkPacket{
private:
    SizeType packetSize;
    TimeType transmissionTime;
    string packetType;

public:
    NetworkPacket(SizeType sizeBytes, TimeType bandwidth,TimeType modulationEfficiency,const string& type = "DATA"):packetType(type){
        if (sizeBytes<=0||bandwidth<=0||modulationEfficiency<=0){
            throw transmission_Exception_code("Invalid packet creation parameters");
        }
        
        packetSize=sizeBytes;
        transmissionTime=calculateTransmissionTime(bandwidth,modulationEfficiency);
    }

    TimeType calculateTransmissionTime(TimeType bandwidth,TimeType modulationEfficiency)const{
        return (packetSize*8.0)/(bandwidth*modulationEfficiency);
    }

    SizeType getSize()const{ 
        return packetSize; 
    }
    TimeType getTransmissionTime()const{ 
        return transmissionTime; 
    }
    string getPacketType()const{ 
        return packetType; 
    }
};


class Freq_channel{
public:
    enum ChannelState{FREE,OCCUPIED,RESERVED};

private:
    ChannelState currentState;
    string channelIdentifier;

public:
    Freq_channel(const string&identifier="Default"):currentState(FREE),channelIdentifier(identifier){}

    void setState(ChannelState newState){
        if (currentState==newState) {
            throw NW_resource_Exception("Redundant channel state change");
        }
        currentState=newState;
    }

    ChannelState getState()const{ 
        return currentState; 
    }
    string getIdentifier()const{ 
        return channelIdentifier; 
    }

    bool isAvailable()const{ 
        return currentState==FREE; 
    }
};


template<typename UserIDType=int,typename TimeType=double>
class User{
protected:
    UserIDType userID;
    TimeType backoffTime;
    int collisiondataCount;
    bool isActive;

public:
    User(UserIDType id):userID(id),backoffTime(0),collisiondataCount(0),isActive(true){}

    virtual bool retrytransmission(Freq_channel& channel,TimeType& latency,TimeType congestionFactor)=0;

    virtual void resetbackofftime(){
        backoffTime=min(((rand()%10+1)*pow(2,collisiondataCount)),256.0);
    }

    void deactivate(){
        isActive=false;
    }
    void activate(){ 
        isActive=true; 
    }
    bool isStatus()const{ 
        return isActive; 
    }

    UserIDType getUserID()const{ 
        return userID; 
    }
};

#endif