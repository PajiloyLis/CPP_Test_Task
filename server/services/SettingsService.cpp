#include "SettingsService.h"

SettingsService::SettingsService(QString sPath, QString tPath, QObject* parent)
    : QObject(parent),
      serverConfigPath_(std::move(sPath)),
      thresholdsPath_(std::move(tPath)) {}

void SettingsService::load() {
    if (QFile f(serverConfigPath_); f.exists() && f.open(QIODevice::ReadOnly)) {
        if (auto s = ServerSettingsJson::fromJson(f.readAll())) {
            server_ = *s;
        }
    }
    if (QFile f(thresholdsPath_); f.exists() && f.open(QIODevice::ReadOnly)) {
        if (auto t = ThresholdsJson::fromJson(f.readAll())) {
            thresholds_ = *t;
        }
    }
    emit serverSettingsChanged(server_);
    emit thresholdsChanged(thresholds_);
}

void SettingsService::setThresholds(const Thresholds& t) {
    if (thresholds_.maxLatency == t.maxLatency &&
        thresholds_.sendLogOnAlert == t.sendLogOnAlert) {
        return;
        }
    thresholds_ = t;
    emit thresholdsChanged(thresholds_);
}