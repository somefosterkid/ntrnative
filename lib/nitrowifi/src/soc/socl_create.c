// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_create.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "gfl/std.h"
#include "nitro/os.h"

// SOCL's socket creation: func_ov009_0215d070 gets the memory for a socket and its pipes and buffers (their sizes come
// from a SOCLSocketParam), then has the socket's control pipe register it with CPS. pokediamond's NitroWiFi names the
// object socl_create.o and every function in it (and has the listen and accept calls here, which this version lacks).
static int func_ov009_0215d124(SOCLCommandPacket *packet);
static SOCLSocket *func_ov009_0215d198(const SOCLSocketParam *param);
static u32 func_ov009_0215d200(const SOCLSocketParam *param);
static u32 func_ov009_0215d280(const SOCLPipeParam *param);
static u8 *func_ov009_0215d2a8(SOCLSocket *socket, const SOCLSocketParam *param);
static u8 *func_ov009_0215d3a0(u8 *data, SOCLSocketBuffer *buffer, u32 size);
static u8 *func_ov009_0215d3c8(u8 *data, SOCLCommandPipe *pipe, const SOCLPipeParam *param);

// pokebw2's SOCL_CreateSocket
int func_ov009_0215d070(const SOCLSocketParam *param) {
    SOCLSocket *socket;

    if (data_ov009_021a1cd8.config == NULL) {
        return SOCL_ENETRESET;
    }
    socket = func_ov009_0215d198(param);
    if (socket == NULL) {
        return SOCL_ENOMEM;
    }
    func_ov009_0215cfa8(socket, func_ov009_0215ce3c(func_ov009_0215d124, socket, 1));
    if (SOCLi_SocketIsCreated(socket)) {
        if (SOCLi_SocketIsTCP(socket)) {
            data_ov009_021aa4ac[0]++;
        } else {
            data_ov009_021aa4ac[1]++;
        }
    }
    return (int)socket;
}

// pokebw2's SOCLi_CreateSocketCallBack
static int func_ov009_0215d124(SOCLCommandPacket *packet) {
    SOCLSocket *socket = packet->socket;
    SOCLSendPipe *send_pipe;

    func_ov009_0215aa5c((CPSSoc *)socket);
    send_pipe = socket->send_pipe;
    switch (socket->flag) {
    case 0:
    case 4:
        func_ov009_0215ab7c(&send_pipe->base.thread);
        func_ov009_0215ab10();
        break;
    case 1:
        func_ov009_0215ab10();
        func_ov009_0215aa88();
        func_ov009_0215ab94(func_ov009_0215df08);
        break;
    case 2:
        func_ov009_0215aa88();
        break;
    }
    socket->state = SOCLI_STATE_CREATED;
    return 0;
}

// pokebw2's SOCLi_StartupSocket
static SOCLSocket *func_ov009_0215d198(const SOCLSocketParam *param) {
    SOCLSocket *socket;
    u32 size;
    u32 irq;

    size = func_ov009_0215d200(param);
    irq = OS_DisableInterrupts();
    socket = SOCLi_Alloc(size);

    if (socket != NULL) {
        MI_CpuFill8(socket, 0, size);
        func_ov009_0215d2a8(socket, param);
        func_ov009_0215f2dc(socket);
    }
    OS_RestoreInterrupts(irq);
    return socket;
}

// pokebw2's SOCLi_GetSizeSocket
static u32 func_ov009_0215d200(const SOCLSocketParam *param) {
    u32 size = sizeof(SOCLSocket);

    if (param->buffer.rcvbuf_size != 0) {
        size += sizeof(SOCLRecvPipe);
        size += func_ov009_0215f0c8(param->buffer.rcvbuf_size);
        size += func_ov009_0215f0c8(param->buffer.rcvbuf_aux_size);
        size += func_ov009_0215d280(&param->rcv_pipe);
    }
    if (param->buffer.sndbuf_size != 0) {
        size += sizeof(SOCLSendPipe);
        size += func_ov009_0215f0c8(param->buffer.sndbuf_size);
        size += func_ov009_0215f0c8(param->buffer.sndbuf_aux_size);
        size += func_ov009_0215f0c8(param->buffer.sndpipe_buf_size);
        size += func_ov009_0215d280(&param->snd_pipe);
    }
    return size;
}

// pokebw2's SOCLi_GetSizeCommandPipe
static u32 func_ov009_0215d280(const SOCLPipeParam *param) {
    return func_ov009_0215f0c8(param->queue_length * sizeof(OSMessage)) + func_ov009_0215f0c8(param->stack_size);
}

// pokebw2's SOCLi_InitSocket
static u8 *func_ov009_0215d2a8(SOCLSocket *socket, const SOCLSocketParam *param) {
    u8 *data = (u8 *)socket + sizeof(SOCLSocket);

    socket->flag = param->flag_mode;
    socket->flag_block = param->flag_block;
    if (param->buffer.rcvbuf_size != 0) {
        SOCLRecvPipe *pipe = (SOCLRecvPipe *)data;

        socket->recv_pipe = pipe;
        pipe->rcvbuf_consume_min = param->buffer.rcvbuf_consume_min;
        data = func_ov009_0215d3c8(data + sizeof(SOCLRecvPipe), &pipe->base, &param->rcv_pipe);
        data = func_ov009_0215d3a0(data, &socket->rcvbuf, param->buffer.rcvbuf_size);
        data = func_ov009_0215d3a0(data, &socket->rcvbuf_aux, param->buffer.rcvbuf_aux_size);
        pipe->udp_queued_max = param->buffer.rcvpipe_unk_e;
        pipe->udp_wait.head = pipe->udp_wait.tail = NULL;
    }
    if (param->buffer.sndbuf_size != 0) {
        SOCLSendPipe *pipe = (SOCLSendPipe *)data;

        socket->send_pipe = pipe;
        pipe->socket = socket;
        data = func_ov009_0215d3c8(data + sizeof(SOCLSendPipe), &pipe->base, &param->snd_pipe);
        data = func_ov009_0215d3a0(data, &socket->sndbuf, param->buffer.sndbuf_size);
        data = func_ov009_0215d3a0(data, &socket->sndbuf_aux, param->buffer.sndbuf_aux_size);
        data = func_ov009_0215d3a0(data, &pipe->buffer, param->buffer.sndpipe_buf_size);
        pipe->wait.head = pipe->wait.tail = NULL;
    } else {
        socket->send_pipe = data_ov009_021aa44c->send_pipe;
    }
    return data;
}

// pokebw2's SOCLi_InitSocketBuffer
static u8 *func_ov009_0215d3a0(u8 *data, SOCLSocketBuffer *buffer, u32 size) {
    u8 *data2;

    data2 = data;
    if (size == 0) {
        data2 = NULL;
    }
    buffer->size = size;
    buffer->data = data2;
    return data + func_ov009_0215f0c8(size);
}

// pokebw2's SOCLi_InitCommandPipe
static u8 *func_ov009_0215d3c8(u8 *data, SOCLCommandPipe *pipe, const SOCLPipeParam *param) {
    u32 size = func_ov009_0215d280(param);

    OS_InitMessageQueue(&pipe->queue, (OSMessage *)data, param->queue_length);
    OS_InitMutex(&pipe->mutex);
    OS_CreateThread(&pipe->thread, (void (*)(void *))func_ov009_0215cfc0, pipe, data + size, param->stack_size, param->priority);
    OS_WakeupThreadDirect(&pipe->thread);
    return data + size;
}
