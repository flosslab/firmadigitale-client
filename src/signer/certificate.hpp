#ifndef __FDOTOOL_SIGNER_CERTIFICATE_H
#define __FDOTOOL_SIGNER_CERTIFICATE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtNetwork/QSslCertificate>

class CertificateUtility : public QObject {
Q_OBJECT

public:

    explicit CertificateUtility(QObject *parent = Q_NULLPTR);

    static QString getCertificateSync(const QString &id);

    static QMap<QString, QSslCertificate> getCertificatesSync();

public slots:

    Q_INVOKABLE void getCertificates();

    Q_INVOKABLE void getCertificate(QString id);

private:

    static QSslCertificate getSSLCertificate(const QString &id);

signals:

    void newCertificates(QMap<QString, QSslCertificate>);

    void newCertificate(QString cert);
};

#endif
