#include "confmattab.h"

#include <QWidget>
#include <QUndoStack>
#include <QTableWidget>
#include <QVBoxLayout>
#include <algorithm>
#include <QTableWidgetSelectionRange>
#include <QString>

#include "confmattabcmds.h"


ConfMatTab::ConfMatTab(QWidget* parent)
    : QWidget(parent)
{
    this->init(7, 7, parent);
}


ConfMatTab::ConfMatTab(int rowCount, int colCount, QWidget* parent)
    : QWidget(parent)
{   
    this->init(rowCount, colCount, parent);
}


ConfMatTab::~ConfMatTab()
{
    delete m_undoStack;
    delete m_tableWidget;
}


void ConfMatTab::init(int rowCount, int colCount, QWidget* parent)
{
    m_unsavedChanges = false;

    m_undoStack = new QUndoStack(this);

    m_tableView = new QTableView(this);


    m_tableWidget = new QTableWidget(rowCount, colCount, this);

    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addWidget(m_tableWidget);

    this->setLayout(m_layout);

    connect(m_tableWidget->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            parent,
            SLOT(slot_selectionChanged(const QItemSelection &, const QItemSelection &)));
}


QList<QTableWidgetSelectionRange> ConfMatTab::getSelectedRanges() const
{
    return m_tableWidget->selectedRanges();
}


QString ConfMatTab::getCMatItem(int row, int col) const
{
    if (m_tableWidget->item(row, col) == 0)
        return QString("");

    return m_tableWidget->item(row, col)->text();
}


QList<QList <QString> > ConfMatTab::getCMatItems() const
{
    QList<QList <QString> > newlist;
    return newlist;
}


void ConfMatTab::deleteCMatItem(int row, int col)
{
    this->setCMatItem(row, col, QString(""));
}


void ConfMatTab::setCMatItem(int row, int col, QString itemValue)
{

    if (m_tableWidget->item(row, col) == 0)
    {
        QTableWidgetItem *twi = new QTableWidgetItem;
        twi->setText(itemValue);
        m_tableWidget->setItem(row, col, twi);
    }
    else
    {
        m_tableWidget->item(row, col)->setText(itemValue);
    }
}


void ConfMatTab::save()
{

}


bool ConfMatTab::canSave() const
{
    return m_unsavedChanges;
}


void ConfMatTab::cutCommand()
{
    this->copy();
    this->deleteCommand();
}


void ConfMatTab::copy() const
{

}


bool ConfMatTab::canCopy() const
{
    return (this->m_tableWidget->selectedRanges().length() > 0);
}


void ConfMatTab::pasteCommand()
{

}


void ConfMatTab::deleteCommand()
{
    QUndoCommand* newDeleteCommand = new DeleteCommand(this);
    m_undoStack->push(newDeleteCommand);
}


bool ConfMatTab::canDelete()
{
    return (this->m_tableWidget->selectedRanges().count() > 0);
}


void ConfMatTab::undoCommand()
{
    m_undoStack->undo();
}


bool ConfMatTab::canUndo() const
{
    return m_undoStack->canUndo();
}


void ConfMatTab::redoCommand()
{
    m_undoStack->redo();
}


bool ConfMatTab::canRedo() const
{
    return m_undoStack->canRedo();
}


void ConfMatTab::resizeCommand(int newRowCount,
                               int newColCount,
                               bool keepQuadratic)
{
    int verifiedColCount;
    if (newColCount < MIN_CMAT_COLS)
        verifiedColCount = MIN_CMAT_COLS;
    else if (newColCount > MAX_CMAT_COLS)
        verifiedColCount = MAX_CMAT_COLS;
    else
        verifiedColCount = newColCount;

    int verifiedRowCount;
    if (newRowCount < MIN_CMAT_ROWS)
        verifiedRowCount = MIN_CMAT_ROWS;
    else if (newRowCount > MAX_CMAT_ROWS)
        verifiedRowCount = MAX_CMAT_ROWS;
    else
        verifiedRowCount = newRowCount;

    if (keepQuadratic)
    {
        if (verifiedColCount > verifiedRowCount)
            verifiedRowCount = verifiedColCount;
        else
            verifiedColCount = verifiedRowCount;
    }

    // We only push to the undo stack, if the resize actually
    // would do something
    if ( (verifiedRowCount != m_tableWidget->rowCount()) ||
         (verifiedColCount != m_tableWidget->columnCount()) )
    {
        QUndoCommand* newResizeCommand = new ResizeCommand(this,
                                                           verifiedRowCount,
                                                           verifiedColCount);
        m_undoStack->push(newResizeCommand);
    }
}


void ConfMatTab::cropCommand(bool keepQuadratic)
{
    // Find out how much of the table is actually filled with values
    int filledRows = 0;
    int filledCols = 0;

    QTableWidgetItem* tableItem;
    for(int r=0; r < m_tableWidget->rowCount(); ++r)
    {
        for (int c=0; c < m_tableWidget->columnCount(); ++c)
        {
            tableItem = m_tableWidget->item(r,c);
            if ((tableItem != 0) && (tableItem->text() != ""))
            {
                filledRows = r;
                filledCols = c;
            }
        }
    }

    // Resize to that number
    if (keepQuadratic)
    {
        this->resizeCommand(std::max(filledRows, filledCols)+1,
                            std::max(filledRows, filledCols)+1);
    }
    else
    {
        this->resizeCommand(filledRows+1,
                            filledCols+1);
    }
}


void ConfMatTab::setMatSize(int rowCount, int colCount)
{
    m_tableWidget->setRowCount(rowCount);
    m_tableWidget->setColumnCount(colCount);
}


int ConfMatTab::getRowCount() const
{
    return m_tableWidget->rowCount();
}


int ConfMatTab::getColCount() const
{
    return m_tableWidget->columnCount();
}


bool ConfMatTab::canShrink() const
{
    return ( (m_tableWidget->rowCount() > MIN_CMAT_ROWS) &&
             (m_tableWidget->columnCount() > MIN_CMAT_COLS) );
}


bool ConfMatTab::canExpand() const
{
    return ( (m_tableWidget->rowCount() < MAX_CMAT_ROWS) &&
             (m_tableWidget->columnCount() < MAX_CMAT_COLS) );
}
