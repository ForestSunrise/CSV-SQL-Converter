#include <datatable.h>
#include <QString>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QtSql>
#include <QSqlRecord>
#include <QDebug>

DataTable::DataTable(QObject *parent): QAbstractTableModel(parent)
{
}

DataTable::DataTable(QList<QString> ColNameN, QList<QString> DataTypeN,  QList<QList<QString>> DataN)
{
    this->ColName=ColNameN;
    this->DataType=DataTypeN;
    this->Data=DataN;
    this->Cols=ColNameN.count();
    this->Rows=DataN.count();
}

void DataTable::TypeId()
{
    bool ok=false, f=true;
    for (int i=0;i<Rows;i++)
    {
        for (int j=0;j<Cols;j++)
        {
            if (f)
            {
                DataType.append("");
            }
            if (DataType[j]!="TEXT")
            {
                if (DataType[j]!="REAL")
                {
                    Data[i][j].toInt(&ok);
                    if (ok)
                    {
                        DataType[j]="INTEGER";
                    }
                }
                if (ok==false)
                {
                    Data[i][j].toDouble(&ok);
                    if (ok)
                    {
                        DataType[j]="REAL";
                    }
                    else
                    {
                        DataType[j]="TEXT";
                    }
                }
            }
            ok=false;
        }
        f=false;
    }
    return;
}

void DataTable::LoadCsv(QString filename)
{
    emit beginResetModel();
    if (Cols!=0)
    {
        Cols=0;
        Rows=0;
        ColName.clear();
        DataType.clear();
        Data.clear();
    }
    QFile file(filename);
    QString S,SS;
    QList<QString> W;
    int s;
    bool Q=false,QQ=false,N=true;
    if (file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        QTextStream input(&file);
        while (!input.atEnd())
        {
            S=input.readLine();
            s=S.count();
            W.clear();
            SS.clear();
            for (int i=0;i<s;i++)
            {
                if (S[i]=='"')
                {
                    if (Q)
                    {
                        if (QQ)
                        {
                            QQ=false;
                            SS=SS+S[i];
                        }
                        else
                        {
                            Q=false;
                        }
                    }
                    else
                    {
                        Q=true;
                        if (QQ)
                        {
                            SS=SS+S[i];
                        }
                        else
                        {
                            if (S[i+1]=='"')
                            {
                                QQ=true;
                                Q=false;
                            }
                        }
                    }
                }
                else
                {
                    if (S[i]==Delimeter)
                    {
                        if ((Q==false) & (QQ==false))
                        {
                            W.append(SS);
                            SS="";
                        }
                        if (Q)
                        {
                            SS=SS+S[i];
                        }
                    }
                    else
                    {
                        SS=SS+S[i];
                    }
                }
            }
            W.append(SS);
            if (N)
            {
                ColName.append(W);
                Cols=W.count();
                N=false;
            }
            else
            {
                Data.append(W);
                Rows++;
            }
        }
    }
    else
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно открыть файл"));
        return;
    }
    file.close();
    emit endResetModel();
    return;
}

void DataTable::SaveCsv(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream output(&file);
        for (int i=0;i<Cols;i++)
        {
            if (ColName[i].contains(Delimeter) || ColName[i].contains('"') || ColName[i].contains(',') || ColName[i].contains('\r') || ColName[i].contains('\n') || ColName[i].contains(';'))
            {
                output<<'"'<<ColName[i]<<'"';
            }
            else
            {
                output<<ColName[i];
            }
            if (i<Cols-1)
            {
                output<<Delimeter;
            }
        }
        output<<"\r\n";
        for (int i=0; i<Rows;i++)
        {
            for(int j=0;j<Cols;j++)
            {
                if (Data[i][j].contains(Delimeter) || Data[i][j].contains('"') || Data[i][j].contains(',') || Data[i][j].contains('\r') || Data[i][j].contains('\n') || Data[i][j].contains(';'))
                {
                    output<<'"'<<Data[i][j]<<'"';
                }
                else
                {
                    output<<Data[i][j];
                }
                if (j<Cols-1)
                {
                    output<<Delimeter;
                }
            }
            output<<"\r\n";
        }
    }
    else
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно открыть файл для записи"));
        return;
    }
    file.close();
    QMessageBox::information(NULL,tr("Уведомление"),tr("Конвертация успешно завершена!"));
    return;
}

