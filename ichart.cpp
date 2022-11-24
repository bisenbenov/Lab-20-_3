#include "ichart.h"

void BarChart::createChart(DataTable m_dataTable, int count_Chart, QList <QColor> color)
{
    chart = new QChart();
    chart->setTitle("Bar chart");
    int count = 0;
    double chartOther = 0.0; // счетчик количества нарисованых столбцов для диаграммы
    series = new QBarSeries(chart);
    qDebug() << "count is " << m_dataTable.count() << "list "<< color.count();
    for (int i(0); i < m_dataTable.count(); i++) {
        for (const Data &data : m_dataTable[i])
        {

                QBarSet *set = new QBarSet(data.second);
                *set << data.first.x(); // запись данных в график
                set->setBrush(color.at(count));
                series->append(set);
            count++;
        }
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::SeriesAnimations);
}

void BarChart::reColor(QList <QColor> color)
{
    int i=0;
    foreach (QBarSet* x, series->barSets()){
        x->setBrush(color.at(i)); // закрашиваем график по уже установленным цветам
        i++;
    }
}

void PieChart::createChart(DataTable m_dataTable, int count_Chart, QList <QColor> color){
    chart = new QChart();
    chart->setTitle("Pie chart");
    int count = 0; // счетчик количества ячеек круговой диаграммы
    double chartOther = 0.0;
    qreal pieSize = 1.0 / m_dataTable.count(); // эта переменная определяет положение графиков (если он один - то по центру)
    for (int i = 0; i < m_dataTable.count(); i++) {// цикл по каждой из таблиц данных (мы рассматриваем одну)
        series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {
                QPieSlice *slice = series->append(data.second, data.first.x()); // запись данных в график
                if (data == m_dataTable[i].first()) {
                    slice->setLabelVisible();
                    slice->setExploded();
                }
                slice->setBrush(color.at(count)); // задание цвета
            count++;
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());
       //series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.6);
        chart->addSeries(series);
        chart->setAnimationOptions(QChart::SeriesAnimations); // анимация рисования
    }
}
void PieChart::reColor(QList <QColor> color){
    int i=0;
    foreach (QPieSlice* x, series->slices()){
        x->setBrush(color.at(i)); // закрашиваем график по уже установленным цветам
        i++;
    }
}
