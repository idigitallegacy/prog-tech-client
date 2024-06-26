#include "mainwindow.h"
#include "listenerwindow.h"

#include <QApplication>
#include <QLabel>
#include <QStyleFactory>
#include <QDebug>
#include <QTest>

#include "project_test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QTest::qExec(new Project_Test, argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/fonts/Cantarell-Regular.ttf");

    ListenerWindow lw;
    MainWindow w(&lw);
    w.show();
    return a.exec();
}
