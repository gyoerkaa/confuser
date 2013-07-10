#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferencesdialog.h"
#include "texcodedialog.h"
#include "texcode.h"
#include "confmattab.h"
#include "mainsettings.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>

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
    ConfMatTab* newCMatTab = new ConfMatTab(rowCount, colCount, this);
    newCMatTab->setAccessibleName(matName);
    newCMatTab->setObjectName(matName);
    ui->cmatTabWidget->addTab(newCMatTab, matName);
    return newCMatTab;
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
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(confMatTab->accessibleName()+" has been modified");
    msgBox.setInformativeText("Do you want to save your changes ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);

    switch(msgBox.exec())
    {
        case QMessageBox::No:     return true;
        case QMessageBox::Cancel: return false;
        case QMessageBox::Yes: this->on_actionSave_triggered(); return true;

    }

    return false;
}


bool MainWindow::confirmToClose(ConfMatTab* confMatTab)
{
    if (confMatTab->canSave())
    {
        return this->unsavedChangesDialog(confMatTab);
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
    ui->actionSaveAs->setEnabled(confMatTab->canSave());
    ui->actionCut->setEnabled(confMatTab->canCopy());
    ui->actionCopy->setEnabled(confMatTab->canCopy());
    ui->actionPaste->setEnabled(confMatTab->canPaste());
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


bool MainWindow::saveConfMat(ConfMatTab *confMatTab, QString filepath)
{
    QString filep = filepath;
    if (filep == "")
    {
        filep = QFileDialog::getSaveFileName(
            this,
            tr("Save Confusion matrix"),
            "/",
            tr("Comma-separated values (*.csv);; Text file (*.txt)"));
    }
    if (filep != "")
    {
        QFileInfo fileInfo(filep);
        confMatTab->setAccessibleName(fileInfo.completeBaseName());
        confMatTab->setObjectName(fileInfo.completeBaseName());
        return confMatTab->saveCMat(filep);
    }
    return false;
}


bool MainWindow::exportConfMat(ConfMatTab *confMatTab, QString filepath)
{
    QString filep = filepath;
    if (filep == "")
    {
        QString filep = QFileDialog::getSaveFileName(
                    this,
                    tr("Export Confusion Matrix"),
                    "/",
                    tr("LaTeX file (*.tex)"));
    }
    if (filep != "")
    {
        return confMatTab->exportCMat(filep);
    }
    return false;
}


void MainWindow::on_actionSave_triggered()
{    
    QString filename = this->getActiveConfMatTab()->getCurrentFileName();
    this->saveConfMat(this->getActiveConfMatTab(), filename);
}


void MainWindow::on_actionSaveAs_triggered()
{
    this->saveConfMat(this->getActiveConfMatTab(), "");
}


void MainWindow::on_actionCmatExport_triggered()
{
    QString filepath = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                "/",
                tr("LaTeX file (*.tex)"));
    if (filepath != "")
    {
        this->exportConfMat(this->getActiveConfMatTab(), filepath);
    }
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


void MainWindow::on_actionCut_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();
    if (activeCMatTab != 0)
    {
        activeCMatTab->cutCMatItems();
    }
}


void MainWindow::on_actionCopy_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();
    if (activeCMatTab != 0)
    {
        activeCMatTab->copyCMatItems();
    }
}


void MainWindow::on_actionPaste_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();
    if (activeCMatTab != 0)
    {
        activeCMatTab->pasteCMatItems();
    }
}


void MainWindow::on_actionDelete_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();
    if (activeCMatTab != 0)
    {
        activeCMatTab->deleteCMatItems();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionUndo_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->undo();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionRedo_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->redo();
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatShrink_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->setCMatSize(activeCMatTab->getCmatRowCount()-1,
                                   activeCMatTab->getCmatColCount()-1,
                                   true);
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatExpand_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->setCMatSize(activeCMatTab->getCmatRowCount()+1,
                                   activeCMatTab->getCmatColCount()+1,
                                   true);
        this->refreshButtonStates(activeCMatTab);
    }
}


void MainWindow::on_actionCmatCrop_triggered()
{
    ConfMatTab* activeCMatTab = this->getActiveConfMatTab();

    if (activeCMatTab != 0)
    {
        activeCMatTab->cropCMat();
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
    Q_UNUSED(index);
    this->refreshButtonStates(this->getActiveConfMatTab());
}


void MainWindow::slot_selectionChanged(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    this->refreshButtonStates(this->getActiveConfMatTab());
}


void MainWindow::slot_dataChanged(const QModelIndex& topLeft,
                                  const QModelIndex& bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    this->refreshButtonStates(this->getActiveConfMatTab());
}


void MainWindow::on_actionEditPreferences_triggered()
{
    PreferencesDialog prefDialog;

    prefDialog.setModal(true);
    prefDialog.exec();
}


void MainWindow::on_actionViewLatexCode_triggered()
{
    TexCode texCode(this->getActiveConfMatTab()->getCMat());
    TexCodeDialog codeDialog(texCode);

    codeDialog.setModal(true);
    codeDialog.exec();
}
