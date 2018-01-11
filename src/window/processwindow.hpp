#ifndef __FDOTOOL_WINDOW_PROCESSWINDOW_H
#define __FDOTOOL_WINDOW_PROCESSWINDOW_H

#include <QDialog>
#include <QtSvg/QSvgWidget>

namespace Ui {
    class ProcessWindow;
}

class ProcessWindow : public QDialog {
Q_OBJECT

public:
    explicit ProcessWindow(QWidget *parent = nullptr);

    ~ProcessWindow();

public slots:

    void rpcError(QString title, QString message);

    void updateProgress(int value, int max);

    void updateStep(QString text = "");

    void updateAddress(QString address = "");

    void updateUser(QString user = "");

    void updateJobs(int jobs = -1);

    void iconWorking();

    void iconCompleted();

    void iconError();

protected:

    void closeEvent(QCloseEvent *event) override;

private:

    Ui::ProcessWindow *ui;

    QSvgWidget *svgWidget;

    void signalConnect();

    QString getPinFromUser();

private slots:

    void handleClose();

signals:

    void windowClose();

    void waitAndClose();
};

#endif
