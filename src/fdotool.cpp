#include <QtCore/QCommandLineParser>
#include <QtCore/QUrlQuery>
#include <QtConcurrent/QtConcurrent>
#include <window/configdialog.hpp>
#include <settings/manager.hpp>

#include "fdotool.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(APPLICATION_VERSION);
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);

    FDOTool fdoTool(argc, argv);
    fdoTool.parseCommandLine();
    return fdoTool.run();
}

FDOTool::FDOTool(int &argc, char **argv) : QApplication(argc, argv) {
    SettingsManager::load();
    SettingsManager::save();

    mainWindow = new MainWindow();
    processWindow = new ProcessWindow();

    mode = MAIN;

    actions.clear();
}

FDOTool::~FDOTool() {
    delete mainWindow;
    delete processWindow;
}

void FDOTool::parseCommandLine() {
    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Tool per l'integrazione della firma digitale su Odoo"));
    parser.addHelpOption();
    parser.addVersionOption();

    QList<QCommandLineOption> commandLineOptions;
    parser.addOptions(commandLineOptions);

    parser.addPositionalArgument("url", tr("URL fornito da Odoo"));

    parser.process(*this);

    const QStringList options = parser.optionNames();
    const QStringList args = parser.positionalArguments();

    if (!options.empty()) {
        if (parser.isSet("h")) {
            parser.showHelp(1);
        } else if (parser.isSet("v")) {

        }
    }

    if (args.empty()) {
        mode = MAIN;
        return;
    }

    mode = ODOO;

    for (const auto &arg : args) {
        QStringList items = arg.split("?");

        QUrlQuery urlQuery(items.at(1));
        if (urlQuery.isEmpty()) {
            continue;
        }

        QString rawData = urlQuery.queryItemValue("action");

        QByteArray byteArray;
        byteArray.append(rawData);
        QString data = QByteArray::fromBase64(byteArray);

        QStringList elems = data.trimmed().split("|");
        Action action(elems.at(0), elems.at(1));
        actions.append(action);
    }
}

int FDOTool::run() {
    switch (mode) {

        case MAIN:
            mainWindow->show();

            connect(mainWindow, SIGNAL(showConfig()), this, SLOT(showConfig()));

            connect(this, SIGNAL(configUpdated()), mainWindow, SLOT(updateToolsValues()));
            connect(this, SIGNAL(configUpdated()), mainWindow, SLOT(updateSmartcardValues()));

            break;

        case ODOO:
            processWindow->show();

            auto *odooWorker = new OdooWorker(actions, this);

            connect(processWindow, SIGNAL(finished(int)), odooWorker, SLOT(quit()));

            connect(odooWorker, SIGNAL(finished()), odooWorker, SLOT(deleteLater()));

            connect(odooWorker, SIGNAL(workCompleted()), processWindow, SLOT(iconCompleted()));
            connect(odooWorker, SIGNAL(workCompleted()), this, SLOT(waitAndClose()));

            connect(odooWorker, SIGNAL(workError()), processWindow, SLOT(iconError()));

            connect(odooWorker, SIGNAL(rpcError(QString, QString)), processWindow, SLOT(rpcError(QString, QString)));

            connect(odooWorker, SIGNAL(updateProgress(int, int)), processWindow, SLOT(updateProgress(int, int)));
            connect(odooWorker, SIGNAL(updateStep(QString)), processWindow, SLOT(updateStep(QString)));

            connect(odooWorker, SIGNAL(updateAddress(QString)), processWindow, SLOT(updateAddress(QString)));
            connect(odooWorker, SIGNAL(updateUser(QString)), processWindow, SLOT(updateUser(QString)));
            connect(odooWorker, SIGNAL(updateJobs(int)), processWindow, SLOT(updateJobs(int)));

            processWindow->iconWorking();

            odooWorker->start();

            break;
    }

    return QApplication::exec();
}

void FDOTool::waitAndClose() {
    auto *timer = new QTimer();

    connect(timer, SIGNAL(timeout()), timer, SLOT(deleteLater()));
    connect(timer, SIGNAL(timeout()), this, SLOT(doClose()));

    timer->setInterval(2000);
    timer->setSingleShot(true);
    timer->start();
}

void FDOTool::doClose() {
    closeAllWindows();
    exit();
}

void FDOTool::showConfig() {
    ConfigDialog configDialog;
    configDialog.exec();

    emit configUpdated();
}
