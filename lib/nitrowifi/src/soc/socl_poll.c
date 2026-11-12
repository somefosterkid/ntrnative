// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_poll.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "nitro/os.h"

// func_ov009_0215efb0: what a socket can do now (data to read, room to write, an error, not connected). pokediamond's
// NitroWiFi names the object socl_poll.o and its functions.

static int func_ov009_0215f080(const SOCLSocket *socket);

// pokebw2's SOCL_GetStatus
int func_ov009_0215efb0(SOCLSocket *socket) {
    int status = 0;
    u32 irq;

    if (func_ov009_0215f3d0(socket)) {
        status |= SOCL_STATUS_INVALID;
    } else {
        if (socket->state & SOCLI_STATE_CONNECT_FAILED) {
            status |= SOCL_STATUS_ERROR;
        }
        if (socket->flag == 1 || (socket->state & SOCLI_STATE_CONNECTED)) {
            irq = OS_DisableInterrupts();
            if (func_ov009_0215f080(socket) > 0) {
                status |= SOCL_STATUS_READABLE;
            }
            if (func_ov009_0215e2c0(socket) > 0) {
                status |= SOCL_STATUS_WRITABLE;
            }
            OS_RestoreInterrupts(irq);
        }
        if (SOCLi_SocketIsTCP(socket)) {
            if ((socket->state & SOCLI_STATE_CONNECTED) && socket->cps_state != CPS_STT_ESTABLISHED &&
                !(status & SOCL_STATUS_READABLE)) {
                socket->state &= ~(SOCLI_STATE_BINDING | SOCLI_STATE_CONNECTED);
            }
            if (!(socket->state & SOCLI_STATE_BINDING) && !(socket->state & SOCLI_STATE_CONNECTED)) {
                status |= SOCL_STATUS_NOTCONN;
            }
        }
    }
    return status;
}

// How much a read could take now: the next datagram's size for UDP, the buffered data for TCP
// pokebw2's SOCLi_GetReadBufferOccpiedSize
static int func_ov009_0215f080(const SOCLSocket *socket) {
    s8 val;
    s32 val2;
    u32 val3;

    val2 = *(s32 *)((u8 *)socket + 0xa0);
    val3 = 0;
    if (val2 != 0) {
        val = *(s8 *)((u8 *)socket + 0xaf);
        if (val == 1) {
            if (*(s32 *)(val2 + 0x104) != 0) {
                val3 = (u32)*(u16 *)(*(s32 *)(val2 + 0x104) + 4);
            }
        }
        else if (val == 0 || val == 4) {
            val3 = *(s32 *)((u8 *)socket + 0x50) - *(s32 *)(val2 + 0xf8);
        }
    }
    return val3;
}
