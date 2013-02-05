#include "confmatwidget.h"
#include <QtGui>
#include <QUndoStack>

#define MAX_CMAT_SIZE 128
#define MIN_CMAT_SIZE 2

ConfMatWidget::ConfMatWidget(QWidget* parent) :
    QTableWidget(parent)
{
    m_undoStack = new QUndoStack(this);
}

ConfMatWidget::~ConfMatWidget()
{
    delete m_undoStack;
}
