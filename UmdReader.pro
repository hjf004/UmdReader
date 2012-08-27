HEADERS += include/Additions.h \
           include/UmdParser.h \
           include/MainWindow.h \
           include/UmdReader.h

SOURCES += src/UmdParser.cpp \
           src/Additions.cpp \
           src/main.cpp \
           src/MainWindow.cpp \
           src/UmdReader.cpp

LIBS += -lz

RESOURCES += \
    resource.qrc
TRANSLATIONS += lang.ts
