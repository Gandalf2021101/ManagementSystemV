QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    stuwindow.cpp \
    tcomboboxdelegate.cpp \
    tdialoglogin.cpp \
    teawindow.cpp

HEADERS += \
    adminwindow.h \
    mainwindow.h \
    stuwindow.h \
    tcomboboxdelegate.h \
    tdialoglogin.h \
    teawindow.h

FORMS += \
    adminwindow.ui \
    mainwindow.ui \
    stuwindow.ui \
    tdialoglogin.ui \
    teawindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
QT += sql

DISTFILES += \
    style.qss
