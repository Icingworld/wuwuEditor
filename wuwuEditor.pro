QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
win32-msvc* {
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/src/database.cpp \
    src/src/login.cpp \
    src/src/main.cpp \
    src/src/mainwindow.cpp

HEADERS += \
    src/include/database.h \
    src/include/login.h \
    src/include/mainwindow.h

FORMS += \
    src/ui/login.ui \
    src/ui/mainwindow.ui

RESOURCES += \
    resources/resources.qrc

TRANSLATIONS += \
    wuwuEditor_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
