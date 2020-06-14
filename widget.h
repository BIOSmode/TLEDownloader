#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineView>
#include <QtNetwork>
#include <QtCore>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct DateTime
{
    int Y;
    int M;
    int D;
    int h;
    int m;
    double s;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_Iridium_clicked();

    void on_IridiumNext_clicked();

    void on_UpdateTLE_clicked();

    void timerUpdate();

private:
    Ui::Widget *ui;

    QString CalculateTimeVSTLE(QString line);

    DateTime days2ymdhms(int y,double d);

    int fix(double a);

    DateTime UTC2BJT(DateTime U);
};
#endif // WIDGET_H
