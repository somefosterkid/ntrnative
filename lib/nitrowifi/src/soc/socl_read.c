// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_read.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/math.h"

// SOCL's reads: func_ov009_0215d6c8 takes data from a socket, under its receive pipe's mutex. A TCP socket reads from
// CPS's receive buffer (func_ov009_0215d92c) or, when it has to wait for data, through a command in its receive pipe
// (func_ov009_0215db0c); a UDP socket reads the datagrams that func_ov009_0215df08 queued in its pipe
// (func_ov009_0215ddc4). What was read from CPS's buffer is consumed (func_ov009_0215b064) once it passes the socket's
// minimum. pokediamond's NitroWiFi names the object socl_read.o and every function in it, except the callback that
// reads without waiting, which this version added. Its write half is socl_write.c here.
static int func_ov009_0215d7fc(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip,
                                      int block, int flags);
static int func_ov009_0215d898(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int block);
static int func_ov009_0215d92c(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip);
static u8 *func_ov009_0215da34(SOCLSocket *socket, int *avail, u16 *local_port, u16 *remote_port, CPSInAddr *remote_ip);
static int func_ov009_0215daa8(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip,
                                 int block);
static int func_ov009_0215db0c(SOCLCommandPacket *packet);
static int func_ov009_0215dc5c(SOCLCommandPacket *packet);
static int func_ov009_0215dd28(SOCLSocket *socket);
static int func_ov009_0215dd78(SOCLCommandPacket *packet);
static int func_ov009_0215dd88(SOCLSocket *socket);
static int func_ov009_0215ddc4(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int block);

// How often the UDP queue had no memory for a datagram, and how often it was full
// pokebw2's SOCLi_CheckCount
static s32 data_ov009_021aa4a4[2];

