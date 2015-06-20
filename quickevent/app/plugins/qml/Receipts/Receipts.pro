message(including plugin $$PWD)

PLUGIN_NAME = Receipts

include ( ../quickeventqmlplugin.pri )

QT += widgets sql printsupport

CONFIG += c++11 hide_symbols

DEFINES += RECEIPTSPLUGIN_BUILD_DLL

INCLUDEPATH += $$PWD/../Event/include
INCLUDEPATH += $$PWD/../Runs/include
INCLUDEPATH += $$PWD/../CardReader/include
LIBS += \
    -L$$DESTDIR \
    -lEventplugin \
    -lRunsplugin \
    -lCardReaderplugin \

include (src/src.pri)

RESOURCES += \
    $${PLUGIN_NAME}.qrc \

DISTFILES += \
    qml/reports/receiptClassic.qml
