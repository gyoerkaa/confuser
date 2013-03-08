#ifndef CONFMATTAB_H
#define CONFMATTAB_H

#define MAX_CMAT_COLS 128
#define MAX_CMAT_ROWS 128
#define MIN_CMAT_COLS 2
#define MIN_CMAT_ROWS 2

#include <QWidget>

class QVBoxLayout;
class QTableWidget;
class QTableView;
class QUndoStack;
class QTableWidgetSelectionRange;
class QString;

namespace Ui
{
    class ConfMatTab;
}

//! @brief Holds confusion matrix and handles sanity checks of the user input
//!
//! This class checks all user inputs for sanity, before adding it to the
//! confusion matrix. It also holds the undo stack for its confusion matrix
//!
//! @author Attila Györkös
//!
class ConfMatTab : public QWidget
{
    Q_OBJECT

public:
    explicit ConfMatTab(QWidget* parent = 0);
    explicit ConfMatTab(int rowCount, int colCount, QWidget* parent = 0);
    ~ConfMatTab();

    QList<QTableWidgetSelectionRange> getSelectedRanges() const;
    QString getCMatItem(int row, int col) const;
    QList< QList<QString> > getCMatItems() const;
    void deleteCMatItem(int row, int col);
    void setCMatItem(int row, int col, QString itemValue);
    void save();
    bool canSave() const;

    void cutCommand();
    void copy() const;
    bool canCopy() const;
    void pasteCommand();

    void deleteCommand();
    bool canDelete();

    void undoCommand();
    bool canUndo() const;
    void redoCommand();
    bool canRedo() const;

    void resizeCommand(int rowCount, int colCount, bool keepQuadratic = true);
    void cropCommand(bool keepQuadratic = true);
    void setMatSize(int rowCount, int colCount);
    int  getRowCount() const;
    int  getColCount() const;

    bool canShrink() const;
    bool canExpand() const;


private:
    //! @brief The undo stack for this tab/matrix
    QUndoStack* m_undoStack;

    QTableWidget* m_tableWidget;

    //! @brief Widget used to display the tab/matrix
    QTableView* m_tableView;

    //! @brief Wether the matrix contains unsaved changes
    bool m_unsavedChanges;

    //! @brief Layout for this page of the tab widget
    QVBoxLayout* m_layout;

    void init(int rowCount, int colCount, QWidget* parent = 0);

};

#endif // CONFMATTAB_H
