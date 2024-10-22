#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "downloader.h"
#include "QListWidgetItem"
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QDateTime>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QListWidgetItem>
#include <QChart>
#include <QtCharts/QDateTimeAxis>
#include <QTimer>
#include <QSplitter>
#include <QMainWindow>
#include <wiringPi.h>

#ifdef _version5
using namespace QtCharts;
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct weatherDatapoint
{
    QString validTime;
    double values;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool on = false;

    QJsonArray forecastArray;

private:
    Ui::MainWindow *ui;
    Downloader *downloader;
    QMap<QString, QJsonValue> jsonValues;
    void showDatainList();
    void inigetForecastURL(const QString &url);
    struct weatherDatapoint
    {
        QDateTime validTime;
        //QDateTime endTime;
        //double quantitativePrecipitation;
        //QString windSpeed;
        double value;
        //double relativeHumidity;
        //QString shortForecast;
    };

    QJsonArray getForecastArray(const QString &url);
    QString forecastURL;
    QVector<weatherDatapoint> weatherData;


    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QTimer *timer;
    QTimer *timer1;
    QTimer *timer2;
    QTimer *valveCloseTimer;
    bool isRainExpected = false;

    void setupChart();
    void updateChartData(const QVector<QPair<QDateTime, double>> &data);

public slots:
    void enable_button();
    void show_json();
    void onlistchanged(QListWidgetItem *item);
    void process_forecast_data();
    void getForecastURL();
    void getWeatherPrediction();
    void plotForecast();
    void on_click();
    void Turnoff();
    void TurnOn();
    void closeWaterValve();

private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listWidget_contents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_ProcessJson_clicked();
    void on_comboBox_activated(int index);
    void updateWeatherData();
};
#endif
