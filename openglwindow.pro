include(openglwindow.pri)

SOURCES += \
    main.cpp \
    trianglewindow.cpp \
    terraingenerator.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

HEADERS += \
    trianglewindow.h \
    terraingenerator.h

RESOURCES += \
    gestionnaire.qrc
