TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


LIBS += -lws2_32


SOURCES += \
        MdApi.cxx \
        main.cxx \
        mongoose.c

HEADERS += \
    MdApi.hpp \
    mongoose.h

DISTFILES +=
