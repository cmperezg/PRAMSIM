TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ram.cpp \
    prcpu.cpp \
    prassembler.cpp \
    prsim.cpp

HEADERS += \
    ram.h \
    prcpu.h \
    prassembler.h \
    prsim.h

DISTFILES += \
    zerocomp.prasm \
    simplest.prasm


INCLUDEPATH += C:\Users\cmpgt\OneDrive\Documentos\boost_1_64_0\boost_1_64_0
