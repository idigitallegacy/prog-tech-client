#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QPushButton>

#include "popupwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *next_window, QWidget *parent = nullptr);
    QPushButton *btn1;
    ~MainWindow();
    QLabel *heading;
    QLabel *description;
    QLabel *instruction;
    QLabel *description3;
    QPushButton *accept_button;
    bool connectionEstablished = false;

private slots:
    void managerFinished(QNetworkReply *reply);
    void run_app();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QMainWindow *next_window;
    PopupWindow *popUp;

    void setup_dark_ui();
    void draw_graph();
};
#endif // MAINWINDOW_H
