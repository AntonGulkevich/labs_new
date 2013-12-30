QT += network widgets

HEADERS       = reciever.h
SOURCES       = reciever.cpp \
                main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/network/broadcastreceiver
INSTALLS += target
