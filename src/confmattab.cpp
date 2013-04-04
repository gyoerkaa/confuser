#include "confmattab.h"

#include <QWidget>
#include <QUndoStack>
#include <QTableView>
#include <QVBoxLayout>
#include <QString>
#include <QTableView>
#include <QClipboard>
#include <QAbstractItemModel>
#include <QApplication>
#include <QDebug>

#include "confmatmodel.h"
#include "confmat.h"

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
    delete m_tableView;
}


void ConfMatTab::init(int rowCount, int colCount, QWidget* parent)
{
    m_unsavedChanges = false;

    m_tableView = new QTableView(this);

    m_cmatModel = new ConfMatModel(rowCount, colCount, this);
    for (int r=0; r<rowCount; ++r)
    {
        for (int c=0; c<colCount; ++c)
        {
            QModelIndex index = m_cmatModel->index(r,c, QModelIndex());
            m_cmatModel->setData(index,"");
        }
    }
    m_tableView->setModel(m_cmatModel);
    m_cmatModel->clearUndoStack();

    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_tableView);

    this->setLayout(m_layout);

    connect(m_tableView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            parent,
            SLOT(slot_selectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(m_cmatModel,
            SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            parent,
            SLOT(slot_dataChanged(const QModelIndex &, const QModelIndex &)));
}


QModelIndexList ConfMatTab::getSelectedIndices() const
{
    return m_tableView->selectionModel()->selection().indexes();
}


QString ConfMatTab::getCMatItem(int row, int col) const
{
    return m_cmatModel->getItem(row, col).toString();
}

ConfMat& ConfMatTab::getCMat() const
{
    return m_cmatModel->getCMat();
}

void ConfMatTab::deleteCMatItem(int row, int col)
{
    this->setCMatItem(row, col, QString(""));
}


void ConfMatTab::setCMatItem(int row, int col, QString itemValue)
{
    QModelIndex cmatIndex = m_cmatModel->index(row, col, QModelIndex());
    if (cmatIndex.isValid())
    {
        QVariant cmatItem = m_cmatModel->data(cmatIndex, Qt::EditRole);
        if (cmatItem.isValid())
        {
            cmatItem.setValue(itemValue);
        }
        else
        {
            cmatItem = QVariant(itemValue);
        }
        m_cmatModel->setData(cmatIndex, cmatItem, Qt::EditRole);
    }
}


void ConfMatTab::save()
{

}


bool ConfMatTab::canSave() const
{
    return m_unsavedChanges;
}


void ConfMatTab::cutCMatItems()
{
    this->copyCMatItems();
    this->deleteCMatItems();
}


void ConfMatTab::copyCMatItems() const
{
    qDebug() << "ConfMatTab::copyCMatItems";

    // Nothing to do, in case of an empty selection
    QModelIndexList selection =
            m_tableView->selectionModel()->selectedIndexes();
    if (selection.empty())
            return;
    qSort(selection);

    // Get the ranges of indices which need to be saved
    int min_row    = this->getCmatRowCount()-1;
    int min_column = this->getCmatColCount()-1;
    int max_row    = 0;
    int max_column = 0;   
    QModelIndexList::const_iterator idx_it;
    for (idx_it = selection.begin(); idx_it != selection.end(); ++idx_it)
    {
        min_row    = qMin(min_row, idx_it->row());
        min_column = qMin(min_column, idx_it->column());
        max_row    = qMax(max_row, idx_it->row());
        max_column = qMax(max_column, idx_it->column());
    }
    qDebug() << "... min: " << min_row << " " << min_column;
    qDebug() << "... max: " << max_row << " " << max_column;

    // Construct a QString, which will then be put into the clipboard
    QString clipboardContents = "";
    idx_it = selection.begin();
    for (int r = min_row; r <= max_row; ++r)
    {
        for (int c = min_column; c <= max_column; ++c)
        {
            //clipboardContents.append(this->getCMatItem(r,c));
            qDebug() << "... " << this->getCMatItem(r,c);
            // Determine if the cell is empty or the index is
            // not selected at all
            if ( (idx_it != selection.end()) &&
                 (idx_it->row() == r) &&
                 (idx_it->column() == c) )
            {
                // cell selected, buy may be empty
                clipboardContents.append(this->getCMatItem(r,c));
                ++idx_it;
            }
            else
            {
                // cell not selected, need to represent this somehow
                clipboardContents.append(QString(CLIPBRD_UNSELECTED));
            }
            clipboardContents.append(QString(CLIPBRD_NEWCELL));
        }
        clipboardContents.append(QString(CLIPBRD_NEWLINE));
    }

    QApplication::clipboard()->setText(clipboardContents);
}


bool ConfMatTab::canCopy() const
{
    return (m_tableView->selectionModel()->selectedIndexes().length() > 0);
}


void ConfMatTab::pasteCMatItems()
{
    qDebug() << "ConfMatTab::pasteCMatItems";

    // Do nothing it there is no selection, i.e. no place to paste the data to
    QModelIndexList selection =
            m_tableView->selectionModel()->selectedIndexes();
    if (selection.empty())
            return;

    // Find out where to start inserting
    int start_row = this->getCmatRowCount()-1;
    int start_col = this->getCmatColCount()-1;
    QModelIndexList::const_iterator idx_it;
    for (idx_it = selection.begin(); idx_it != selection.end(); ++idx_it)
    {
        start_row = qMin(start_row, idx_it->row());
        start_col = qMin(start_col, idx_it->column());
    }
    qDebug() << "..." << start_row << " " << start_col;

    // Get clipboard data
    QList<QVariant> pastedValues;
    QModelIndexList pastedIndices;
    QModelIndex     index;
    QStringList lineList =
            QApplication::clipboard()->text().split(CLIPBRD_NEWLINE);
    QStringList::const_iterator lineList_it;
    QStringList line;
    QStringList::const_iterator line_it;
    int row = start_row;
    int col = start_col;
    for (lineList_it = lineList.begin();
         lineList_it != lineList.end();
         ++lineList_it)
    {
        qDebug() << "..." << row << " " << col;
        line = lineList_it->split(CLIPBRD_NEWCELL);
        if (row < this->getCmatRowCount())
        {
            col = start_col;
            for (line_it = line.begin(); line_it != line.end(); ++line_it)
            {
                if (col < this->getCmatColCount())
                {
                    if (*line_it != CLIPBRD_UNSELECTED)
                    {
                        index = m_cmatModel->index(row, col, QModelIndex());
                        pastedIndices.append(index);
                        pastedValues.append(QVariant(*line_it));
                        qDebug() << "... ..." << row << " " << col;
                        qDebug() << "... ..." << *line_it;
                    }
                    col++;
                }
            }
            row++;
        }
    }
    m_cmatModel->setData(pastedIndices, pastedValues, Qt::EditRole);
}


bool ConfMatTab::canPaste() const
{
    return !m_tableView->selectionModel()->selectedIndexes().empty();
}


void ConfMatTab::deleteCMatItems()
{
    m_cmatModel->setData(m_tableView->selectionModel()->selectedIndexes(),
                         QList<QVariant>());
}


bool ConfMatTab::canDelete()
{
    return !m_tableView->selectionModel()->selectedIndexes().empty();
}


void ConfMatTab::undo()
{
    m_cmatModel->undo();
}


bool ConfMatTab::canUndo() const
{
    return m_cmatModel->canUndo();
}


void ConfMatTab::redo()
{
    m_cmatModel->redo();
}


bool ConfMatTab::canRedo() const
{
    return m_cmatModel->canRedo();
}


void ConfMatTab::setCMatSize(int newRowCount,
                             int newColCount,
                             bool keepSquare)
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

    if (keepSquare)
    {
        if (verifiedColCount > verifiedRowCount)
            verifiedRowCount = verifiedColCount;
        else
            verifiedColCount = verifiedRowCount;
    }

    // We only push to the undo stack, if the resize actually
    // would do something
    if ( (verifiedRowCount != this->getCmatRowCount()) ||
         (verifiedColCount != this->getCmatColCount()) )
    {
        m_cmatModel->setDataDimensions(verifiedRowCount, verifiedColCount);
    }
}


