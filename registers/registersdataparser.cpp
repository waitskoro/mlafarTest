#include "registersdataparser.h"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

using namespace Registers;

namespace {
    Q_GLOBAL_STATIC(QString, PATH, QLatin1String("../registers.json"))

    QJsonObject registerToJson(const Register &reg) {
        QJsonObject regObj;
        regObj["type"] = reg.type;
        regObj["name"] = reg.name;
        regObj["access"] = reg.access;
        regObj["address"] = reg.address;
        regObj["description"] = reg.description;
        return regObj;
    }

    Register registerFromJson(const QJsonObject &regObj) {
        return Register::fromJson(regObj);
    }

    QJsonArray registersToJsonArray(const QVector<Register> &registers) {
        QJsonArray array;
        for (const auto &reg : registers) {
            array.append(registerToJson(reg));
        }
        return array;
    }

    QVector<Register> registersFromJsonArray(const QJsonArray &array) {
        QVector<Register> registers;
        registers.reserve(array.count());
        for (const QJsonValue &value : array) {
            registers.append(registerFromJson(value.toObject()));
        }
        return registers;
    }
}

RegistersDataParser::RegistersDataParser(QObject *parent)
    : QObject(parent)
    , m_registers(new QMap<RegisterType, QVector<Register>>())
{
    readFromJson();
}

QVector<Register> *RegistersDataParser::registers(RegisterType type)
{
    return &(*m_registers)[type];
}

void RegistersDataParser::saveRegisters(RegisterType type, QVector<Register> &regs)
{
    (*m_registers)[type] = regs;
    saveAllRegistersToFile();
    qDebug() << "Регистры типа" << registerTypeToString(type) << "успешно сохранены в файл";
}

void RegistersDataParser::createFile()
{
    QFile file(*PATH());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for writing.";
        return;
    }
    file.close();
}

void RegistersDataParser::writeInFile()
{
    QJsonObject rootObject;
    rootObject["common_registers"] = registersToJsonArray(Register::createCommonRegisters());

    if (!writeJsonToFile(rootObject)) {
        qDebug() << "Файл не открывается для записи";
        return;
    }

    qDebug() << "Регистры успешно записаны в файл";
}

void RegistersDataParser::readFromJson()
{
    QJsonObject obj = fileObj();
    if (obj.isEmpty()) {
        return;
    }

    for (const auto &key : obj.keys()) {
        RegisterType type = stringToRegisterType(key);
        QJsonArray registersArray = obj[key].toArray();
        (*m_registers)[type] = registersFromJsonArray(registersArray);
    }

    qDebug() << "Все регистры занесены в таблицу из файла";
}

QJsonObject RegistersDataParser::fileObj()
{
    QFile file(*PATH());

    if (!file.exists()) {
        createFile();
        writeInFile();
        readFromJson();
        qDebug() << "Создан файл";
        return {};
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл!";
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Ошибка разбора JSON:" << parseError.errorString();
        return {};
    }

    return doc.object();
}

QString RegistersDataParser::registerTypeToString(RegisterType type)
{
    switch (type) {
        case Common: return "common_registers";
        case Dos: return "dos_registers";
        case Pco: return "pos_registers";
        default: return "unknown";
    }
}

RegisterType RegistersDataParser::stringToRegisterType(const QString &str)
{
    if (str == "common_registers") return Common;
    if (str == "dos_registers") return Dos;
    if (str == "pos_registers") return Pco;
}

bool RegistersDataParser::writeJsonToFile(const QJsonObject &json)
{
    QFile file(*PATH());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    return true;
}

void RegistersDataParser::saveAllRegistersToFile()
{
    QJsonObject rootObject;

    for (const auto &registerType : m_registers->keys()) {
        rootObject[registerTypeToString(registerType)] =
            registersToJsonArray((*m_registers)[registerType]);
    }

    if (!writeJsonToFile(rootObject)) {
        qDebug() << "Не удалось сохранить регистры в файл";
    }
}
