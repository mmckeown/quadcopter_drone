#-------------------------------------------------
#
# Project created by QtCreator 2014-01-16T03:31:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imu_gui_proto
TEMPLATE = app


SOURCES += main.cpp\
        imu_gui_proto_main_window.cpp \
    attitude_indicator.cpp \
    compass.cpp

HEADERS  += imu_gui_proto_main_window.h \
    attitude_indicator.h \
    compass.h