void ConfMatTab::cropCMat(bool keepSquare)
{
    // Find out how much of the table is actually filled with values
    int filledRows = 0;
    int filledCols = 0;

    QVariant cmatItem;
    for(int r=0; r < this->getCmatRowCount(); ++r)
    {
        for (int c=0; c < this->getCmatColCount(); ++c)
        {
            cmatItem = m_cmatModel->getItem(r,c);
            if (cmatItem.toString().isEmpty())
            {
                filledRows = r;
                filledCols = c;
            }
        }
    }

    // Resize to that number
    if (keepSquare)
    {
        this->setCMatSize(qMax(filledRows, filledCols)+1,
                          qMax(filledRows, filledCols)+1);
    }
    else
    {
        this->setCMatSize(filledRows+1,
                          filledCols+1);
    }
}


int ConfMatTab::getCmatRowCount() const
{
    return m_cmatModel->rowCount(QModelIndex());
}


int ConfMatTab::getCmatColCount() const
{
    return m_cmatModel->columnCount(QModelIndex());
}


bool ConfMatTab::canShrink() const
{
    return ( (this->getCmatRowCount() > MIN_CMAT_ROWS) &&
             (this->getCmatColCount() > MIN_CMAT_COLS) );
}


bool ConfMatTab::canExpand() const
{
    return ( (this->getCmatRowCount() < MAX_CMAT_ROWS) &&
             (this->getCmatColCount() < MAX_CMAT_COLS) );
}
