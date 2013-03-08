#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConfMatTab;
class MainSettings;
class QItemSelection;

namespace Ui
{
    class MainWindow;
}

//! @brief Main GUI and user interaction
//!
//! This class builds the GUI and handles all user interactions, i.e. set
//! button states and call the right methods to manipulate the currently
//! displayed matrix
//!
//! @author Attila Györkös
//!
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slot_selectionChanged(const QItemSelection &selected,
                               const QItemSelection &deselected);

private:
    //! @brief Provides access to all GUI elements
    Ui::MainWindow* ui;

    //! @brief Number of created confusion matrices
    //! This is used to automatically generate a name for a tab/matrix
    int m_confMatIdx;

    //! @brief Provides access to user settings/preferences
    MainSettings* m_mainSettings;

    //! @brief Remove sthe specified tab/matrix
    //! @param confMatTab Pointer to the tab/matrix to remove
    void removeConfMatTab(ConfMatTab* confMatTab);

    //! @brief Create a new empty tab/matrix
    //! This method will add a new tab/matrix to the tab browser and set it as
    //! the active tab.
    //! @return the created tab/matrix.
    ConfMatTab* addConfMatTab(int rowCount, int colCount, QString matName);

    //! @brief Create a new empty tab/matrix
    //! This method will add a new tab/matrix to the tab browser and set it as
    //! the active tab. The matrix size will be the size specified in the
    //! preferences, its name will be automatically generated.
    //! @return the created tab/matrix.
    ConfMatTab* addConfMatTab();

    //! @brief Get the currently active tab/matrix in the GUI
    //! @return the active tab/matrix.
    ConfMatTab* getActiveConfMatTab();

    //! @brief Set the currently active tab/matrix in the GUI
    //! @param confMatTab Pointer to the tab/matrix to (maybe) save
    void        setActiveConfMatTab(ConfMatTab* confMatTab);

    //! @brief Display an unsaved changes dialog
    //! This method will set up an unsaved changes dialog, display itand
    //! return the users choice. The dialog will ask the user to save the
    //! matrix, discard changes or cancel.
    //! @param confMatTab Pointer to the tab/matrix to (maybe) save.
    //! @return true, if the user has comfirmed to close the tab/matrix
    //!         either after saving it or discaring the unsaved changes.
    bool unsavedChangesDialog(ConfMatTab* confMatTab);

    //! @brief Handles unsaved changes in a specific tab/matrix
    //! This method will check the specified tab/matrix for unsaved changes and
    //! display an unsaved changes dialog, if necessary.
    //! @param *confMatTab Pointer to the tab/matrix to close.
    //! @return true, if the user has comfirmed to close the tab/matrix
    //!         either after saving it or discaring the unsaved changes.
    //! @sa unsavedChangesDialog(ConfMatTab *confMatTab)
    bool confirmToClose(ConfMatTab* confMatTab);

    //! @brief Handles unsaved changes in all tabs/matrices
    //! This method will check all opened tabs/matrices for unsaved changes and
    //! display an unsaved changes dialog for each tab/matrix, if necessary.
    //! @return true, if the user has comfirmed to close all tabs/matrices
    //!         either after saving them or discaring the unsaved changes.
    //! @sa confirmToClose(ConfMatTab* confMatTab)
    bool confirmToClose();

    //! @brief Set the button states
    //! A call to this method will set all button states depending on
    //! the state of the specified selected tab/matrix.
    //! @param *confMatTab Pointer to the tab/matrix to get the state from.
    void refreshButtonStates(ConfMatTab *confMatTab);

private slots:
    void on_actionCmatNew_triggered();
    void on_actionOpenFile_triggered();
    void on_actionSave_triggered();
    void on_actionCmatClose_triggered();
    void on_actionCmatCloseAll_triggered();
    void on_actionCmatExport_triggered();

    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionDelete_triggered();

    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    void on_actionCmatShrink_triggered();
    void on_actionCmatExpand_triggered();
    void on_actionCmatCrop_triggered();

    void on_actionExit_triggered();

    void on_cmatTabWidget_currentChanged(int index);
    void on_actionEditPreferences_triggered();

};

#endif // MAINWINDOW_H
