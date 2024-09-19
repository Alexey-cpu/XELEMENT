CONFIG -= qt
CONFIG += c++20
QMAKE_CXXFLAGS += -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCE_PATH = ../../source

# retrieve project headers and sources
for(path,SOURCE_PATH):HEADERS += $$files($${path}/*.hpp,true)
for(path,SOURCE_PATH):HEADERS += $$files($${path}/*.h,true)
for(path,HEADERS):INCLUDEPATH += $$dirname(path)

for(path,SOURCE_PATH):SOURCES += $$files($${path}/*.cpp,true)
for(path,SOURCE_PATH):SOURCES += $$files($${path}/*.c,true)
SOURCES += main.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../source/example/XExamples.h
