#ifndef __FDOTOOL_SETTINGS_SETTINGS_H
#define __FDOTOOL_SETTINGS_SETTINGS_H

#include <QtCore/QString>

class FDOSettings {

private:
    FDOSettings();

    static FDOSettings *instance;

    QString pkcsToolBin;
    QString pkcsEngineLib;
    QString opensslBin;
    QString smartcardProducer;
    QString smartcardLib;
    QString certificateId;

public:
    static FDOSettings *getInstance();

    const QString &getPkcsToolBin() const;

    void setPkcsToolBin(const QString &pkcsToolBin);

    const QString &getPkcsEngineLib() const;

    void setPkcsEngineLib(const QString &pkcsEngineLib);

    const QString &getOpensslBin() const;

    void setOpensslBin(const QString &opensslBin);

    const QString &getSmartcardProducer() const;

    void setSmartcardProducer(const QString &smartcardProducer);

    const QString &getSmartcardLib() const;

    void setSmartcardLib(const QString &smartcardLib);

    const QString &getCertificateId() const;

    void setCertificateId(const QString &certificateId);
};

#endif
