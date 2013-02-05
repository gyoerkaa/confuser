#ifndef CONFMATTAB_H
#define CONFMATTAB_H

#define MAX_CMAT_SIZE 128
#define MIN_CMAT_SIZE 2

#include  <QWidget>

class QVBoxLayout;
class QTableWidget;
class QUndoStack;

namespace Ui
{
    class ConfMatTab;
}

class ConfMatTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConfMatTab(QWidget* parent = 0);
    explicit ConfMatTab(int matSize, QWidget* parent = 0);
    //explicit ConfMatTab(QWidget* parent = 0);
    ~ConfMatTab();

    void setMatSize(int newSize);
    int  getMatSize();

private:
    QUndoStack* m_undoStack;

    QTableWidget* m_tableWidget;

    int m_confMatSize;
    bool m_unsavedChanges;


    QVBoxLayout* m_layout;

    void ConfMatTab::init();

};

#endif // CONFMATTAB_H
