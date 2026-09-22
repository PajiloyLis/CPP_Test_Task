#ifndef SERVER_THRESHOLDS_H
#define SERVER_THRESHOLDS_H

#include <QMetaType>

struct Thresholds {
    double maxLatency = 50.0; // ms
    double maxPacketLoss = 5.0; // %
    double minBandwidth = 10.0; // Mbps
    int maxCpuUsage = 90; // %
    int maxMemoryUsage = 90; // %
    bool sendLogOnAlert = true;
};

Q_DECLARE_METATYPE(Thresholds)

#endif //SERVER_THRESHOLDS_H
