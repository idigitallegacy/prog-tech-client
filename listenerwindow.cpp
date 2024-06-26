#include "listenerwindow.h"
#include "ui_listenerwindow.h"


ListenerWindow::ListenerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListenerWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    popUp = new PopupWindow();
    connectionEstablished = false;
    netWorthLoaded = false;
    gameTimeLoaded = false;
    gpmLoaded = false;
    xpmLoaded = false;
    kdaLoaded = false;

    QFont defaultFont = this->font();
    defaultFont.setFamily("Cantarell");
    defaultFont.setPixelSize(12);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    setFont(defaultFont);

    steamid_input = new QLineEdit();
    gameid_input = new QLineEdit();

    graph_color_primary = QList<QColor>{QColor(255, 102, 0, 150), QColor(0, 153, 255, 150), QColor(0, 119, 255, 150), QColor(164, 0, 255, 150)};
    graph_color_complementary = QList<QColor>{QColor(0, 153, 255, 150), QColor(255, 102, 0, 150), QColor(255, 136, 0, 150), QColor(89, 255, 0, 150)};
    graph_color_triadic1 = QList<QColor>{QColor(153, 255, 0, 150), QColor(102, 0, 255, 150), QColor(136, 0, 255, 150), QColor(255, 0, 89, 150)};
    graph_color_triadic2 = QList<QColor>{QColor(0, 255, 102, 150), QColor(255, 0, 153, 150), QColor(255, 0, 119, 150), QColor(255, 166, 0, 150)};

    graph_amount = 0;
    graph_row = 1;
    graph_col = 0;

    gameTimeManager = new QNetworkAccessManager();
    netWorthManager = new QNetworkAccessManager();
    gpmManager = new QNetworkAccessManager();
    xpmManager = new QNetworkAccessManager();
    kdaManager = new QNetworkAccessManager();

    netWorthGraph = new QCustomPlot();
    gpmGraph = new QCustomPlot();
    xpmGraph = new QCustomPlot();
    kdaGraph = new QCustomPlot();

    draw_controls();
    draw_empty_graphs();

    connect(gameTimeManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gameTimeManagerLoaded(QNetworkReply*)));
    connect(netWorthManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(netWorthManagerLoaded(QNetworkReply*)));
    connect(gpmManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gpmManagerLoaded(QNetworkReply*)));
    connect(xpmManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(xpmManagerLoaded(QNetworkReply*)));
    connect(kdaManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(kdaManagerLoaded(QNetworkReply*)));
}

ListenerWindow::~ListenerWindow()
{
    delete ui;

    delete steamid_input;
    delete gameid_input;

    delete gameTimeManager;
    delete netWorthManager;
    delete gpmManager;
    delete xpmManager;
    delete kdaManager;

    delete popUp;
}

void ListenerWindow::gameTimeManagerLoaded(QNetworkReply *reply)
{
    connectionEstablished = true;
    if (reply->error()) {
        connectionEstablished = false;
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Some error occured loading data: " + reply->errorString());
        popUp->show();
        return;
    }

    QString data = (QString)reply->readAll();

    if (data.size() == 0) {
        connectionEstablished = false;
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Error reading data. Empty response.");
        popUp->show();
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    game_time_values = jsonObject["values"].toArray();
    gameTimeLoaded = true;

    load_net_worth();
    load_gpm();
    load_xpm();
    load_kda();
}

void ListenerWindow::netWorthManagerLoaded(QNetworkReply *reply)
{
    if (reply->error()) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Some error occured loading data: " + reply->errorString());
        popUp->show();
        return;
    }

    QString data = (QString)reply->readAll();

    if (data.size() == 0) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Error reading data. Empty response.");
        popUp->show();
        return;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    net_worth_values = jsonObject["values"].toArray();
    netWorthLoaded = true;

    draw_net_worth_graph();
}

void ListenerWindow::gpmManagerLoaded(QNetworkReply *reply)
{
    if (reply->error()) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Some error occured loading data: " + reply->errorString());
        popUp->show();
        return;
    }

    QString data = (QString)reply->readAll();

    if (data.size() == 0) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Error reading data. Empty response.");
        popUp->show();
        return;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    gpm_values = jsonObject["values"].toArray();
    gpmLoaded = true;

    draw_gpm_graph();
}

