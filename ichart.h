#ifndef ICHART_H
#define ICHART_H

#include "chartfiledata.h"
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QList>
#include <QDataStream>
#include <QPair>
#include <QString>
#include <QPointF>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

class IChart // класс-интерфейс для построения графиков различных типов
{
public:
    virtual QChart* getChart() = 0;//метод получения графика
    virtual void createChart(DataTable data, int count, QList <QColor> color) = 0;// метод построения графика
    virtual void reColor(QList <QColor>) = 0; //метод смены цвета графика
};

class BarChart: public IChart // класс-наследник для построения столбчатых графиков
{
private:
    QChart *chart;
    QBarSeries *series;
public:
    void createChart(DataTable data, int count, QList <QColor> color);
    void reColor(QList <QColor>);
    QChart* getChart() {
        return chart;
    };

};

class PieChart: public IChart // класс-наследник для построения круговых графиков
{
private:
    QChart *chart;
    QPieSeries *series;
public:
    void createChart(DataTable data, int count, QList <QColor> color);
    void reColor(QList <QColor>);
    QChart* getChart() {
        return chart;
    };

};

#endif // ICHART_H
