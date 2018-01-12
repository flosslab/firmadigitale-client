#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include "config.hpp"
#include "utility.hpp"

QString DiscoverUtility::discoverPkcsToolBin() {
    QStringList searchPath;

#ifdef Q_OS_WIN32
    searchPath << R"(C:\Program Files (x86)\OpenSC Project\OpenSC\tools\pkcs11-tool.exe)";
    searchPath << R"(C:\Program Files\OpenSC Project\OpenSC\tools\pkcs11-tool.exe)";
#else
    QStringList args;
    args << "pkcs11-tool";

    QProcess process;
    process.start("which", args);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit)
        return "";

    QString rawOutput(process.readAll());
    QStringList lines = rawOutput.split('\n');
    for (const QString &line: lines)
        if (line.contains(FDOTOOL_TOOLS_PKCSTOOL_BIN_NAME))
            searchPath.append(line.trimmed());
#endif

    for (const QString &path: searchPath) {
        QFileInfo fileInfo(path);
        QString absolutePath = fileInfo.absoluteFilePath();
        if (isValid(absolutePath, FDOTOOL_TOOLS_PKCSTOOL_BIN_NAME))
            return absolutePath;
    }

    return "";
}

QString DiscoverUtility::discoverPkcsEngineLib() {
    QStringList searchPath;

#ifdef Q_OS_WIN32
    searchPath << R"(C:\Windows\SysWOW64\opensc-pkcs11.dll)";
    searchPath << R"(C:\Windows\System32\opensc-pkcs11.dll)";
#elif defined Q_OS_MAC
    searchPath << "opensc-pkcs11.dylib";
#else
    QStringList args;
    args << "-L" << "libengine-pkcs11-openssl";

    QProcess process;
    process.start("dpkg", args);
    process.waitForStarted();
    process.waitForFinished();
    if (process.exitStatus() != QProcess::NormalExit)
        return "";

    QString rawOutput(process.readAll());
    QStringList lines = rawOutput.split('\n');

    for (const QString &line: lines)
        if (line.contains(FDOTOOL_TOOLS_PKCSENGINE_LIB_NAME))
            searchPath.append(line.trimmed());
#endif

    for (const QString &path: searchPath) {
        QFileInfo fileInfo(path);
        QString absolutePath = fileInfo.absoluteFilePath();
        if (isValid(absolutePath, FDOTOOL_TOOLS_PKCSENGINE_LIB_NAME))
            return absolutePath;
    }

    return "";
}

QString DiscoverUtility::discoverOpenSSLBin() {
    QStringList searchPath;

    QDir installDir(FDOTOOL_TOOLS_INSTALL_DIR);
    searchPath.append(installDir.absoluteFilePath(FDOTOOL_TOOLS_OPENSSL_BIN_NAME));

    for (const QString &path: searchPath) {
        QFileInfo fileInfo(path);
        QString absolutePath = fileInfo.absoluteFilePath();
        if (isValid(absolutePath, FDOTOOL_TOOLS_OPENSSL_BIN_NAME))
            return absolutePath;
    }

    return "";
}

QString DiscoverUtility::discoverSmartcardLibAthena() {
    QStringList searchPath;

#ifdef Q_OS_WIN32
    searchPath << R"(C:\Windows\SysWOW64\asepkcs.dll)";
    searchPath << R"(C:\Windows\System32\asepkcs.dll)";
#elif defined Q_OS_MAC
    QDir installDir(FDOTOOL_TOOLS_INSTALL_DIR);

    searchPath.append(installDir.absoluteFilePath(FDOTOOL_TOOLS_INSTALL_DIR));
#else
    QDir installDir(FDOTOOL_TOOLS_INSTALL_DIR);

    searchPath.append(installDir.absoluteFilePath(FDOTOOL_TOOLS_SMARTCARD_LIB_NAME));
#endif

    for (const QString &path: searchPath) {
        QFileInfo fileInfo(path);
        QString absolutePath = fileInfo.absoluteFilePath();
        if (isValid(absolutePath, FDOTOOL_TOOLS_SMARTCARD_LIB_NAME_ATHENA))
            return absolutePath;
    }

    return "";
}

QString DiscoverUtility::discoverSmartcardLibIncardOberthur() {
    QStringList searchPath;

#ifdef Q_OS_WIN32
    searchPath << R"(C:\Windows\SysWOW64\bit4xpki.dll)";
    searchPath << R"(C:\Windows\System32\bit4xpki.dll)";
#elif defined Q_OS_MAC
    QDir installDir(FDOTOOL_TOOLS_INSTALL_DIR);

    searchPath.append(installDir.absoluteFilePath(FDOTOOL_TOOLS_INSTALL_DIR));
#else
    QDir installDir(FDOTOOL_TOOLS_INSTALL_DIR);

    searchPath.append(installDir.absoluteFilePath(FDOTOOL_TOOLS_SMARTCARD_LIB_NAME));
#endif

    for (const QString &path: searchPath) {
        QFileInfo fileInfo(path);
        QString absolutePath = fileInfo.absoluteFilePath();
        if (isValid(absolutePath, FDOTOOL_TOOLS_SMARTCARD_LIB_NAME_INCARD_OBERTHUR))
            return absolutePath;
    }

    return "";
}

bool DiscoverUtility::isValid(const QString &path, const QString &name) {
    if (path.length() == 0)
        return false;

    QFileInfo fileInfo(path);

    if (!fileInfo.exists() || !fileInfo.isFile())
        return false;

    if (name.length() > 0) {
        QString libName = name;
        if (!name.startsWith("lib"))
            libName = QString("lib%1").arg(name);

        if (fileInfo.fileName() != name
            && fileInfo.fileName() != libName)
            return false;
    }

    return true;
}
