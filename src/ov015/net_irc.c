// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov031/net_irc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Overlay 31, the second of its four files (see net_devirc.c): data_ov015_0217aac0, which makes the packets of the infrared
// device and takes turns with the other machine, over the link of frames in irc_link.c. It also holds the timeout and
// peer filter that the games set through net_devirc.h.
#include "field/net_irc.h"
#include "field/net_devirc.h"
#include "field/irc_link.h"
#include "gfl/net_handle.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "nitro/wm.h"

// The most data one packet carries, and the size of a whole one
#define _IRC_SEND_SIZE 0xa6

// What a machine sends the other: a packet of the system, which the frames of the link carry
typedef struct {
    u16 crc;
    u8 cmd;
    // The turn this machine sends in the low four bits, the one it last got from the other in the high four
    u8 turn;
    // Both machines make a random ID. The packet has its own and the one it saw in the other's
    u32 senderId;
    u32 receiverId;
    u8 gameCode;
    u8 pad;
    u8 data[_IRC_SEND_SIZE];
} _NETIRC_DATA;

typedef struct {
    // Called with each packet that came in
    GFLNetRecvFunc recvCallback;
    // The packet that was last sent
    _NETIRC_DATA packet;
    // The ID of the machine this one is linking with, 0 until it is known
    u32 peerId;
    // How many frames the link may be down before it is lost (0 for no limit), and how many have passed since it went
    // down
    u32 timeout;
    u32 elapsed;
    // The last data sent, which is sent again when the other missed it
    const u8 *sendData;
    BOOL (*peerFilter)(int gameCode, int peerGameCode);
    u8 initialize;
    u8 unkCD;
    // Whether this machine may send now: the machines take turns
    u8 send_turn;
    u8 unkCF_0 : 1;
    u8 peerSeen : 1;
    u8 unkD0[0xc];
    u8 isParent;
    u8 running;
    u8 lastSize;
    u8 lastCmd;
    // The turn that was last sent, whether its answer is still to come, the turn to send next and the turn that was
    // last received
    u8 lastTurn;
    u8 sendPending;
    u8 sendTurn;
    u8 recvTurn;
    u8 resend;
    u8 timedOut;
    u8 tgid;
    u8 peerSession;
} NetIrcSysData;

static void func_ov015_021798dc(const u8 *data, u8 size, u8 cmd, u8 turn);
static void func_ov015_02179b38(u8 *data, u8 size, u8 cmd, u8 id);
static BOOL func_ov015_02179c98(void);
static void func_ov015_02179d14(void);
static u8 func_ov015_02179d30(void);

// pokebw2's NetIrcSys
static NetIrcSysData data_ov015_0217aac0;

// pokebw2's NetIrc_SendPacket
static void func_ov015_021798dc(const u8 *data, u8 size, u8 cmd, u8 turn) {
    GFL_ASSERT(size < _IRC_SEND_SIZE);
    data_ov015_0217aac0.packet.cmd = cmd;
    data_ov015_0217aac0.packet.turn = turn;
    if (data != NULL) {
        MI_CpuCopy8(data, data_ov015_0217aac0.packet.data, _IRC_SEND_SIZE);
    }
    data_ov015_0217aac0.packet.crc = getCRC16(&data_ov015_0217aac0.packet.cmd, size + 0xc);
    func_ov015_0217a148((u8 *)&data_ov015_0217aac0.packet, size + 0xe, cmd);
}

// pokebw2's NetIrc_Init
void func_ov015_0217993c(u32 timeout, u8 gameCode) {
    MI_CpuFill8(&data_ov015_0217aac0, 0, sizeof(data_ov015_0217aac0));
    data_ov015_0217aac0.timeout = timeout;
    data_ov015_0217aac0.tgid = WM_GetNextTgid();
    func_ov015_02179f74(3);
    func_ov015_0217a584(func_ov015_02179b38);
    data_ov015_0217aac0.packet.senderId = GFL_RandomLC(0);
    if (data_ov015_0217aac0.packet.senderId == 0) {
        data_ov015_0217aac0.packet.senderId = 1;
    }
    data_ov015_0217aac0.packet.gameCode = gameCode;
}

// pokebw2's NetDevIrc_SetTimeout
void func_ov015_0217998c(u32 frames) {
    data_ov015_0217aac0.timeout = frames;
}

// pokebw2's NetIrc_Exit
void func_ov015_02179998(void) {
    if (data_ov015_0217aac0.initialize == TRUE) {
        func_ov015_0217a134();
    }
    data_ov015_0217aac0.initialize = FALSE;
}

// pokebw2's NetIrc_SetRecvCallback
void func_ov015_021799b4(GFLNetRecvFunc callback) {
    data_ov015_0217aac0.recvCallback = callback;
}

// pokebw2's NetDevIrc_SetPeerFilter
void func_ov015_021799c0(BOOL (*filter)(int gameCode, int peerGameCode)) {
    data_ov015_0217aac0.peerFilter = filter;
}

