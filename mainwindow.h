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
    bool ConvDir=0;
    QString LoadFile;
    QString SaveFile;
    QString TableNameFile;
    DataTable TheTable;
    //what else?

private slots:
    void SqlCsv(bool);
    void CsvSql(bool);
    void ChooseLoadFile();
    void ChooseSaveFile();
    void GetTableName();
    void Convertation();
};

#endif // MAINWINDOW_H
