#include "registersdialog.h"
#include "ui_registersdialog.h"

using namespace Registers;

RegistersDialog::RegistersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegistersDialog)
{
    ui->setupUi(this);
}

RegistersDialog::~RegistersDialog()
{
    delete ui;
}
