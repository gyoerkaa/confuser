#include "confmatmodel.h"
#include <QList>
#include <QVariant>
#include <QUndoStack>
#include "confmat.h"
#include "confmatcmds.h"

#include <QDebug>

ConfMatModel::ConfMatModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    this->init(2, 2);
}


ConfMatModel::ConfMatModel(int rowCount, int colCount, QObject *parent)
    : QAbstractTableModel(parent)
{
    this->init(rowCount, colCount);
}


void ConfMatModel::init(int rowCount, int colCount)
{
    m_undoStack = new QUndoStack(this);   
    m_cmat = new ConfMat(rowCount, colCount);
}


int ConfMatModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_cmat->getRowCount();
}


int ConfMatModel::columnCount(const QModelIndex &parent) const
{    
    Q_UNUSED(parent);

    return m_cmat->getColCount();
}


QVariant ConfMatModel::getItem(int row, int column) const
{
    return m_cmat->getItem(row, column);
}


ConfMat& ConfMatModel::getCMat() const
{
    return *m_cmat;
}


QVariant ConfMatModel::getItem(const QModelIndex &index) const
{
    return this->data(index, Qt::DisplayRole);
}


bool ConfMatModel::setItem(const QModelIndex &index, const QVariant &value)
{
    if (m_cmat->setItem(index.row(), index.column(), value))
    {
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


bool ConfMatModel::setItem(int row, int column, const QVariant &value)
{
    // Create an index and just use overloaded ConfMatModel::setItem
    QModelIndex index = QAbstractItemModel::createIndex(row, column);
    return (this->setItem(index, value));
}


bool ConfMatModel::setItem(const QModelIndexList &indices,
                           const QList<QVariant> &values)
{
    // Check all indices for validity first. If any index
    // is invalid, do nothing, just return false.
    QList<QModelIndex>::const_iterator idx_it;
    for (idx_it = indices.begin(); idx_it != indices.end(); ++idx_it)
    {
        if (!idx_it->isValid())
            return false;
    }

    QList<QVariant>::const_iterator val_it = values.begin();
    for (idx_it = indices.begin(); idx_it != indices.end(); ++idx_it)
    {
        QVariant value("");
        if (val_it != values.end())
        {
            value.setValue(*val_it);
            ++val_it;
        }
        this->setItem(*idx_it, value);
    }
    return true;
}


void ConfMatModel::setSize(int rowCount, int colCount)
{
    this->setColumnCount(colCount);
    this->setRowCount(rowCount);
}


QVariant ConfMatModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (role == Qt::DisplayRole))
    {
        return m_cmat->getItem(index.row(), index.column());
    }

    return QVariant();
}


bool ConfMatModel::setData(const QModelIndex &index,
                           const QVariant &value,
                           int role)
{
    if (index.isValid() && (role == Qt::EditRole))
    {
        QUndoCommand* newSetItemCommand = new SetItemCommand(*this,
                                                             index,
                                                             value);
        m_undoStack->push(newSetItemCommand);
    }
    return false;
}


bool ConfMatModel::setData(const QModelIndexList &indices,
                           const QList<QVariant> &values,
                           int role)
{
    if (!indices.empty() && (role == Qt::EditRole))
    {
        QUndoCommand* newSetItemCommand = new SetItemCommand(*this,
                                                             indices,
                                                             values);
        m_undoStack->push(newSetItemCommand);
    }
    return false;
}


void ConfMatModel::setDataDimensions(int rowCount, int colCount)
{
    QUndoCommand* newSetSizeCommand = new SetSizeCommand(*this,
                                                         rowCount,
                                                         colCount);
    m_undoStack->push(newSetSizeCommand);
}


QVariant ConfMatModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole)
        return QVariant(QString::number(section));

    return QVariant();
}


Qt::ItemFlags ConfMatModel::flags(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(parent) | Qt::ItemIsEditable;
}


bool ConfMatModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    qDebug() << "ConfMatModel::Columns";
    qDebug() << "... current count " << this->columnCount(QModelIndex());
    qDebug() << "... remove " << count << " cols starting at col " << column;
    if ( (m_cmat->getColCount() <= 0) ||
         (count <= 0) ||
         (column < 0) ||
         ((column + count) > m_cmat->getColCount()) )
        return false;

    beginRemoveColumns(QModelIndex(), column, column+count-1);
    m_cmat->removeCols(column, count);
    endRemoveColumns();
    return true;
}


bool ConfMatModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if ((count <= 0) ||
        (column < 0) ||
        (column > m_cmat->getColCount()) )
        return false;

    bool success = false;
    beginInsertColumns(QModelIndex(), column, column+count-1);
    success = m_cmat->insertCols(column, count);
    endInsertColumns();

    return success;
}


void ConfMatModel::setColumnCount(int newColumnCount)
{
    qDebug() << "ConfMatModel::setColumnCount";
    qDebug() << "... count " << newColumnCount;

    int currentColumnCount = this->columnCount(QModelIndex());
    if (currentColumnCount > newColumnCount)
    {
        this->removeColumns(newColumnCount,
                            currentColumnCount-newColumnCount,
                            QModelIndex());
    }
    else if (currentColumnCount < newColumnCount)
    {
        this->insertColumns(currentColumnCount,
                            newColumnCount-currentColumnCount,
                            QModelIndex());
    }
}


bool ConfMatModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    qDebug() << "ConfMatModel::removeRows";
    qDebug() << "... current count " << this->rowCount(QModelIndex());
    qDebug() << "... remove " << count << " rows starting at row " << row;

    if ( (m_cmat->getRowCount() <= 0) ||
         (count <= 0) ||
         (row < 0) ||
         ((row + count) > m_cmat->getRowCount()) )
        return false;

    beginRemoveRows(QModelIndex(), row, row+count-1);
    m_cmat->removeRows(row, count);
    endRemoveRows();
    qDebug() << "... new count " << this->rowCount(QModelIndex());
    return true;
}


bool ConfMatModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    qDebug() << "ConfMatModel::removeRows";
    qDebug() << "... current count " << this->rowCount(QModelIndex());
    qDebug() << "... remove " << count << " rows starting at row " << row;

    if ((count <= 0) ||
        (row < 0) ||
        (row > m_cmat->getColCount()) )
        return false;

    bool success = false;
    beginInsertRows(QModelIndex(), row, row+count-1);
    success = m_cmat->insertRows(row, count);
    endInsertRows();
    return success;
}


void ConfMatModel::setRowCount(int newRowCount)
{   
    qDebug() << "ConfMatModel::setRowCount";
    qDebug() << "... count " << newRowCount;

    int currentRowCount = this->rowCount(QModelIndex());
    if (currentRowCount > newRowCount)
    {
        this->removeRows(newRowCount,
                         currentRowCount-newRowCount,
                         QModelIndex());
    }
    else if (currentRowCount < newRowCount)
    {
        this->insertRows(currentRowCount,
                         newRowCount-currentRowCount,
                         QModelIndex());
    }
}


void ConfMatModel::undo()
{
    m_undoStack->undo();
}


bool ConfMatModel::canUndo() const
{
    return m_undoStack->canUndo();
}


void ConfMatModel::redo()
{
    m_undoStack->redo();
}


bool ConfMatModel::canRedo() const
{
    return m_undoStack->canRedo();
}


void ConfMatModel::clearUndoStack()
{
    m_undoStack->clear();
}
