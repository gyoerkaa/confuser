#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::new_cmat_tab()
{
    //ui->mainTabWidget();

    //return newCMatTab
}

void MainWindow::on_actionCmatNew_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{
    QStringList filenameList = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "/",
                            "Comma-separated values (*.csv *.txt)");
    if (!filenameList.isEmpty())
    {
        //
    }
}
