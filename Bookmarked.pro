#-------------------------------------------------
#
# Project created by QtCreator 2014-01-18T19:38:55
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bookmarked
TEMPLATE = app


SOURCES += main.cpp\
		mainwindow.cpp \
	database.cpp

HEADERS  += mainwindow.h \
	database.h

FORMS    += mainwindow.ui
