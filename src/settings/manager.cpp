#include <QtCore/QSettings>
#include <utility.hpp>

#include "manager.hpp"
#include "fdosettings.hpp"

void SettingsManager::load() {
    FDOSettings *fdoSettings = FDOSettings::getInstance();
    QSettings settings;

    settings.beginGroup("tools");
    fdoSettings->setPkcsToolBin(settings.value("pkcsToolBin", DiscoverUtility::discoverPkcsToolBin()).toString());
    fdoSettings->setPkcsEngineLib(settings.value("pkcsEngineLib", DiscoverUtility::discoverPkcsEngineLib()).toString());
    fdoSettings->setOpensslBin(settings.value("opensslBin", DiscoverUtility::discoverOpenSSLBin()).toString());
    settings.endGroup();

    settings.beginGroup("smartcard");
    fdoSettings->setSmartcardProducer(settings.value("smartcardProducer", "").toString());
    fdoSettings->setSmartcardLib(settings.value("smartcardLib", "").toString());
    settings.endGroup();

    settings.beginGroup("certificate");
    fdoSettings->setCertificateId(settings.value("certificateId", "").toString());
    settings.endGroup();
}

void SettingsManager::save() {
    FDOSettings *config = FDOSettings::getInstance();
    QSettings settings;

    settings.beginGroup("tools");
    settings.setValue("pkcsToolBin", config->getPkcsToolBin());
    settings.setValue("pkcsEngineLib", config->getPkcsEngineLib());
    settings.setValue("opensslBin", config->getOpensslBin());
    settings.endGroup();

    settings.beginGroup("smartcard");
    settings.setValue("smartcardProducer", config->getSmartcardProducer());
    settings.setValue("smartcardLib", config->getSmartcardLib());
    settings.endGroup();

    settings.beginGroup("certificate");
    settings.setValue("certificateId", config->getCertificateId());
    settings.endGroup();

    settings.sync();
}
