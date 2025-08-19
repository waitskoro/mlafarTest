#include "mainwindow.h"

#include <QFile>
#include <QApplication>

#include <iostream>

using namespace std;

QScopedPointer<QFile> m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    m_logFile.reset(new QFile("../logFile.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    MainWindow w;
    w.show();
    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    QString logMessage = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    switch (type) {
        case QtInfoMsg:     logMessage += "[INFO] "; break;
        case QtDebugMsg:    logMessage += "[DEBUG] "; break;
        case QtWarningMsg:  logMessage += "[WARNING] "; break;
        case QtCriticalMsg: logMessage += "[CRITICAL] "; break;
        case QtFatalMsg:    logMessage += "[FATAL] "; break;
    }

    logMessage += ": ";
    logMessage += msg;

    QTextStream out(m_logFile.data());
    out << logMessage << Qt::endl;
    out.flush();

    cout << logMessage.toStdString() << endl;
}
