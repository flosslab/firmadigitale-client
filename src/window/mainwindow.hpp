#ifndef __FDOTOOL_WINDOW_MAINWINDOW_H
#define __FDOTOOL_WINDOW_MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QThread>
#include <certificate.hpp>

#include "fdosettings.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:

    void updateSmartcardValues();

    void updateToolsValues();

protected:

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    FDOSettings *settings;

    QThread *thread;
    CertificateUtility *certificateUtility;

    void updateLineEdit(QLineEdit *lineEdit, const QString &text = "", bool error = false);

    void initLineEdits();

private slots:

    void handleNewCertificateId(QString certificateId);

signals:

    void windowClose();

    void showConfig();
};

#endif
