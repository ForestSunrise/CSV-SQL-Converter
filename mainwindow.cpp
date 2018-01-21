#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect (ui->radioButton_1, SIGNAL(clicked(bool)), this, SLOT(SqlCsv(bool)));
    connect (ui->radioButton_2, SIGNAL(clicked(bool)), this, SLOT(CsvSql(bool)));
    connect (ui->pushButton_1, SIGNAL(clicked(bool)), this, SLOT(ChooseLoadFile()));
    connect (ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(ChooseSaveFile()));
    connect (ui->pushButton_0, SIGNAL(clicked(bool)), this, SLOT(Convertation()));
    connect (ui->lineEdit_1, SIGNAL(editingFinished()),this,SLOT(GetTableName()));
    ui->tableView->setModel(&TheTable);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SqlCsv(bool check)
{
    if (check)
    {
        ConvDir=0;
        if (TableNameFile=="")
        {
            ui->pushButton_1->setEnabled(false);
        }
        else
        {
            ui->pushButton_1->setEnabled(true);
        }
    }
}

void MainWindow::CsvSql(bool check)
{
    if (check)
    {
        ConvDir=1;
        ui->pushButton_1->setEnabled(true);
    }
}

void MainWindow::ChooseLoadFile()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Открыть файл"),QDir::currentPath(),"*.db *.csv");
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Невозможно открыть файл"));
            return;
        }
        file.close();
    }
    else return;
    if (ConvDir)
    {
        if (filename.contains(".csv"))
        {
            LoadFile=filename;
            TheTable.LoadCsv(LoadFile);
        }
        else
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Неверный формат файла"));
            return;
        }
    }
    else
    {
        if (filename.contains(".db"))
        {
            LoadFile=filename;
            TheTable.LoadSql(LoadFile,TableNameFile);
        }
        else
        {
            QMessageBox::critical(this,tr("Ошибка"),tr("Неверный формат файла"));
            return;
        }
    }
    ui->label_01->setText(filename);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    if (!SaveFile.isEmpty() & !TableNameFile.isEmpty())
    {
        ui->pushButton_0->setEnabled(true);
    }
}

void MainWindow::ChooseSaveFile()
{
    QString filename=QFileDialog::getSaveFileName(this, tr("Сохранить файл"),QDir::currentPath());
    if (!filename.isEmpty())
    {
        if (ConvDir)
        {
            if (!filename.contains(".db"))
            {
                filename+=".db";
            }
        }
        else
        {
            if (!filename.contains(".csv"))
            {
                filename+=".csv";
            }
        }
        ui->label_02->setText(filename);
        SaveFile=filename;
        if (!LoadFile.isEmpty() & !TableNameFile.isEmpty())
        {
            ui->pushButton_0->setEnabled(true);
        }
    }
}

void MainWindow::GetTableName()
{
    TableNameFile=ui->lineEdit_1->text();
    ui->pushButton_1->setEnabled(true);
    if (!LoadFile.isEmpty() & !SaveFile.isEmpty())
    {
        ui->pushButton_0->setEnabled(true);
    }
    if (!LoadFile.isEmpty() & (ConvDir==false))
    {
        ui->pushButton_0->setEnabled(false);
        ui->label_01->setText("");
        LoadFile="";
    }
}

void MainWindow::Convertation()
{
    if (ConvDir)
    {
        TheTable.SaveSql(SaveFile,TableNameFile);
    }
    else
    {
        TheTable.SaveCsv(SaveFile);
    }
}
