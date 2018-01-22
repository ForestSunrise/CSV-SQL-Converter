/*!
 * \file
 * \brief Заголовочный файл с описанием основного класса проекта.
 */
#ifndef DATATABLE_H
#define DATATABLE_H

#include <QObject>
#include <QList>
#include <QAbstractTableModel>

#include <QString>

/*!
 * \brief Класс DataTable.
 * Основной класс в этом проекте.
 */
class DataTable: public QAbstractTableModel
{
    Q_OBJECT

public:
    /*!
     * \brief Основной конструктор.
     * \param parent
     */
    DataTable(QObject *parent = nullptr);
    /*!
     * \brief Конструктор с параметрами.
     * \param ColName Имена столбцов
     * \param DataType Типы данных
     * \param Data Основные данные таблицы
     */
    DataTable(QList<QString> ColName, QList<QString> DataType,  QList<QList<QString>> Data);

    /*!
     * \brief Функция определения типа данных
     */
    void TypeId();
    /*!
     * \brief Чтение файла .csv
     * \param filename Имя файла
     */
    void LoadCsv(QString filename);
    /*!
     * \brief Запись в файл .csv
     * \param filename Имя файла
     */
    void SaveCsv(QString filename);
    /*!
     * \brief Чтение файла .db
     * \param filename Имя файла
     * \param tablename Имя таблицы
     */
    void LoadSql(QString filename, QString tablename);
    /*!
     * \brief Запись в файл .db
     * \param filename Имя файла
     * \param tablename Имя таблицы
     */
    void SaveSql(QString filename, QString tablename);
    /*!
     * \brief Сравнение двух переменных класса DataTable
     * \param L Левая переменная
     * \param R Права переменная
     * \return Результат
     */
    friend bool operator==(const DataTable &L, const DataTable &R);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QList<QString> ColName;
    QList<QString> DataType;
    QList<QList<QString>> Data;
    QChar Delimeter = ';'; ///Разделитель столбцов в файле .csv
    int Cols = 0, Rows = 0;
};

#endif // DATATABLE_H
