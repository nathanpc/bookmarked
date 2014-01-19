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
	database.cpp \
    newbookdialog.cpp

HEADERS  += mainwindow.h \
	database.h \
    newbookdialog.h

FORMS    += mainwindow.ui \
    newbookdialog.ui
