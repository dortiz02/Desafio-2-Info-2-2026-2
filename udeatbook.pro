TEMPLATE = app
TARGET = udeatbook

# Aplicacion de consola en C++17, no usa ningun modulo de Qt
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/album.cpp \
    src/almacenamiento.cpp \
    src/cadena.cpp \
    src/consola.cpp \
    src/fecha.cpp \
    src/foto.cpp \
    src/indiceusuarios.cpp \
    src/medidor.cpp \
    src/mensaje.cpp \
    src/redsocial.cpp \
    src/usuario.cpp

HEADERS += \
    include/album.h \
    include/almacenamiento.h \
    include/cadena.h \
    include/consola.h \
    include/fecha.h \
    include/foto.h \
    include/indiceusuarios.h \
    include/lista.h \
    include/medidor.h \
    include/mensaje.h \
    include/redsocial.h \
    include/usuario.h

DISTFILES += \
    data/formato.txt \
    data/generador_datos.cpp
