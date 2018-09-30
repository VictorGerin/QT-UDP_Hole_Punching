QT -= gui
QT += network

CONFIG += c++14 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    conn.cpp \
    stunclient.cpp \
    testeclass.cpp \
    models/stunattributemodel.cpp \
    models/mappedaddressmodel.cpp \
    models/addressmodel.cpp \
    models/endpointmodel.cpp \
    models/stunheadermodel.cpp

HEADERS += \
    conn.h \
    stunclient.h \
    testeclass.h \
    models/stunattributemodel.h \
    models/mappedaddressmodel.h \
    models/addressmodel.h \
    models/endpointmodel.h \
    models/stunheadermodel.h

DISTFILES +=
