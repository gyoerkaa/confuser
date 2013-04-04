#ifndef TEXCODEDIALOG_H
#define TEXCODEDIALOG_H

#include <QDialog>

class LatexCode;

namespace Ui {
class TexCodeDialog;
}

class TexCodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TexCodeDialog(LatexCode& texCode,
                           QWidget* parent = 0);
    ~TexCodeDialog();
    
private slots:
    void on_buttonExit_clicked();

private:
    Ui::TexCodeDialog* ui;

    void loadCode(LatexCode& texCode);
};

#endif // TEXCODEDIALOG_H