void ListenerWindow::xpmManagerLoaded(QNetworkReply *reply)
{
    if (reply->error()) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Some error occured loading data: " + reply->errorString());
        popUp->show();
        return;
    }

    QString data = (QString)reply->readAll();

    if (data.size() == 0) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Error reading data. Empty response.");
        popUp->show();
        return;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    xpm_values = jsonObject["values"].toArray();
    xpmLoaded = true;

    draw_xpm_graph();
}

void ListenerWindow::kdaManagerLoaded(QNetworkReply *reply)
{
    if (reply->error()) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Some error occured loading data: " + reply->errorString());
        popUp->show();
        return;
    }

    QString data = (QString)reply->readAll();

    if (data.size() == 0) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Error reading data. Empty response.");
        popUp->show();
        return;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    kda_values = jsonResponse.object();
    kdaLoaded = true;

    draw_kda_graph();
}

void ListenerWindow::load_data()
{
    if (steamid_input->text().size() == 0 || gameid_input->text() == 0) {
        popUp->setPopupLogo(":/images/images/error.svg");
        popUp->setPopupTitle("Error");
        popUp->setPopupText("Type steam id and game id into corresponding fields.");
        popUp->show();
        return;
    }
    gameTimeRequest.setUrl(QUrl("http://127.0.0.1:25000/get_game_time?steamid=" + steamid_input->text() + "&gameid=" + gameid_input->text()));
    gameTimeManager->get(gameTimeRequest);
}

void ListenerWindow::load_net_worth()
{
    netWorthRequest.setUrl(QUrl("http://127.0.0.1:25000/get_net_worth?steamid=" + steamid_input->text() + "&gameid=" + gameid_input->text()));
    netWorthManager->get(netWorthRequest);
}

void ListenerWindow::load_gpm()
{
    gpmRequest.setUrl(QUrl("http://127.0.0.1:25000/get_gpm?steamid=" + steamid_input->text() + "&gameid=" + gameid_input->text()));
    gpmManager->get(gpmRequest);
}

void ListenerWindow::load_xpm()
{
    xpmRequest.setUrl(QUrl("http://127.0.0.1:25000/get_xpm?steamid=" + steamid_input->text() + "&gameid=" + gameid_input->text()));
    xpmManager->get(xpmRequest);
}

void ListenerWindow::load_kda()
{
    kdaRequest.setUrl(QUrl("http://127.0.0.1:25000/get_kda?steamid=" + steamid_input->text() + "&gameid=" + gameid_input->text()));
    kdaManager->get(kdaRequest);
}

void ListenerWindow::setup_dark_ui()
{
    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);
}

void ListenerWindow::draw_controls()
{
    QGridLayout *glay = ui->gridLayout;
    buttonLoadData = new QPushButton("Load data");
    buttonClearData = new QPushButton("Clear data");

    QLabel *steamid_label = new QLabel("Steam id:");
    QLabel *gameid_label = new QLabel("Game id:");

    QHBoxLayout *hlay1 = new QHBoxLayout();
    hlay1->addWidget(steamid_label);
    hlay1->addWidget(steamid_input);

    QHBoxLayout *hlay2 = new QHBoxLayout();
    hlay2->addWidget(gameid_label);
    hlay2->addWidget(gameid_input);

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);
    vlay->addWidget(buttonLoadData);

    connect(buttonLoadData, SIGNAL(clicked()), this, SLOT(load_data()));


    glay->addLayout(vlay, 0, 0, 1, 1);
    glay->addWidget(buttonClearData, 0, 1, 1, 1);

    glay->setRowStretch(0, 1);
    glay->setRowStretch(1, 1);

    glay->setColumnStretch(0, 1);
    glay->setColumnStretch(1, 1);
}

