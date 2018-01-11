#include "fdosettings.hpp"
#include "defaults.hpp"

FDOSettings *FDOSettings::instance = nullptr;

FDOSettings *FDOSettings::getInstance() {
    if (instance == nullptr)
        instance = new FDOSettings();

    return instance;
}

FDOSettings::FDOSettings() {
    pkcsToolBin = SETTINGS_PKCSTOOL_BIN_DEFAULT;
    pkcsEngineLib = SETTINGS_PKCSENGINE_LIB_DEFAULT;
    opensslBin = SETTINGS_OPENSSL_BIN_DEFAULT;
    smartcardProducer = SETTINGS_SMARTCARD_PRODUCER_DEFAULT;
    smartcardLib = SETTINGS_SMARTCARD_LIB_DEFAULT;
    certificateId = SETTINGS_CERTIFICATE_ID_DEFAULT;
}

const QString &FDOSettings::getPkcsToolBin() const {
    return pkcsToolBin;
}

void FDOSettings::setPkcsToolBin(const QString &pkcsToolBin) {
    FDOSettings::pkcsToolBin = pkcsToolBin;
}

const QString &FDOSettings::getPkcsEngineLib() const {
    return pkcsEngineLib;
}

void FDOSettings::setPkcsEngineLib(const QString &pkcsEngineLib) {
    FDOSettings::pkcsEngineLib = pkcsEngineLib;
}

const QString &FDOSettings::getOpensslBin() const {
    return opensslBin;
}

void FDOSettings::setOpensslBin(const QString &opensslBin) {
    FDOSettings::opensslBin = opensslBin;
}

const QString &FDOSettings::getSmartcardProducer() const {
    return smartcardProducer;
}

void FDOSettings::setSmartcardProducer(const QString &smartcardProducer) {
    FDOSettings::smartcardProducer = smartcardProducer;
}

const QString &FDOSettings::getSmartcardLib() const {
    return smartcardLib;
}

void FDOSettings::setSmartcardLib(const QString &smartcardLib) {
    FDOSettings::smartcardLib = smartcardLib;
}

const QString &FDOSettings::getCertificateId() const {
    return certificateId;
}

void FDOSettings::setCertificateId(const QString &certificateId) {
    FDOSettings::certificateId = certificateId;
}
