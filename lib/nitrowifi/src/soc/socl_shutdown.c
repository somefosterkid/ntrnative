// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_shutdown.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"

// func_ov009_0215e6b8: ends the sending half of a TCP connection. It marks the socket closing and has the send pipe's
// thread shut the connection down in CPS (func_ov009_0215e764). pokediamond's NitroWiFi names the object
// socl_shutdown.o and both functions.

static int func_ov009_0215e764(SOCLCommandPacket *packet);

// pokebw2's SOCL_Shutdown
int func_ov009_0215e6b8(SOCLSocket *socket) {
    SOCLSendPipe *pipe;
    SOCLCommandPacket *packet;

    if (func_ov009_0215f3d0(socket)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (!(socket->state & SOCLI_STATE_CONNECTED) || (socket->state & SOCLI_STATE_CLOSING)) {
        return SOCL_ENOTCONN;
    }
    socket->state |= SOCLI_STATE_CLOSING;
    pipe = socket->send_pipe;
    if (pipe != NULL && pipe->socket != NULL) {
        packet = func_ov009_0215ce3c(func_ov009_0215e764, pipe->socket, socket->flag_block);
        if (packet != NULL) {
            return func_ov009_0215cf98(pipe->socket, packet);
        }
        return SOCL_EMFILE;
    }
    return 0;
}

// pokebw2's SOCLi_ShutdownCallBack
static int func_ov009_0215e764(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;

    if (SOCLi_SocketIsTCP(socket)) {
        func_ov009_0215ad00();
    }
    return 0;
}
