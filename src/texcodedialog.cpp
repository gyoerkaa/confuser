#include "texcodedialog.h"
#include "ui_texcodedialog.h"

#include "latexcode.h"

TexCodeDialog::TexCodeDialog(LatexCode& texCode,
                             QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TexCodeDialog)
{
    ui->setupUi(this);
    this->loadCode(texCode);
}


TexCodeDialog::~TexCodeDialog()
{
    delete ui;
}


void TexCodeDialog::on_buttonExit_clicked()
{
    this->close();
}


void TexCodeDialog::loadCode(LatexCode& texCode)
{
    QTextDocument *document = ui->textEditTexCode->document();
    QTextCursor cursor(document);

    cursor.select (QTextCursor::Document);
    for (int i = 0; i < texCode.getMatLineCount(); ++i)
    {
        cursor.insertText(texCode.getMatLine(i));
    }
}
