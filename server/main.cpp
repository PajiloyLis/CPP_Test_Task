#include <QApplication>

#include "ui/MainWindow.h"
#include "controller/ServerController/ServerController.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/ClientStatus.h"
#include "domainModels/Thresholds.h"
#include "commonProtocolModels/Incoming.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

// Регистрация типов передаваемых через очереди между GUI и воркером
    qRegisterMetaType<ClientInfo>("ClientInfo");
    qRegisterMetaType<ClientStatus>("ClientStatus");
    qRegisterMetaType<Thresholds>("Thresholds");
    qRegisterMetaType<ServerSettings>("ServerSettings");
    qRegisterMetaType<QVector<IncomingPacket>>("QVector<IncomingPacket>");
    qRegisterMetaType<QStringList>("QStringList");

    const QString appDir = QCoreApplication::applicationDirPath();

// Конфиги лежат рядом с main.cpp в корне сервера и копируются при сборке
    SettingsService settings(
        appDir + "/server.json",
        appDir + "/thresholds.json");

    ServerController controller(&app);

    QObject::connect(&settings, &SettingsService::thresholdsChanged,
                 &controller, &IServerController::applyThresholds);
    settings.load();

    MainWindow window(&controller, &settings);
    window.show();

    return app.exec();
}