QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Source/UI/dockwidget.cpp \
    Source/UI/expanderwidget.cpp \
    Source/firstdialogwindow.cpp \
    Source/UI/graphicsview.cpp \
    Source/main.cpp \
    Source/mainwindow.cpp \
    Source/UI/tablewidget.cpp \
    Source/UI/tabwidget.cpp \
    Source/UI/toolbar.cpp \
    Source/Elements/Gates/and.cpp \
    Source/Elements/gate.cpp \
    Source/Elements/Gates/nand.cpp \
    Source/Elements/Gates/nor.cpp \
    Source/Elements/Gates/not.cpp \
    Source/Elements/Gates/or.cpp \
    Source/Elements/Gates/xnor.cpp \
    Source/Elements/Gates/xor.cpp

HEADERS += \
    Include/UI/dockwidget.h \
    Include/UI/expanderwidget.h \
    Include/firstdialogwindow.h \
    Include/UI/graphicsview.h \
    Include/mainwindow.h \
    Include/UI/tablewidget.h \
    Include/UI/tabwidget.h \
    Include/UI/toolbar.h \
    Include/Elements/Gates/and.h \
    Include/Elements/gate.h \
    Include/Elements/Gates/nand.h \
    Include/Elements/Gates/nor.h \
    Include/Elements/Gates/not.h \
    Include/Elements/Gates/or.h \
    Include/Elements/Gates/xnor.h \
    Include/Elements/Gates/xor.h

FORMS += \
    firstdialogwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

DISTFILES +=
