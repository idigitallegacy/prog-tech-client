#ifndef PROJECT_TEST_H
#define PROJECT_TEST_H

#include <QObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include "listenerwindow.h"
#include "mainwindow.h"


class Project_Test : public QObject
{
    Q_OBJECT
public:
    explicit Project_Test(QObject *parent = 0);

private slots: // должны быть приватными
    void test_popup();
    void test_mainwindow();
    void test_listenerwindow();
    void managerFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    bool serverAvailable;
    bool serverReached;
};

#endif // PROJECT_TEST_H
