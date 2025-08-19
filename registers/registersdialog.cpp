#include "registersdialog.h"
#include "ui_registersdialog.h"

#include "registerschangemodel.h"

using namespace Registers;

RegistersDialog::RegistersDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::RegistersDialog)
    , m_models()
{
    m_ui->setupUi(this);

    m_models[Dos] = new RegistersChangeModel(this);
    m_models[Pco] = new RegistersChangeModel(this);
    m_models[Common] = new RegistersChangeModel(this);

    m_ui->tableViewDos->setModel(m_models[Dos]);
    m_ui->tableViewPco->setModel(m_models[Pco]);
    m_ui->tableViewCommon->setModel(m_models[Common]);

    m_ui->tableViewDos->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_ui->tableViewPco->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_ui->tableViewCommon->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(m_ui->pushButtonCancel, &QPushButton::clicked, [this] {
        emit neededRegisters(currentTabType());
    });

    connect(m_ui->pushButtonCancelAll, &QPushButton::clicked, [this] {
        emit neededAllRegisters();
    });

    connect(m_ui->pushButtonSaveAll, &QPushButton::clicked, [this] {
        emit saveRegisters(Dos);
        emit saveRegisters(Pco);
        emit saveRegisters(Common);
    });

    connect(m_ui->pushButtonSaveCurrent, &QPushButton::clicked, [this] {
        emit saveRegisters(currentTabType());
    });
}

void RegistersDialog::setRegisters(RegisterType type, QVector<Register> *registers)
{
    if (!m_models.contains(type) || !m_models[type]) {
        qWarning() << "No model exists for RegisterType" << type;
        return;
    }
    if (!registers) {
        qWarning() << "Null registers provided for type" << type;
        return;
    }
    m_models[type]->setRegisters(*registers);
}

RegistersDialog::~RegistersDialog()
{
    delete m_ui;
}

RegisterType RegistersDialog::currentTabType()
{
    int tabIndex = m_ui->tabWidget->currentIndex();

    if      (tabIndex == 0) return Common;
    else if (tabIndex == 1) return Dos;
    else if (tabIndex == 2) return Pco;

    return {};
}
