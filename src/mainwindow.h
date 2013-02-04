#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_actionOpen_triggered();

private:
    void new_cmat_tab();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
