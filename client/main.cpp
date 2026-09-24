#include <QCoreApplication>
#include <QFile>
#include <QTimer>

#include "configParsers/ClientConfigParser/ClientSettingsParser.h"
#include "services/ClientService/ClientService.h"

namespace {
    ClientSettings loadSettings(const QString& path) {
        ClientSettings settings;

        QFile f(path);
        if (!f.exists()) {
            qWarning() << "client.json not found, using defaults:" << path;
            return settings;
        }
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open client.json:" << f.errorString();
            return settings;
        }

        auto parsed = ClientConfigJson::fromJson(f.readAll());
        if (!parsed) {
            qWarning() << "Invalid client.json, using defaults:" << path;
            return settings;
        }
        return *parsed;
    }

}

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("telecom-client"));

    const QString path =
        QCoreApplication::applicationDirPath() + QStringLiteral("/ClientConfig.json");
    const ClientSettings settings = loadSettings(path);

    ClientService client(settings);

    QTimer::singleShot(0, &client, &ClientService::start);

    return app.exec();
}