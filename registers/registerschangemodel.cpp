#include "registerschangemodel.h"

#include <QAbstractItemView>

using namespace Registers;

RegistersChangeModel::RegistersChangeModel(QObject *parent)
    : QAbstractListModel(parent)
{}

QVector<Register> *RegistersChangeModel::registers()
{
    return &m_registers;
}

void RegistersChangeModel::setRegisters(QVector<Register> &registers)
{
    if (registers.isEmpty())
        return;

    beginResetModel();
    m_registers = registers;
    endResetModel();
}

Qt::ItemFlags RegistersChangeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

int RegistersChangeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_registers.count();
}

bool RegistersChangeModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows (parent, row, row+count);
    endInsertRows ();
    return true;
}

int RegistersChangeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant RegistersChangeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_registers.size())
        return QVariant();

    const Register &reg = m_registers.at(index.row());

    if (role == Qt::EditRole || role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return reg.name;
        case 1: return reg.description;
        case 2: return reg.address;
        case 3: return reg.type;
        }
    }

    return QVariant();
}

QVariant RegistersChangeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Наименование");
        case 1:
            return QString("Описание");
        case 2:
            return QString("Адрес");
        case 3:
            return QString("Тип данных");
        }
    }
    return QVariant();
}

bool RegistersChangeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole || index.row() >= m_registers.size())
        return false;

    QString text = value.toString();
    if (text.isEmpty()) return false;

    Register &reg = (m_registers)[index.row()];

    switch (index.column()) {
    case 0: reg.name = text; break;
    case 1: reg.description = text; break;
    case 2: reg.address = text; break;
    case 3: reg.type = text; break;
    default: return false;
    }

    emit dataChanged(index, index);
    return true;
}
