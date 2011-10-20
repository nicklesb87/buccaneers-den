TARGET = BuccaneersDen
TEMPLATE = app
INCLUDEPATH += ../qserialdevice/include
LIBS += ../qserialdevice/src/libSerialPort.a
unix:LIBS += -ludev
win32:LIBS += -lsetupapi -luuid -ladvapi32
SOURCES += main.cpp \
    mainwindow.cpp \
    BusPirate/buspirategui.cpp \
    BusPirate/consolepage.cpp \
    BusPirate/firmwarepage.cpp \
    vt100/vt100device.cpp \
    line/linedevice.cpp \
    BusPirate/basichighlighter.cpp \
    lib/macrospanel.cpp \
    lib/xbdmreader.cpp \
    lib/xbdmwriter.cpp \
    BusPirate/buspiratedevice.cpp \
    lib/macrosmodel.cpp \
    lib/macroitem.cpp \
    lib/macrosview.cpp \
    BusPirate/buspirateconfig.cpp \
    BusPirate/buspiratemacroparser.cpp \
    line/linedevicegui.cpp \
    line/linedeviceconfig.cpp \
    lib/commandhistory.cpp \
    lib/hexfileparser.cpp \
    lib/ds30bootloader.cpp \
    lib/picmemoryimage.cpp
HEADERS += mainwindow.h \
    BusPirate/buspirategui.h \
    BusPirate/consolepage.h \
    BusPirate/firmwarepage.h \
    vt100/vt100device.h \
    line/linedevice.h \
    BusPirate/basichighlighter.h \
    lib/macrospanel.h \
    lib/xbdmreader.h \
    lib/xbdmwriter.h \
    BusPirate/buspiratedevice.h \
    lib/macrosmodel.h \
    lib/macroitem.h \
    lib/macrosview.h \
    BusPirate/buspirateconfig.h \
    BusPirate/buspiratemacroparser.h \
    line/linedevicegui.h \
    line/linedeviceconfig.h \
    lib/commandhistory.h \
    lib/hexfileparser.h \
    lib/ds30bootloader.h \
    lib/picmemoryimage.h \
    gadgetgui.h
