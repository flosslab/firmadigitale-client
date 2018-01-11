#include <QtWidgets>

#include "configdialog.hpp"
#include "ui_configdialog.h"

#include <manager.hpp>
#include <utility.hpp>
#include <defaults.hpp>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog) {
    ui->setupUi(this);

    settings = FDOSettings::getInstance();

    prepare();

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(handleOK()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), this, SLOT(handleApply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(handleClose()));
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked(bool)), this, SLOT(handleReset()));

    connect(ui->pkcsToolBinBrowse, SIGNAL(clicked(bool)), this, SLOT(handlePkcsToolBrowse()));
    connect(ui->pkcsEngineLibBrowse, SIGNAL(clicked(bool)), this, SLOT(handlePkcsEngineBrowse()));
    connect(ui->opensslBinBrowse, SIGNAL(clicked(bool)), this, SLOT(handleOpenSSLBrowse()));

    connect(ui->pkcsToolBinAuto, SIGNAL(clicked(bool)), this, SLOT(handlePkcsToolAuto()));
    connect(ui->pkcsEngineLibAuto, SIGNAL(clicked(bool)), this, SLOT(handlePkcsEngineAuto()));
    connect(ui->opensslBinAuto, SIGNAL(clicked(bool)), this, SLOT(handleOpenSSLAuto()));

    connect(ui->smartcardLibBrowse, SIGNAL(clicked(bool)), this, SLOT(handleSmartcardBrowse()));

    connect(ui->smartcardProducerAuto, SIGNAL(clicked(bool)), this, SLOT(handleSmartcardProducerAuto()));
    connect(ui->smartcardLibAuto, SIGNAL(clicked(bool)), this, SLOT(handleSmartcardLibAuto()));

    connect(ui->smartcardProducerValue, SIGNAL(activated(int)), this, SLOT(searchSmartcardLib()));

    load();
}

ConfigDialog::~ConfigDialog() {
    delete ui;
}

void ConfigDialog::handleOK() {
    save();
    close();
}

void ConfigDialog::handleApply() {
    save();
}

void ConfigDialog::handleClose() {
    close();
}

void ConfigDialog::handleReset() {
    load();
}

void ConfigDialog::prepare() {
    ui->smartcardProducerValue->clear();
    ui->smartcardProducerValue->addItem("Athena", SETTINGS_SMARTCARD_PRODUCER_ATHENA);
    ui->smartcardProducerValue->addItem("Incard / Oberthur", SETTINGS_SMARTCARD_PRODUCER_INCARDOBERTHUR);
}

void ConfigDialog::load() {
    SettingsManager::load();

    ui->pkcsToolBinValue->setText(settings->getPkcsToolBin());
    ui->pkcsEngineLibValue->setText(settings->getPkcsEngineLib());
    ui->opensslBinValue->setText(settings->getOpensslBin());

    int index = ui->smartcardProducerValue->findData(settings->getSmartcardProducer());
    if (index != -1)
        ui->smartcardProducerValue->setCurrentIndex(index);

    ui->smartcardLibValue->setText(settings->getSmartcardLib());
}

void ConfigDialog::save() {
    settings->setPkcsToolBin(ui->pkcsToolBinValue->text());
    settings->setPkcsEngineLib(ui->pkcsEngineLibValue->text());
    settings->setOpensslBin(ui->opensslBinValue->text());

    settings->setSmartcardProducer(ui->smartcardProducerValue->currentData().toString());
    settings->setSmartcardLib(ui->smartcardLibValue->text());

    SettingsManager::save();
}

void ConfigDialog::handlePkcsToolBrowse() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setOption(QFileDialog::ReadOnly, true);

    if (ui->pkcsToolBinValue->text().length() > 0)
        fileDialog.selectFile(ui->pkcsToolBinValue->text());

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    ui->pkcsToolBinValue->setText(selectedFiles[0]);
}

void ConfigDialog::handlePkcsEngineBrowse() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setOption(QFileDialog::ReadOnly, true);

    if (ui->pkcsEngineLibValue->text().length() > 0)
        fileDialog.selectFile(ui->pkcsEngineLibValue->text());

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    ui->pkcsEngineLibValue->setText(selectedFiles[0]);
}

void ConfigDialog::handleOpenSSLBrowse() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setOption(QFileDialog::ReadOnly, true);

    if (ui->opensslBinValue->text().length() > 0)
        fileDialog.selectFile(ui->opensslBinValue->text());

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    ui->opensslBinValue->setText(selectedFiles[0]);
}

void ConfigDialog::handleSmartcardBrowse() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setOption(QFileDialog::ReadOnly, true);

    if (ui->smartcardLibValue->text().length() > 0)
        fileDialog.selectFile(ui->smartcardLibValue->text());

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    ui->smartcardLibValue->setText(selectedFiles[0]);
}

void ConfigDialog::handlePkcsToolAuto() {
    QString defaultValue = DiscoverUtility::discoverPkcsToolBin();
    if (defaultValue.length() == 0)
        QMessageBox::critical(this, "Error finding automatic value", "Unable to find the right value automatically");

    ui->pkcsToolBinValue->setText(defaultValue);
}

void ConfigDialog::handlePkcsEngineAuto() {
    QString defaultValue = DiscoverUtility::discoverPkcsEngineLib();
    if (defaultValue.length() == 0)
        QMessageBox::critical(this, "Error finding automatic value", "Unable to find the right value automatically");

    ui->pkcsEngineLibValue->setText(defaultValue);
}

void ConfigDialog::handleOpenSSLAuto() {
    QString defaultValue = DiscoverUtility::discoverOpenSSLBin();
    if (defaultValue.length() == 0)
        QMessageBox::critical(this, "Error finding automatic value", "Unable to find the right value automatically");

    ui->opensslBinValue->setText(defaultValue);
}

void ConfigDialog::handleSmartcardProducerAuto() {
    QMessageBox::critical(this, "Error finding automatic value", "Unable to find the right value automatically");
}

void ConfigDialog::handleSmartcardLibAuto() {
    QString defaultValue = searchSmartcardLib();

    if (defaultValue.length() == 0)
        QMessageBox::critical(this, "Error finding automatic value", "Unable to find the right value automatically");
}

QString ConfigDialog::searchSmartcardLib() {
    QString defaultValue;

    QString actualProducer = ui->smartcardProducerValue->currentData().toString();

    if (actualProducer == SETTINGS_SMARTCARD_PRODUCER_ATHENA)
        defaultValue = DiscoverUtility::discoverSmartcardLibAthena();

    if (actualProducer == SETTINGS_SMARTCARD_PRODUCER_INCARDOBERTHUR)
        defaultValue = DiscoverUtility::discoverSmartcardLibIncardOberthur();

    ui->smartcardLibValue->setText(defaultValue);

    return defaultValue;
}