// pokebw2's SOCLi_ReadFrom
int func_ov009_0215d6c8(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int flags) {
    u8 block;
    int result;
    SOCLRecvPipe *pipe;

    if (func_ov009_0215f3d0(socket)) {
        return SOCL_EINVAL;
    }
    if ((flags & SOCLI_READ_NOBLOCK) || socket->flag_block == 0) {
        block = 0;
    } else {
        if (OS_GetProcMode() == 0x12) {
            return SOCL_EINVAL;
        }
        block = 1;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (SOCLi_SocketIsTCP(socket)) {
        if (!(socket->state & SOCLI_STATE_CONNECTED) || (socket->state & SOCLI_STATE_CLOSING)) {
            return SOCL_ENOTCONN;
        }
    }
    pipe = socket->recv_pipe;
    if (!(block & 1)) {
        if (!OS_TryLockMutex(&pipe->base.mutex)) {
            return SOCL_EAGAIN;
        }
    } else {
        OS_LockMutex(&pipe->base.mutex);
    }
    result = func_ov009_0215d7fc(socket, buf, len, remote_port, remote_ip, block, flags);
    OS_UnlockMutex(&pipe->base.mutex);
    return result;
}

static int func_ov009_0215d7fc(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip,
                                      int block, int flags) {
    SOCLRecvPipe *pipe = socket->recv_pipe;
    BOOL peek = (flags & SOCLI_READ_PEEK) && pipe != NULL;
    s8 was_peeking;
    int result;

    if (peek) {
        was_peeking = pipe->peek;
        pipe->peek = 1;
    }
    if (socket->flag == 1) {
        result = func_ov009_0215ddc4(socket, buf, len, remote_port, remote_ip, block);
    } else {
        result = func_ov009_0215d898(socket, buf, len, remote_port, remote_ip, block);
        if (result >= 0) {
            func_ov009_0215dd28(socket);
        }
    }
    if (peek) {
        pipe->peek = was_peeking;
    }
    return result;
}

// pokebw2's SOCLi_ReadBuffer
static int func_ov009_0215d898(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int block) {
    int result;

    if (socket->flag == 4) {
        return func_ov009_0215daa8(socket, buf, len, remote_port, remote_ip, block);
    }
    result = func_ov009_0215d92c(socket, buf, len, remote_port, remote_ip);
    if (result == SOCL_EAGAIN && (block & 1)) {
        result = func_ov009_0215daa8(socket, buf, len, remote_port, remote_ip, 1);
    }
    return result;
}

// pokebw2's SOCLi_CopyCPSBuffer
static int func_ov009_0215d92c(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip) {
    OSIntrMode enable;
    SOCLRecvPipe *pipe;
    u8 *data;
    int avail;
    int copy_len;
    int consume_len;
    u16 local_port;
    u16 sender_port;
    CPSInAddr sender_ip;
    int result = 0;

    enable = OS_DisableInterrupts();
    {
        data = func_ov009_0215da34(socket, &avail, &local_port, &sender_port, &sender_ip);

        if (data) {
            if (avail == 0) {
                result = SOCL_EAGAIN;
            } else {
                copy_len = MATH_MIN(len, avail);
                consume_len = SOCLi_SocketIsTCP(socket) ? copy_len : avail;

                sys_memcpy_fast(data, buf, copy_len);
                pipe = socket->recv_pipe;
                if (!pipe->peek) {
                    pipe->cps_read_len += consume_len;
                }
                result = consume_len;
            }
        } else {
            if (avail == 0) {
                result = 0;
            } else {
                result = SOCL_EINVAL;
            }
            socket->state &= ~(SOCLI_STATE_BINDING | SOCLI_STATE_CONNECTED);
        }

        if (result >= 0) {
            if (remote_port && remote_ip) {
                *remote_port = sender_port;
                *remote_ip = sender_ip;
            }
            if (socket->local_port == 0) {
                socket->local_port = local_port;
            }
        }
    }
    OS_RestoreInterrupts(enable);
    return result;
}

// Where CPS's receive buffer stands for the socket: the data not yet read (avail, -1 when CPS has gone ahead of the
// pipe) and the ports and address of its sender. NULL when there is nothing to read or the connection has gone
// pokebw2's SOCLi_ReadCPSBuffer
static u8 *func_ov009_0215da34(SOCLSocket *socket, int *avail, u16 *local_port, u16 *remote_port, CPSInAddr *remote_ip) {
    SOCLRecvPipe *pipe = socket->recv_pipe;
    CPSSoc *soc = (CPSSoc *)pipe->base.thread.specific[0];
    int read_len = pipe->cps_read_len;
    int left = soc->rcvbuf_len - read_len;

    if (left >= 0) {
        *local_port = soc->local_port;
        *remote_port = soc->remote_port;
        *remote_ip = soc->remote_ip;
        *avail = left;
        if (left == 0 && soc->state != CPS_STT_ESTABLISHED) {
            return NULL;
        }
    } else {
        *avail = -1;
        return NULL;
    }
    return soc->rcvbuf + read_len;
}

static int func_ov009_0215daa8(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip,
                                 int block) {
    SOCLCommandPacket *packet;

    if (block & 1) {
        packet = func_ov009_0215ce3c(func_ov009_0215db0c, socket, 1);
    } else {
        packet = func_ov009_0215ce3c(func_ov009_0215dc5c, socket, 1);
    }
    packet->args.read.buf = buf;
    packet->args.read.len = len;
    packet->args.read.remote_port = remote_port;
    packet->args.read.remote_ip = remote_ip;
    return func_ov009_0215cf88(socket, packet);
}

// pokebw2's SOCLi_ReadCallBack
static int func_ov009_0215db0c(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    SOCLRecvPipe *pipe = socket->recv_pipe;
    u8 *buf = packet->args.read.buf;
    u16 *remote_port = packet->args.read.remote_port;
    CPSInAddr *remote_ip;
    u32 read_len = pipe->cps_read_len;
    int len = packet->args.read.len;
    u32 cps_len;
    u8 *data;
    int result;

    remote_ip = packet->args.read.remote_ip;
    while (1) {
        data = func_ov009_0215aeb0(&cps_len);
        if (data != NULL && (int)(cps_len - read_len) <= 0) {
            if (SOCLi_SocketIsTCP(socket) && ((CPSSoc *)socket)->state != CPS_STT_ESTABLISHED) {
                data = NULL;
                break;
            }
            OS_Sleep(10);
            continue;
        }
        break;
    }
    if (socket->state & SOCLI_STATE_RESET) {
        return SOCL_ECONNRESET;
    }
    if (socket->flag == 4) {
        if (data == NULL) {
            return 0;
        }
        if (len > cps_len) {
            len = cps_len;
        }
        sys_memcpy_fast(data, buf, len);
        func_ov009_0215b064(len);
        return len;
    }
    if (data != NULL) {
        result = func_ov009_0215d92c(socket, buf, len, remote_port, remote_ip);
    } else {
        result = 0;
    }
    if (result <= 0) {
        return result;
    }
    if (pipe->cps_read_len >= pipe->rcvbuf_consume_min) {
        func_ov009_0215dd88(socket);
    }
    return result;
}

// The read command for a call that does not wait: it takes what CPS has now
// pokebw2's SOCLi_ReadNoWaitCallBack
static int func_ov009_0215dc5c(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    u8 *buf = packet->args.read.buf;
    int len = packet->args.read.len;
    int read_len = socket->recv_pipe->cps_read_len;
    s32 length;
    u32 cps_len;
    u8 *data;

    if (socket->state & SOCLI_STATE_RESET) {
        return SOCL_ECONNRESET;
    }
    length = func_ov009_0215b504();
    if (length == 0) {
        return SOCL_EAGAIN;
    }
    if (length == -1) {
        return 0;
    }
    data = func_ov009_0215aeb0(&cps_len);
    if (data != NULL && (int)(cps_len - read_len) <= 0) {
        if (SOCLi_SocketIsTCP(socket)) {
            if (((CPSSoc *)socket)->state != CPS_STT_ESTABLISHED) {
                data = NULL;
            }
        }
    }
    if (socket->state & SOCLI_STATE_RESET) {
        return SOCL_ECONNRESET;
    }
    if (data == NULL) {
        return 0;
    }
    if (len > cps_len) {
        len = cps_len;
    }
    sys_memcpy_fast(data, buf, len);
    func_ov009_0215b064(len);
    return len;
}

// pokebw2's SOCLi_ConsumeBuffer
static int func_ov009_0215dd28(SOCLSocket *socket) {
    SOCLRecvPipe *pipe = socket->recv_pipe;
    SOCLCommandPacket *packet;

    if (pipe->cps_read_len < pipe->rcvbuf_consume_min) {
        return 0;
    }
    packet = func_ov009_0215ce3c(func_ov009_0215dd78, socket, 0);
    if (packet != NULL) {
        return func_ov009_0215cf88(socket, packet);
    }
    return SOCL_EMFILE;
}

// pokebw2's SOCLi_ConsumeCallBack
static int func_ov009_0215dd78(SOCLCommandPacket *packet) {
    return func_ov009_0215dd88(packet->socket);
}

// pokebw2's SOCLi_ConsumeCPSBuffer
static int func_ov009_0215dd88(SOCLSocket *socket) {
    SOCLRecvPipe *pipe = socket->recv_pipe;
    u32 irq;
    int len;

    irq = OS_DisableInterrupts();
    len = pipe->cps_read_len;
    if (len != 0) {
        pipe->cps_read_len = 0;
        func_ov009_0215b064(len);
    }
    OS_RestoreInterrupts(irq);
    return len;
}

// pokebw2's SOCLi_ReadUdpBuffer
static int func_ov009_0215ddc4(SOCLSocket *socket, u8 *buf, int len, u16 *remote_port, CPSInAddr *remote_ip, int block) {
    SOCLRecvPipe *pipe = socket->recv_pipe;
    SOCLUdpPacket *packet;
    u32 irq = OS_DisableInterrupts();
    int result;

    packet = pipe->udp_head;
    if (packet == NULL) {
        BOOL wait = block & 1;

        while (1) {
            if (!wait) {
                result = SOCL_EAGAIN;
                break;
            }
            OS_SleepThread(&pipe->udp_wait);
            if (func_ov009_0215f3d0(socket) || !SOCLi_SocketIsCreated(socket)) {
                result = SOCL_ENOTCONN;
                break;
            }
            packet = pipe->udp_head;
            if (packet != NULL) {
                break;
            }
        }
    }
    if (packet != NULL) {
        if (len > packet->len) {
            len = packet->len;
        }
        sys_memcpy_fast(packet->data, buf, len);
        if (remote_port != NULL) {
            *remote_port = packet->port;
        }
        if (remote_ip != NULL) {
            *remote_ip = packet->ip;
        }
        result = packet->len;
        if (pipe->peek == 0) {
            pipe->udp_head = packet->next;
            if (packet->next == NULL) {
                pipe->udp_tail = NULL;
            }
            data_ov009_021a1cd8.config->free(packet);
            pipe->udp_queued -= result;
        }
    }
    OS_RestoreInterrupts(irq);
    return result;
}

// pokebw2's SOCLi_UdpRecvCallback
BOOL func_ov009_0215df08(u8 *data, u32 len, CPSSoc *soc) {
    SOCLSocket *socket = (SOCLSocket *)soc;
    SOCLRecvPipe *pipe = socket->recv_pipe;
    SOCLUdpPacket *packet;
    u32 irq = OS_DisableInterrupts();

    if (pipe->udp_queued_max >= pipe->udp_queued + len) {
        packet = data_ov009_021a1cd8.config->alloc(len + sizeof(SOCLUdpPacket));
        if (packet != NULL) {
            pipe->udp_queued += len;
            packet->next = NULL;
            packet->len = len;
            packet->port = soc->remote_port;
            packet->ip = soc->remote_ip;
            sys_memcpy_fast(data, packet->data, len);
            if (pipe->udp_tail != NULL) {
                pipe->udp_tail->next = packet;
            }
            pipe->udp_tail = packet;
            if (pipe->udp_head == NULL) {
                pipe->udp_head = packet;
            }
        } else {
            data_ov009_021aa4a4[0]++;
        }
    } else {
        data_ov009_021aa4a4[1]++;
    }
    if (socket->local_port == 0) {
        socket->local_port = soc->local_port;
    }
    soc->remote_port = soc->remote_port_bound;
    soc->remote_ip = soc->remote_ip_bound;
    OS_WakeupThread(&pipe->udp_wait);
    OS_RestoreInterrupts(irq);
    return TRUE;
}
