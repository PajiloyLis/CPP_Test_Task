/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *mainLayout;
    QGroupBox *groupThresholds;
    QFormLayout *formLayout;
    QLabel *lblMaxLatency;
    QDoubleSpinBox *dsbMaxLatency;
    QLabel *lblMaxPacketLoss;
    QDoubleSpinBox *dsbMaxPacketLoss;
    QLabel *lblMinBandwidth;
    QDoubleSpinBox *dsbMinBandwidth;
    QLabel *lblMaxCpuUsage;
    QSpinBox *sbMaxCpuUsage;
    QLabel *lblMaxMemoryUsage;
    QSpinBox *sbMaxMemoryUsage;
    QCheckBox *chkSendLogOnAlert;
    QHBoxLayout *auxButtonsLayout;
    QSpacerItem *auxSpacer;
    QPushButton *btnImportFromFile;
    QPushButton *btnResetToDefaults;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(420, 292);
        SettingsDialog->setModal(true);
        mainLayout = new QVBoxLayout(SettingsDialog);
        mainLayout->setObjectName("mainLayout");
        groupThresholds = new QGroupBox(SettingsDialog);
        groupThresholds->setObjectName("groupThresholds");
        formLayout = new QFormLayout(groupThresholds);
        formLayout->setObjectName("formLayout");
        lblMaxLatency = new QLabel(groupThresholds);
        lblMaxLatency->setObjectName("lblMaxLatency");

        formLayout->setWidget(0, QFormLayout::LabelRole, lblMaxLatency);

        dsbMaxLatency = new QDoubleSpinBox(groupThresholds);
        dsbMaxLatency->setObjectName("dsbMaxLatency");
        dsbMaxLatency->setDecimals(2);
        dsbMaxLatency->setMinimum(0.010000000000000);
        dsbMaxLatency->setMaximum(1000000.000000000000000);
        dsbMaxLatency->setValue(50.000000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, dsbMaxLatency);

        lblMaxPacketLoss = new QLabel(groupThresholds);
        lblMaxPacketLoss->setObjectName("lblMaxPacketLoss");

        formLayout->setWidget(1, QFormLayout::LabelRole, lblMaxPacketLoss);

        dsbMaxPacketLoss = new QDoubleSpinBox(groupThresholds);
        dsbMaxPacketLoss->setObjectName("dsbMaxPacketLoss");
        dsbMaxPacketLoss->setDecimals(2);
        dsbMaxPacketLoss->setMinimum(0.000000000000000);
        dsbMaxPacketLoss->setMaximum(100.000000000000000);
        dsbMaxPacketLoss->setValue(5.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, dsbMaxPacketLoss);

        lblMinBandwidth = new QLabel(groupThresholds);
        lblMinBandwidth->setObjectName("lblMinBandwidth");

        formLayout->setWidget(2, QFormLayout::LabelRole, lblMinBandwidth);

        dsbMinBandwidth = new QDoubleSpinBox(groupThresholds);
        dsbMinBandwidth->setObjectName("dsbMinBandwidth");
        dsbMinBandwidth->setDecimals(2);
        dsbMinBandwidth->setMinimum(0.010000000000000);
        dsbMinBandwidth->setMaximum(1000000.000000000000000);
        dsbMinBandwidth->setValue(10.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, dsbMinBandwidth);

        lblMaxCpuUsage = new QLabel(groupThresholds);
        lblMaxCpuUsage->setObjectName("lblMaxCpuUsage");

        formLayout->setWidget(3, QFormLayout::LabelRole, lblMaxCpuUsage);

        sbMaxCpuUsage = new QSpinBox(groupThresholds);
        sbMaxCpuUsage->setObjectName("sbMaxCpuUsage");
        sbMaxCpuUsage->setMinimum(1);
        sbMaxCpuUsage->setMaximum(100);
        sbMaxCpuUsage->setValue(90);

        formLayout->setWidget(3, QFormLayout::FieldRole, sbMaxCpuUsage);

        lblMaxMemoryUsage = new QLabel(groupThresholds);
        lblMaxMemoryUsage->setObjectName("lblMaxMemoryUsage");

        formLayout->setWidget(4, QFormLayout::LabelRole, lblMaxMemoryUsage);

        sbMaxMemoryUsage = new QSpinBox(groupThresholds);
        sbMaxMemoryUsage->setObjectName("sbMaxMemoryUsage");
        sbMaxMemoryUsage->setMinimum(1);
        sbMaxMemoryUsage->setMaximum(100);
        sbMaxMemoryUsage->setValue(90);

        formLayout->setWidget(4, QFormLayout::FieldRole, sbMaxMemoryUsage);

        chkSendLogOnAlert = new QCheckBox(groupThresholds);
        chkSendLogOnAlert->setObjectName("chkSendLogOnAlert");
        chkSendLogOnAlert->setChecked(true);

        formLayout->setWidget(5, QFormLayout::FieldRole, chkSendLogOnAlert);


        mainLayout->addWidget(groupThresholds);

        auxButtonsLayout = new QHBoxLayout();
        auxButtonsLayout->setObjectName("auxButtonsLayout");
        auxSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        auxButtonsLayout->addItem(auxSpacer);

        btnImportFromFile = new QPushButton(SettingsDialog);
        btnImportFromFile->setObjectName("btnImportFromFile");

        auxButtonsLayout->addWidget(btnImportFromFile);

        btnResetToDefaults = new QPushButton(SettingsDialog);
        btnResetToDefaults->setObjectName("btnResetToDefaults");

        auxButtonsLayout->addWidget(btnResetToDefaults);


        mainLayout->addLayout(auxButtonsLayout);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Client Settings", nullptr));
        groupThresholds->setTitle(QCoreApplication::translate("SettingsDialog", "Thresholds", nullptr));
        lblMaxLatency->setText(QCoreApplication::translate("SettingsDialog", "Max latency:", nullptr));
        dsbMaxLatency->setSuffix(QCoreApplication::translate("SettingsDialog", " ms", nullptr));
        lblMaxPacketLoss->setText(QCoreApplication::translate("SettingsDialog", "Max packet loss:", nullptr));
        dsbMaxPacketLoss->setSuffix(QCoreApplication::translate("SettingsDialog", " %", nullptr));
        lblMinBandwidth->setText(QCoreApplication::translate("SettingsDialog", "Min bandwidth:", nullptr));
        dsbMinBandwidth->setSuffix(QCoreApplication::translate("SettingsDialog", " Mbps", nullptr));
        lblMaxCpuUsage->setText(QCoreApplication::translate("SettingsDialog", "Max CPU usage:", nullptr));
        sbMaxCpuUsage->setSuffix(QCoreApplication::translate("SettingsDialog", " %", nullptr));
        lblMaxMemoryUsage->setText(QCoreApplication::translate("SettingsDialog", "Max memory usage:", nullptr));
        sbMaxMemoryUsage->setSuffix(QCoreApplication::translate("SettingsDialog", " %", nullptr));
        chkSendLogOnAlert->setText(QCoreApplication::translate("SettingsDialog", "Send Log alert to client", nullptr));
        btnImportFromFile->setText(QCoreApplication::translate("SettingsDialog", "Import from file...", nullptr));
        btnResetToDefaults->setText(QCoreApplication::translate("SettingsDialog", "Reset to defaults", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
