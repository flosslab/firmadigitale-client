#ifndef __FDOTOOL_FDOTOOL_H
#define __FDOTOOL_FDOTOOL_H

#include <QApplication>
#include <QString>
#include <QList>

#include "version.hpp"
#include "action.hpp"

enum FDOMode {

    MAIN,
    ODOO
};

class FDOTool : public QApplication {
Q_OBJECT

public:

    explicit FDOTool(int &argc, char **argv);

    void parseCommandLine();

    int run();

    Q_INVOKABLE QString getPinFromUser();

private:
    FDOMode mode;
    QList<Action> actions;

    void runMain();

    void runOdoo();

private slots:


    void waitAndClose();

    void doClose();

    void showConfig();

signals:

    void configUpdated();
};

#endif
