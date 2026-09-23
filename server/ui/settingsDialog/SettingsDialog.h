#ifndef SERVER_SETTINGSDIALOG_H
#define SERVER_SETTINGSDIALOG_H

#include <QDialog>

#include "domainModels/Thresholds.h"
#include "configParsers/thresholdsConfigParser/ThresholdsParser.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class SettingsDialog;
}

QT_END_NAMESPACE


class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(const Thresholds &initial, QWidget *parent = nullptr);

    ~SettingsDialog() override;

    Thresholds thresholds() const;

private slots:
    void onImportFromFile();

    void onResetToDefaults();

    void onAccepted();

    void onAnyFieldChanged();

private:
    void loadIntoFields(const Thresholds &t);

    Thresholds readFromFields() const;

    bool validateAndHighlight();

    void clearHighlights();

    Ui::SettingsDialog *ui_ = nullptr;
    Thresholds initial_;
};

#endif //SERVER_SETTINGSDIALOG_H
