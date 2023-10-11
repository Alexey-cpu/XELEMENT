QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# main project paths paths
BIN_DIR = ../../bin
LIB_DIR = ../../libs
TOOLS_PATH = ../../tools
SOURCE_PATH = \
    ../../source/gui \
    ../../source/kernel \
    ../../source/tests

# retrieve project sorce code files
for(path,SOURCE_PATH):HEADERS += $$files($${path}/*.h,true)
for(path,TOOLS_PATH):HEADERS += $$files($${path}/*.h,true)
for(path,SOURCE_PATH):SOURCES += $$files($${path}/*.cpp,true)

SOURCES += main.cpp

# configure binaries target folder
DESTDIR = $${BIN_DIR}

# object files directory
OBJECTS_DIR = objectsFiles/

# attach external/internal resources
for(path,HEADERS):INCLUDEPATH += $$dirname(path)
