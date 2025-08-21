#pragma once

#include <QAbstractListModel>

#include "iregistersparameters.h"

namespace Registers {

class RegistersDataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RegistersDataModel(QObject *parent = nullptr);

    QVector<Register> *registers();
    void setRegisters(const QVector<Register> &registers);
signals:

private:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVector<Register> *m_registers;
};

}

