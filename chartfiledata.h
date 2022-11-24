#ifndef CHARTFILEDATA_H
#define CHARTFILEDATA_H


#include <QtSql>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QDataStream>
#include <QFile>

#include <QList>
#include <QDataStream>
#include <QPair>
#include <QString>
#include <QPointF>
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;


class IChartFileData
{
public:
    virtual DataTable getData(QString path) = 0;
};

class ChartFileDataSqlite : public IChartFileData
{
public:
    DataTable getData(QString path);
};

class ChartFileDataJson: public IChartFileData
{
public:
    DataTable getData(QString path);
};

#endif // CHARTFILEDATA_H
