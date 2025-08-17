#include "registersdataparser.h"

#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

using namespace Registers;

namespace {
    Q_GLOBAL_STATIC(QString, PATH, QLatin1String("../registers.json"))
}

RegistersDataParser::RegistersDataParser(QObject *parent)
    : QObject(parent)
    , m_registers(new QMap<RegisterType, QVector<Register>>())
{
    readFromJson();
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
    QFile file(*PATH());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Файл не открывается для записи";
        return;
    }

    QJsonObject rootObject;
    QJsonArray registersArray;

    for (const auto &reg : Register::createCommonRegisters()) {
        QJsonObject regObj;
        regObj["type"] = reg.type;
        regObj["name"] = reg.name;
        regObj["access"] = reg.access;
        regObj["address"] = reg.address;
        regObj["description"] = reg.description;
        registersArray.append(regObj);
    }

    rootObject["common_registers"] = registersArray;

    QJsonDocument doc(rootObject);
    file.write(doc.toJson());
    file.close();

    qDebug() << "Регистры успешно записаны в файл";
}

void RegistersDataParser::readFromJson()
{
    if (fileObj().isEmpty())
        return;

    auto obj = fileObj();

    QJsonArray registersArray = obj["common_registers"].toArray();

    (*m_registers)[Common].clear();
    (*m_registers)[Common].reserve(registersArray.count());

    for (const QJsonValue &registerValue : registersArray) {
        QJsonObject regObj = registerValue.toObject();
        auto reg = Register::fromJson(regObj);
        (*m_registers)[Common].push_back(reg);
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
