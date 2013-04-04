#ifndef CONFMATTAB_H
#define CONFMATTAB_H

#define MAX_CMAT_COLS 512
#define MAX_CMAT_ROWS 512
#define MIN_CMAT_COLS 2
#define MIN_CMAT_ROWS 2
#define CLIPBRD_NEWLINE "\n"
#define CLIPBRD_NEWCELL "\t"
#define CLIPBRD_UNSELECTED "[]"

#include <QWidget>
#include <QModelIndexList>

class QVBoxLayout;
class QTableView;
class QString;
class ConfMatModel;
class ConfMat;

namespace Ui
{
    class ConfMatTab;
}

//! @brief Holds confusion matrix and handles sanity checks of the user input
//!
//! This class checks all user inputs for sanity, before adding it to the
//! confusion matrix.
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

    QModelIndexList getSelectedIndices() const;
    QString getCMatItem(int row, int col) const;
    ConfMat& getCMat() const;

    void deleteCMatItem(int row, int col);
    void setCMatItem(int row, int col, QString itemValue);
    void save();
    bool canSave() const;

    void cutCMatItems();
    void copyCMatItems() const;
    bool canCopy() const;
    void pasteCMatItems();
    bool canPaste() const;

    void deleteCMatItems();
    bool canDelete();

    void undo();
    bool canUndo() const;
    void redo();
    bool canRedo() const;

    void setCMatSize(int rowCount,
                     int colCount,
                     bool keepSquare = true);
    void cropCMat(bool keepSquare = true);
    int  getCmatRowCount() const;
    int  getCmatColCount() const;

    bool canShrink() const;
    bool canExpand() const;


private:
    //! @brief Widget used to display the tab/matrix
    QTableView* m_tableView;

    ConfMatModel* m_cmatModel;

    //! @brief Wether the matrix contains unsaved changes
    bool m_unsavedChanges;

    //! @brief Layout for this page of the tab widget
    QVBoxLayout* m_layout;

    void init(int rowCount, int colCount, QWidget* parent = 0);

};

#endif // CONFMATTAB_H
