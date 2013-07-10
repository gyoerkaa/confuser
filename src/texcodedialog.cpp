#include "texcodedialog.h"
#include "ui_texcodedialog.h"

#include "texcode.h"

TexCodeDialog::TexCodeDialog(TexCode& texCode,
                             QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TexCodeDialog)
{
    ui->setupUi(this);
    this->loadPreamble(texCode);
    this->loadTable(texCode);
}


TexCodeDialog::~TexCodeDialog()
{
    delete ui;
}


void TexCodeDialog::on_buttonExit_clicked()
{
    this->close();
}


void TexCodeDialog::loadTable(TexCode& texCode)
{
    QTextDocument *document = ui->textEditTable->document();
    QTextCursor cursor(document);
    cursor.select(QTextCursor::Document);

    QStringList table = texCode.getTable();
    QStringList::iterator it;
    for (it = table.begin(); it != table.end(); ++it)
    {
        cursor.insertText(*it + "\n");
    }
}


void TexCodeDialog::loadPreamble(TexCode& texCode)
{
    QTextDocument *document = ui->textEditPreamble->document();
    QTextCursor cursor(document);
    cursor.select(QTextCursor::Document);

    QStringList preamble = texCode.getPreamble();
    QStringList::iterator it;
    for (it = preamble.begin(); it != preamble.end(); ++it)
    {
        cursor.insertText(*it + "\n");
    }
}
