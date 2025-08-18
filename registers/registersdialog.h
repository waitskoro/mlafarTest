#pragma once

#include <QWidget>
#include <QDialog>

#include "iregistersparameters.h"

namespace Ui {
class RegistersDialog;
}

namespace Registers {

class RegistersChangeModel;

class RegistersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistersDialog(QWidget *parent = nullptr);

    void setRegisters(RegisterType type, QVector<Register> *registers);

    ~RegistersDialog();

signals:
    void neededAllRegisters();
    void neededRegisters(RegisterType);
    void saveRegisters(RegisterType, QVector<Register> &);

private:
    RegisterType currentTabType();
    QVector<Register> &currentVectorByTab(RegisterType);

    Ui::RegistersDialog *m_ui;

    RegistersChangeModel *m_dosModel;
    RegistersChangeModel *m_pcoModel;
    RegistersChangeModel *m_commonModel;

};

}

