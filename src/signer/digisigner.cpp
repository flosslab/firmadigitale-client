#include "digisigner.hpp"

#include <QtCore/QTemporaryFile>
#include <QtCore/QThread>
#include <QtNetwork/QSslCertificate>
#include <iostream>

DigiSigner::DigiSigner(QObject *parent) : QObject(parent) {
    settings = FDOSettings::getInstance();
}

bool DigiSigner::initCard() {
    certificateId = CertificateUtility::getCertificateIdSync();
    if (certificateId.length() == 0) {
        emit error("Smart card not found");
        return false;
    }

    certificate = CertificateUtility::getCertificateSync(certificateId);

    return true;
}

void DigiSigner::setPin(const QString &pin) {
    this->pin = pin;
}

QByteArray DigiSigner::cadesSign(const QByteArray &inputData) {
    QStringList response;

    if (certificateId.length() == 0
        || certificate.length() == 0
        || pin.length() == 0) {
        emit error("Not initialized or pin not set");
        return QByteArray();
    }

    QString tempFilePrefix = QDir(QDir::tempPath()).absoluteFilePath(FDOTOOL_SIGNER_DIGISIGNER_TEMFILE_PREFIX);

    QTemporaryFile crtFile(tempFilePrefix);
    crtFile.setAutoRemove(false);
    crtFile.open();
    crtFile.write(certificate.toLatin1());
    crtFile.close();

    QTemporaryFile inFile(tempFilePrefix);
    inFile.setAutoRemove(false);
    inFile.open();
    inFile.write(inputData);
    inFile.close();

    QTemporaryFile outFile(tempFilePrefix);
    outFile.setAutoRemove(false);
    outFile.open();
    outFile.close();

    QString engineName = "pkcs11";

    QStringList engineArgs;
    engineArgs << "engine";
    engineArgs << "-t";
    engineArgs << "dynamic";
    engineArgs << "-pre" << QString("SO_PATH:%1").arg(settings->getPkcsEngineLib());
    engineArgs << "-pre" << QString("ID:%1").arg(engineName);
    engineArgs << "-pre" << "LIST_ADD:1";
    engineArgs << "-pre" << "LOAD";
    engineArgs << "-pre" << QString("MODULE_PATH:%1").arg(settings->getSmartcardLib());
    engineArgs << "-pre" << QString("PIN:%1").arg(pin);
    engineArgs << "-pre" << "FORCE_LOGIN";

    QStringList smimeArgs;
    smimeArgs << "cms";
    smimeArgs << "-nosmimecap";
    smimeArgs << "-md" << "sha256";
    smimeArgs << "-nodetach";
    smimeArgs << "-binary";
    smimeArgs << "-cades";
    smimeArgs << "-stream";
    smimeArgs << "-outform" << "DER";
    smimeArgs << "-sign";
    smimeArgs << "-signer" << crtFile.fileName();
    smimeArgs << "-inkey" << QString("id_%1").arg(certificateId);
    smimeArgs << "-keyform" << "engine";
    smimeArgs << "-in" << inFile.fileName();
    smimeArgs << "-out" << outFile.fileName();
    smimeArgs << "-engine" << engineName;

    QString engineCmd = engineArgs.join(" ") + "\n";
    QString smimeCmd = smimeArgs.join(" ") + "\n";
    QString pinCmd = pin + "\n";

    emit progressStep("Starting OpenSSL");
    QProcess process;
    process.start(settings->getOpensslBin());
    process.waitForStarted();

    do {
        response.clear();
        response.append(readReponse(process));
        if (!response.isEmpty()) {
            if (responseContains(response, "OpenSSL>")) {
                break;
            } else {
                emit error("Unable to start OpenSSL");
                return QByteArray();
            }
        }
    } while (!response.isEmpty());

    emit progressStep("Starting engine");
    process.write(engineCmd.toLatin1());
    process.waitForBytesWritten();

    do {
        response.clear();
        response.append(readReponse(process));
        if (!response.isEmpty()) {
            if (responseContains(response, "OpenSSL>")) {
                break;
            } else if (responseContains(response, "unavailable")) {
                emit error("Unable to start PKCS#11 OpenSSL Engine");
                return QByteArray();
            }
        }
    } while (!response.isEmpty());

    QThread::msleep(500);

    emit progressStep("Signing file");
    process.write(smimeCmd.toLatin1());
    process.waitForBytesWritten();

    do {
        response.clear();
        response.append(readReponse(process));
        if (!response.isEmpty()) {
            if (responseContains(response, "PIN")) {
                break;
            } else if (responseContains(response, "error in cms")) {
                emit error("Unable to sign file");
                return QByteArray();
            }
        }
    } while (!response.isEmpty());

    process.write(pinCmd.toLatin1());
    process.waitForBytesWritten();

    do {
        response.clear();
        response.append(readReponse(process));
        if (!response.isEmpty()) {
            if (responseContains(response, "OpenSSL>")) {
                break;
            } else if (responseContains(response, "PIN")) {
                emit error("PIN NOT VALID");
                return QByteArray();
            }
        }
    } while (!response.isEmpty());

    QByteArray stdOut = process.readAllStandardOutput();
    QByteArray stdErr = process.readAllStandardError();

    process.terminate();
    process.waitForFinished();

    emit progressStep("Reading output");
    outFile.open();
    QByteArray outputData = outFile.readAll();
    outFile.close();

    crtFile.remove();
    inFile.remove();
    outFile.remove();

    return outputData;
}

QString DigiSigner::getCertId() {
    return "";
}

QString DigiSigner::getCert(QString id) {
    return "";
}

QStringList DigiSigner::readReponse(QProcess &process) {
    QByteArray rawData;
    QByteArrayList lines;
    QStringList response;

    process.setReadChannel(QProcess::StandardOutput);
    process.waitForReadyRead(250);
    rawData = process.readAll();
    rawData.replace('\r', '\n');
    rawData.replace("\n\n", "\n");
    lines.append(rawData.split('\n'));

    process.setReadChannel(QProcess::StandardError);
    process.waitForReadyRead(250);
    rawData = process.readAll();
    rawData.replace('\r', '\n');
    rawData.replace("\n\n", "\n");
    lines.append(rawData.split('\n'));

    for (const QByteArray &line: lines)
        response.append(QString(line));

    return response;
}

bool DigiSigner::responseContains(QStringList response, QString text) {
    for (const QString &item: response)
        if (item.contains(text))
            return true;

    return false;
}
