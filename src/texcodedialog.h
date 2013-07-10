#ifndef TEXCODEDIALOG_H
#define TEXCODEDIALOG_H

#include <QDialog>

class TexCode;

namespace Ui {
class TexCodeDialog;
}

class TexCodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TexCodeDialog(TexCode &texCode,
                           QWidget* parent = 0);
    ~TexCodeDialog();
    
private slots:
    void on_buttonExit_clicked();

private:
    Ui::TexCodeDialog* ui;

    void loadTable(TexCode& texCode);
    void loadPreamble(TexCode& texCode);
};

#endif // TEXCODEDIALOG_H
