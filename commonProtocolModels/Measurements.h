#ifndef SERVER_MEASUREMENTS_H
#define SERVER_MEASUREMENTS_H

#include <QString>
#include "domainModels/Severity.h"

struct NetworkMetrics {
    double bandwidth = 0.0; // Mbps
    double latency = 0.0; // ms
    double packetLoss = 0.0; // %
};

struct DeviceStatus {
    qint64 uptime = 0; // sec
    int cpuUsage = 0; // %
    int memoryUsage = 0; // %
};

struct LogMessage {
    QString message;
    Severity severity;
};

#endif //SERVER_MEASUREMENTS_H