void DataTable::LoadSql(QString filename, QString tablename)
{
    emit beginResetModel();
    if (Cols!=0)
    {
        Cols=0;
        Rows=0;
        ColName.clear();
        DataType.clear();
        Data.clear();
    }
    emit endResetModel();
    QSqlDatabase TheBase;
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        TheBase=QSqlDatabase::database();
    }
    else
    {
        TheBase=QSqlDatabase::addDatabase("QSQLITE");
        TheBase.setDatabaseName(filename);
    }
    if (!TheBase.open())
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно открыть файл базы"));
        return;
    }
    QSqlRecord record=TheBase.record(tablename);
    if (record.isEmpty())
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Таблица с таким именем не существует"));
        TheBase.close();
        return;
    }
    Cols=record.count();
    emit beginInsertColumns(QModelIndex(),0,Cols-1);
    for (int i=0;i<Cols;i++)
    {
        ColName.append(record.fieldName((i)));
    }
    emit endInsertColumns();
    QSqlQuery DataLoad1("SELECT count(*) FROM "+tablename);
    DataLoad1.last();
    Rows=DataLoad1.value(DataLoad1.at()).toInt();
    QList<QString> S;
    QSqlQuery DataLoad2("SELECT * FROM "+tablename);
    emit beginInsertRows(QModelIndex(),0,Rows-1);
    while (DataLoad2.next())
    {
        for (int i=0;i<Cols;i++)
        {
            S.append(DataLoad2.value(i).toString());
        }
        Data.append(S);
        S.clear();
    }
    emit endInsertRows();
    TheBase.close();
    return;
}

void DataTable::SaveSql(QString filename, QString tablename)
{
    QSqlDatabase TheBase;
    TheBase=QSqlDatabase::addDatabase("QSQLITE");
    TheBase.setDatabaseName(filename);
    if (!TheBase.open())
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно открыть файл базы для записи"));
        return;
    }
    QSqlRecord record=TheBase.record(tablename);
    QSqlQuery DataLoad;
    if (!record.isEmpty())
    {
        if (!DataLoad.exec("DROP TABLE '"+tablename+"'"))
        {
            QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно удалить таблицу"));
            return;
        }
    }
    TypeId();
    QString S;
    S="CREATE TABLE "+tablename+"(";
    for (int i=0;i<Cols-1;i++)
    {
        S=S+'`'+ColName[i]+'`'+" "+DataType[i]+", ";
    }
    S=S+'`'+ColName[Cols-1]+'`'+" "+DataType[Cols-1]+");";
    if (!DataLoad.exec(S))
    {
        QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно создать таблицу"));
        return;
    }
    for (int i=0; i<Rows;i++)
    {
        TheBase.transaction();
        S.clear();
        S="INSERT INTO "+tablename+" (";
        for (int j=0;j<Cols-1;j++)
        {
            S=S+'`'+ColName[j]+'`'+", ";
        }
        S=S+'`'+ColName[Cols-1]+'`'+")"+" VALUES (";
        for (int j=0;j<Cols-1;j++)
        {
            if (DataType[j]=="TEXT")
            {
                S=S+"'"+Data[i][j]+"', ";
            }
            else
            {
                 S=S+Data[i][j]+", ";
            }
        }
        if (DataType[Cols-1]=="TEXT")
        {
            S=S+"'"+Data[i][Cols-1]+"');";
        }
        else
        {
            S=S+Data[i][Cols-1]+");";
        }
        if (!DataLoad.exec(S))
        {
            QMessageBox::critical(NULL,tr("Ошибка"),tr("Невозможно провести вставку данных"));
            return;
        }
        TheBase.commit();
    }
    TheBase.close();
    QMessageBox::information(NULL,tr("Уведомление"),tr("Конвертация успешно завершена!"));
    return;
}

int DataTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return Rows;
}

int DataTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return Cols;
}

QVariant DataTable::data(const QModelIndex &index, int role) const
{
    if (role==Qt::DisplayRole && index.isValid())
    {
        return Data.at(index.row()).at(index.column());
    }
    return QVariant();
}

QVariant DataTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole && orientation==Qt::Horizontal)
    {
        return ColName[section];
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
