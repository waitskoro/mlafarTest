#include "registersdialog.h"
#include "ui_registersdialog.h"

#include "registerschangemodel.h"

using namespace Registers;

RegistersDialog::RegistersDialog(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::RegistersDialog)
    , m_dosModel(new RegistersChangeModel(this))
    , m_pcoModel(new RegistersChangeModel(this))
    , m_commonModel(new RegistersChangeModel(this))
{
    m_ui->setupUi(this);

    m_ui->tableViewDos->setModel(m_dosModel);
    m_ui->tableViewPco->setModel(m_pcoModel);
    m_ui->tableViewCommon->setModel(m_commonModel);

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
        emit saveRegisters(Dos, m_dosModel->registers());
        emit saveRegisters(Pco, m_pcoModel->registers());
        emit saveRegisters(Common, m_commonModel->registers());
    });

    connect(m_ui->pushButtonSaveCurrent, &QPushButton::clicked, [this] {
        emit saveRegisters(currentTabType(), currentVectorByTab(currentTabType()));
    });
}

void RegistersDialog::setRegisters(RegisterType type, QVector<Register> *registers)
{
    switch (type) {
        case Common: m_commonModel->setRegisters(*registers); break;
        case Dos: m_dosModel->setRegisters(*registers); break;
        case Pco: m_pcoModel->setRegisters(*registers); break;
    }
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
}

QVector<Register> &RegistersDialog::currentVectorByTab(RegisterType)
{
    switch (currentTabType()) {
        case Common: return m_commonModel->registers();
        case Dos: return m_dosModel->registers();
        case Pco: return m_pcoModel->registers();
    }
}
