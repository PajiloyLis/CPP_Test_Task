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

    ServerController controller(&app);
    MainWindow window(&controller);
    window.show();
    return app.exec();
}