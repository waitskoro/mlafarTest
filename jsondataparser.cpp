#include "jsondataparser.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include "registers/registersjsonconverter.h"
#include "connection/connectionjsonconverter.h"

using namespace Registers;
using namespace Connection;

namespace {
const QString PATH_REGISTERS = QLatin1String("../registers.json");
const QString PATH_CONNECTION = QLatin1String("../connection_info.json");

QJsonDocument readJsonFile(const QString& filePath, bool& success)
{
    success = false;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file:" << filePath << "-" << file.errorString();

        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject emptyObj;
            file.write(QJsonDocument(emptyObj).toJson());
            file.close();
            qInfo() << "Created empty JSON file:" << filePath;
        } else {
            qWarning() << "Couldn't create file:" << filePath << "-" << file.errorString();
            return QJsonDocument();
        }

        return QJsonDocument();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isNull()) {
        qWarning() << "Invalid JSON in file:" << filePath;
        return QJsonDocument();
    }

    success = true;
    return doc;
}

bool writeJsonFile(const QString& filePath, const QJsonObject& jsonObject)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file for writing:" << filePath << "-" << file.errorString();
        return false;
    }

    bool success = file.write(QJsonDocument(jsonObject).toJson()) > 0;
    file.close();

    if (!success) {
        qWarning() << "Failed to write to file:" << filePath;
    }

    return success;
}

bool writeJsonFile(const QString& filePath, const QJsonDocument& jsonDoc)
{
    return writeJsonFile(filePath, jsonDoc.object());
}

}

JsonDataParser::JsonDataParser(QObject *parent)
    : QObject(parent)
{}

Connection::ConnectionParameters *JsonDataParser::readConnectionParameters()
{
    bool success = false;
    QJsonDocument doc = readJsonFile(PATH_CONNECTION, success);

    if (!success || !doc.isObject()) {
        qWarning() << "Failed to read or parse connection file";
        return new ConnectionParameters();
    }

    QJsonObject obj = doc.object();
    if (obj.contains("connection")) {
        return connectionParametersFromJson(obj["connection"].toObject());
    }

    qWarning() << "No 'connection' key found in connection file";
    return new ConnectionParameters();
}

bool JsonDataParser::writeConnectionParameters(Connection::ConnectionParameters *parameters)
{
    if (!parameters) {
        qWarning() << "Null pointer passed to writeConnectionParameters";
        return false;
    }

    QJsonObject rootObj;
    rootObj["connection"] = connectionParametersToJson(parameters); 

    return writeJsonFile(PATH_CONNECTION, rootObj);
}

QVector<Registers::Register> *JsonDataParser::readRegisters(Registers::RegisterType type)
{
    bool success = false;
    QJsonDocument doc = readJsonFile(PATH_REGISTERS, success);

    if (!success || !doc.isObject()) {
        return nullptr;
    }

    QJsonObject rootObj = doc.object();
    QString typeKey = registerTypeToString(type);

    if (rootObj.contains(typeKey)) {
        QJsonArray registersArray = rootObj[typeKey].toArray();
        auto registers = new QVector<Register>();
        *registers = registersFromJsonArray(registersArray);
        return registers;
    }

    qWarning() << "No registers found for type:" << typeKey;

    return nullptr;
}

bool JsonDataParser::writeRegisters(Registers::RegisterType type, const QVector<Registers::Register>& registers)
{
    QJsonObject rootObject;

    bool readSuccess = false;
    QJsonDocument existingDoc = readJsonFile(PATH_REGISTERS, readSuccess);
    if (readSuccess && existingDoc.isObject()) {
        rootObject = existingDoc.object();
    }

    QString typeKey = registerTypeToString(type);
    rootObject[typeKey] = registersToJsonArray(registers);

    return writeJsonFile(PATH_REGISTERS, rootObject);
}

QString JsonDataParser::registerTypeToString(Registers::RegisterType type)
{
    switch (type) {
        case Registers::Common: return "common_registers";
        case Registers::Dos: return "dos_registers";
        case Registers::Pco: return "pos_registers";
        default: return "unknown";
    }
}

Registers::RegisterType JsonDataParser::stringToRegisterType(const QString &str)
{
    if (str == "common_registers") return Registers::Common;
    if (str == "dos_registers") return Registers::Dos;
    if (str == "pos_registers") return Registers::Pco;
    return {};
}
