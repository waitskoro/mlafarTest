#include "registersdatamodel.h"

using namespace Registers;

RegistersDataModel::RegistersDataModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_registers(new QVector<Register>())
{

}

QVector<Register> *RegistersDataModel::registers()
{
    return m_registers;
}

void RegistersDataModel::setRegisters(QVector<Register> *registers)
{
    if (registers->isEmpty())
        return;

    beginResetModel();
    m_registers = registers;
    endResetModel();
}

int RegistersDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_registers->count();
}

int RegistersDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant RegistersDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_registers->size())
        return QVariant();

    const Register &reg = m_registers->at(index.row());

    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return reg.name;
        case 1: return reg.description;
        case 2: return reg.value;
        case 3: return reg.address;
        }
    }

    return {};
}

QVariant RegistersDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Наименование");
        case 1:
            return QString("Описание");
        case 2:
            return QString("Значение");
        case 3:
            return QString("Адрес");
        }
    }
    return QVariant();
}
