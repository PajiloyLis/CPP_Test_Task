#include "SettingsDialog.h"
#include "ui/SettingsDialog/ui_SettingsDialog.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QPushButton>


SettingsDialog::SettingsDialog(const Thresholds &initial, QWidget *parent)
    : QDialog(parent),
      ui_(new Ui::SettingsDialog),
      initial_(initial) {
    ui_->setupUi(this);

    loadIntoFields(initial_);

    connect(ui_->buttonBox, &QDialogButtonBox::accepted,
            this, &SettingsDialog::onAccepted);
    connect(ui_->buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);

    connect(ui_->btnImportFromFile, &QPushButton::clicked,
            this, &SettingsDialog::onImportFromFile);
    connect(ui_->btnResetToDefaults, &QPushButton::clicked,
            this, &SettingsDialog::onResetToDefaults);

    connect(ui_->dsbMaxLatency, &QDoubleSpinBox::valueChanged,
            this, &SettingsDialog::onAnyFieldChanged);
    connect(ui_->dsbMaxPacketLoss, &QDoubleSpinBox::valueChanged,
            this, &SettingsDialog::onAnyFieldChanged);
    connect(ui_->dsbMinBandwidth, &QDoubleSpinBox::valueChanged,
            this, &SettingsDialog::onAnyFieldChanged);
    connect(ui_->sbMaxCpuUsage, &QSpinBox::valueChanged,
            this, &SettingsDialog::onAnyFieldChanged);
    connect(ui_->sbMaxMemoryUsage, &QSpinBox::valueChanged,
            this, &SettingsDialog::onAnyFieldChanged);
}

SettingsDialog::~SettingsDialog() {
    delete ui_;
}

Thresholds SettingsDialog::thresholds() const {
    return readFromFields();
}

void SettingsDialog::loadIntoFields(const Thresholds &t) {
    ui_->dsbMaxLatency->setValue(t.maxLatency);
    ui_->dsbMaxPacketLoss->setValue(t.maxPacketLoss);
    ui_->dsbMinBandwidth->setValue(t.minBandwidth);
    ui_->sbMaxCpuUsage->setValue(t.maxCpuUsage);
    ui_->sbMaxMemoryUsage->setValue(t.maxMemoryUsage);
    ui_->chkSendLogOnAlert->setChecked(t.sendLogOnAlert);
    clearHighlights();
}

Thresholds SettingsDialog::readFromFields() const {
    Thresholds t;
    t.maxLatency = ui_->dsbMaxLatency->value();
    t.maxPacketLoss = ui_->dsbMaxPacketLoss->value();
    t.minBandwidth = ui_->dsbMinBandwidth->value();
    t.maxCpuUsage = ui_->sbMaxCpuUsage->value();
    t.maxMemoryUsage = ui_->sbMaxMemoryUsage->value();
    t.sendLogOnAlert = ui_->chkSendLogOnAlert->isChecked();
    return t;
}

void SettingsDialog::onAnyFieldChanged() {
    clearHighlights();
}

void SettingsDialog::onAccepted() {
    if (!validateAndHighlight()) {
        return;
    }
    accept();
}

void SettingsDialog::onResetToDefaults() {
    loadIntoFields(Thresholds{});
}

void SettingsDialog::onImportFromFile() {
    const QString path = QFileDialog::getOpenFileName(
        this,
        tr("Import thresholds"),
        QString(),
        tr("JSON files (*.json);;All files (*)"));

    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("Import failed"), f.errorString());
        return;
    }

    auto parsed = ThresholdsJson::fromJson(f.readAll());
    if (!parsed) {
        QMessageBox::warning(
            this,
            tr("Import failed"),
            tr("File does not contain valid threshold values."));
        return;
    }

    loadIntoFields(*parsed);
}

bool SettingsDialog::validateAndHighlight() {
    clearHighlights();
    bool ok = true;

    auto mark = [](QWidget *w) {
        w->setStyleSheet(QStringLiteral(
            "border: 1px solid #d33; border-radius: 2px;"));
    };

    if (ui_->dsbMaxLatency->value() <= 0.0) {
        mark(ui_->dsbMaxLatency);
        ok = false;
    }
    if (ui_->dsbMaxPacketLoss->value() < 0.0 ||
        ui_->dsbMaxPacketLoss->value() > 100.0) {
        mark(ui_->dsbMaxPacketLoss);
        ok = false;
    }
    if (ui_->dsbMinBandwidth->value() <= 0.0) {
        mark(ui_->dsbMinBandwidth);
        ok = false;
    }
    if (ui_->sbMaxCpuUsage->value() < 1 ||
        ui_->sbMaxCpuUsage->value() > 100) {
        mark(ui_->sbMaxCpuUsage);
        ok = false;
    }
    if (ui_->sbMaxMemoryUsage->value() < 1 ||
        ui_->sbMaxMemoryUsage->value() > 100) {
        mark(ui_->sbMaxMemoryUsage);
        ok = false;
    }

    return ok;
}

void SettingsDialog::clearHighlights() {
    ui_->dsbMaxLatency->setStyleSheet(QString());
    ui_->dsbMaxPacketLoss->setStyleSheet(QString());
    ui_->dsbMinBandwidth->setStyleSheet(QString());
    ui_->sbMaxCpuUsage->setStyleSheet(QString());
    ui_->sbMaxMemoryUsage->setStyleSheet(QString());
}
