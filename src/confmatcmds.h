#ifndef CONFMATCMDS_H
#define CONFMATCMDS_H

#include <QUndoCommand>
#include <QModelIndexList>

class ConfMatModel;

namespace ConfMatCmds
{
    class DeleteCommand;
    class SetSizeCommand;
    class SetItemCommand;
    class PasteCommand;
}


class SetItemCommand : public QUndoCommand
{
public:
    SetItemCommand(ConfMatModel& confMatModel,
                   QModelIndex index,
                   QVariant newValue,
                   QUndoCommand* parent = 0);
    SetItemCommand(ConfMatModel& confMatModel,
                   QModelIndexList indices,
                   QList<QVariant> values,
                   QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    ConfMatModel& m_confMatModel;
    QModelIndexList m_indices;
    QList<QVariant> m_newValues;
    QList<QVariant> m_oldValues;
};


class SetSizeCommand : public QUndoCommand
{
public:
    SetSizeCommand(ConfMatModel& confMatModel,
                   int newRowCount,
                   int newColCount,
                   QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    ConfMatModel& m_confMatModel;
    QList<QVariant> m_deletedValues;

    const int m_oldRowCount;
    const int m_oldColCount;
    const int m_newRowCount;
    const int m_newColCount;
};


class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(ConfMatModel* confMatModel,
                 QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    ConfMatModel* m_confMatModel;

    QModelIndexList m_pastedIndices;
    QList<QVariant> m_pastedValues;
    QList<QVariant> m_overriddenValues;
};

#endif // CONFMATCMDS_H
