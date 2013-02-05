#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConfMatWidget;

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

private:
    Ui::MainWindow *ui;

    ConfMatWidget* getActiveConfMat();
};

#endif // MAINWINDOW_H
