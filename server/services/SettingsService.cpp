#include "SettingsService.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>

SettingsService::SettingsService(QString sPath, QString tPath, QObject *parent)
    : QObject(parent),
      serverConfigPath_(std::move(sPath)),
      thresholdsPath_(std::move(tPath)) {
}

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

void SettingsService::setServerSettings(const ServerSettings &s) {
    if (server_ == s) return;
    server_ = s;
    emit serverSettingsChanged(server_);
}

void SettingsService::setThresholds(const Thresholds &t) {
    if (thresholds_ == t) return;
    thresholds_ = t;
    emit thresholdsChanged(thresholds_);
}

namespace {
    QString writeJson(const QString &path, const QJsonObject &root) {
        const QFileInfo info(path);
        const QDir dir = info.absoluteDir();
        if (!dir.exists() && !dir.mkpath(QStringLiteral("."))) {
            return QStringLiteral("Cannot create directory: %1")
                    .arg(dir.absolutePath());
        }

        QFile f(path);
        if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            return f.errorString();
        }

        const qint64 written =
                f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));

        if (written < 0) {
            return f.errorString();
        }
        return QString();
    }
}

QString SettingsService::saveServerSettings(const QString &path) const {
    const QString target = path.isEmpty() ? serverConfigPath_ : path;
    return writeJson(target, ServerSettingsJson::toJson(server_));
}

QString SettingsService::saveThresholds(const QString &path) const {
    const QString target = path.isEmpty() ? thresholdsPath_ : path;
    return writeJson(target, ThresholdsJson::toJson(thresholds_));
}
