#include <QtCore/QCommandLineParser>
#include <QtCore/QUrlQuery>
#include <QtConcurrent/QtConcurrent>
#include <window/configdialog.hpp>
#include <settings/manager.hpp>
#include <window/mainwindow.hpp>
#include <worker/odooworker.hpp>
#include <window/processwindow.hpp>
#include <QtWidgets/QInputDialog>

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

    mode = MAIN;

    actions.clear();
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
            runMain();
            break;

        case ODOO:
            runOdoo();
            break;
    }

    return QApplication::exec();
}

void FDOTool::runMain() {
    auto *window = new MainWindow();
    connect(window, SIGNAL(finished()), window, SLOT(deleteLater()));

    connect(window, SIGNAL(showConfig()), this, SLOT(showConfig()));

    connect(this, SIGNAL(configUpdated()), window, SLOT(updateToolsValues()));
    connect(this, SIGNAL(configUpdated()), window, SLOT(updateSmartcardValues()));

    window->show();
}

void FDOTool::runOdoo() {
    auto *window = new ProcessWindow();
    connect(window, SIGNAL(finished()), window, SLOT(deleteLater()));

    auto *worker = new OdooWorker(actions, this);
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    connect(window, SIGNAL(finished(int)), worker, SLOT(quit()));

    connect(worker, SIGNAL(workCompleted()), window, SLOT(iconCompleted()));
    connect(worker, SIGNAL(workCompleted()), this, SLOT(waitAndClose()));

    connect(worker, SIGNAL(workError()), window, SLOT(iconError()));

    connect(worker, SIGNAL(rpcError(QString, QString)), window, SLOT(rpcError(QString, QString)));

    connect(worker, SIGNAL(updateProgress(int, int)), window, SLOT(updateProgress(int, int)));
    connect(worker, SIGNAL(updateStep(QString)), window, SLOT(updateStep(QString)));

    connect(worker, SIGNAL(updateAddress(QString)), window, SLOT(updateAddress(QString)));
    connect(worker, SIGNAL(updateUser(QString)), window, SLOT(updateUser(QString)));
    connect(worker, SIGNAL(updateJobs(int)), window, SLOT(updateJobs(int)));

    window->iconWorking();
    window->show();

    worker->start();
}

QString FDOTool::getPinFromUser() {
    QInputDialog inputDialog;
    inputDialog.setWindowModality(Qt::ApplicationModal);
    inputDialog.setInputMode(QInputDialog::TextInput);
    inputDialog.setTextEchoMode(QLineEdit::Password);
    inputDialog.setWindowTitle(tr("Smart Card PIN"));
    inputDialog.setLabelText(tr("Please enter your PIN number"));
    inputDialog.exec();

    QString pin = inputDialog.textValue();

    return pin;
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
