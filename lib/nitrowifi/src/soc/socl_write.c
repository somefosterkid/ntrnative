// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_write.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "gfl/std.h"
#include "nitro/os.h"

// SOCL's writes: func_ov009_0215e024 puts the data in the send pipe's ring buffer (func_ov009_0215e138, which waits for room
// when the socket blocks) and queues a command (func_ov009_0215e2e4) for the pipe's thread, which copies it to
// the socket's send buffer in packets and has CPS send it (func_ov009_0215e450). pokediamond's NitroWiFi has this in
// the object socl_read.o, where this version has the write half in an object of its own.

static int func_ov009_0215e138(SOCLSocket *socket, const u8 *buf, int len, u16 remote_port, CPSInAddr remote_ip,
                             int block);
static int func_ov009_0215e23c(SOCLSocket *socket, int len, int chunk, int *write_pos, int block);
static int func_ov009_0215e2e4(SOCLSocket *socket, const u8 *buf, int len, int pos, u16 remote_port,
                                  CPSInAddr remote_ip, int block);
static int func_ov009_0215e450(SOCLCommandPacket *packet);
static int func_ov009_0215e588(int len);
static int func_ov009_0215e620(u8 *dst, int len, SOCLCommandPacket *packet);

// pokebw2's SOCL_WriteTo
int func_ov009_0215e024(SOCLSocket *socket, const u8 *buf, int len, u16 remote_port, CPSInAddr remote_ip, int flags) {
    SOCLSendPipe *pipe;
    int block;
    int result;

    if (func_ov009_0215f3d0(socket)) {
        return SOCL_EINVAL;
    }
    if (!SOCLi_SocketIsCreated(socket)) {
        return SOCL_ENETRESET;
    }
    if (SOCLi_SocketIsTCP(socket)) {
        if (!(socket->state & SOCLI_STATE_CONNECTED) || (socket->state & SOCLI_STATE_CLOSING)) {
            return SOCL_ENOTCONN;
        }
    }
    pipe = socket->send_pipe;
    if ((flags & SOCLI_WRITE_NOBLOCK) || socket->flag_block == 0) {
        if (!OS_TryLockMutex(&pipe->base.mutex)) {
            return SOCL_EAGAIN;
        }
        block = 0;
    } else {
        OS_LockMutex(&pipe->base.mutex);
        block = 1;
    }
    result = func_ov009_0215e138(socket, buf, len, remote_port, remote_ip, block);
    OS_UnlockMutex(&pipe->base.mutex);
    return result;
}

static int func_ov009_0215e138(SOCLSocket *socket, const u8 *buf, int len, u16 remote_port, CPSInAddr remote_ip,
                             int block) {
    SOCLSendPipe *pipe = socket->send_pipe;
    int chunk_max;
    int write_pos;
    int alloc_len;
    int chunk_min;
    int written = 0;
    int result;

    chunk_max = pipe->socket->sndbuf.size;
    if (socket->flag == 1) {
        chunk_max -= 0x2a;
        if (len > chunk_max) {
            return SOCL_EMSGSIZE;
        }
        chunk_min = len;
    } else {
        chunk_max -= 0x36;
        chunk_min = len > chunk_max ? chunk_max : len;
    }
    while (len > 0) {
        alloc_len = func_ov009_0215e23c(socket, len, chunk_min, &write_pos, block);
        if (alloc_len > 0) {
            result = func_ov009_0215e2e4(socket, buf, alloc_len, write_pos, remote_port, remote_ip, block);
            if (result <= 0) {
                return SOCL_EAGAIN;
            }
            buf += alloc_len;
            len -= alloc_len;
            written += alloc_len;
        }
        if (!(block & 1)) {
            if (alloc_len <= 0) {
                return SOCL_EAGAIN;
            }
            break;
        }
    }
    return written;
}

// Finds room for up to len bytes (at least chunk) in the pipe's buffer, sleeping for it when the socket blocks, and
// gives the position to write at
// pokebw2's SOCLi_AllocWriteBuffer
static int func_ov009_0215e23c(SOCLSocket *socket, int len, int chunk, int *write_pos, int block) {
    SOCLSendPipe *pipe = socket->send_pipe;
    int free;
    u32 irq;

    if (chunk > len) {
        chunk = len;
    }
    irq = OS_DisableInterrupts();
    while (1) {
        free = func_ov009_0215e2c0(socket);
        if (free >= chunk) {
            if (free >= len) {
                free = len;
            }
            *write_pos = pipe->write_pos;
            break;
        }
        if (!(block & 1)) {
            free = 0;
            break;
        }
        OS_SleepThread(&pipe->wait);
    }
    OS_RestoreInterrupts(irq);
    return free;
}

// pokebw2's SOCLi_GetWriteBufferFreeSize
int func_ov009_0215e2c0(const SOCLSocket *socket) {
    SOCLSendPipe *pipe = socket->send_pipe;
    int size = pipe->buffer.size;
    int write_pos = pipe->write_pos;
    int read_pos = pipe->read_pos;
    int free = read_pos - write_pos - 1;

    if (free < 0) {
        free += size;
    }
    return free;
}

