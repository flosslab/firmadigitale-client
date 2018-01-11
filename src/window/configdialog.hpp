#ifndef __FDOTOOL_WINDOW_CONFIGDIALOG_H
#define __FDOTOOL_WINDOW_CONFIGDIALOG_H

#include <QDialog>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtNetwork/QSslCertificate>

#include <fdosettings.hpp>

namespace Ui {
    class ConfigDialog;
}

class ConfigDialog : public QDialog {
Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);

    ~ConfigDialog() override;

private:
    Ui::ConfigDialog *ui;

    FDOSettings *settings;

    void prepareSmartcardProducerValues();


    void load();

    void save();

private slots:

    void prepareCertificateIdValues();

    void updateCertificateIdValues(QMap<QString, QSslCertificate> certificates);

    void handleOK();

    void handleApply();

    void handleClose();

    void handleReset();

    void handlePkcsToolBrowse();

    void handlePkcsEngineBrowse();

    void handleOpenSSLBrowse();

    void handleSmartcardBrowse();

    void handlePkcsToolAuto();

    void handlePkcsEngineAuto();

    void handleOpenSSLAuto();

    void handleSmartcardProducerAuto();

    void handleSmartcardLibAuto();

    void handleCertificateIdAuto();

    QString searchSmartcardLib();
};

#endif
