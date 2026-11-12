// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_command.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "nitro/os.h"

// SOCL's command packets: the socket calls do not run in the caller's thread but as packets that a command pipe's
// thread takes from its queue and runs, one at a time. This file keeps the pool of packets (a queue of the free ones)
// and sends the packets to the pipes. pokediamond's NitroWiFi names the object socl_command.o and every function in it.

static int func_ov009_0215cf14(SOCLCommandPipe *pipe, SOCLCommandPacket *packet);

// pokebw2's SOCLiCommandPackets
static SOCLCommandPacket *data_ov009_021aa480;
// pokebw2's SOCLiCommandPacketQueue
static OSMessageQueue data_ov009_021aa484;

// pokebw2's SOCLi_StartupCommandPacketQueue
int func_ov009_0215cd3c(u32 max) {
    int packets_offset = (max * sizeof(OSMessage) + 3) & ~3;
    long packets_size = (max * sizeof(SOCLCommandPacket) + 3) & ~3;
    SOCLCommandPacket *packets = (SOCLCommandPacket *)SOCLi_Alloc(packets_size + packets_offset);
    u8 *packet;

    if (packets == NULL) {
        return -1;
    }
    OS_InitMessageQueue(&data_ov009_021aa484, (OSMessage *)packets, max);
    packet = (u8 *)packets + packets_offset;
    while ((s32)max > 0) {
        func_ov009_0215ce7c((SOCLCommandPacket *)packet);
        max--;
        packet += sizeof(SOCLCommandPacket);
    }
    data_ov009_021aa480 = packets;
    return 0;
}

// pokebw2's SOCLi_CleanupCommandPacketQueue
int func_ov009_0215cdd0(void) {
    if (data_ov009_021aa484.usedCount < data_ov009_021aa484.msgCount) {
        return -1;
    }
    SOCLi_Free(data_ov009_021aa480);
    data_ov009_021aa480 = NULL;
    return 0;
}

// pokebw2's SOCLi_AllocCommandPacket
SOCLCommandPacket *func_ov009_0215ce14(s32 flags) {
    SOCLCommandPacket *packet;

    if (OS_ReceiveMessage(&data_ov009_021aa484, (OSMessage *)&packet, flags)) {
        return packet;
    }
    return NULL;
}

// pokebw2's SOCLi_CreateCommandPacket
SOCLCommandPacket *func_ov009_0215ce3c(SOCLCommandHandler handler, SOCLSocket *socket, s32 flag_block) {
    SOCLCommandPacket *packet = func_ov009_0215ce14(flag_block);

    if (packet != NULL) {
        packet->handler = handler;
        packet->socket = socket;
        packet->reply_queue = NULL;
        packet->socket_flag = socket->flag;
        packet->flag_block = flag_block;
    }
    return packet;
}

// pokebw2's SOCLi_FreeCommandPacket
void func_ov009_0215ce7c(SOCLCommandPacket *packet) {
    if (packet != NULL) {
        OS_SendMessage(&data_ov009_021aa484, packet, OS_MESSAGE_NOBLOCK);
    }
}

// pokebw2's SOCLi_GetCtrlPipe
SOCLCommandPipe *func_ov009_0215ce9c(SOCLSocket *socket) {
    SOCLCommandPipe *pipe = &socket->recv_pipe->base;

    if (pipe == NULL) {
        pipe = &socket->send_pipe->base;
    }
    return pipe;
}

// pokebw2's SOCLi_SendCommandPacket
int func_ov009_0215ceb0(SOCLCommandPipe *pipe, SOCLCommandPacket *packet) {
    BOOL sent = OS_SendMessage(&pipe->queue, packet, packet == NULL || (packet->flag_block & 1) ? OS_MESSAGE_BLOCK : OS_MESSAGE_NOBLOCK);

    if (!sent) {
        func_ov009_0215ce7c(packet);
    }
    return sent ? 0 : -42;
}

// pokebw2's SOCLi_SendCommandPacketToCtrlPipe
int func_ov009_0215cefc(SOCLSocket *socket, SOCLCommandPacket *packet) {
    return func_ov009_0215ceb0(func_ov009_0215ce9c(socket), packet);
}

// pokebw2's SOCLi_ExecCommandPacket
static int func_ov009_0215cf14(SOCLCommandPipe *pipe, SOCLCommandPacket *packet) {
    OSMessage result;
    OSMessage message[1];
    OSMessageQueue queue;

    if (packet->flag_block == 1) {
        OS_InitMessageQueue(&queue, message, 1);
        packet->reply_queue = &queue;
        func_ov009_0215ceb0(pipe, packet);
        OS_ReceiveMessage(&queue, &result, OS_MESSAGE_BLOCK);
    } else {
        packet->reply_queue = NULL;
        result = (OSMessage)func_ov009_0215ceb0(pipe, packet);
    }
    return (int)result;
}

// pokebw2's SOCLi_ExecCommandPacketInRecvPipe
int func_ov009_0215cf88(SOCLSocket *socket, SOCLCommandPacket *packet) {
    return func_ov009_0215cf14(&socket->recv_pipe->base, packet);
}

// pokebw2's SOCLi_ExecCommandPacketInSendPipe
int func_ov009_0215cf98(SOCLSocket *socket, SOCLCommandPacket *packet) {
    return func_ov009_0215cf14(&socket->send_pipe->base, packet);
}

// pokebw2's SOCLi_ExecCommandPacketInCtrlPipe
int func_ov009_0215cfa8(SOCLSocket *socket, SOCLCommandPacket *packet) {
    return func_ov009_0215cf14(func_ov009_0215ce9c(socket), packet);
}

// pokebw2's SOCLi_CommandPacketHandler
void func_ov009_0215cfc0(OSMessageQueue *queue) {
    SOCLCommandPacket *packet;
    u32 irq;
    s32 result;

    while (1) {
        OS_ReadMessage(queue, (OSMessage *)&packet, OS_MESSAGE_BLOCK);

        if (packet) {
            result = packet->handler(packet);
            irq = OS_DisableInterrupts();

            OS_DisableScheduler();
            OS_ReceiveMessage(queue, NULL, OS_MESSAGE_NOBLOCK);

            if (packet->socket) {
                packet->socket->result = result;
            }

            if (packet->reply_queue) {
                OS_SendMessage(packet->reply_queue, (OSMessage)result, OS_MESSAGE_NOBLOCK);
            }

            func_ov009_0215ce7c(packet);

            OS_EnableScheduler();
            OS_RestoreInterrupts(irq);
        } else {
            break;
        }
    }
}
