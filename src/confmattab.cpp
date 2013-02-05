#include "confmattab.h"
#include <QWidget>
#include <QUndoStack>
#include <QTableWidget>
#include <QVBoxLayout>


ConfMatTab::ConfMatTab(QWidget *parent)
    : QWidget(parent)
{
    m_confMatSize = 7;
    m_unsavedChanges = false;
    this->init();
}


ConfMatTab::ConfMatTab(int matSize, QWidget *parent)
    : QWidget(parent)
{
    m_confMatSize = matSize;
    this->init();
}


ConfMatTab::~ConfMatTab()
{
    delete m_undoStack;
    delete m_tableWidget;
}


void ConfMatTab::init()
{
    m_undoStack = new QUndoStack(this);

    m_tableWidget = new QTableWidget(m_confMatSize, m_confMatSize, this);

    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);

    m_layout->addWidget(m_tableWidget);
    setLayout(m_layout);

}


void ConfMatTab::setMatSize(int newSize)
{
    if (newSize < MIN_CMAT_SIZE)
        m_confMatSize = MIN_CMAT_SIZE;
    else if (newSize > MAX_CMAT_SIZE)
        m_confMatSize = MAX_CMAT_SIZE;
    else
        m_confMatSize = newSize;

    m_tableWidget->setRowCount(m_confMatSize);
    m_tableWidget->setColumnCount(m_confMatSize);
}


int ConfMatTab::getMatSize()
{
    return m_confMatSize;
}
