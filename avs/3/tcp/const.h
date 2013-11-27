#ifndef CONST_H
#define CONST_H
class QString;
class QByteArray;

namespace t_msg {
    const quint8 TCP_M_INIT = 10;
    const quint8 TCP_M_DATA = 11;

    const quint8 TCP_F_INIT = 20;
    const quint8 TCP_F_DATA = 21;
    const quint8 TCP_F_SIZE = 22;

    const quint8 UDP_M_INIT = 30;
    const quint8 UDP_M_DATA = 31;

    const quint8 UDP_F_INIT = 40;
    const quint8 UDP_F_DATA = 41;
    const quint8 UDP_F_SIZE = 42;
    const quint8 UDP_F_PACK = 43;
    const quint8 UDP_F_ACKN = 44;
    const quint8 UDP_F_RECV = 45;
}


#endif // CONST_H
