#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exportoptionsdialog.h"
#include <QFileDialog>
#include "confmatwidget.h"

#define MAX_CMAT_SIZE 128
#define MIN_CMAT_SIZE 3


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


ConfMatWidget* MainWindow::getActiveConfMat()
{
    QWidget* activeTab = ui->cmatTabWidget->currentWidget();
    return activeTab->findChild<ConfMatWidget*>();
}


void MainWindow::on_actionCmatNew_triggered()
{

}


void MainWindow::on_actionCmatShrink_triggered()
{
    QWidget* activeTab = ui->cmatTabWidget->currentWidget();
    QTableWidget* activeCmat = activeTab->findChild<QTableWidget*>();

    if (activeCmat->rowCount() > MIN_CMAT_SIZE)
    {
        activeCmat->setRowCount(activeCmat->rowCount()-1);
    }
    else
    {
        // TODO: Add an error message
    }

    if (activeCmat->columnCount() > MIN_CMAT_SIZE)
    {
        activeCmat->setColumnCount(activeCmat->columnCount()-1);
    }
    else
    {
        // TODO: Add an error message
    }
}

void MainWindow::on_actionCmatExpand_triggered()
{
    QWidget* activeTab = ui->cmatTabWidget->currentWidget();
    QTableWidget* activeCmat = activeTab->findChild<QTableWidget*>();

    if (activeCmat->rowCount() < MAX_CMAT_SIZE)
    {
        activeCmat->setRowCount(activeCmat->rowCount()+1);
    }
    if (activeCmat->columnCount() < MAX_CMAT_SIZE)
    {
        activeCmat->setColumnCount(activeCmat->columnCount()+1);
    }

}

void MainWindow::on_actionOpenFile_triggered()
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

void MainWindow::on_actionEditExportOptions_triggered()
{
    ExportOptionsDialog eoDialog;

    eoDialog.setModal(true);
    eoDialog.exec();
}