// pokebw2's NetIrc_SetInitialized
void func_ov015_021799cc(void) {
    GFL_ASSERT(data_ov015_0217aac0.initialize == FALSE);
    data_ov015_0217aac0.initialize = TRUE;
}

// pokebw2's NetIrc_IsInitialized
BOOL func_ov015_021799f0(void) {
    return data_ov015_0217aac0.initialize;
}

// pokebw2's NetIrc_GetSendTurn
u8 func_ov015_021799fc(void) {
    return data_ov015_0217aac0.send_turn;
}

// pokebw2's NetIrc_GetUnkCF0
u8 func_ov015_02179a08(void) {
    return data_ov015_0217aac0.unkCF_0;
}

// pokebw2's NetIrc_CanSend
BOOL func_ov015_02179a18(void) {
    if (data_ov015_0217aac0.send_turn == TRUE && !data_ov015_0217aac0.unkCF_0 && data_ov015_0217aac0.elapsed == 0 && !data_ov015_0217aac0.sendPending) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetIrc_Send
void func_ov015_02179a4c(const u8 *data, u8 size, u8 cmd) {
    GFL_ASSERT(data_ov015_0217aac0.send_turn == TRUE);
    func_ov015_021798dc(data, size, cmd, data_ov015_0217aac0.sendTurn | (data_ov015_0217aac0.recvTurn << 4));
    data_ov015_0217aac0.sendData = data;
    data_ov015_0217aac0.lastSize = size;
    data_ov015_0217aac0.lastCmd = cmd;
    data_ov015_0217aac0.lastTurn = data_ov015_0217aac0.sendTurn;
    data_ov015_0217aac0.sendPending = TRUE;
    data_ov015_0217aac0.send_turn = FALSE;
}

// pokebw2's NetIrc_SendPending
void func_ov015_02179aac(void) {
    NetIrcSysData *sys = &data_ov015_0217aac0;

    if (data_ov015_0217aac0.send_turn == TRUE) {
        if (sys->resend == TRUE) {
            func_ov015_021798dc(sys->sendData, sys->lastSize, sys->lastCmd, sys->lastTurn | (data_ov015_0217aac0.recvTurn << 4));
            sys->resend = FALSE;
        } else {
            func_ov015_021798dc(NULL, 0, IRC_CMD_EMPTY, sys->lastTurn | (data_ov015_0217aac0.recvTurn << 4));
        }
        sys->send_turn = FALSE;
    }
}

// pokebw2's NetIrc_GetNetId
BOOL func_ov015_02179b24(void) {
    if (data_ov015_0217aac0.isParent == TRUE) {
        return FALSE;
    }
    return TRUE;
}

// pokebw2's NetIrc_OnFrame
static void func_ov015_02179b38(u8 *data, u8 size, u8 cmd, u8 id) {
    _NETIRC_DATA *packet = (_NETIRC_DATA *)data;
    // Never read, as in pokebw2's NetIrc_OnFrame: the code zeroes it all the same
    u8 zero[6] = { 0, 0, 0, 0, 0, 0 };
    int peerTurn = packet->turn & 0xf;
    int peerAck = packet->turn >> 4;
    BOOL senderNetId;

    data_ov015_0217aac0.send_turn = TRUE;
    GFL_ASSERT(size <= sizeof(_NETIRC_DATA));
    if (size == 0 || size > sizeof(_NETIRC_DATA)) {
        return;
    }
    if (packet->crc != getCRC16(&packet->cmd, size - 2)) {
        return;
    }
    if (data_ov015_0217aac0.peerFilter != NULL) {
        if (!data_ov015_0217aac0.peerFilter(data_ov015_0217aac0.packet.gameCode, packet->gameCode)) {
            return;
        }
    } else if (data_ov015_0217aac0.packet.gameCode != packet->gameCode) {
        return;
    }
    if (packet->receiverId == data_ov015_0217aac0.packet.senderId && packet->senderId == data_ov015_0217aac0.peerId) {
        if (!data_ov015_0217aac0.peerSeen) {
            func_ov015_02179d14();
            data_ov015_0217aac0.peerSeen = TRUE;
        }
    } else if (packet->receiverId == 0 && data_ov015_0217aac0.peerId == 0) {
        data_ov015_0217aac0.packet.receiverId = packet->senderId;
        data_ov015_0217aac0.peerId = packet->senderId;
        return;
    } else if (packet->receiverId == data_ov015_0217aac0.packet.senderId && data_ov015_0217aac0.peerId == 0) {
        data_ov015_0217aac0.packet.receiverId = packet->senderId;
        data_ov015_0217aac0.peerId = packet->senderId;
    } else {
        if (data_ov015_0217aac0.peerSeen) {
            return;
        }
        if (data_ov015_0217aac0.peerId == packet->senderId) {
            return;
        }
        data_ov015_0217aac0.peerId = 0;
        return;
    }
    if (cmd >= IRC_CMD_MIN) {
        return;
    }
    if (data_ov015_0217aac0.sendPending == TRUE && !data_ov015_0217aac0.resend) {
        if (peerAck == data_ov015_0217aac0.sendTurn) {
            if (data_ov015_0217aac0.sendTurn < 0xf) {
                data_ov015_0217aac0.sendTurn = data_ov015_0217aac0.sendTurn + 1;
            } else {
                data_ov015_0217aac0.sendTurn = 0;
            }
            data_ov015_0217aac0.sendPending = FALSE;
        } else {
            data_ov015_0217aac0.resend = TRUE;
        }
    }
    if (cmd == IRC_CMD_EMPTY) {
        return;
    }
    if (data_ov015_0217aac0.recvTurn == peerTurn) {
        return;
    }
    senderNetId = func_ov015_02179c98();
    if (data_ov015_0217aac0.recvCallback != NULL) {
        if (!data_ov015_0217aac0.recvCallback(senderNetId, packet->data, size - 0xe)) {
            return;
        }
    }
    data_ov015_0217aac0.recvTurn = peerTurn;
}

// pokebw2's NetIrc_GetSenderNetId
static BOOL func_ov015_02179c98(void) {
    if (func_ov015_02179d30() == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetIrc_IsError
BOOL func_ov015_02179cac(void) {
    if (!data_ov015_0217aac0.initialize) {
        return FALSE;
    }
    if (data_ov015_0217aac0.timedOut) {
        return TRUE;
    }
    if (data_ov015_0217aac0.running == TRUE && !func_ov015_02179d3c()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetIrc_StartLink
void func_ov015_02179ce4(void) {
    data_ov015_0217aac0.isParent = func_ov015_0217a014();
    data_ov015_0217aac0.peerSession = func_ov015_0217a5a4();
    data_ov015_0217aac0.running = TRUE;
    data_ov015_0217aac0.sendTurn = 0;
    data_ov015_0217aac0.recvTurn = 0xe;
    if (data_ov015_0217aac0.isParent != TRUE) {
        data_ov015_0217aac0.send_turn = TRUE;
    }
}

// pokebw2's NetIrc_RefreshRole
static void func_ov015_02179d14(void) {
    data_ov015_0217aac0.isParent = func_ov015_0217a014();
    data_ov015_0217aac0.peerSession = func_ov015_0217a5a4();
}

// pokebw2's NetIrc_GetIsParent
static u8 func_ov015_02179d30(void) {
    return data_ov015_0217aac0.isParent;
}

// pokebw2's NetDevIrc_IsConnected
BOOL func_ov015_02179d3c(void) {
    if (func_ov015_0217a024() == TRUE) {
        return TRUE;
    }
    if (data_ov015_0217aac0.elapsed < data_ov015_0217aac0.timeout || (data_ov015_0217aac0.elapsed == 0 && data_ov015_0217aac0.timeout == 0)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's NetIrc_GetPeerSeen
BOOL func_ov015_02179d68(void) {
    return data_ov015_0217aac0.peerSeen;
}

// pokebw2's NetIrc_IsLinked
BOOL func_ov015_02179d78(void) {
    return func_ov015_0217a024();
}

// pokebw2's NetIrc_Update
void func_ov015_02179d80(void) {
    NetIrcSysData *sys = &data_ov015_0217aac0;

    if (!sys->running && sys->elapsed >= sys->timeout && sys->elapsed != 0) {
        return;
    }
    if (sys->elapsed == 0) {
        func_ov015_0217a334();
    }
    if (sys->running == TRUE) {
        if (sys->elapsed != 0) {
            if (func_ov015_0217a024() == TRUE) {
                sys->elapsed = 0;
                if (func_ov015_0217a014() == TRUE) {
                    sys->send_turn = FALSE;
                } else {
                    sys->send_turn = TRUE;
                }
                if (sys->sendPending == TRUE) {
                    sys->resend = TRUE;
                }
            } else if (sys->elapsed < 5) {
                sys->elapsed++;
            } else if (sys->elapsed == 5) {
                func_ov015_02179f74(3);
                func_ov015_0217a584(func_ov015_02179b38);
                sys->elapsed++;
            } else {
                func_ov015_0217a334();
                if (sys->elapsed < sys->timeout) {
                    if (sys->isParent == TRUE && sys->elapsed % 5 == 0) {
                        func_ov015_02179fc4();
                    }
                    sys->elapsed++;
                } else {
                    sys->running = FALSE;
                    sys->timedOut = TRUE;
                    func_0203a338(1 - func_ov015_02179b24());
                    func_ov015_0217a134();
                }
            }
        } else if (!func_ov015_0217a024()) {
            if (sys->timeout != 0) {
                func_ov015_0217a134();
                sys->elapsed = 1;
            } else {
                sys->running = FALSE;
                sys->timedOut = TRUE;
                sys->elapsed = 0xffff;
                func_0203a338(1 - func_ov015_02179b24());
                func_ov015_0217a134();
            }
        }
    }
}
