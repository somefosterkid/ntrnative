// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_close.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "nitro/os.h"

// func_ov009_0215e84c: marks the socket closing and queues func_ov009_0215e980 in its control pipe, which shuts the connection
// down and moves the socket to the trash list. func_ov009_0215ebb4 (CPS's scavenger callback) cleans the trash up:
// func_ov009_0215ea1c frees the socket's pipes (func_ov009_0215eb14, which answers the commands still queued with
// an error) and then the socket. pokediamond's NitroWiFi names the object socl_close.o and its functions, except the
// two that ask about the socket's state, which this version added.

static int func_ov009_0215e980(SOCLCommandPacket *packet);
static void func_ov009_0215ea1c(SOCLSocket *socket);
static void func_ov009_0215eb14(SOCLCommandPipe *pipe);

// pokebw2's SOCL_IsClosed
BOOL func_ov009_0215e794(int socket) {
    if (socket >= 0 && func_ov009_0215f3d0((SOCLSocket *)socket) && !func_ov009_0215f404((SOCLSocket *)socket)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov009_0215e7c8(int socket) {
    SOCLSocket *s = (SOCLSocket *)socket;
    u32 irq;

    if (socket <= 0) {
        return FALSE;
    }
    irq = OS_DisableInterrupts();
    if (func_ov009_0215f404(s)) {
        OS_RestoreInterrupts(irq);
        return FALSE;
    }
    if (func_ov009_0215f3d0(s)) {
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (s->state & SOCLI_STATE_CLOSE_CALLED) {
        OS_RestoreInterrupts(irq);
        return FALSE;
    }
    OS_RestoreInterrupts(irq);
    return TRUE;
}

// pokebw2's SOCL_Close
int func_ov009_0215e84c(int socket) {
    SOCLSocket *s = (SOCLSocket *)socket;
    SOCLCommandPacket *packet;
    u32 irq;

    if (socket <= 0) {
        return SOCL_EINVAL;
    }
    irq = OS_DisableInterrupts();
    if (func_ov009_0215f404(s)) {
        OS_RestoreInterrupts(irq);
        return SOCL_EINPROGRESS;
    }
    if (func_ov009_0215f3d0(s)) {
        OS_RestoreInterrupts(irq);
        return 0;
    }
    if (!SOCLi_SocketIsCreated(s)) {
        OS_RestoreInterrupts(irq);
        return SOCL_ENETRESET;
    }
    if (s->state & SOCLI_STATE_CLOSE_CALLED) {
        OS_RestoreInterrupts(irq);
        return SOCL_EINPROGRESS;
    }
    OS_RestoreInterrupts(irq);
    s->state |= SOCLI_STATE_CLOSING | SOCLI_STATE_CLOSE_CALLED;
    if (SOCLi_SocketIsTCP(s)) {
        func_ov009_0215ceb0(&s->send_pipe->base, NULL);
        data_ov009_021aa4ac[0]--;
    } else {
        data_ov009_021aa4ac[1]--;
    }
    packet = func_ov009_0215ce3c(func_ov009_0215e980, s, 1);
    packet->reply_queue = NULL;
    func_ov009_0215cefc(s, packet);
    return 0;
}

// pokebw2's SOCLi_CloseCallBack
static int func_ov009_0215e980(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    u32 irq;

    if (SOCLi_SocketIsTCP(socket)) {
        OS_JoinThread(&socket->send_pipe->base.thread);
        func_ov009_0215ad00();
        func_ov009_0215ad3c();
        func_ov009_0215ab5c();
    }
    func_ov009_0215aa70();
    socket->state &= ~(SOCLI_STATE_BINDING | SOCLI_STATE_CONNECTED);
    func_ov009_0215ceb0(socket->flag == 2 ? &socket->send_pipe->base : &socket->recv_pipe->base, NULL);
    irq = OS_DisableInterrupts();
    func_ov009_0215f31c(socket);
    func_ov009_0215f304(socket);
    OS_RestoreInterrupts(irq);
    socket->state |= SOCLI_STATE_CLOSED;
    return 0;
}

// pokebw2's SOCLi_CleanupSocket
static void func_ov009_0215ea1c(SOCLSocket *socket) {
    BOOL is_tcp;
    s8 flag;
    u32 irq;

    if (socket == NULL) {
        return;
    }
    socket->state = 0;
    flag = socket->flag;
    is_tcp = TRUE;
    if (flag != 0 && flag != 4) {
        is_tcp = FALSE;
    }
    if (is_tcp) {
        func_ov009_0215eb14(&socket->send_pipe->base);
        func_ov009_0215eb14(&socket->recv_pipe->base);
    } else if (flag == 1) {
        SOCLUdpPacket *udp = socket->recv_pipe->udp_head;

        while (udp != NULL) {
            SOCLUdpPacket *next = udp->next;

            data_ov009_021a1cd8.config->free(udp);
            udp = next;
        }
        socket->recv_pipe->udp_queued = 0;
        socket->recv_pipe->udp_tail = NULL;
        socket->recv_pipe->udp_head = NULL;
        OS_WakeupThread(&socket->recv_pipe->udp_wait);
        func_ov009_0215eb14(&socket->recv_pipe->base);
    } else if (flag == 2) {
        func_ov009_0215eb14(&socket->send_pipe->base);
    }
    irq = OS_DisableInterrupts();
    func_ov009_0215f31c(socket);
    func_ov009_0215f3b8(socket);
    data_ov009_021a1cd8.config->free(socket);
    OS_RestoreInterrupts(irq);
}

// Waits for the pipe's thread to end, and answers the commands that it never ran with an error
// pokebw2's SOCLi_FreeCommandPipe
static void func_ov009_0215eb14(SOCLCommandPipe *pipe) {
    OSMessage message;
    u32 irq;

    if (pipe == NULL) {
        return;
    }
    OS_JoinThread(&pipe->thread);
    irq = OS_DisableInterrupts();
    OS_DisableScheduler();
    if (OS_ReceiveMessage(&pipe->queue, &message, OS_MESSAGE_NOBLOCK)) {
        do {
            if (message != NULL) {
                if (((SOCLCommandPacket *)message)->reply_queue != NULL) {
                    OS_SendMessage(((SOCLCommandPacket *)message)->reply_queue, (OSMessage)SOCL_ECANCELED,
                                   OS_MESSAGE_NOBLOCK);
                }
                func_ov009_0215ce7c(message);
            }
        } while (OS_ReceiveMessage(&pipe->queue, &message, OS_MESSAGE_NOBLOCK));
    }
    OS_EnableScheduler();
    OS_RescheduleThread();
    OS_RestoreInterrupts(irq);
}

// pokebw2's SOCLi_TrashSocket
void func_ov009_0215ebb4(void) {
    u32 irq = OS_DisableInterrupts();

    while (data_ov009_021aa4b8 != NULL) {
        func_ov009_0215ea1c(data_ov009_021aa4b8);
    }
    OS_RestoreInterrupts(irq);
}
