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
        emit saveRegisters(Dos, m_models[Dos]->registers());
        emit saveRegisters(Pco, m_models[Pco]->registers());
        emit saveRegisters(Common, m_models[Common]->registers());
    });

    connect(m_ui->pushButtonSaveCurrent, &QPushButton::clicked, [this] {
        emit saveRegisters(currentTabType(), m_models[currentTabType()]->registers());
    });
}

void RegistersDialog::setModels(QMap<RegisterType, RegistersChangeModel*> registers)
{
    for (const auto type : typesChange) {
        m_models[type] = registers[type];
    }

    m_ui->tableViewCommon->setModel(m_models[Common]);
    m_ui->tableViewPco->setModel(m_models[Pco]);
    m_ui->tableViewDos->setModel(m_models[Dos]);
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
