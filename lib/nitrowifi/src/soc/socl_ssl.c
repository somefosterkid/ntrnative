// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_ssl.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"

// func_ov009_0215fc80: turns SSL on for a TCP socket that has not been bound, or off (with a NULL connection). It has the
// receive pipe's thread, which owns CPS's socket, switch CPS's SSL layer (func_ov009_0215fd40). pokediamond's
// NitroWiFi names the object socl_ssl.o and the three functions.

static int func_ov009_0215fd00(SOCLSocket *socket, void *ssl);
static int func_ov009_0215fd40(SOCLCommandPacket *packet);

// pokebw2's SOCL_EnableSsl
int func_ov009_0215fc80(int sock, void *sslInfo) {
    SOCLSocket *socket = (SOCLSocket *)sock;

    if (func_ov009_0215f3d0(socket)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_ModeIsTCP(socket->flag)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (socket->state & SOCLI_STATE_BINDING) {
        return SOCL_EINVAL;
    }
    return func_ov009_0215fd00(socket, sslInfo);
}

// pokebw2's SOCLi_ExecEnableSslCommand
static int func_ov009_0215fd00(SOCLSocket *socket, void *ssl) {
    SOCLCommandPacket *packet = func_ov009_0215ce3c(func_ov009_0215fd40, socket, TRUE);

    if (packet == NULL) {
        return SOCL_EMFILE;
    }
    packet->args.ssl.info = ssl;
    return func_ov009_0215cf88(socket, packet);
}

// pokebw2's SOCLi_EnableSslCallBack
static int func_ov009_0215fd40(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    SOCLRecvPipe *pipe = socket->recv_pipe;
    CPSSoc *soc = (CPSSoc *)pipe->base.thread.specific[0];

    OS_LockMutex(&pipe->base.mutex);
    if (packet->args.ssl.info != NULL) {
        soc->con = packet->args.ssl.info;
        CPS_SetSsl(TRUE);
        socket->flag = 4;
    } else {
        socket->flag = 0;
        CPS_SetSsl(FALSE);
        soc->con = NULL;
    }
    OS_UnlockMutex(&pipe->base.mutex);
    return 0;
}
