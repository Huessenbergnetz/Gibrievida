# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-gibrievida

CONFIG += sailfishapp
CONFIG += c++11

QT += sql network

include(../common/common.pri)

SOURCES += \
    src/main.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

DISTFILES += \
    qml/harbour-gibrievida.qml \
    harbour-gibrievida.desktop \
    rpm/harbour-gibrievida.changes.in \
    rpm/harbour-gibrievida.yaml \
    qml/pages/MainPage.qml \
    qml/common/ImageHighlight.qml

