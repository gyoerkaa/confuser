#ifndef EXPORTOPTIONSDIALOG_H
#define EXPORTOPTIONSDIALOG_H

#include <QDialog>

namespace Ui
{
    class ExportOptionsDialog;
}

class ExportOptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportOptionsDialog(QWidget *parent = 0);
    ~ExportOptionsDialog();
    
private:
    Ui::ExportOptionsDialog *ui;
};

#endif // EXPORTOPTIONSDIALOG_H
