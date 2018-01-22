/*!
 * \file
 * \brief Заголовочный файл с описанием главного окна проекта.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <datatable.h>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool ConvDir = 0; ///Направление конвертации
    QString LoadFile; ///Путь к загружаемому файлу
    QString SaveFile; ///Пусть к сохраняемому файлу
    QString TableNameFile; ///Имя таблицы
    DataTable TheTable; ///Переменная основного класса

private slots:
    /*!
     * \brief Нажатие радикнопки SQL->csv
     */
    void SqlCsv(bool);
    /*!
     * \brief Нажатие радиокнопки csv->SQL
     */
    void CsvSql(bool);
    /*!
     * \brief Выбор загружаемого файла
     */
    void ChooseLoadFile();
    /*!
     * \brief Выбор сохраняемого файла
     */
    void ChooseSaveFile();
    /*!
     * \brief Получение имени таблицы
     */
    void GetTableName();
    /*!
     * \brief Конвертация
     */
    void Convertation();
};

#endif // MAINWINDOW_H
