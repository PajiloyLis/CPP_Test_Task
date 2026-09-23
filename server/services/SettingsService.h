#ifndef SERVER_SETTINGSSERVICE_H
#define SERVER_SETTINGSSERVICE_H

#include <QObject>
#include <QString>
#include <QFile>

#include "configParsers/serverConfigParser/ServerSettingsParser.h"
#include "configParsers/thresholdsConfigParser/ThresholdsParser.h"
#include "configModels/ServerSettings.h"
#include "domainModels/Thresholds.h"

class SettingsService : public QObject {
    Q_OBJECT

public:
    SettingsService(QString serverConfigPath,
                    QString thresholdsPath,
                    QObject *parent = nullptr);

    void load();

    ServerSettings serverSettings() const { return server_; }
    Thresholds thresholds() const { return thresholds_; }

    void setServerSettings(const ServerSettings &s);

    void setThresholds(const Thresholds &t);

    QString saveServerSettings(const QString &path = {}) const;

    QString saveThresholds(const QString &path = {}) const;

    QString serverConfigPath() const { return serverConfigPath_; }
    QString thresholdsPath() const { return thresholdsPath_; }

signals:
    void serverSettingsChanged(const ServerSettings &s);

    void thresholdsChanged(const Thresholds &t);

private:
    QString serverConfigPath_;
    QString thresholdsPath_;
    ServerSettings server_;
    Thresholds thresholds_;
};


#endif //SERVER_SETTINGSSERVICE_H
