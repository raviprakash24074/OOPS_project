#ifndef WIFI6_H
#define WIFI6_H

#include "wifi.h"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <cmath>

template<typename UserIDType = int, typename TimeType = double>
class OFDMAUser : public User<UserIDType, TimeType> {
private:
    std::vector<int> subChannelAllocation;
    TimeType dataSize;
    bool transmissionComplete;

public:
    OFDMAUser(UserIDType id, TimeType size) 
        : User<UserIDType, TimeType>(id), dataSize(size), transmissionComplete(false) {}

    virtual bool retrytransmission(Freq_channel& channel, TimeType& latency, TimeType congestionFactor) override {
        if (channel.isAvailable() && this->isStatus()) {
            if (rand() % 100 < 90) {
                transmissionComplete = true;
                latency += 10;
                return true;
            } else {
                this->collisiondataCount++;
                this->resetbackofftime();
                latency += congestionFactor * this->backoffTime;
                return false;
            }
        }
        latency += congestionFactor;
        return false;
    }

    TimeType getDataSize() const { return dataSize; }
    bool isTransmissionComplete() const { return transmissionComplete; }

    void allocateSubChannels(int totalSubChannels) {
        int requiredSubChannels = std::ceil(dataSize / (2.0 * 1024));
        requiredSubChannels = std::min(requiredSubChannels, totalSubChannels);

        subChannelAllocation.clear();
        for (int i = 0; i < requiredSubChannels; ++i) {
            subChannelAllocation.push_back(i);
        }
    }

    const std::vector<int>& getSubChannelAllocation() const { 
        return subChannelAllocation; 
    }
};

class OFDMAChannelSimulator {
private:
    int totalSubChannels;
    double channelBandwidth;
    double modulationEfficiency;
    Freq_channel mainChannel;

public:
    OFDMAChannelSimulator(int subChannels = 5, double ch_bw = 20.0, double modulationEff = 0.75) 
        : totalSubChannels(subChannels), channelBandwidth(ch_bw), modulationEfficiency(modulationEff), 
          mainChannel("OFDMA_Channel") {}

    void simulateOFDMATransmission(std::vector<OFDMAUser<>*>& users) {
        std::cout << "\n--- OFDMA Transmission Simulation ---" << std::endl;
        std::cout << "Total Sub-Channels: " << totalSubChannels << std::endl;

        double totalLatency = 0;
        double congestionFactor = 1.5;
        std::vector<bool> completedTransmissions(users.size(), false);
        int completedUsers = 0;

        for (auto* user : users) {
            user->allocateSubChannels(totalSubChannels);
        }

        while (completedUsers < users.size()) {
            for (size_t i = 0; i < users.size(); ++i) {
                if (!completedTransmissions[i]) {
                    double latency = 0;
                    if (users[i]->retrytransmission(mainChannel, latency, congestionFactor)) {
                        completedTransmissions[i] = true;
                        completedUsers++;
                        totalLatency += latency;

                        try {
                            mainChannel.setState(Freq_channel::OCCUPIED);
                            mainChannel.setState(Freq_channel::FREE);
                        } catch (const NW_resource_Exception& e) {
                            std::cerr << "Channel State Error: " << e.what() << std::endl;
                        }
                    }
                }
            }
        }

        double averageLatency = totalLatency / (users.size() > 0 ? users.size() : 1);
        double maxLatency = totalLatency;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Total Users: " << users.size() << std::endl;
        std::cout << "Average Latency: " << averageLatency << " ms" << std::endl;
        std::cout << "Maximum Latency: " << maxLatency << " ms" << std::endl;

        double totalThroughput = 0;
        for (auto* user : users) {
            if (averageLatency > 0) {
                totalThroughput += user->getDataSize() / (averageLatency / 1000.0);
            }
        }
        std::cout << "Total Throughput: " << totalThroughput / (1024 * 1024) << " Mbps" << std::endl;
    }
};

#endif
