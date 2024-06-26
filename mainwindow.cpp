#include <QLabel>
#include <QFont>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QMainWindow *next, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , next_window(next)
{
    ui->setupUi(this);

    QFont defaultFont = this->font();
    defaultFont.setFamily("Cantarell");
    defaultFont.setPixelSize(12);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    setFont(defaultFont);

    popUp = new PopupWindow();
    setup_dark_ui();

    QFont custom_font = QFont();
    custom_font.setWeight(28);

    QVBoxLayout *vlay = ui->verticalLayout;

    heading = new QLabel("DotaParser client v. 1.0");
    heading->setAlignment(Qt::AlignHCenter);
    heading->setStyleSheet("QLabel {"
                           "font-size: 20px;"
                           "font-weight: 700;"
                           "}");

    description = new QLabel(
                "This is my first application to show Dota GSI features.\n"
                "Since it's raw enough, you need to do the following steps:"
                );
    description->setAlignment(Qt::AlignHCenter);

    instruction = new QLabel(
                "- Open \"%dota2_directory%/game/dota/cfg/\" or \"%dota2_directory%/game/dota/cfg/gamestate_integration\" path;\n"
                "- Create a file named \"gamestate_integration_*.cfg\" there;\n"
                "- Edit it as stated here: https://github.com/xzion/dota2-gsi (\"uri\" \"http://localhost:25000/\");\n"
                "- Run a server part of DotaParser;\n"
                "- Click Accept button below.\n\n"
                ""
                "There are only few graphs I have made yet:\n"
                "- XPM graph;\n"
                "- GPM graph;\n"
                "- Net worth graph.; \n"
                "- KDA graph."
                );

    accept_button = new QPushButton("Accept");
    accept_button->setMaximumWidth(400);

    vlay->addStretch();
    vlay->addWidget(heading);
    vlay->addSpacing(20);
    vlay->addWidget(description);
    vlay->addSpacing(20);
    vlay->addWidget(instruction);
    vlay->addWidget(accept_button);
    vlay->addStretch();

    vlay->setAlignment(heading, Qt::AlignHCenter);
    vlay->setAlignment(description, Qt::AlignHCenter);
    vlay->setAlignment(instruction, Qt::AlignHCenter);
    vlay->setAlignment(accept_button, Qt::AlignHCenter);

    manager = new QNetworkAccessManager();
    connect(accept_button, SIGNAL(clicked()), this, SLOT(run_app()));

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(managerFinished(QNetworkReply *reply)));
    connect(manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply)
    {
      managerFinished(reply);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete popUp;
    delete heading;
    delete instruction;
}

void MainWindow::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Connection to server failed.");
        popUp->show();
        connectionEstablished = false;
        return;
    }

    popUp->setPopupLogo(":/images/images/success.svg");
    popUp->setPopupTitle("Success");
    popUp->setPopupText("Server connection successfully estsablished.");
    popUp->show();
    connectionEstablished = true;

    next_window->show();
    close();
}

void MainWindow::run_app()
{
    request.setUrl(QUrl("http://127.0.0.1:25000/test_connection"));
    manager->get(request);
}

void MainWindow::setup_dark_ui()
{
    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(51, 51, 51));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(51, 51, 51));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(51, 51, 51));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);
}
