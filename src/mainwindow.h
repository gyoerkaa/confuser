#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConfMatTab;
class MainSettings;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionCmatNew_triggered();

    void on_actionCmatShrink_triggered();

    void on_actionCmatExpand_triggered();

    void on_actionOpenFile_triggered();

    void on_actionEditExportOptions_triggered();

    void on_actionCmatClose_triggered();

    void on_actionCmatCloseAll_triggered();

private:
    Ui::MainWindow *ui;

    int m_confMatIdx;

    MainSettings* m_mainSettings;

    void MainWindow::removeConfMatTab(ConfMatTab* confMatTab);
    ConfMatTab* MainWindow::addConfMatTab(int nSize, QString matName);
    ConfMatTab* MainWindow::addConfMatTab();

    ConfMatTab* getActiveConfMatTab();
};

#endif // MAINWINDOW_H
