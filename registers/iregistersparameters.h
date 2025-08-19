#pragma once

#include <QString>
#include <QMetaType>
#include <QJsonObject>
#include <QVector>

namespace Registers {

enum RegisterType {
    Pco,
    Dos,
    Common
};

const std::array<RegisterType, 3> types = {Pco, Dos, Common};

class Register {

public:
    static Register fromJson(const QJsonObject &json) {
        Register reg;
        reg.type = json["type"].toString();
        reg.name = json["name"].toString();
        reg.access = json["access"].toString();
        reg.address = json["address"].toString();
        reg.description = json["description"].toString();
        return reg;
    }

    static QVector<Register> createCommonRegisters() {
        QVector<Register> registers;

        registers.append(Register::fromJson({{"address", "0x0000"}, {"name", "fw_version_proc"},    {"access", "R"},   {"type", "uint16"},   {"description", "Версия прошивки микропроцессора"}}));
        registers.append(Register::fromJson({{"address", "0x0001"}, {"name", "fw_version_fpga"},    {"access", "R"},   {"type", "uint16"},   {"description", "Версия прошивки ПЛИС"}}));
        registers.append(Register::fromJson({{"address", "0x0002"}, {"name", "serial_number"},      {"access", "R/W"}, {"type", "char[10]"}, {"description", "Серийный номер модуля в кодах ASCII1"}}));
        registers.append(Register::fromJson({{"address", "0x0007"}, {"name", "identifier"},         {"access", "R"},   {"type", "uint64"},   {"description", "Идентификатор аппаратного обеспечения"}}));
        registers.append(Register::fromJson({{"address", "0x000B"}, {"name", "temp_fpga_mc"},       {"access", "R"},   {"type", "int16"},    {"description", "Температура ПЛИС, -128…+127, 0.1°С"}}));
        registers.append(Register::fromJson({{"address", "0x000C"}, {"name", "temp_board_mc"},      {"access", "R"},   {"type", "int16"},    {"description", "Температура платы, -128…+127, 0.1°С"}}));
        registers.append(Register::fromJson({{"address", "0x000D"}, {"name", "counter_rx"},         {"access", "R"},   {"type", "uint32"},   {"description", "Количество принятых пакетов"}}));
        registers.append(Register::fromJson({{"address", "0x000F"}, {"name", "counter_tx"},         {"access", "R"},   {"type", "uint32"},   {"description", "Количество переданных пакетов"}}));
        registers.append(Register::fromJson({{"address", "0x0011"}, {"name", "counter_crc_error"},  {"access", "R"},   {"type", "uint32"},   {"description", "Количество ошибок контрольных сумм"}}));
        registers.append(Register::fromJson({{"address", "0x0013"}, {"name", "counter_cmd_error"},  {"access", "R"},   {"type", "uint32"},   {"description", "Количество ошибок формата пакета"}}));
        registers.append(Register::fromJson({{"address", "0x0015"}, {"name", "reboot_cmd"},         {"access", "R/W"}, {"type", "uint16"},   {"description", "Сброс платы при записи 0xAF91"}}));
        registers.append(Register::fromJson({{"address", "0x0016"}, {"name", "fw_update_state"},    {"access", "R"},   {"type", "uint16"},   {"description", "Состояние процедуры обновления"}}));
        registers.append(Register::fromJson({{"address", "0x0017"}, {"name", "fw_max_size"},        {"access", "R"},   {"type", "uint32"},   {"description", "Размер FWBL, байт"}}));
        registers.append(Register::fromJson({{"address", "0x0019"}, {"name", "uptime_sec"},         {"access", "R"},   {"type", "uint32"},   {"description", "Счётчик секунд с момента загрузки"}}));
        registers.append(Register::fromJson({{"address", "0x001B"}, {"name", "init_status"},        {"access", "R"},   {"type", "uint32"},   {"description", "Слово состояния инициализации (статусы)"}}));
        registers.append(Register::fromJson({{"address", "0x001D"}, {"name", "module_status"},      {"access", "R"},   {"type", "uint16"},   {"description", "Состояния аварий (текущее состояние)"}}));
        registers.append(Register::fromJson({{"address", "0x001E"}, {"name", "module_error_event"}, {"access", "R/W"}, {"type", "uint16"},   {"description", "События аварий (сбрасываемые флаги)"}}));

        return registers;
    }

    QString type;
    QString name;
    QString access;
    QString address;
    QString description;
    int value;
};

}
