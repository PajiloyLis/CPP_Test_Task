#ifndef TEST_TASK_CPP_MAINWINDOW_H
#define TEST_TASK_CPP_MAINWINDOW_H

#include "controller/IServerController.h"
#include "protocolModels/Incoming.h"
#include "protocolModels/packetCodec/PacketCodec.h"
#include "domainModels/Thresholds.h"
#include "ui/settingsDialog/SettingsDialog.h"
#include "configParsers/thresholdsConfigParser/ThresholdsParser.h"

#include <QDateTime>
#include <QTableWidgetItem>
#include <QMainWindow>
#include <QHash>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "services/SettingsService.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(IServerController *controller,
                        SettingsService *settings,
                        QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:
    void onServerStarted(quint16 port);

    void onServerStopped();

    void onFatalError(const QString &message);

    void onClientConnected(ClientInfo info);

    void onClientDisconnected(ClientId id);

    void onClientStatusChanged(ClientId id, ClientStatus status);

    void onPacketReceived(IncomingPacket packet);

    void onLogMessage(const QString &message);

private slots:
    void handleStartServerClicked();

    void handleStopServerClicked();

    void handleStartClientsClicked();

    void handleStopClientsClicked();

    void handleSettingsClicked();

    void handleClearLogClicked();

    void handleExitAction();

    void handleSaveConfigAction();

private:
    void setupUi();

    void setupConnections();

    void connectToController();

    int rowForClient(ClientId id) const;

    void appendLog(const QString &message);

    void setServerStatusLabel(bool running, quint16 port = 0);

    SettingsService *settings_ = nullptr;
    Ui::MainWindow *ui_ = nullptr;
    IServerController *controller_ = nullptr; // не владеет
    QHash<ClientId, int> clientRow_;
};


#endif //TEST_TASK_CPP_MAINWINDOW_H
