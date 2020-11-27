QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 -Wall

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flowlayout.cpp \
    main.cpp \
    src/lib/lexer.cpp \
    src/lib/parser.cpp \
    src/lib/pokemon_value.cpp \
    src/lib/trainer_util.cpp \
    src/lib/trainer_value.cpp \
    src/mainwindow.cpp \
    src/parser_util.cpp

HEADERS += \
    include/flowlayout.h \
    include/lib/array.h \
    include/lib/array_value.h \
    include/lib/define_statement.h \
    include/lib/lexer.h \
    include/lib/parser.h \
    include/lib/pokemon_value.h \
    include/lib/trainer_util.h \
    include/lib/trainer_value.h \
    include/mainwindow.h \
    include/parser_util.h

FORMS += \
    forms/mainwindow.ui

INCLUDEPATH += include
INCLUDEPATH += include/lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
