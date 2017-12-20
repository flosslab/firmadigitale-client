#include "certificate.hpp"

#include <QtCore/QProcess>
#include <QtNetwork/QSslCertificate>

CertificateUtility::CertificateUtility(QObject *parent) : QObject(parent) {
    settings = FDOSettings::getInstance();
}

void CertificateUtility::getCertificateId() {
    QStringList arguments;
    arguments << "--module" << settings->getSmartcardLib();
    arguments << "--list-objects";
    arguments << "--type" << "cert";

    QProcess process;
    process.start(settings->getPkcsToolBin(), arguments);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit) {
        emit newCertificateId("");
        return;
    }

    QString rawOutput(process.readAll());
    QStringList lines = rawOutput.split('\n');

    QList<QPair<QString, QString>> certificates;
    bool containsCertificate = false;
    QString certificateLabel;
    QString certificateId;

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
            if (line.startsWith("  label:")) {
                QStringList items = line.trimmed().split(':', QString::SkipEmptyParts);
                if (items.length() == 2)
                    certificateLabel = items[1].trimmed();
            }
            if (line.startsWith("  ID:")) {
                QStringList items = line.trimmed().split(':', QString::SkipEmptyParts);
                if (items.length() == 2)
                    certificateId = items[1].trimmed();
            }

            if (certificateLabel.length() > 0 && certificateId.length() > 0) {
                QPair<QString, QString> newCertificate(certificateLabel, certificateId);
                certificates.append(newCertificate);
            }
        }
    }

    for (const QPair<QString, QString> &certificate: certificates) {
        if (certificate.first.toLower().contains("firma")) {
            emit newCertificateId(certificate.second);
            return;
        }

        emit newCertificateId("");
    }
}

void CertificateUtility::getCertificate(QString id) {
    QStringList arguments;
    arguments << "--module" << settings->getSmartcardLib();
    arguments << "--read-object";
    arguments << "--type" << "cert";
    arguments << "--id" << id;

    QProcess process;
    process.start(settings->getPkcsToolBin(), arguments);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit) {
        emit newCertificate("");
        return;
    }

    QByteArray certBytes = process.readAll();

    QSslCertificate certificate(certBytes, QSsl::Der);
    QString cert = QString(certificate.toPem());
    emit newCertificate(cert);
}
