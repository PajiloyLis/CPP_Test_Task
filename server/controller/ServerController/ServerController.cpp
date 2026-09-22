#include "ServerController.h"
#include "network/serverWorker/ServerWorker.h"

#include <QMetaObject>
#include <QThread>

ServerController::ServerController(QObject *parent)
    : IServerController(parent) {
    thread_ = new QThread(this);
    worker_ = new ServerWorker();
    worker_->moveToThread(thread_);

    connect(thread_, &QThread::finished, worker_, &QObject::deleteLater);

    // Ретрансляция сигналов worker -> интерфейс.
    connect(worker_, &ServerWorker::serverStarted,
            this, &IServerController::serverStarted);
    connect(worker_, &ServerWorker::serverStopped,
            this, &IServerController::serverStopped);
    connect(worker_, &ServerWorker::fatalError,
            this, &IServerController::fatalError);

    connect(worker_, &ServerWorker::clientConnected,
            this, &IServerController::clientConnected);
    connect(worker_, &ServerWorker::clientDisconnected,
            this, &IServerController::clientDisconnected);
    connect(worker_, &ServerWorker::clientStatusChanged,
            this, &IServerController::clientStatusChanged);

    connect(worker_, &ServerWorker::packetReceived,
            this, &IServerController::packetReceived);
    connect(worker_, &ServerWorker::logMessage,
            this, &IServerController::logMessage);

    thread_->start();
}

ServerController::~ServerController() {
    if (!worker_) return;

    // Просим worker корректно остановиться и ждём выполнения.
    // BlockingQueuedConnection безопасен: мы в GUI-потоке, worker — в thread_.
    QMetaObject::invokeMethod(worker_, &ServerWorker::stop,
                              Qt::BlockingQueuedConnection);

    thread_->quit();
    thread_->wait();
    // thread_ удалится как дочерний QObject этого объекта.
    // worker_ удалится по finished → deleteLater.
}

// ---------------------------------------------------------------------------
// IServerController implementation
// ---------------------------------------------------------------------------

void ServerController::startServer(quint16 port) {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w, port] { w->start(port); },
                              Qt::QueuedConnection);
}

void ServerController::stopServer() {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w] { w->stop(); },
                              Qt::QueuedConnection);
}

void ServerController::broadcastStart() {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w] { w->broadcastStart(); },
                              Qt::QueuedConnection);
}

void ServerController::broadcastStop() {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w] { w->broadcastStop(); },
                              Qt::QueuedConnection);
}

void ServerController::applyThresholds(const Thresholds &t) {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w, t] { w->updateThresholds(t); },
                              Qt::QueuedConnection);
}

void ServerController::kickClient(ClientId id) {
    auto *w = worker_;
    QMetaObject::invokeMethod(w, [w, id] { w->kickClient(id); },
                              Qt::QueuedConnection);
}