static int func_ov009_0215e2e4(SOCLSocket *socket, const u8 *buf, int len, int pos, u16 remote_port,
                                  CPSInAddr remote_ip, int block) {
    SOCLSendPipe *pipe = socket->send_pipe;
    SOCLCommandPacket *packet = func_ov009_0215ce3c(func_ov009_0215e450, pipe->socket, block);
    u16 old_write_pos;
    u8 *data;
    int size;

    if (packet == NULL) {
        return SOCL_EMFILE;
    }
    if ((block & 1) && socket->flag != 1) {
        packet->flag_block = 3;
    } else {
        packet->flag_block = 0;
    }
    data = pipe->buffer.data;
    size = pipe->buffer.size;
    if (pos + len < size) {
        packet->args.write.buf1 = data + pos;
        packet->args.write.len1 = len;
        packet->args.write.buf2 = NULL;
        packet->args.write.len2 = 0;
        pos += len;
    } else {
        packet->args.write.buf1 = data + pos;
        packet->args.write.len1 = size - pos;
        packet->args.write.buf2 = data;
        packet->args.write.len2 = len - packet->args.write.len1;
        pos = packet->args.write.len2;
        sys_memcpy_fast(buf + packet->args.write.len1, packet->args.write.buf2, pos);
    }
    sys_memcpy_fast(buf, packet->args.write.buf1, packet->args.write.len1);
    old_write_pos = pipe->write_pos;
    packet->args.write.write_end = pos;
    pipe->write_pos = packet->args.write.write_end;
    if (socket->flag == 1) {
        if (socket->local_port == 0) {
            socket->local_port = func_ov009_0215a970();
            ((CPSSoc *)socket)->local_port = socket->local_port;
        }
        packet->args.write.local_port = socket->local_port;
        if (socket->remote_ip == 0 || remote_ip != 0) {
            packet->args.write.remote_ip = remote_ip;
            packet->args.write.remote_port = remote_port;
        } else {
            packet->args.write.remote_ip = socket->remote_ip;
            packet->args.write.remote_port = socket->remote_port;
        }
    } else {
        packet->args.write.remote_ip = 0;
    }
    if (func_ov009_0215cf98(pipe->socket, packet) != 0) {
        pipe->write_pos = old_write_pos;
        len = 0;
    }
    return len;
}

// pokebw2's SOCLi_WriteCallBack
static int func_ov009_0215e450(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    SOCLSendPipe *pipe = socket->send_pipe;
    u8 *sndbuf;
    int sndbuf_len;
    int sndbuf_offset;
    int copied = 0;
    int result;

    if (socket->flag == 2 && (socket->state & SOCLI_STATE_CLOSING)) {
        return SOCL_EINVAL;
    }
    if ((socket->flag != 0 && socket->flag != 4) || (socket->state & SOCLI_STATE_CONNECTED)) {
        if (packet->args.write.remote_ip != 0) {
            func_ov009_0215aab0(packet->args.write.local_port, packet->args.write.remote_port, packet->args.write.remote_ip);
        }
        sndbuf_offset = SOCLi_SocketIsTCP(socket) ? 0x36 : 0x2a;
        sndbuf = socket->sndbuf.data + sndbuf_offset;
        if (SOCLi_SocketIsTCP(socket)) {
            sndbuf_len = func_ov009_0215e588(socket->sndbuf.size - sndbuf_offset);
        } else {
            sndbuf_len = socket->sndbuf.size - sndbuf_offset;
        }
        for (;;) {
            result = func_ov009_0215e620(sndbuf, sndbuf_len, packet);
            if (result <= 0) {
                break;
            }
            result = func_ov009_0215b470(sndbuf, result);
            if (result <= 0) {
                if (SOCLi_SocketIsTCP(socket)) {
                    socket->state &= ~(SOCLI_STATE_BINDING | SOCLI_STATE_CONNECTED | SOCLI_STATE_CLOSING);
                    socket->state |= SOCLI_STATE_RESET;
                    func_ov009_0215ad3c();
                }
                copied = SOCL_ETIMEDOUT;
                break;
            }
            copied += result;
        }
    } else {
        copied = SOCL_ETIMEDOUT;
    }
    pipe->read_pos = packet->args.write.write_end;
    OS_WakeupThread(&pipe->wait);
    return copied;
}

// The largest size that fits a whole number of the connection's segments, for TCP
// pokebw2's SOCLi_GetOptimumSendBufLen
static int func_ov009_0215e588(int len) {
    CPSSoc *soc = (CPSSoc *)OS_GetCurrentThread()->specific[0];
    u32 irq = OS_DisableInterrupts();
    int unit;
    int mss;
    int win;
    int mymss;
    int count;

    if (soc != NULL) {
        mss = soc->remote_mss;
        if (mss != 0 && (win = soc->remote_win) != 0) {
            mymss = data_ov009_021aa450.mymss;
            if (mss > win) {
                mss = win;
            }
            if (mss > mymss) {
                mss = mymss;
            }
            unit = mss * 2;
        } else {
            unit = data_ov009_021aa450.mymss * 2;
        }
    } else {
        unit = data_ov009_021aa450.mymss * 2;
    }
    OS_RestoreInterrupts(irq);
    if (unit > 0) {
        count = len / unit;
        if (count > 0) {
            return unit * count;
        }
    }
    return len;
}

// Copies up to len bytes of the command's data (in two parts when it wraps around the pipe's buffer) to dst, and
// takes them off the command
// pokebw2's SOCLi_MemCpy
static int func_ov009_0215e620(u8 *dst, int len, SOCLCommandPacket *packet) {
    int len1 = packet->args.write.len1;
    int len2 = packet->args.write.len2;

    if (len1 > len) {
        len1 = len;
        len2 = 0;
    } else if (len2 > len - len1) {
        len2 = len - len1;
    }
    if (len1 > 0) {
        sys_memcpy_fast(packet->args.write.buf1, dst, len1);
        packet->args.write.buf1 += len1;
        packet->args.write.len1 -= len1;
    }
    if (len2 > 0) {
        sys_memcpy_fast(packet->args.write.buf2, dst + len1, len2);
        packet->args.write.buf2 += len2;
        packet->args.write.len2 -= len2;
    }
    return len1 + len2;
}
