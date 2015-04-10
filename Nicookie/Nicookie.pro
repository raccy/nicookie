#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T19:57:09
#
#-------------------------------------------------

CONFIG += c++11


QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Nicookie
TEMPLATE = app


SOURCES	+= main.cpp\
        nicookiemainwindow.cpp \
        nicookie.cpp

HEADERS	+= nicookiemainwindow.h \
        nicookie.h

FORMS += nicookiemainwindow.ui
