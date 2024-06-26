#ifndef LISTENERWINDOW_H
#define LISTENERWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#include "qcustomplot.h"
#include "popupwindow.h"

namespace Ui {
class ListenerWindow;
}

class ListenerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListenerWindow(QWidget *parent = nullptr);

    QLineEdit *steamid_input;
    QLineEdit *gameid_input;
    QPushButton *buttonLoadData;
    QPushButton *buttonClearData;

    bool connectionEstablished;
    bool gameTimeLoaded;
    bool netWorthLoaded;
    bool gpmLoaded;
    bool xpmLoaded;
    bool kdaLoaded;

    QJsonArray game_time_values;
    QJsonArray net_worth_values;
    QJsonArray gpm_values;
    QJsonArray xpm_values;
    QJsonObject kda_values;

    ~ListenerWindow();

private slots:
    void gameTimeManagerLoaded(QNetworkReply *reply);
    void netWorthManagerLoaded(QNetworkReply *reply);
    void gpmManagerLoaded(QNetworkReply *reply);
    void xpmManagerLoaded(QNetworkReply *reply);
    void kdaManagerLoaded(QNetworkReply *reply);
    void load_data();

private:
    Ui::ListenerWindow *ui;
    PopupWindow *popUp;

    QList<QColor> graph_color_primary;
    QList<QColor> graph_color_complementary;
    QList<QColor> graph_color_triadic1;
    QList<QColor> graph_color_triadic2;


    QNetworkAccessManager *gameTimeManager;
    QNetworkAccessManager *netWorthManager;
    QNetworkAccessManager *gpmManager;
    QNetworkAccessManager *xpmManager;
    QNetworkAccessManager *kdaManager;

    QNetworkRequest gameTimeRequest;
    QNetworkRequest netWorthRequest;
    QNetworkRequest gpmRequest;
    QNetworkRequest xpmRequest;
    QNetworkRequest kdaRequest;

    QJsonObject tmp_response;

    QCustomPlot *netWorthGraph;
    QCustomPlot *gpmGraph;
    QCustomPlot *xpmGraph;
    QCustomPlot *kdaGraph;

    const int target_total_graphs_amount = 6;
    int graph_amount;
    int graph_row;
    int graph_col;

    void setup_dark_ui();
    void draw_controls();
    void process_data(QString &data);

    void load_net_worth();
    void load_game_time();
    void load_gpm();
    void load_xpm();
    void load_kda();

    void setup_graph(QCustomPlot *customPlot);
    void draw_empty_graphs();
    void update_view();

    void draw_net_worth_graph();
    void draw_gpm_graph();
    void draw_xpm_graph();
    void draw_kda_graph();
};

#endif // LISTENERWINDOW_H
