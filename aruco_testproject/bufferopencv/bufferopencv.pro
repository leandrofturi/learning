QT += concurrent widgets
CONFIG += cmdline link_pkgconfig
PKGCONFIG += opencv

SOURCES += new_thread.cpp main.cpp

target.path = .
INSTALLS += target