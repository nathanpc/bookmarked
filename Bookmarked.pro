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
    newbookdialog.cpp \
    addbookmarkdialog.cpp \
    aboutdialog.cpp \
    editbookdialog.cpp

HEADERS  += mainwindow.h \
	database.h \
    newbookdialog.h \
    addbookmarkdialog.h \
    aboutdialog.h \
    editbookdialog.h

FORMS    += mainwindow.ui \
    newbookdialog.ui \
    addbookmarkdialog.ui \
    aboutdialog.ui \
    editbookdialog.ui
