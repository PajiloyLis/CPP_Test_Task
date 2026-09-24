#ifndef TESTTASK_CLIENTCONFIGPARSER_H
#define TESTTASK_CLIENTCONFIGPARSER_H

#include "configModels/ClientSettings.h"

namespace ClientConfigJson {

    QJsonObject toJson(const ClientSettings& c);

    std::optional<ClientSettings> fromJson(const QByteArray& data);

}

#endif //TESTTASK_CLIENTCONFIGPARSER_H