void ListenerWindow::draw_empty_graphs()
{
    netWorthGraph->addGraph();
    gpmGraph->addGraph();
    xpmGraph->addGraph();

    kdaGraph->addGraph();
    kdaGraph->addGraph();
    kdaGraph->addGraph();

    QColor color = graph_color_primary[0];
    netWorthGraph->graph()->setLineStyle(QCPGraph::lsLine);
    netWorthGraph->graph()->setPen(QPen(color.lighter(200)));
    netWorthGraph->graph()->setBrush(QBrush(color));
    setup_graph(netWorthGraph);
    netWorthGraph->graph(0)->setName("Net worth");
    netWorthGraph->xAxis->setLabel("Game time");
    netWorthGraph->yAxis->setLabel("Net worth");

    color = graph_color_primary[1];
    gpmGraph->graph()->setLineStyle(QCPGraph::lsLine);
    gpmGraph->graph()->setPen(QPen(color.lighter(200)));
    gpmGraph->graph()->setBrush(QBrush(color));
    setup_graph(gpmGraph);
    gpmGraph->graph(0)->setName("Gold per minute");
    gpmGraph->xAxis->setLabel("Game time");
    gpmGraph->yAxis->setLabel("GPM");

    color = graph_color_primary[2];
    xpmGraph->graph()->setLineStyle(QCPGraph::lsLine);
    xpmGraph->graph()->setPen(QPen(color.lighter(200)));
    xpmGraph->graph()->setBrush(QBrush(color));
    setup_graph(xpmGraph);
    xpmGraph->graph(0)->setName("Experience per minute");
    xpmGraph->xAxis->setLabel("Game time");
    xpmGraph->yAxis->setLabel("XPM");

    color = graph_color_primary[3];
    kdaGraph->graph(0)->setLineStyle(QCPGraph::lsLine);
    kdaGraph->graph(0)->setPen(QPen(color.lighter(200)));
    kdaGraph->graph(0)->setBrush(QBrush(color));
    kdaGraph->graph(0)->setName("Kills");
    color = graph_color_triadic1[3];
    kdaGraph->graph(1)->setLineStyle(QCPGraph::lsLine);
    kdaGraph->graph(1)->setPen(QPen(color.lighter(200)));
    kdaGraph->graph(1)->setBrush(QBrush(color));
    kdaGraph->graph(1)->setName("Deaths");
    color = graph_color_triadic2[3];
    kdaGraph->graph(2)->setLineStyle(QCPGraph::lsLine);
    kdaGraph->graph(2)->setPen(QPen(color.lighter(200)));
    kdaGraph->graph(2)->setBrush(QBrush(color));
    kdaGraph->graph(2)->setName("Assists");
    setup_graph(kdaGraph);

    kdaGraph->xAxis->setLabel("Game time");
    kdaGraph->yAxis->setLabel("KDA");
}

void ListenerWindow::setup_graph(QCustomPlot *customPlot)
{
    QGridLayout *glay = ui->gridLayout;
    customPlot->setBackground(QColor(0,0,0,0));
    glay->addWidget(customPlot,graph_row, graph_col,1,1);
    glay->setRowStretch(graph_row, 1);
    glay->setColumnStretch(graph_col, 1);

    // set a more compact font size for bottom and left axis tick labels:
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // change hints color
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->setBasePen(QPen(Qt::white));
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setTickPen(QPen(Qt::white));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white));

    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setBasePen(QPen(Qt::white));
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setTickPen(QPen(Qt::white));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white));

    customPlot->xAxis2->setTickLabelColor(Qt::white);
    customPlot->xAxis2->setBasePen(QPen(Qt::white));
    customPlot->xAxis2->setLabelColor(Qt::white);
    customPlot->xAxis2->setTickPen(QPen(Qt::white));
    customPlot->xAxis2->setSubTickPen(QPen(Qt::white));

    customPlot->yAxis2->setTickLabelColor(Qt::white);
    customPlot->yAxis2->setBasePen(QPen(Qt::white));
    customPlot->yAxis2->setLabelColor(Qt::white);
    customPlot->yAxis2->setTickPen(QPen(Qt::white));
    customPlot->yAxis2->setSubTickPen(QPen(Qt::white));

    // make top and right axes visible but without ticks and labels:
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);

    // show tick labels
    customPlot->yAxis->setTickLabels(true);

    // show legend with slightly transparent background brush:
    customPlot->legend->setVisible(true);
    customPlot->legend->setBrush(QColor(255, 255, 255, 150));


    ++graph_amount;
    ++graph_col;
    if (graph_col == 2) {
        graph_col = 0;
        ++graph_row;
    }
}

