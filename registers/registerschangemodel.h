#pragma once

#include <QAbstractListModel>

#include "iregistersparameters.h"

namespace Registers {

class RegistersChangeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RegistersChangeModel(QObject *parent = nullptr);

    QVector<Register> *registers();
    void setRegisters(const QVector<Register> &registers);

private:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows (int row, int count, const QModelIndex& parent) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVector<Register> m_registers;

};

}
