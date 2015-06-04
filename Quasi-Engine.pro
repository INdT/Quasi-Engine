TEMPLATE = subdirs

SUBDIRS += src

contains(BUILD_EXAMPLES, 1) {
    SUBDIRS += examples demos
}
unix {
	QMAKE_CXXFLAGS += -I/usr/local/include
	LIBS += -L/usr/local/lib
	INCLUDEPATH += -I/usr/local/include
}

