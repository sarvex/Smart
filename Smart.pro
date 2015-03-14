QT += core gui sql network

win32 {
    LIBS += -lrasapi32 -lshell32
    DEFINES += WINVER=0x0501
}

TARGET = Smart
TEMPLATE = app

RESOURCES += \
    resource/images.qrc

SOURCES += main.cpp\
        MainWindow.cpp \
    AnimateButton.cpp \
    At.cpp \
    DeamonDatabase.cpp \
    DeamonInterface.cpp \
    DeamonSetting.cpp \
    ExitDialog.cpp \
    ImageButton.cpp \
    SettingDialog.cpp \
    PinDialog.cpp \
    Setting.cpp \
    SingleApplication.cpp \
    SpeedLabel.cpp \
    Data.cpp \
    DialogSetting.cpp \
    TimeLabel.cpp

HEADERS  += MainWindow.h \
    AnimateButton.h \
    At.h \
    DeamonConnection.h \
    DeamonDatabase.h \
    DeamonInterface.h \
    DeamonSetting.h \
    ExitDialog.h \
    ImageButton.h \
    SettingDialog.h \
    PinDialog.h \
    Setting.h \
    SingleApplication.h \
    SpeedLabel.h \
    Define.h \
    Data.h \
    DialogSetting.h \
    TimeLabel.h



HEADERS += SerialPort.h \
        SerialEnumerator.h \

SOURCES	+= SerialPort.cpp

unix:SOURCES += SerialPortPosix.cpp DeamonConnectionLinux.cpp
unix:!macx:SOURCES += SerialEnumeratorUnix.cpp

macx {
  SOURCES   += SerialEnumeratorOsx.cpp DeamonConnectionMac.cpp
  LIBS      += -framework IOKit -framework CoreFoundation
}

win32 {
  SOURCES   += SerialPortWin.cpp SerialEnumeratorWin.cpp DeamonConnectionWindows.cpp
  LIBS      += -lsetupapi -ladvapi32 -luser32
}

OTHER_FILES +=















































