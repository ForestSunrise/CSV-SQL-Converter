#ifndef DATATABLE_H
#define DATATABLE_H

#include <QObject>
#include <QList>
#include <QAbstractTableModel>

//#include <QtSql>
//#include <QSqlRecord>
//#include <QFile>
#include <QString>

class DataTable:public QAbstractTableModel
{
    Q_OBJECT

public:
    DataTable(QObject *parent=nullptr);
    DataTable(QList<QString> ColName, QList<QString> DataType,  QList<QList<QString>> Data);

    void TypeId();
    void LoadCsv(QString filename);
    void SaveCsv(QString filename);
    void LoadSql(QString filename, QString tablename);
    void SaveSql(QString filename, QString tablename);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<QString> ColName;
    QList<QString> DataType;
    QList<QList<QString>> Data;
    QChar Delimeter=';';
    int Cols=0,Rows=0;
};

#endif // DATATABLE_H
