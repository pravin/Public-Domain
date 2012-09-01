#-------------------------------------------------
#
# Project created by QtCreator 2011-01-31T17:59:04
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BTree
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    btree.cpp

HEADERS += \
    btree.h

LIBS += -lmicrohttpd
