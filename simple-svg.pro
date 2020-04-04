QT       += core gui widgets svg

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

desktopfile.files = res/simple-svg.desktop
desktopfile.path = /usr/share/applications/
iconfile.files = res/simple-svg.svg
iconfile.path = /usr/share/icons/hicolor/scalable/apps/



target.path = /usr/bin

INSTALLS += target desktopfile iconfile

RESOURCES += \
    simple-svg.qrc
