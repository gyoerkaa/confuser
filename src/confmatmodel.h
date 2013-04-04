#ifndef CONFMATMODEL_H
#define CONFMATMODEL_H

#include <QAbstractTableModel>

class QUndoStack;
class ConfMat;

//! @brief Custom model for QTableView
//!
//! Custom model for QTableView holding an undo stack
//!
//! @author Attila Györkös
//!
class ConfMatModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ConfMatModel(QObject* parent = 0);
    ConfMatModel(int rowCount, int colCount, QObject* parent=0);

    QVariant getItem(int row, int column) const;
    QVariant getItem(const QModelIndex& index) const;
    ConfMat& getCMat() const;

    bool setItem(int row, int column, const QVariant& value);

    //! @brief Sets the role data for the item at index to value.
    //! This method will replace the item at the specified index with the
    //! given value. This is NOT undoable, i.e no command will be
    //! added to the undo stack.
    //! @param index Index of the element to be changed
    //! @param value Replacement element
    //! @return Returns true if successful, otherwise false.
    bool setItem(const QModelIndex& index, const QVariant& value);

    //! @brief Sets the role data for multiple elements
    //! This method will replace multiple items at the specified indices
    //! with the given values. This is NOT undoable, i.e no command will be
    //! added to the undo stack.
    //! @param indices List of indices to be replaced
    //! @param values List of values matching the indices. If the list is
    //!               shorter than indices list or NULL, the missing values
    //!               will be empty strings
    //! @return Returns true if successful for all items, otherwise false
    bool setItem(const QModelIndexList& indices,
                 const QList<QVariant>& values);

    //! @brief Set the size of the matrix
    //! This method will replace the item at the specified position with the
    //! given value. This is NOT undoable, i.e no command will be
    //! added to the undo stack.
    //! @param rowCount New row count
    //! @param colCount New column count
    void setSize(int rowCount, int colCount);

    QVariant data(const QModelIndex& index, int role) const;

    //! @brief Sets the role data for the item at index to value.
    //! This method will replace the item at the specified index with the
    //! given value. This IS undoable, i.e. a command will be added
    //! to the undo stack.
    //! @param index Index of the element to be changed
    //! @param value Replacement Element
    //! @param role TODO
    //! @return Returns true if successful, otherwise false.
    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole);

    //! @brief Sets the role data for multiple elements
    //! This method will replace multiple items at the specified indices
    //! with the given values. This IS undoable, i.e. a command will be added
    //! to the undo stack.
    //! @param indices List of indices to be replaced
    //! @param values List of values matching the indices. If the list is
    //!               shorter than indices list or NULL, the missing values
    //!               will be empty strings
    //! @return Returns true if successful for all items, otherwise false
    bool setData(const QModelIndexList& indices,
                 const QList<QVariant>& values,
                 int role = Qt::EditRole);

    //! @brief Set the size of the matrix
    //! This method will replace the item at the specified position with the
    //! given value. This IS undoable, i.e. a command will be added
    //! to the undo stack.
    //! @param rowCount New row count
    //! @param colCount New column count
    void setDataDimensions(int rowCount, int colCount);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    void setColumnCount(int newColumnCount);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void setRowCount(int newRowCount);

    void undo();
    bool canUndo() const;
    void redo();
    bool canRedo() const;
    void clearUndoStack();

private:
    //! @brief The confusion matrix holding the data for this model
    ConfMat* m_cmat;

    //! @brief The undo stack for this matrix
    QUndoStack* m_undoStack;

    void init(int rowCount, int colCount);

signals:

public slots:

};

#endif // CONFMATMODEL_H
