#include <QTest>
#include "project_test.h"
#include "popupwindow.h"

Project_Test::Project_Test(QObject *parent) :
    QObject(parent)
{
    serverReached = false;
    manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(managerFinished(QNetworkReply *reply)));
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply)
    {
      managerFinished(reply);
    });

    request.setUrl(QUrl("http://127.0.0.1:25000/test_connection"));
    manager->get(request);

    while (!serverReached) {
        QTest::qWait(100);
    }
}

void Project_Test::managerFinished(QNetworkReply *reply)
{
    if (reply->error())
        serverAvailable = false;
    else
        serverAvailable = true;
    serverReached = true;
}

void Project_Test::test_popup() {
    PopupWindow popupWindow;
    popupWindow.setPopupTitle("asdf");
    QCOMPARE(popupWindow.title.text(), "asdf");

    popupWindow.setPopupTitle("sda");
    QCOMPARE(popupWindow.title.text(), "sda");

    popupWindow.setPopupText("qaz");
    QCOMPARE(popupWindow.label.text(), "qaz");

    popupWindow.setPopupText("wsx");
    QCOMPARE(popupWindow.label.text(), "wsx");

    popupWindow.setPopupText("qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-=");
    QCOMPARE(popupWindow.label.text(), "qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-= qwertyuiopasdfghjkl;'zxcvbnm,./1234567890-=");
}

void Project_Test::test_mainwindow() {
    ListenerWindow lw;
    MainWindow w(&lw);

    QCOMPARE(w.heading->text(), "DotaParser client v. 1.0");

    QCOMPARE(w.palette().color(QPalette::Window), QColor(51, 51, 51));
    QCOMPARE(w.palette().color(QPalette::WindowText), Qt::white);
    QCOMPARE(w.palette().color(QPalette::Base), QColor(25, 25, 25));
    QCOMPARE(w.palette().color(QPalette::AlternateBase), QColor(51, 51, 51));
    QCOMPARE(w.palette().color(QPalette::ToolTipBase), Qt::white);
    QCOMPARE(w.palette().color(QPalette::ToolTipText), Qt::white);
    QCOMPARE(w.palette().color(QPalette::Text), Qt::white);
    QCOMPARE(w.palette().color(QPalette::Button), QColor(51, 51, 51));
    QCOMPARE(w.palette().color(QPalette::ButtonText), Qt::white);
    QCOMPARE(w.palette().color(QPalette::BrightText), Qt::red);
    QCOMPARE(w.palette().color(QPalette::Link), QColor(42, 130, 218));
    QCOMPARE(w.palette().color(QPalette::Highlight), QColor(42, 130, 218));
    QCOMPARE(w.palette().color(QPalette::HighlightedText), Qt::black);

    if (serverAvailable) {
        QTest::mouseClick(w.accept_button, Qt::LeftButton);
        lw.close();
        QTest::qWait(500);
        QCOMPARE(w.connectionEstablished, true);
    } else {
        QTest::mouseClick(w.accept_button, Qt::LeftButton);
        lw.close();
        QTest::qWait(5000);
        QCOMPARE(w.connectionEstablished, false);
    }
}

void Project_Test::test_listenerwindow() {
    serverReached = false;
    request.setUrl(QUrl("http://127.0.0.1:25000/test_connection"));
    manager->get(request);

    while (!serverReached) {
        QTest::qWait(100);
    }


    ListenerWindow lw;
    MainWindow w(&lw);

    QTest::keyClicks(lw.steamid_input, "test");
    QTest::keyClicks(lw.gameid_input, "12345");
    QTest::mouseClick(lw.buttonLoadData, Qt::LeftButton);

    if (!serverAvailable) {
        QTest::qWait(5000);
        QCOMPARE(lw.connectionEstablished, false);
        return;
    }

    while (!lw.gameTimeLoaded)
        QTest::qWait(100);
    QCOMPARE(lw.game_time_values[0].toString().toInt(), 10);

    while(!lw.netWorthLoaded)
        QTest::qWait(100);
    QCOMPARE(lw.net_worth_values[0].toString().toInt(), 600);

    while(!lw.gpmLoaded)
        QTest::qWait(100);
    QCOMPARE(lw.gpm_values[0].toString().toInt(), 0);

    while(!lw.xpmLoaded)
        QTest::qWait(100);
    QCOMPARE(lw.xpm_values[0].toString().toInt(), 0);

    while(!lw.kdaLoaded)
        QTest::qWait(100);
    QJsonArray keyData = lw.kda_values["kills"].toArray();
    QCOMPARE(keyData[0].toString().toInt(), 0);
    keyData = lw.kda_values["deaths"].toArray();
    QCOMPARE(keyData[0].toString().toInt(), 0);
    keyData = lw.kda_values["assists"].toArray();
    QCOMPARE(keyData[0].toString().toInt(), 0);
}
