#ifndef __FDOTOOL_SIGNER_DIGISIGNER_H
#define __FDOTOOL_SIGNER_DIGISIGNER_H

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <fdosettings.hpp>
#include "certificate.hpp"

#define FDOTOOL_SIGNER_DIGISIGNER_TEMFILE_PREFIX "tmpdigisigner"

class DigiSigner : public QObject {
Q_OBJECT

public:

    explicit DigiSigner(QObject *parent = Q_NULLPTR);

    bool initCard();

    void setPin(const QString &pin);

    QByteArray cadesSign(const QByteArray &inputData);

    QString getCertId();

    QString getCert(QString id);

private:

    FDOSettings *settings;

    QString certificateId;
    QString certificate;
    QString pin;

    static QStringList readReponse(QProcess &process);

    static bool responseContains(QStringList response, QString text);

signals:

    void progressStep(QString message);

    void error(QString message);
};

#endif
