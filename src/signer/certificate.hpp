#ifndef __FDOTOOL_SIGNER_CERTIFICATE_H
#define __FDOTOOL_SIGNER_CERTIFICATE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <fdosettings.hpp>

class CertificateUtility : public QObject {
Q_OBJECT

public:

    explicit CertificateUtility(QObject *parent = Q_NULLPTR);

public slots:

    void getCertificateId();

    void getCertificate(QString id);

private:

    FDOSettings *settings;

signals:

    void newCertificateId(QString certId);

    void newCertificate(QString cert);
};

#endif
