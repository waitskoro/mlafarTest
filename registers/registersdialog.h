#pragma once

#include <QWidget>
#include <QDialog>

namespace Ui {
class RegistersDialog;
}

namespace Registers {

class RegistersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistersDialog(QWidget *parent = nullptr);
    ~RegistersDialog();

private:
    Ui::RegistersDialog *ui;
};

}

