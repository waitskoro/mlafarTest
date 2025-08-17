QT       += core gui widgets serialbus serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection/connectiondialog.cpp \
    connection/connectionmanager.cpp \
    connection/connectionparameters.cpp \
    connection/modbusclient.cpp \
    main.cpp \
    mainwindow.cpp \
    registers/registerschangemodel.cpp \
    registers/registersdatamodel.cpp \
    registers/registersdataparser.cpp \
    registers/registersdialog.cpp \
    registers/registersmanager.cpp

HEADERS += \
    connection/connectiondialog.h \
    connection/connectionmanager.h \
    connection/connectionparameters.h \
    connection/modbusclient.h \
    mainwindow.h \
    registers/iregistersparameters.h \
    registers/registerschangemodel.h \
    registers/registersdatamodel.h \
    registers/registersdataparser.h \
    registers/registersdialog.h \
    registers/registersmanager.h

FORMS += \
    connection/connectiondialog.ui \
    mainwindow.ui \
    registers/registersdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
