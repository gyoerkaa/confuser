#include "exportoptionsdialog.h"
#include "ui_exportoptionsdialog.h"

ExportOptionsDialog::ExportOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportOptionsDialog)
{
    ui->setupUi(this);
}

ExportOptionsDialog::~ExportOptionsDialog()
{
    delete ui;
}
