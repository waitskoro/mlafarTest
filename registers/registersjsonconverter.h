#pragma once

#include <QJsonArray>

#include "iregistersparameters.h"

namespace Registers {

static QJsonObject registerToJson(const Registers::Register *reg)
{
    QJsonObject obj;
    obj["type"] = reg->type;
    obj["name"] = reg->name;
    obj["access"] = reg->access;
    obj["address"] = reg->address;
    obj["description"] = reg->description;
    return obj;
}

static QJsonArray registersToJsonArray(const QVector<Register> &registers) {
    QJsonArray array;

    for (const auto &reg : registers) {
        array.append(registerToJson(&reg));
    }

    return array;
}

static Registers::Register registerFromJson(const QJsonObject &json)
{
    Register reg;
    reg.type = json["type"].toString();
    reg.name = json["name"].toString();
    reg.access = json["access"].toString();
    reg.address = json["address"].toString();
    reg.description = json["description"].toString();
    return reg;
}

static QVector<Registers::Register> registersFromJsonArray(const QJsonArray &array)
{
    QVector<Register> registers;
    registers.reserve(array.count());
    for (const QJsonValue &value : array) {
        registers.append(Register::fromJson(value.toObject()));
    }
    return registers;
}

}
