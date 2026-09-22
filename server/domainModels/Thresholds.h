#ifndef SERVER_THRESHOLDS_H
#define SERVER_THRESHOLDS_H

struct Thresholds {
    double maxLatency = 50.0; // ms
    double maxPacketLoss = 5.0; // %
    double minBandwidth = 10.0; // Mbps
    int maxCpuUsage = 90; // %
    int maxMemoryUsage = 90; // %
    bool sendLogOnAlert = true;
};

#endif //SERVER_THRESHOLDS_H
