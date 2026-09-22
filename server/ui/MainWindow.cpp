#include "MainWindow.h"

MainWindow::MainWindow(IServerController* controller, QWidget* parent)
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
    connect(ui_->btnStartServer,  &QPushButton::clicked,
            this, &MainWindow::handleStartServerClicked);
    connect(ui_->btnStopServer,   &QPushButton::clicked,
            this, &MainWindow::handleStopServerClicked);
    connect(ui_->btnStartClients, &QPushButton::clicked,
            this, &MainWindow::handleStartClientsClicked);
    connect(ui_->btnStopClients,  &QPushButton::clicked,
            this, &MainWindow::handleStopClientsClicked);
    connect(ui_->btnSettings,     &QPushButton::clicked,
            this, &MainWindow::handleSettingsClicked);
    connect(ui_->btnClearLog,     &QPushButton::clicked,
            this, &MainWindow::handleClearLogClicked);

    connect(ui_->actionExit,       &QAction::triggered,
            this, &MainWindow::handleExitAction);
    connect(ui_->actionAbout,      &QAction::triggered,
            this, &MainWindow::handleAboutAction);
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

    connect(controller_, &ServerController::serverStarted,
            this, &MainWindow::onServerStarted);
    connect(controller_, &ServerController::serverStopped,
            this, &MainWindow::onServerStopped);
    connect(controller_, &ServerController::fatalError,
            this, &MainWindow::onFatalError);

    connect(controller_, &ServerController::clientConnected,
            this, &MainWindow::onClientConnected);
    connect(controller_, &ServerController::clientDisconnected,
            this, &MainWindow::onClientDisconnected);
    connect(controller_, &ServerController::clientStatusChanged,
            this, &MainWindow::onClientStatusChanged);

    connect(controller_, &ServerController::packetReceived,
            this, &MainWindow::onPacketReceived);
    connect(controller_, &ServerController::logMessage,
            this, &MainWindow::onLogMessage);
}


void MainWindow::handleStartServerClicked() {
    // TODO: controller_->startServer(12345);
    appendLog(QStringLiteral("[stub] Start Server clicked"));
}

void MainWindow::handleStopServerClicked() {
    // TODO: controller_->stopServer();
    appendLog(QStringLiteral("[stub] Stop Server clicked"));
}

void MainWindow::handleStartClientsClicked() {
    // TODO: controller_->broadcastStart();
    appendLog(QStringLiteral("[stub] Start Clients clicked"));
}

void MainWindow::handleStopClientsClicked() {
    // TODO: controller_->broadcastStop();
    appendLog(QStringLiteral("[stub] Stop Clients clicked"));
}

void MainWindow::handleSettingsClicked() {
    // TODO: открыть SettingsDialog, затем controller_->applyThresholds(...)
    appendLog(QStringLiteral("[stub] Settings clicked"));
}

void MainWindow::handleClearLogClicked() {
    ui_->textLog->clear();
    appendLog(QStringLiteral("Log cleared."));
}

void MainWindow::handleExitAction() {
    close();
}

void MainWindow::handleAboutAction() {
    // TODO: rewrite this shit
    QMessageBox::about(this,
        tr("About Telecom Server"),
        tr("Telecom Server — Qt 6 / QTcpServer demo.\n"
           "Client-server test assignment."));
}

void MainWindow::handleSaveConfigAction() {
    // TODO: сериализовать thresholds в JSON и сохранить в файл.
    appendLog(QStringLiteral("[stub] Save Configuration clicked"));
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

void MainWindow::onFatalError(const QString& message) {
    appendLog(QStringLiteral("FATAL: ") + message);
    QMessageBox::critical(this, tr("Server error"), message);
}

void MainWindow::onClientConnected(ClientInfo info) {
    const int row = ui_->tableClients->rowCount();
    ui_->tableClients->insertRow(row);

    auto set = [&](int col, const QString& text) {
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
        if (auto* item = ui_->tableClients->item(row, 3)) {
            item->setText(QStringLiteral("Disconnected"));
        }
    }
    appendLog(QStringLiteral("Client %1 disconnected").arg(id));
}

void MainWindow::onClientStatusChanged(ClientId id, ClientStatus status) {
    const int row = rowForClient(id);
    if (row < 0) return;

    QString text;
    QColor  color = Qt::black;
    switch (status) {
        case ClientStatus::Connected:    text = "Connected";    break;
        case ClientStatus::Disconnected: text = "Disconnected"; color = Qt::gray; break;
        case ClientStatus::Warning:      text = "Warning";      color = QColor(200,120,0); break;
    }
    if (auto* item = ui_->tableClients->item(row, 3)) {
        item->setText(text);
        item->setForeground(color);
    }
}

void MainWindow::onPacketReceived(IncomingPacket packet) {
    const int row = ui_->tableData->rowCount();
    ui_->tableData->insertRow(row);

    auto set = [&](int col, const QString& text) {
        ui_->tableData->setItem(row, col, new QTableWidgetItem(text));
    };
    set(0, QString::number(packet.clientId));
    set(1, PacketCodec::dataTypeToString(packet.type));
    set(2, packet.summary);
    set(3, packet.receivedAt.toString(Qt::ISODate));

    constexpr int kMaxRows = 1000;
    while (ui_->tableData->rowCount() > kMaxRows) {
        ui_->tableData->removeRow(0);
    }
}

void MainWindow::onLogMessage(const QString& message) {
    appendLog(message);
}

int MainWindow::rowForClient(ClientId id) const {
    return clientRow_.value(id, -1);
}

void MainWindow::appendLog(const QString& message) {
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