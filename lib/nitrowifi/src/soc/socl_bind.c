// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_bind.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "nitro/os.h"

// SOCL's bind and connect: func_ov009_0215d43c and func_ov009_0215d4ac set the socket's ports and address, and for a socket that
// blocks they have its receive pipe's thread bind it in CPS, and for TCP connect it (func_ov009_0215d5f8 and
// func_ov009_0215d650). pokediamond's NitroWiFi names the object socl_bind.o and every function in it (func_ov009_0215d43c and
// func_ov009_0215d4ac; its other calls from here, such as SOCL_Listen, this version lacks).

static int func_ov009_0215d5f8(SOCLSocket *socket);
static int func_ov009_0215d650(SOCLCommandPacket *packet);

// What func_ov009_0215d4ac returns for a connect that is still running and has no error yet
// pokebw2's SOCLiResultCodeInConnecting
static s32 data_ov009_021a1ce0 = SOCL_EINPROGRESS;

// Whether an address is a broadcast one: the all-ones address, or the network's with every host bit set
static inline BOOL SOCLi_IsBroadcast(CPSInAddr ip) {
    if (((data_ov009_021a82bc ^ ip) & data_ov009_021a827c) != 0) {
        return ip == 0xffffffff;
    } else {
        u32 host = ~data_ov009_021a827c;

        return host == (ip & host);
    }
}

// pokebw2's SOCL_Bind
static inline BOOL SOCLi_SocketIsUDP(SOCLSocket *socket) {
    return socket->flag == 1;
}
static inline BOOL SOCLi_SocketIsConnecting(SOCLSocket *socket) {
    return (s32)socket->state & (s32)SOCLI_STATE_BINDING;
}
int func_ov009_0215d43c(int s, u16 local_port) {
    SOCLSocket *socket = (SOCLSocket *)s;

    if (func_ov009_0215f3d0(socket)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (socket->state & SOCLI_STATE_BINDING) {
        return SOCL_EALREADY;
    }
    socket->local_port = local_port;
    return socket->flag == 1 ? func_ov009_0215d5f8(socket) : 0;
}

// pokebw2's SOCL_Connect
int func_ov009_0215d4ac(int s, u16 remote_port, CPSInAddr remote_ip) {
    SOCLSocket *socket = (SOCLSocket *)s;
    int result;

    if (func_ov009_0215f3d0(socket) || (socket->state & SOCLI_STATE_CLOSING)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (SOCLi_SocketIsTCP(socket)) {
        if ((remote_ip & 0xf0000000) == 0xe0000000 || SOCLi_IsBroadcast(remote_ip)) {
            return SOCL_EINVAL;
        }
        if (socket->state & SOCLI_STATE_CONNECTED) {
            if (socket->flag_block == 1) {
                return SOCL_EISCONN;
            }
            return 0;
        }
        if (socket->state & SOCLI_STATE_BINDING) {
            if (socket->state & SOCLI_STATE_CONNECT_FAILED) {
                return socket->result;
            }
            return data_ov009_021a1ce0;
        }
        socket->remote_port = remote_port;
        socket->remote_ip = remote_ip;
        result = func_ov009_0215d5f8(socket);
        if (socket->flag_block == 1 || result < 0) {
            return result;
        }
        return SOCL_EINPROGRESS;
    }
    socket->remote_port = remote_port;
    socket->remote_ip = remote_ip;
    return 0;
}

// pokebw2's SOCLi_ExecBindCommand
static int func_ov009_0215d5f8(SOCLSocket *socket) {
    SOCLCommandPacket *packet = func_ov009_0215ce3c(func_ov009_0215d650, socket, socket->flag_block);

    if (packet == NULL) {
        return SOCL_EMFILE;
    }
    packet->args.bind.local_port = socket->local_port;
    packet->args.bind.remote_port = socket->remote_port;
    packet->args.bind.remote_ip = socket->remote_ip;
    socket->state |= SOCLI_STATE_BINDING;
    return func_ov009_0215cf88(socket, packet);
}

// pokebw2's SOCLi_BindCallBack
static int func_ov009_0215d650(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    SOCLRecvPipe *pipe = socket->recv_pipe;
    int result = 0;

    OS_LockMutex(&pipe->base.mutex);
    func_ov009_0215aab0(packet->args.bind.local_port, packet->args.bind.remote_port, packet->args.bind.remote_ip);
    pipe->cps_read_len = 0;
    if (packet->socket_flag == 0 || packet->socket_flag == 4) {
        result = func_ov009_0215ac60();
    }
    OS_UnlockMutex(&pipe->base.mutex);
    if (result != 0) {
        socket->state |= SOCLI_STATE_CONNECT_FAILED;
        return SOCL_ETIMEDOUT;
    }
    socket->state |= SOCLI_STATE_CONNECTED;
    return 0;
}
