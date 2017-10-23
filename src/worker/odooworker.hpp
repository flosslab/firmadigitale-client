#ifndef __FDOTOOL_WORKER_ODOOWORKER_H
#define __FDOTOOL_WORKER_ODOOWORKER_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>

#include "../action.hpp"

#define FDOTOOL_ODOOWORKER_PROGRESS_MIN_STEPS 10

#define FDOTOOL_ODOOWORKER_API_BOOTSTRAP "/fdo/1/action/bootstrap"
#define FDOTOOL_ODOOWORKER_API_GETJOB "/fdo/1/action/getJob"
#define FDOTOOL_ODOOWORKER_API_GETATTACHMENT "/fdo/1/action/getAttachment"
#define FDOTOOL_ODOOWORKER_API_UPLOADSIGNED "/fdo/1/action/uploadSigned"

class OdooWorker : public QObject {
Q_OBJECT

public:

    explicit OdooWorker(QObject *parent = Q_NULLPTR);

    ~OdooWorker();

    bool doAction(Action action);

private:
    QString odooUrl;
    QString token;

    QNetworkAccessManager qNetworkAccessManager;

    QVariantMap jsonRpc(const QString &api, const QVariantMap &data);

    QByteArray doPost(const QUrl &url, const QByteArray &requestBody);

signals:

    void rpcError(QString title, QString message);

    void updateProgress(int value, int max);

    void updateStep(QString text);

    void updateAddress(QString address);

    void updateUser(QString user);

    void updateJobs(int jobs);

};

#endif
