#ifndef SERVER_THRESHOLDSPARSER_H
#define SERVER_THRESHOLDSPARSER_H

#include <QByteArray>
#include <QJsonObject>
#include <optional>

#include "domainModels/Thresholds.h"

namespace ThresholdsJson {
    QJsonObject toJson(const Thresholds &t);

    std::optional<Thresholds> fromJson(const QByteArray &data);
}


#endif //SERVER_THRESHOLDSPARSER_H
