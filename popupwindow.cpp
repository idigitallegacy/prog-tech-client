#include "popupwindow.h"
#include "ui_popupwindow.h"

#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPainter>

PopupWindow::PopupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopupWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint |        // Отключаем оформление окна
                   Qt::Tool |                       // Отменяем показ в качестве отдельного окна
                   Qt::WindowStaysOnTopHint);       // Устанавливаем поверх всех окон
    setAttribute(Qt::WA_TranslucentBackground);     // Указываем, что фон будет прозрачным
    setAttribute(Qt::WA_ShowWithoutActivating);     // При показе, виджет не получается фокуса автоматически

    QFont defaultFont = this->font();
    defaultFont.setFamily("Cantarell");
    defaultFont.setPixelSize(12);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    setFont(defaultFont);

    animation.setTargetObject(this);                // Устанавливаем целевой объект анимации
    animation.setPropertyName("popupOpacity");      // Устанавливаем анимируемое свойство
    connect(&animation, &QAbstractAnimation::finished, this, &PopupWindow::hide); /* Подключаем сигнал окончания
                                                                             * анимации к слоты скрытия
                                                                             * */

    QVBoxLayout *vlay = new QVBoxLayout();
    vlay->addStretch();

    imageWidget.setStyleSheet("QLabel {"
                              "margin-left: 20px;"
                              "margin-right: 20px;"
                              "}");
    layout.addWidget(&imageWidget, 0, 0);

    // Настройка текста уведомления
    title.setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Устанавливаем по центру
    // И настраиваем стили
    title.setStyleSheet("QLabel { color : white;"
                        "font-size: 14px;"
                        "font-weight: 500;"
                        "font-family: \"Cantarell\", sans-serif;"
                        " }");

    // Производим установку текста в размещение, ...
    title.setFixedWidth(400);
    title.setWordWrap(true);
    vlay->addWidget(&title);

    // Настройка текста уведомления
    label.setAlignment(Qt::AlignLeft | Qt::AlignVCenter); // Устанавливаем по центру
    // И настраиваем стили
    label.setStyleSheet("QLabel { color : white; "
                        "font-size: 12px;"
                        "font-weight: 400;"
                        "font-family: \"Cantarell\", sans-serif;"
                        "}");

    // Производим установку текста в размещение, ...
    label.setFixedWidth(400);
    label.setWordWrap(true);
    vlay->addWidget(&label);
    vlay->addStretch();

    layout.addLayout(vlay, 0, 1);

    layout.setAlignment(&imageWidget, Qt::AlignVCenter);
    layout.setAlignment(vlay, Qt::AlignVCenter);

    setLayout(&layout); // которое помещаем в виджет

    // По сигналу таймера будет произведено скрытие уведомления, если оно видимо
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopupWindow::hideAnimation);
}

PopupWindow::~PopupWindow()
{
    delete ui;
}

void PopupWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    /* А теперь настраиваем фон уведомления,
     * который является прямоугольником с чёрным фоном
     * */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Включаем сглаживание

    // Подготавливаем фон. rect() возвращает внутреннюю геометрию виджета уведомления, по содержимому
    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    // Кисть настраиваем на чёрный цвет в режиме полупрозрачности 180 из 255
    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen); // Край уведомления не будет выделен

    // Отрисовываем фон с закруглением краёв в 5px
    painter.drawRoundedRect(roundedRect, 5, 5);
}

void PopupWindow::setPopupLogo(const QString &resource_path)
{
    logo = QPixmap(resource_path);

    QPixmap *pix = new QPixmap(40,40);
    pix->fill(Qt::transparent);
    QPainter *p = new QPainter(pix);
    p->setRenderHint(QPainter::Antialiasing,true);
    p->setRenderHint(QPainter::SmoothPixmapTransform,true);
    p->drawPixmap(QRect(0, 0, 40, 40), logo, logo.rect());
    imageWidget.setPixmap(*pix);
    adjustSize();           // С пересчётом размеров уведомления
}

void PopupWindow::setPopupText(const QString &text)
{
    label.setText(text);    // Устанавливаем текст в Label
    adjustSize();           // С пересчётом размеров уведомления
}

void PopupWindow::setPopupTitle(const QString &text)
{
    title.setText(text);    // Устанавливаем текст в Label
    adjustSize();           // С пересчётом размеров уведомления
}

void PopupWindow::show()
{
    setWindowOpacity(0.0);  // Устанавливаем прозрачность в ноль

    animation.setDuration(150);     // Настраиваем длительность анимации
    animation.setStartValue(0.0);   // Стартовое значение будет 0 (полностью прозрачный виджет)
    animation.setEndValue(1.0);     // Конечное - полностью непрозрачный виджет

    setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x(),
                QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                width(),
                height());
    QWidget::show();                // Отображаем виджет, который полностью прозрачен

    animation.start();              // И запускаем анимацию
    timer->start(100 * (title.text().size() + label.text().size()));             // А также стартуем таймер, который запустит скрытие уведомления через 3 секунды
}

void PopupWindow::hideAnimation()
{
    timer->stop();                  // Останавливаем таймер
    animation.setDuration(1000);    // Настраиваем длительность анимации
    animation.setStartValue(1.0);   // Стартовое значение будет 1 (полностью непрозрачный виджет)
    animation.setEndValue(0.0);     // Конечное - полностью прозрачный виджет
    animation.start();              // И запускаем анимацию
}

void PopupWindow::hide()
{
    // Если виджет прозрачный, то скрываем его
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void PopupWindow::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float PopupWindow::getPopupOpacity() const
{
    return popupOpacity;
}
