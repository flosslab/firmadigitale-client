#include "certificate.hpp"

#include <QtCore/QProcess>

#include <fdosettings.hpp>

CertificateUtility::CertificateUtility(QObject *parent) : QObject(parent) {
}

void CertificateUtility::getCertificates() {
    QMap<QString, QSslCertificate> certificates = getCertificatesSync();
    emit newCertificates(certificates);
}

void CertificateUtility::getCertificate(QString id) {
    QString certificate = getCertificateSync(id);
    emit newCertificate(certificate);
}

QMap<QString, QSslCertificate> CertificateUtility::getCertificatesSync() {
    QMap<QString, QSslCertificate> certificates;

    FDOSettings *settings = FDOSettings::getInstance();

    QStringList arguments;
    arguments << "--module" << settings->getSmartcardLib();
    arguments << "--list-objects";
    arguments << "--type" << "cert";

    QProcess process;
    process.start(settings->getPkcsToolBin(), arguments);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit)
        return certificates;

    QString rawOutput(process.readAll());
    QStringList lines = rawOutput.split('\n');

    bool containsCertificate = false;
    QString certificateLabel;
    QString certificateId;

    QStringList certificateIds;

    for (const QString &line: lines) {
        if (line.startsWith("Certificate Object") && line.contains("type = X.509 cert")) {
            certificateLabel.clear();
            certificateId.clear();
            containsCertificate = true;
            continue;
        } else if (!line.startsWith("  ")) {
            containsCertificate = false;
        }

        if (containsCertificate) {
            if (line.startsWith("  ID:")) {
                QStringList items = line.trimmed().split(':', QString::SkipEmptyParts);
                if (items.length() == 2)
                    certificateIds.append(items[1].trimmed());
            }
        }
    }

    for (const QString &certId: certificateIds) {
        QSslCertificate sslCertificate = getSSLCertificate(certId);
        certificates.insert(certId, sslCertificate);
    }

    return certificates;
}

QString CertificateUtility::getCertificateSync(const QString &id) {
    QSslCertificate certificate = getSSLCertificate(id);
    QString cert = QString(certificate.toPem());
    return cert;
}

QSslCertificate CertificateUtility::getSSLCertificate(const QString &certId) {
    FDOSettings *settings = FDOSettings::getInstance();

    QStringList arguments;
    arguments << "--module" << settings->getSmartcardLib();
    arguments << "--read-object";
    arguments << "--type" << "cert";
    arguments << "--id" << certId;

    QProcess process;
    process.start(settings->getPkcsToolBin(), arguments);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit)
        return QSslCertificate();

    QByteArray certBytes = process.readAll();
    QSslCertificate certificate(certBytes, QSsl::Der);
    return certificate;
}