void ListenerWindow::draw_net_worth_graph()
{
    QVector<QCPGraphData> timeData(game_time_values.size());
    for(int i = 0; i < game_time_values.size(); ++i) {
        timeData[i].key = game_time_values[i].toString().toInt();
        timeData[i].value = net_worth_values[i].toString().toInt();
    }

    netWorthGraph->graph(0)->data()->set(timeData);

    // set axis ranges to show all data:
    netWorthGraph->xAxis->setRange(game_time_values[0].toString().toInt(), game_time_values[game_time_values.size() - 1].toString().toInt());

    int minNetWorth = 1e9;
    int maxNetWorth = 0;

    foreach(const QJsonValue &netWorth, net_worth_values) {
        if (netWorth.toString().toInt() < minNetWorth)
            minNetWorth = netWorth.toString().toInt();
        if (netWorth.toString().toInt() > maxNetWorth)
            maxNetWorth = netWorth.toString().toInt();
    }
    netWorthGraph->yAxis->setRange(minNetWorth, maxNetWorth);
    netWorthGraph->replot();
}

void ListenerWindow::draw_gpm_graph()
{
    QVector<QCPGraphData> timeData(game_time_values.size());
    for(int i = 0; i < game_time_values.size(); ++i) {
        timeData[i].key = game_time_values[i].toString().toInt();
        timeData[i].value = gpm_values[i].toString().toInt();
    }

    gpmGraph->graph()->data()->set(timeData);
    gpmGraph->graph(0)->setName("Gold per minute");

    // set axis ranges to show all data:
    gpmGraph->xAxis->setRange(game_time_values[0].toString().toInt(), game_time_values[game_time_values.size() - 1].toString().toInt());

    int minGpm = 1e9;
    int maxGpm = 0;

    foreach(const QJsonValue &gpm, gpm_values) {
        if (gpm.toString().toInt() < minGpm)
            minGpm = gpm.toString().toInt();
        if (gpm.toString().toInt() > maxGpm)
            maxGpm = gpm.toString().toInt();
    }
    gpmGraph->yAxis->setRange(minGpm, maxGpm);
    gpmGraph->replot();
}

void ListenerWindow::draw_xpm_graph()
{
    QVector<QCPGraphData> timeData(game_time_values.size());
    for(int i = 0; i < game_time_values.size(); ++i) {
        timeData[i].key = game_time_values[i].toString().toInt();
        timeData[i].value = xpm_values[i].toString().toInt();
    }

    xpmGraph->graph()->data()->set(timeData);
    xpmGraph->graph(0)->setName("Experience per minute");

    // set axis labels:
    xpmGraph->xAxis->setLabel("Game time");
    xpmGraph->yAxis->setLabel("XPM");

    // set axis ranges to show all data:
    xpmGraph->xAxis->setRange(game_time_values[0].toString().toInt(), game_time_values[game_time_values.size() - 1].toString().toInt());

    int minXpm = 1e9;
    int maxXpm = 0;

    foreach(const QJsonValue &xpm, xpm_values) {
        if (xpm.toString().toInt() < minXpm)
            minXpm = xpm.toString().toInt();
        if (xpm.toString().toInt() > maxXpm)
            maxXpm = xpm.toString().toInt();
    }
    xpmGraph->yAxis->setRange(minXpm, maxXpm);
    xpmGraph->replot();
}

void ListenerWindow::draw_kda_graph()
{
    QList<QString> keysList = QList<QString>{"kills", "deaths", "assists"};
    int minOfKda = 1e9;
    int maxOfKda = 0;

    for(int i = 0; i < keysList.size(); ++i) {
        QString key = keysList[i];

        QJsonArray keyData = kda_values[key].toArray();
        QVector<QCPGraphData> timeData(game_time_values.size());
        for(int j = 0; j < game_time_values.size(); ++j) {
            timeData[j].key = game_time_values[j].toString().toInt();
            timeData[j].value = keyData[j].toString().toInt();
        }
        kdaGraph->graph(i)->data()->set(timeData);
        kdaGraph->graph(i)->setName(key);

        foreach(const QJsonValue &kda, keyData) {
            if (kda.toString().toInt() < minOfKda)
                minOfKda = kda.toString().toInt();
            if (kda.toString().toInt() > maxOfKda)
                maxOfKda = kda.toString().toInt();
        }
    }

    // set axis ranges to show all data:
    kdaGraph->xAxis->setRange(game_time_values[0].toString().toInt(), game_time_values[game_time_values.size() - 1].toString().toInt());
    kdaGraph->yAxis->setRange(minOfKda, maxOfKda);
    kdaGraph->replot();
}
