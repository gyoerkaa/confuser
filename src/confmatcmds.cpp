#include "confmatcmds.h"

#include <QList>
#include <QTableWidgetSelectionRange>

#include "confmatmodel.h"

#include <QDebug>


SetItemCommand::SetItemCommand(ConfMatModel& confMatModel,
                               QModelIndex index,
                               QVariant value,
                               QUndoCommand* parent) :
    m_confMatModel(confMatModel)
{
    Q_UNUSED(parent);

    m_indices = QModelIndexList();
    m_indices.append(index);

    m_newValues = QList<QVariant>();
    m_newValues.append(value);

    m_oldValues = QList<QVariant>();
    m_oldValues.append(m_confMatModel.getItem(index));
}


SetItemCommand::SetItemCommand(ConfMatModel& confMatModel,
                               QModelIndexList indices,
                               QList<QVariant> values,
                               QUndoCommand* parent) :
    m_confMatModel(confMatModel)
{
    Q_UNUSED(parent);

    m_indices = indices;
    m_newValues = values;

    m_oldValues = QList<QVariant>();
    QModelIndexList::const_iterator idx_it = m_indices.begin();
    while (idx_it != m_indices.end())
    {
        m_oldValues.append(m_confMatModel.getItem(*idx_it));
        ++idx_it;
    }
}


void SetItemCommand::undo()
{    
    QModelIndexList::const_iterator idx_it = m_indices.begin();
    QList<QVariant>::const_iterator val_it = m_oldValues.begin();
    while (idx_it != m_indices.end())
    {
        if (val_it != m_oldValues.end())
        {
            m_confMatModel.setItem(*idx_it, *val_it);
            ++val_it;
        }
        else
        {
            m_confMatModel.setItem(*idx_it, QVariant(""));
        }
        ++idx_it;
    }
}


void SetItemCommand::redo()
{
    QModelIndexList::const_iterator idx_it = m_indices.begin();
    QList<QVariant>::const_iterator val_it = m_newValues.begin();
    while (idx_it != m_indices.end())
    {
        if (val_it != m_newValues.end())
        {
            m_confMatModel.setItem(*idx_it, *val_it);
            ++val_it;
        }
        else
        {
            m_confMatModel.setItem(*idx_it, QVariant(""));
        }
        ++idx_it;
    }
}


SetSizeCommand::SetSizeCommand(ConfMatModel& confMatModel,
                               int newRowCount,
                               int newColCount,
                               QUndoCommand* parent) :
    m_confMatModel(confMatModel),
    m_newRowCount(newRowCount),
    m_newColCount(newColCount),
    m_oldRowCount(confMatModel.rowCount(QModelIndex())),
    m_oldColCount(confMatModel.columnCount(QModelIndex()))
{
    Q_UNUSED(parent);

    QList<QVariant> m_deletedValues;
}


void SetSizeCommand::undo()
{
    m_confMatModel.setSize(m_oldRowCount, m_oldColCount);
    // If the matrix has been shrunk, we need to restore the deleted items
    // We assume newCount < oldCount for this purpose (nothing to do otherwise)
    for (int r=0; r < m_newRowCount; ++r)
    {
        for (int c=m_newColCount; c < m_oldColCount; ++c)
        {
            // Restore Item
            m_confMatModel.setItem(r, c, m_deletedValues.front());
            m_deletedValues.pop_front();
        }
    }
    for (int r=m_newRowCount; r < m_oldRowCount; ++r)
    {
        for (int c=0; c < m_oldColCount; ++c)
        {
            // Restore Item
            m_confMatModel.setItem(r, c, m_deletedValues.front());
            m_deletedValues.pop_front();
        }
    }
}


void SetSizeCommand::redo()
{
    // If the matrix has been shrunk, we need to save the deleted items
    // We assume newCount < oldCount for this purpose (nothing to do otherwise)
    for (int r=0; r < m_newRowCount; ++r)
    {
        for (int c=m_newColCount; c < m_oldColCount; ++c)
        {
            // save item
            m_deletedValues.append(m_confMatModel.getItem(r,c));
        }
    }
    for (int r=m_newRowCount; r < m_oldRowCount; ++r)
    {
        for (int c=0; c < m_oldColCount; ++c)
        {
            // save item
            m_deletedValues.append(m_confMatModel.getItem(r,c));
        }
    }
    m_confMatModel.setSize(m_newRowCount, m_newColCount);
}
