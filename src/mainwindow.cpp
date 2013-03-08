#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencesdialog.h"
#include "confmattab.h"
#include "mainsettings.h"

#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget* parent) :
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
    if (confMatTab == 0)
        return;

    ui->cmatTabWidget->removeTab(ui->cmatTabWidget->indexOf(confMatTab));
    confMatTab->deleteLater();

    // Make sure there is always 1 tab open
    if (ui->cmatTabWidget->count() <= 0)
    {
        this->addConfMatTab();
    }
}


ConfMatTab* MainWindow::addConfMatTab(int rowCount, int colCount, QString matName)
{
    ConfMatTab* newCMat = new ConfMatTab(rowCount, colCount, this);
    newCMat->setAccessibleName(matName);

    ui->cmatTabWidget->addTab(newCMat, matName);
    return newCMat;
}


ConfMatTab* MainWindow::addConfMatTab()
{
    // Generate a name
    QString matName = "new " + QString::number(m_confMatIdx);
    m_confMatIdx++;

    return this->addConfMatTab(7, 7, matName);
}


ConfMatTab* MainWindow::getActiveConfMatTab()
{
    return (ConfMatTab*)ui->cmatTabWidget->currentWidget();
}


void MainWindow::setActiveConfMatTab(ConfMatTab* confMatTab)
{

}


bool MainWindow::unsavedChangesDialog(ConfMatTab* confMatTab)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save");
    msgBox.setIcon (QMessageBox::Question);
    msgBox.setText(confMatTab->accessibleName()+" has been modified");
    msgBox.setInformativeText("Do you want to save your changes ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    return true;
}


bool MainWindow::confirmToClose(ConfMatTab* confMatTab)
{
    if (confMatTab->canSave())
    {
        this->unsavedChangesDialog(confMatTab);
    }
    return true;
}


bool MainWindow::confirmToClose()
{
    ConfMatTab* confMatTab;
    for (int i=0; i<ui->cmatTabWidget->count(); ++i)
    {
        confMatTab = (ConfMatTab*)ui->cmatTabWidget->widget(i);
        this->confirmToClose(confMatTab);
    }
    return true;
}


void MainWindow::refreshButtonStates(ConfMatTab* confMatTab)
{
    if (confMatTab == 0)
        return;

    ui->actionSave->setEnabled(confMatTab->canSave());
    ui->actionCopy->setEnabled(confMatTab->canCopy());
    ui->actionCut->setEnabled(confMatTab->canCopy());
    ui->actionDelete->setEnabled(confMatTab->canDelete());
    ui->actionUndo->setEnabled(confMatTab->canUndo());
    ui->actionRedo->setEnabled(confMatTab->canRedo());    
    ui->actionCmatShrink->setEnabled(confMatTab->canShrink());
    ui->actionCmatExpand->setEnabled(confMatTab->canExpand());
    ui->actionCmatCrop->setEnabled(confMatTab->canShrink());
}


void MainWindow::on_actionCmatNew_triggered()
{    
    ui->cmatTabWidget->setCurrentWidget(this->addConfMatTab());
}


void MainWindow::on_actionOpenFile_triggered()
{
    QStringList filenameList = QFileDialog::getOpenFileNames(
                            this,
                            tr("Select one or more files to open"),
                            "/",
                            tr("Comma-separated values (*.csv *.txt)"));
    if (!filenameList.isEmpty())
    {
        //
    }
}


void MainWindow::on_actionSave_triggered()
{

}


void MainWindow::on_actionCmatClose_triggered()
{
    ConfMatTab* activeCmat = this->getActiveConfMatTab();
    if (this->confirmToClose(activeCmat))
    {
        this->removeConfMatTab(activeCmat);
    }
}


void MainWindow::on_actionCmatCloseAll_triggered()
{
    ConfMatTab* activeCmat;
    for (int n=ui->cmatTabWidget->count(); n>0; n--)
    {
        activeCmat = this->getActiveConfMatTab();
        if (this->confirmToClose(activeCmat))
        {
            this->removeConfMatTab(activeCmat);
        }
    }
}


void MainWindow::on_actionCmatExport_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "/",
                tr("LaTeX file (*.tex);; Text file (*.txt)"));
    if (filename != "")
    {
        //
    }
}


void MainWindow::on_actionCut_triggered()
{

}


void MainWindow::on_actionCopy_triggered()
{

}


void MainWindow::on_actionPaste_triggered()
{

}


void MainWindow::on_actionDelete_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->deleteCommand();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionUndo_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->undoCommand();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionRedo_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->redoCommand();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatShrink_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->resizeCommand(activeCMatTab->getRowCount()-1,
                                     activeCMatTab->getColCount()-1,
                                     true);
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatExpand_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->resizeCommand(activeCMatTab->getRowCount()+1,
                                     activeCMatTab->getColCount()+1,
                                     true);
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatCrop_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->cropCommand();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionExit_triggered()
{
    if (this->confirmToClose())
    {
        this->close();
    }
}


void MainWindow::on_cmatTabWidget_currentChanged(int index)
{
    this->refreshButtonStates(this->getActiveConfMatTab());
}


void MainWindow::slot_selectionChanged(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    this->refreshButtonStates(this->getActiveConfMatTab());
}


void MainWindow::on_actionEditPreferences_triggered()
{
    PreferencesDialog prefDialog;

    prefDialog.setModal(true);
    prefDialog.exec();
}
