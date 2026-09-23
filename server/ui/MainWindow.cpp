#include "MainWindow.h"
#include "ui/ui_MainWindow.h"

MainWindow::MainWindow(IServerController *controller, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      controller_(controller) {
    ui_->setupUi(this);
    setupUi();
    setupConnections();
    connectToController();

    appendLog(QStringLiteral("MainWindow initialized."));
}

MainWindow::~MainWindow() {
    delete ui_;
}

void MainWindow::setupUi() {
    ui_->tableClients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->tableData->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui_->tableClients->setSelectionMode(QAbstractItemView::SingleSelection);
    ui_->tableData->setSelectionMode(QAbstractItemView::SingleSelection);

    ui_->tableClients->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui_->tableClients->horizontalHeader()
            ->setStretchLastSection(true);

    ui_->tableData->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui_->tableData->horizontalHeader()
            ->setStretchLastSection(true);

    setServerStatusLabel(false);
}

void MainWindow::setupConnections() {
    connect(ui_->btnStartServer, &QPushButton::clicked,
            this, &MainWindow::handleStartServerClicked);
    connect(ui_->btnStopServer, &QPushButton::clicked,
            this, &MainWindow::handleStopServerClicked);
    connect(ui_->btnStartClients, &QPushButton::clicked,
            this, &MainWindow::handleStartClientsClicked);
    connect(ui_->btnStopClients, &QPushButton::clicked,
            this, &MainWindow::handleStopClientsClicked);
    connect(ui_->btnSettings, &QPushButton::clicked,
            this, &MainWindow::handleSettingsClicked);
    connect(ui_->btnClearLog, &QPushButton::clicked,
            this, &MainWindow::handleClearLogClicked);

    connect(ui_->actionExit, &QAction::triggered,
            this, &MainWindow::handleExitAction);
    connect(ui_->actionSaveConfig, &QAction::triggered,
            this, &MainWindow::handleSaveConfigAction);
    connect(ui_->actionClientParams, &QAction::triggered,
            this, &MainWindow::handleSettingsClicked);
}

void MainWindow::connectToController() {
    if (!controller_) {
        appendLog(QStringLiteral("No ServerController — running in UI-only mode."));
        return;
    }

    connect(controller_, &IServerController::serverStarted,
            this, &MainWindow::onServerStarted);
    connect(controller_, &IServerController::serverStopped,
            this, &MainWindow::onServerStopped);
    connect(controller_, &IServerController::fatalError,
            this, &MainWindow::onFatalError);

    connect(controller_, &IServerController::clientConnected,
            this, &MainWindow::onClientConnected);
    connect(controller_, &IServerController::clientDisconnected,
            this, &MainWindow::onClientDisconnected);
    connect(controller_, &IServerController::clientStatusChanged,
            this, &MainWindow::onClientStatusChanged);

    connect(controller_, &IServerController::packetReceived,
            this, &MainWindow::onPacketReceived);
    connect(controller_, &IServerController::logMessage,
            this, &MainWindow::onLogMessage);
}


void MainWindow::handleStartServerClicked() {
    if (!controller_ || !settings_) {
        appendLog(QStringLiteral("ERROR: no controller"));
        return;
    }
    const quint16 port = settings_->serverSettings().port;
    controller_->startServer(port);
    appendLog(QStringLiteral("Requested server start on port %1...").arg(port));
}

void MainWindow::handleStopServerClicked() {
    if (!controller_) return;
    controller_->stopServer();
    appendLog(QStringLiteral("Requested server stop..."));
}

void MainWindow::handleStartClientsClicked() {
    if (!controller_) return;
    controller_->broadcastStart();
    appendLog(QStringLiteral("Requested Start command for all clients..."));
}

void MainWindow::handleStopClientsClicked() {
    if (!controller_) return;
    controller_->broadcastStop();
    appendLog(QStringLiteral("Requested Stop command for all clients..."));
}

void MainWindow::handleSettingsClicked() {
    if (!controller_) {
        appendLog(QStringLiteral("ERROR: no controller"));
        return;
    }
    if (!settings_) {
        appendLog(QStringLiteral("ERROR: no settings"));
        return;
    }

    SettingsDialog dlg(settings_->thresholds(), this);
    if (dlg.exec() != QDialog::Accepted) {
        appendLog(QStringLiteral("Settings dialog cancelled."));
        return;
    }

    settings_->setThresholds(dlg.thresholds());
    appendLog(QStringLiteral("Thresholds updated."));
}

void MainWindow::handleClearLogClicked() {
    ui_->textLog->clear();
    appendLog(QStringLiteral("Log cleared."));
}

void MainWindow::handleExitAction() {
    close();
}

void MainWindow::handleSaveConfigAction() {
    if (!settings_) return;

    QFileDialog dlg(this, tr("Save thresholds"),
                    settings_->thresholdsPath());
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setNameFilter(tr("JSON files (*.json);;All files (*)"));
    dlg.setDefaultSuffix(QStringLiteral("json"));

    if (dlg.exec() != QDialog::Accepted) return;
    const QString path = dlg.selectedFiles().value(0);
    if (path.isEmpty()) return;

    const QString err = settings_->saveThresholds(path);
    if (!err.isEmpty()) {
        QMessageBox::warning(this, tr("Save failed"), err);
        return;
    }
    appendLog(QStringLiteral("Thresholds saved to %1").arg(path));
}

void MainWindow::onServerStarted(quint16 port) {
    setServerStatusLabel(true, port);
    ui_->btnStartServer->setEnabled(false);
    ui_->btnStopServer->setEnabled(true);
    appendLog(QStringLiteral("Server started on port %1").arg(port));
}

void MainWindow::onServerStopped() {
    setServerStatusLabel(false);
    ui_->btnStartServer->setEnabled(true);
    ui_->btnStopServer->setEnabled(false);
    appendLog(QStringLiteral("Server stopped."));
}

void MainWindow::onFatalError(const QString &message) {
    appendLog(QStringLiteral("FATAL: ") + message);
    QMessageBox::critical(this, tr("Server error"), message);
}

void MainWindow::onClientConnected(ClientInfo info) {
    const int row = ui_->tableClients->rowCount();
    ui_->tableClients->insertRow(row);

    auto set = [&](int col, const QString &text) {
        ui_->tableClients->setItem(row, col, new QTableWidgetItem(text));
    };
    set(0, QString::number(info.id));
    set(1, info.ip);
    set(2, QString::number(info.port));
    set(3, QStringLiteral("Connected"));
    set(4, info.connectedAt.toString(Qt::ISODate));

    clientRow_.insert(info.id, row);
    appendLog(QStringLiteral("Client %1 connected from %2:%3")
        .arg(info.id).arg(info.ip).arg(info.port));
}

void MainWindow::onClientDisconnected(ClientId id) {
    const int row = rowForClient(id);
    if (row >= 0) {
        if (auto *item = ui_->tableClients->item(row, 3)) {
            item->setText(QStringLiteral("Disconnected"));
        }
    }
    appendLog(QStringLiteral("Client %1 disconnected").arg(id));
}

void MainWindow::onClientStatusChanged(ClientId id, ClientStatus status) {
    const int row = rowForClient(id);
    if (row < 0) return;

    QString text;
    QColor color = Qt::black;
    switch (status) {
        case ClientStatus::Connected: text = "Connected";
            break;
        case ClientStatus::Disconnected: text = "Disconnected";
            color = Qt::gray;
            break;
        case ClientStatus::Warning: text = "Warning";
            color = QColor(200, 120, 0);
            break;
    }
    if (auto *item = ui_->tableClients->item(row, 3)) {
        item->setText(text);
        item->setForeground(color);
    }
}

void MainWindow::onPacketReceived(IncomingPacket packet) {
    const int row = ui_->tableData->rowCount();
    ui_->tableData->insertRow(row);

    auto set = [&](int col, const QString &text) {
        ui_->tableData->setItem(row, col, new QTableWidgetItem(text));
    };
    set(0, QString::number(packet.clientId));
    set(1, PacketCodec::dataTypeToString(packet.payload.type));
    set(2, packet.summary);
    set(3, packet.receivedAt.toString(Qt::ISODate));

    constexpr int kMaxRows = 1000;
    while (ui_->tableData->rowCount() > kMaxRows) {
        ui_->tableData->removeRow(0);
    }
}

void MainWindow::onLogMessage(const QString &message) {
    appendLog(message);
}

int MainWindow::rowForClient(ClientId id) const {
    return clientRow_.value(id, -1);
}

void MainWindow::appendLog(const QString &message) {
    const QString line = QStringLiteral("[%1] %2")
            .arg(QDateTime::currentDateTime().toString("HH:mm:ss"), message);
    ui_->textLog->append(line);
}

void MainWindow::setServerStatusLabel(bool running, quint16 port) {
    if (running) {
        ui_->lblServerStatus->setText(
            QStringLiteral("Server: Running on port %1").arg(port));
        ui_->lblServerStatus->setStyleSheet(
            QStringLiteral("color: #070; font-weight: bold;"));
    } else {
        ui_->lblServerStatus->setText(QStringLiteral("Server: Stopped"));
        ui_->lblServerStatus->setStyleSheet(
            QStringLiteral("color: #b00; font-weight: bold;"));
    }
}
