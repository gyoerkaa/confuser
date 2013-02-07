#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exportoptionsdialog.h"
#include "confmattab.h"
#include "mainsettings.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_confMatIdx = 0;
    m_mainSettings = new MainSettings;

    this->addConfMatTab();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_mainSettings;
}


void MainWindow::removeConfMatTab(ConfMatTab* confMatTab)
{
    if (confMatTab == NULL)
        return;

    ui->cmatTabWidget->removeTab(ui->cmatTabWidget->indexOf(confMatTab));
    delete confMatTab;

    // Make sure there is always 1 tab open
    if (ui->cmatTabWidget->count() <= 0)
    {
        this->addConfMatTab();
    }
}


ConfMatTab* MainWindow::addConfMatTab(int matSize, QString matName)
{
    ConfMatTab* newCMat = new ConfMatTab(matSize);
    ui->cmatTabWidget->addTab(newCMat, matName);
    return newCMat;
}


ConfMatTab* MainWindow::addConfMatTab()
{
    int matSize = 7; // TODO: Move into settings

    // Generate a name
    QString matName = "new " + QString::number(m_confMatIdx);
    m_confMatIdx++;

    return this->addConfMatTab(matSize, matName);
}


ConfMatTab* MainWindow::getActiveConfMatTab()
{
    return (ConfMatTab*)ui->cmatTabWidget->currentWidget();
}


void MainWindow::on_actionCmatNew_triggered()
{    
    ui->cmatTabWidget->setCurrentWidget(this->addConfMatTab());
}


void MainWindow::on_actionCmatShrink_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != NULL)
        activeCMatTab->setMatSize(activeCMatTab->getMatSize()-1);
}


void MainWindow::on_actionCmatExpand_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != NULL)
        activeCMatTab->setMatSize(activeCMatTab->getMatSize()+1);
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


void MainWindow::on_actionCmatClose_triggered()
{
    this->removeConfMatTab(this->getActiveConfMatTab());
}


void MainWindow::on_actionCmatCloseAll_triggered()
{
    for (int n=ui->cmatTabWidget->count(); n>0; n--)
    {
        this->removeConfMatTab(this->getActiveConfMatTab());
    }
}
