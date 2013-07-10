#include "confmat.h"


ConfMat::ConfMat(int rowCount, int colCount)
{
    this->init(rowCount, colCount);
}


ConfMat::ConfMat(QList<QList<QVariant> > values)
{
    this->init(2, 2);
}


void ConfMat::init(int rowCount, int colCount)
{
    m_rowCount = qMax(rowCount, 0);
    m_colCount = qMax(colCount, 0);

    for (int r=0; r < m_rowCount; ++r)
    {
        QList<QVariant> newRow;
        for (int c=0; c < m_colCount; ++c)
        {
            newRow << QVariant("");
        }
        m_cmat.append(newRow);
    }
}


QVariant ConfMat::getItem(int row, int col) const
{
    if ( (row > m_rowCount) ||
         (row < 0) ||
         (col > m_colCount) ||
         (col < 0) )
        return QVariant();

    return m_cmat[row][col];
}


bool ConfMat::setItem(int row, int col, QVariant value)
{
    if ( (row > m_rowCount) ||
         (row < 0) ||
         (col > m_colCount) ||
         (col < 0) )
    {
        return false;
    }

    m_cmat[row].replace(col, value);
    return true;
}


QStringList ConfMat::toStringList() const
{
    QStringList lineList;

    for (int row=0; row < m_rowCount; ++row)
    {
        QString line;
        for (int col=0; col < m_colCount-1; ++col)
        {
            line.append(m_cmat[row][col].toString() + ", ");
        }
        line.append(m_cmat[row][m_colCount-1].toString());
        lineList.append(line);
    }
    return lineList;
}


int ConfMat::getColCount() const
{
    return m_colCount;
}


bool ConfMat::removeCols(int col, int count)
{
    if ( (m_colCount <= 0) ||
         (count < 1) ||
         (col < 0) ||
         ((col + count) > m_colCount) )
        return false;

    QList<QList<QVariant> >::iterator row_it;
    for (row_it = m_cmat.begin(); row_it != m_cmat.end(); ++row_it)
    {
        for (int i=0; i < count; ++i)
        {
            row_it->removeAt(col);
        }
    }
    m_colCount = m_colCount-count;
    return true;
}


bool ConfMat::insertCols(int col, int count)
{
    if ( (count < 0) ||
         (col < 0) ||
         (col > m_colCount))
        return false;

    QList<QList<QVariant> >::iterator row_it;
    for (row_it = m_cmat.begin(); row_it != m_cmat.end(); ++row_it)
    {
        for (int i=0; i < count; ++i)
        {
            row_it->insert(col, QVariant(""));
        }
    }
    m_colCount = m_colCount+count;
    return true;
}


void ConfMat::setColCount(int newColCount)
{
    int curColCount = this->getColCount();
    if (curColCount == newColCount)
        return;
    if (curColCount < newColCount)
        insertCols(qMax(curColCount, 0), newColCount - curColCount);
    else
        removeCols(qMax(newColCount, 0), curColCount - newColCount);
}


int ConfMat::getRowCount() const
{
    return m_rowCount;
}


bool ConfMat::removeRows(int row, int count)
{
    if ( (m_rowCount <= 0) ||
         (count < 1) ||
         (row < 0) ||
         ((row + count) > m_rowCount) )
        return false;

    for (int i=0; i < count; ++i)
    {
        m_cmat.removeAt(row);
    }
    m_rowCount = m_rowCount-count;
    return true;
}


bool ConfMat::insertRows(int row, int count)
{
    if ( (count < 0) ||
         (row < 0) ||
         (row > m_rowCount))
        return false;

    for (int i=0; i < count; ++i)
    {
        QList<QVariant> newRow;
        for (int c=0; c < m_colCount; ++c)
        {
            newRow << QVariant("");
        }
        m_cmat.insert(row, newRow);
    }
    m_rowCount = m_rowCount+count;
    return true;
}


void ConfMat::setRowCount(int newRowCount)
{
    int curRowCount = this->getRowCount();
    if (curRowCount == newRowCount)
        return;
    if (curRowCount < newRowCount)
        insertRows(qMax(curRowCount, 0), newRowCount - curRowCount);
    else
        removeRows(qMax(newRowCount, 0), curRowCount - newRowCount);
}
