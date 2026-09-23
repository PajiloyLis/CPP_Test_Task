#include <QApplication>
#include "ui/MainWindow.h"
#include "controller/ServerController/ServerController.h"
#include "domainModels/ClientInfo.h"
#include "domainModels/ClientStatus.h"
#include "domainModels/Thresholds.h"
#include "protocolModels/Incoming.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    qRegisterMetaType<ClientInfo>("ClientInfo");
    qRegisterMetaType<ClientStatus>("ClientStatus");
    qRegisterMetaType<Thresholds>("Thresholds");
    qRegisterMetaType<IncomingPacket>("IncomingPacket");
    qRegisterMetaType<ServerSettings>("ServerSettings");

    const QString appDir = QCoreApplication::applicationDirPath();

    SettingsService settings(
        appDir + "/server.json",
        appDir + "/thresholds.json");
    settings.load();

    ServerController controller(&app);

    QObject::connect(&settings, &SettingsService::thresholdsChanged,
                     &controller, &IServerController::applyThresholds);

    MainWindow window(&controller, &settings);
    window.show();

    return app.exec();
}