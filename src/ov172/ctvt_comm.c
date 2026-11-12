// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/ctvt_comm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/ctvt_comm.h"
#include "types.h"
#include "app/comm_tvt.h"
#include "app/comm_tvt/comm_tvt_sys.h"
#include "app/comm_tvt/ctvt_call.h"
#include "app/comm_tvt/ctvt_camera.h"
#include "app/comm_tvt/ctvt_mic.h"
#include "app/comm_tvt/draw_system.h"
#include "app/comm_tvt/ima_adpcm.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_lower_data.h"
#include "gfl/std.h"
#include "gfl/wih.h"
#include "save/player_info.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "twl/ssp/common/ssp_jpeg_type.h"
#include "twl/ssp/ARM9/jpegdec.h"
#include "twl/ssp/ARM9/jpegenc.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
DrawSystem *func_ov172_021bdffc(CommTvtWork *sys);
void func_ov172_021be058(CommTvtWork *sys, u8 count);
void func_02008494(const PlayerInfo *src, PlayerInfo *dest);
u8 func_02008bfc(PlayerInfo *info);
BOOL func_0203cdbc(void);
void func_0203cff0(const u8 *mac);
void func_0203d010(void);
u8 func_0203d1a4(NetHandle *handle);
int func_0203d1b4(void);
void func_0203d644(BOOL a0);
void func_0203e5f8(HeapID heapId, BOOL a1);
void func_0203e758(void *data, u32 size, int dest, BOOL fromServer);
void func_0203e808(u32 bufferSize, int netId, HeapID heapId, void *buffer);
void func_0203e8c8(int netId);
BOOL func_ov172_021be0ac(CommTvtWork *sys);
void func_ov172_021be0cc(CommTvtWork *sys, BOOL value);
void func_ov257_021aab38(CommTvtWork *sys, BOOL value);
BOOL func_ov172_021be0f8(CommTvtWork *sys);
void func_ov172_021be0fc(CommTvtWork *sys, BOOL value);
void sys_memcpy(const void *src, void *dest, u32 size);

// The Xtransceiver's communication over the wireless connection: the connection itself, the members of the call,
// their pictures, sent as JPEG, the voice of whoever talks, the strokes of the drawing mode and the minigames' data

#define CTVT_COMM_PICTURE_SIZE 0xc000
#define CTVT_COMM_VOICE_BUFFER_SIZE 0xc800
#define CTVT_COMM_VOICE_CHUNK_SIZE 0x800

// The most strokes in one command
#define CTVT_COMM_DRAW_SEND_MAX 8
// How long an invitation stands, in frames
#define CTVT_COMM_INVITE_FRAMES 1200
#define CTVT_COMM_JPEG_QUALITY 70

// The game's commands, from CTVT_COMM_CMD_BASE
#define CTVT_COMM_CMD_BASE 0x2000
enum {
    CTVT_COMM_CMD_PACKET = CTVT_COMM_CMD_BASE,
    CTVT_COMM_CMD_VOICE,
    CTVT_COMM_CMD_DRAW,
    CTVT_COMM_CMD_INFO,
    CTVT_COMM_CMD_END,
};

#define CTVT_COMM_SEND_ALL 0xff
#define CTVT_COMM_NONE 0xff

// The steps of the connection
enum {
    CTVT_NET_IDLE,
    CTVT_NET_INIT,
    CTVT_NET_WAIT_INIT,
    CTVT_NET_WAIT_CONNECT,
    CTVT_NET_WAIT_NEGOTIATION,
    CTVT_NET_ADD_COMMANDS,
    CTVT_NET_WAIT_SYNC,
    CTVT_NET_CONNECTED,
    CTVT_NET_EXIT,
    CTVT_NET_SEND_END_SYNC,
    CTVT_NET_WAIT_END_SYNC,
    CTVT_NET_WAIT_EXIT,
    CTVT_NET_DONE,
};

// The steps of receiving a member's picture
enum {
    CTVT_PICTURE_NONE,
    CTVT_PICTURE_SEND_READY,
    CTVT_PICTURE_WAIT_START,
    CTVT_PICTURE_WAIT_END,
    CTVT_PICTURE_DONE,
};

typedef struct {
    u32 value;
    u8 type;
} CtvtCommPacket;

// What a scan finds: the rest is not used
// Black 2's; twinned code confirms the offsets of mac in Black
typedef struct {
    u8 unk0[10];
    u8 mac[6];
} CtvtCommScanInfo;

static void func_ov172_021b8050(CommTvtWork *sys, CtvtComm *comm);
static void *func_ov172_021b8118(void *work);
static int func_ov172_021b811c(void *work);
static BOOL func_ov172_021b8120(u32 gameId, u32 value);
static BOOL func_ov172_021b8130(const void *data, void *work);
static void func_ov172_021b8154(CommTvtWork *sys, CtvtComm *comm);
static void func_ov172_021b8448(CommTvtWork *sys, CtvtComm *comm);
static void func_ov172_021b8454(CommTvtWork *sys, CtvtComm *comm);
static void func_ov172_021b8614(CommTvtWork *sys, CtvtComm *comm, CtvtCommMember *member);
static void func_ov172_021b8628(CommTvtWork *sys, CtvtComm *comm, CtvtCommMember *member, u8 netId);
static void func_ov172_021b8770(int netId, int size, const void *data, void *work, NetHandle *handle);
void *func_ov172_021b8954(int netId, void *work, int size);
static void func_ov172_021b8978(int netId, int size, const void *data, void *work, NetHandle *handle);
static BOOL func_ov172_021b8a14(CommTvtWork *sys, CtvtComm *comm);
static void func_ov172_021b8a98(int netId, int size, const void *data, void *work, NetHandle *handle);
static BOOL func_ov172_021b8ad4(CommTvtWork *sys, CtvtComm *comm);
static void func_ov172_021b8b00(int netId, int size, const void *data, void *work, NetHandle *handle);
static void *func_ov172_021b8b20(int netId, void *work, int size);

// pokebw2's sCtvtCommCommands
const NetCommand data_ov172_021c6ed4[] = {
    { func_ov172_021b8770, NULL },
    { func_ov172_021b8978, func_ov172_021b8954 },
    { func_ov172_021b8a98, NULL },
    { func_ov172_021b8b00, func_ov172_021b8b20 },
};

static const GFLNetInitData data_ov172_021c6ef4 = {
    data_ov172_021c6ed4,
    NELEMS(data_ov172_021c6ed4),
    NULL,
    NULL,
    NULL,
    NULL,
    func_ov172_021b8118,
    func_ov172_021b811c,
    func_ov172_021b8120,
    NULL,
    NULL,
    NULL,
    NULL,
    { 0 },
    NULL,
    NULL,
    0,
    { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
    HEAPID_USER,
    0xd,
    0xf,
    0xd,
    0xf0,
    0,
    CTVT_COMM_MEMBERS,
    0x6e,
    0x10,
    1,
    0,
    0,
    1,
    CTVT_COMM_CMD_BASE >> 8,
    { 0x2c, 1, 0, 0 },
    0,
    0,
};

// pokebw2's CtvtComm_Create
CtvtComm *func_ov172_021b7ba8(CommTvtWork *sys, HeapID heapId) {
    u8 i, j;
    CtvtComm *comm = GFL_HeapAllocate(heapId, sizeof(CtvtComm), TRUE, "ctvt_comm.c", 244);

    comm->sys = sys;
    comm->netState = CTVT_NET_IDLE;
    comm->memberCount = 1;
    comm->photoReadyMask = 0;
    comm->connectType = 0;
    comm->nextConnectType = 0;
    comm->inviteTimer = 0;
    for (i = 0; i < CTVT_COMM_MEMBERS; i++) {
        func_ov172_021b8614(sys, comm, &comm->members[i]);
    }
    for (i = 0; i < CTVT_COMM_PICTURE_BUFFERS; i++) {
        if (!func_ov172_021be0d8(sys) || i == 0) {
            comm->pictureBuffers[i] = allocConfigDSSoftwareFeature(heapId, CTVT_COMM_PICTURE_SIZE, "ctvt_comm.c", 263);
        }
    }
    comm->jpegWork = allocConfigDSSoftwareFeature(
        heapId, SSP_GetJpegEncoderBufferSize(128, 192, SSP_JPEG_OUTPUT_YUV444, SSP_JPEG_RGB555), "ctvt_comm.c", 268);
    comm->photoRaw = allocConfigDSSoftwareFeature(heapId, CTVT_COMM_PICTURE_SIZE, "ctvt_comm.c", 270);
    comm->photoJpeg = allocConfigDSSoftwareFeature(heapId, CTVT_COMM_PICTURE_SIZE, "ctvt_comm.c", 271);
    comm->pictureBufferMask = 0;
    comm->photoTargetMask = 0;
    comm->photoSending = FALSE;
    comm->voiceBusy = FALSE;
    comm->voiceReady = FALSE;
    comm->voicePacket = GFL_HeapAllocate(heapId, sizeof(CtvtVoicePacket), TRUE, "ctvt_comm.c", 278);
    comm->voiceBuffer = GFL_HeapAllocate(heapId, CTVT_COMM_VOICE_BUFFER_SIZE, TRUE, "ctvt_comm.c", 279);
    comm->drawWriteIndex = 0;
    comm->drawReadIndex = 0;
    comm->talker = CTVT_COMM_NONE;
    comm->talkRequestsChanged = FALSE;
    comm->sendInfo = FALSE;
    comm->sendZoom = FALSE;
    comm->zoom = FALSE;
    comm->sendTalker = FALSE;
    comm->unk3c0 = TRUE;
    comm->sendUnk3c0 = FALSE;
    comm->unk3c8 = TRUE;
    comm->sendUnk3c8 = FALSE;
    comm->nextTalker = CTVT_COMM_NONE;

    func_02008494(GetGameDataPlayerInfo(func_ov172_021be004(sys)->gameData), &comm->beacon.player);
    comm->beacon.memberCount = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 6; j++) {
            comm->beacon.inviteMacs[i][j] = 0xff;
        }
    }
    comm->beacon.cameraEnabled = func_ov172_021be100();
    comm->beacon.inviteOnly = FALSE;
    func_02008494(GetGameDataPlayerInfo(func_ov172_021be004(sys)->gameData), (PlayerInfo *)comm->selfInfo.playerInfo);
    comm->selfInfo.cameraEnabled = func_ov172_021be100();
    comm->unk0 = 1;
    func_0203e5f8(heapId, FALSE);
    return comm;
}

// pokebw2's CtvtComm_Delete
void func_ov172_021b7dcc(CommTvtWork *sys, CtvtComm *comm) {
    u8 i;

    func_0203e6c0();
    GFL_HeapFree(comm->voiceBuffer);
    GFL_HeapFree(comm->voicePacket);
    func_0203d7fc(comm->photoRaw);
    func_0203d7fc(comm->photoJpeg);
    func_0203d7fc(comm->jpegWork);
    for (i = 0; i < CTVT_COMM_PICTURE_BUFFERS; i++) {
        if (!func_ov172_021be0d8(sys) || i == 0) {
            func_0203d7fc(comm->pictureBuffers[i]);
        }
    }
    GFL_HeapFree(comm);
}

// pokebw2's CtvtComm_Update
void func_ov172_021b7e34(CommTvtWork *sys, CtvtComm *comm) {
    u8 i, j;

    func_ov172_021be00c(sys);
    switch (comm->netState) {
    case CTVT_NET_IDLE:
        if (comm->nextConnectType != 0) {
            if (comm->nextConnectType == CTVT_CONNECT_EXISTING) {
                func_0203d644(FALSE);
                comm->netState = CTVT_NET_ADD_COMMANDS;
                comm->connectType = comm->nextConnectType;
            } else {
                comm->netState = CTVT_NET_INIT;
            }
        }
        break;
    case CTVT_NET_INIT:
        func_ov172_021b8050(sys, comm);
        comm->netState = CTVT_NET_WAIT_INIT;
        break;
    case CTVT_NET_WAIT_INIT:
        if (func_0203cd98() == TRUE) {
            switch (comm->nextConnectType) {
            case CTVT_CONNECT_PARENT:
                func_0203d010();
                comm->netState = CTVT_NET_CONNECTED;
                break;
            case CTVT_CONNECT_SCAN:
                func_0203d01c();
                func_ov014_02179328(func_ov172_021b8130);
                comm->netState = CTVT_NET_WAIT_CONNECT;
                break;
            case CTVT_CONNECT_MAC:
                if (comm->connectType == CTVT_CONNECT_PARENT) {
                    func_ov172_021b8614(sys, comm, &comm->members[0]);
                }
                func_0203cff0(comm->parentMac);
                comm->netState = CTVT_NET_WAIT_CONNECT;
                break;
            }
            comm->connectType = comm->nextConnectType;
        }
        break;
    case CTVT_NET_WAIT_CONNECT:
        if (func_02039acc() == TRUE) {
            comm->netState = CTVT_NET_WAIT_NEGOTIATION;
        }
        if (comm->connectType != comm->nextConnectType) {
            comm->netState = CTVT_NET_EXIT;
        }
        break;
    case CTVT_NET_WAIT_NEGOTIATION:
        if (func_020399b8(func_020399a8()) == TRUE) {
            comm->netState = CTVT_NET_CONNECTED;
        }
        if (comm->connectType != comm->nextConnectType) {
            comm->netState = CTVT_NET_EXIT;
        }
        break;
    case CTVT_NET_ADD_COMMANDS: {
        NetHandle *handle = func_020399a8();

        func_0203a574(CTVT_COMM_CMD_BASE, data_ov172_021c6ed4, NELEMS(data_ov172_021c6ed4), comm);
        func_02039c68(handle, 8, 32);
        comm->netState = CTVT_NET_WAIT_SYNC;
        break;
    }
    case CTVT_NET_WAIT_SYNC:
        if (func_02039cb4(func_020399a8(), 8, 32) == TRUE) {
            comm->netState = CTVT_NET_CONNECTED;
        }
        break;
    case CTVT_NET_CONNECTED:
        func_ov172_021b8154(sys, comm);
        if (comm->connectType != comm->nextConnectType) {
            comm->netState = CTVT_NET_EXIT;
        }
        if (comm->connectType == CTVT_CONNECT_PARENT) {
            func_ov172_021b8448(sys, comm);
        }
        break;
    case CTVT_NET_EXIT:
        func_0203ceac(NULL);
        comm->netState = CTVT_NET_WAIT_EXIT;
        break;
    case CTVT_NET_WAIT_EXIT:
        if (func_0203cdbc() == TRUE) {
            if (comm->nextConnectType == CTVT_CONNECT_DISCONNECT) {
                comm->netState = CTVT_NET_DONE;
            } else {
                comm->netState = CTVT_NET_INIT;
            }
        }
        break;
    case CTVT_NET_SEND_END_SYNC:
        func_02039c68(func_020399a8(), 9, 32);
        comm->netState = CTVT_NET_WAIT_END_SYNC;
        break;
    case CTVT_NET_WAIT_END_SYNC:
        if (func_02039cb4(func_020399a8(), 9, 32) == TRUE) {
            func_0203a5e4(CTVT_COMM_CMD_BASE);
            comm->netState = CTVT_NET_DONE;
        }
        if (func_0200fb24()) {
            func_0203a5e4(CTVT_COMM_CMD_BASE);
            comm->netState = CTVT_NET_DONE;
        }
        break;
    }

    if (comm->inviteTimer != 0) {
        comm->inviteTimer--;
        if (comm->inviteTimer == 0) {
            for (i = 0; i < 3; i++) {
                for (j = 0; j < 6; j++) {
                    comm->beacon.inviteMacs[i][j] = 0xff;
                }
            }
            comm->beacon.inviteOnly = FALSE;
        }
    }
}

// pokebw2's CtvtComm_InitNet
static void func_ov172_021b8050(CommTvtWork *sys, CtvtComm *comm) {
    GFLNetInitData init = data_ov172_021c6ef4;

    func_ov172_021be00c(sys);
    func_0203cbac(&init, NULL, comm);
    func_0203d7bc(FALSE);
}

// pokebw2's CtvtComm_Disconnect
void func_ov172_021b8084(CommTvtWork *sys, CtvtComm *comm) {
    if (comm->memberCount <= 1) {
        if (comm->connectType == CTVT_CONNECT_EXISTING) {
            comm->netState = CTVT_NET_DONE;
            func_0203a5e4(CTVT_COMM_CMD_BASE);
        } else {
            func_0203ceac(NULL);
            comm->netState = CTVT_NET_WAIT_EXIT;
        }
    } else if (comm->connectType == CTVT_CONNECT_EXISTING) {
        comm->netState = CTVT_NET_SEND_END_SYNC;
    } else {
        comm->netState = CTVT_NET_EXIT;
    }
    comm->nextConnectType = CTVT_CONNECT_DISCONNECT;
}

// pokebw2's CtvtComm_IsDone
BOOL func_ov172_021b80cc(CommTvtWork *sys, CtvtComm *comm) {
    if (comm->netState == CTVT_NET_DONE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov172_021b80dc(CommTvtWork *sys, CtvtComm *comm, u32 type) {
    comm->nextConnectType = type;
}


// pokebw2's CtvtComm_GetConnectType
int func_ov172_021b80e4(CommTvtWork *sys, CtvtComm *comm) {
    return comm->connectType;
}

// pokebw2's CtvtComm_SetParentMac
void func_ov172_021b80ec(CommTvtWork *sys, CtvtComm *comm, const u8 *mac) {
    u8 i;

    for (i = 0; i < 6; i++) {
        comm->parentMac[i] = mac[i];
    }
}

// pokebw2's CtvtComm_IsConnected
BOOL func_ov172_021b8108(CommTvtWork *sys, CtvtComm *comm) {
    if (comm->netState == CTVT_NET_CONNECTED) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtComm_GetBeaconData
static void *func_ov172_021b8118(void *work) {
    CtvtComm *comm = work;

    return &comm->beacon;
}

// pokebw2's CtvtComm_GetBeaconSize
static int func_ov172_021b811c(void *work) {
    return sizeof(CtvtCommBeacon);
}

// pokebw2's CtvtComm_CheckBeacon
static BOOL func_ov172_021b8120(u32 gameId, u32 value) {
    if (value == 0x20 || value == 3) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtComm_FilterInvited
static BOOL func_ov172_021b8130(const void *data, void *work) {
    const CtvtCommScanInfo *info = data;
    CtvtComm *comm = work;

    if (comm->beacon.inviteOnly == FALSE) {
        return TRUE;
    }
    return func_ov172_021b85d0(&comm->beacon, info->mac);
}

static void func_ov172_021b8154(CommTvtWork *sys, CtvtComm *comm) {
    u8 selfNetId = func_0203d1a4(func_020399a8());
    u8 i;

    if (comm->connectType == CTVT_CONNECT_PARENT) {
        if (comm->members[0].active == FALSE) {
            CtvtCamera *camera = func_ov172_021bdfdc(sys);

            comm->members[0].active = TRUE;
            comm->members[0].infoReceived = TRUE;
            comm->members[0].infoApplied = TRUE;
            comm->members[0].isSelf = TRUE;
            comm->members[0].wantsToTalk = FALSE;
            comm->members[0].pictureBuffer = CTVT_COMM_NONE;
            comm->members[0].pictureState = CTVT_PICTURE_NONE;
            comm->members[0].info = comm->selfInfo;
            comm->beacon.memberCount = 1;
            comm->memberCount = 1;
            func_ov172_021b7740(sys, camera, FALSE, FALSE);
        }
    } else {
        u8 count = func_0203d1b4();

        if (comm->memberCount != count) {
            func_ov172_021b8454(sys, comm);
            func_ov172_021be1a4(sys);
            if (comm->memberCount < count && selfNetId == 0) {
                comm->sendZoom = TRUE;
                comm->zoom = func_ov172_021be078(sys);
            }
            comm->sendInfo = TRUE;
            comm->memberCount = count;
            comm->beacon.memberCount = count;
        }
    }

    for (i = 0; i < CTVT_COMM_MEMBERS; i++) {
        func_ov172_021b8628(sys, comm, &comm->members[i], i);
    }

    if (comm->memberCount > 1) {
        if (comm->photoSending == TRUE) {
            if (func_0203ea54() == TRUE) {
                comm->photoSending = FALSE;
            }
        } else if (comm->photoTargetMask == (comm->photoTargetMask & comm->photoReadyMask) &&
                   comm->photoTargetMask != 0 && func_ov172_021be0f8(sys) == FALSE) {
            func_ov172_021be0fc(sys, TRUE);
            if (func_ov172_021be0ac(sys) == FALSE) {
                CtvtCamera *camera = func_ov172_021bdfdc(sys);
                void *picture = func_ov172_021b7818(sys, camera);
                u32 size = func_ov172_021b785c(sys, camera);
                u32 jpegSize;
                u16 width;

                MIi_CpuCopy32(picture, comm->photoRaw, size);
                width = func_ov172_021b77b8(sys, camera);
                jpegSize = SSP_StartJpegEncoder(comm->photoRaw, comm->photoJpeg, size, comm->jpegWork, width,
                                                func_ov172_021b77e8(sys, camera), CTVT_COMM_JPEG_QUALITY,
                                                SSP_JPEG_OUTPUT_YUV444, SSP_JPEG_RGB555);
                if (jpegSize != 0) {
                    func_0203e758(comm->photoJpeg, jpegSize, comm->photoTargetMask, FALSE);
                    comm->photoReadyMask = 0;
                    comm->photoSending = TRUE;
                }
            }
            func_ov172_021be0fc(sys, FALSE);
        }

        if (comm->drawWriteIndex != comm->drawReadIndex && comm->unk3c0 == TRUE) {
            func_ov172_021b8a14(sys, comm);
        }
        if (comm->sendUnk3c0 == TRUE && func_ov172_021b8740(sys, comm, CTVT_PACKET_UNK4, 0) == TRUE) {
            comm->sendUnk3c0 = FALSE;
        }
        if (comm->sendUnk3c8 == TRUE && func_ov172_021b8740(sys, comm, CTVT_PACKET_UNK5, 0) == TRUE) {
            comm->sendUnk3c8 = FALSE;
        }
        if (comm->sendInfo == TRUE && func_ov172_021b8ad4(sys, comm) == TRUE) {
            comm->sendInfo = FALSE;
        }
    }

    if (selfNetId == 0) {
        if (comm->sendZoom == TRUE && func_ov172_021b8740(sys, comm, CTVT_PACKET_ZOOM, comm->zoom) == TRUE) {
            comm->sendZoom = FALSE;
        }
        if (comm->talkRequestsChanged == TRUE) {
            BOOL found = FALSE;
            u8 netId;

            for (netId = 0; netId < CTVT_COMM_MEMBERS; netId++) {
                if (comm->members[netId].wantsToTalk == TRUE) {
                    if (comm->nextTalker == CTVT_COMM_NONE) {
                        comm->nextTalker = netId;
                    }
                    found = TRUE;
                    comm->members[netId].wantsToTalk = FALSE;
                }
            }
            if (!found) {
                comm->nextTalker = CTVT_COMM_NONE;
            }
            if (comm->nextTalker != comm->talker) {
                comm->sendTalker = TRUE;
            }
            comm->talkRequestsChanged = FALSE;
        }
        if (comm->nextTalker != CTVT_COMM_NONE && comm->members[comm->nextTalker].active == FALSE) {
            comm->nextTalker = CTVT_COMM_NONE;
            comm->sendTalker = TRUE;
            comm->talkRequestsChanged = FALSE;
        }
        if (comm->sendTalker == TRUE && func_ov172_021b8740(sys, comm, CTVT_PACKET_TALKER, comm->nextTalker) == TRUE) {
            comm->sendTalker = FALSE;
        }
    }
}

// pokebw2's CtvtComm_SetScanTime
static void func_ov172_021b8448(CommTvtWork *sys, CtvtComm *comm) {
    func_ov014_02179650(5);
}

// pokebw2's CtvtComm_UpdateMembers
static void func_ov172_021b8454(CommTvtWork *sys, CtvtComm *comm) {
    u8 count = 0;
    BOOL changed = FALSE;
    u8 selfNetId = func_0203d1a4(func_020399a8());
    HeapID heapId = func_ov172_021be00c(sys);
    u8 netId;
    u8 i;

    for (netId = 0; netId < CTVT_COMM_MEMBERS; netId++) {
        if (func_0203d1c0(netId) == TRUE) {
            if (comm->members[netId].active == FALSE) {
                CtvtCamera *camera = func_ov172_021bdfdc(sys);

                comm->members[netId].active = TRUE;
                if (netId == selfNetId) {
                    comm->members[netId].isSelf = TRUE;
                } else {
                    for (i = 0; i < CTVT_COMM_PICTURE_BUFFERS; i++) {
                        u32 bit = 1 << i;

                        if (!(comm->pictureBufferMask & bit)) {
                            func_0203e808(CTVT_COMM_PICTURE_SIZE, netId, heapId, comm->pictureBuffers[i]);
                            comm->pictureBufferMask |= bit;
                            comm->members[netId].pictureBuffer = i;
                            break;
                        }
                    }
                    comm->photoTargetMask |= 1 << netId;
                    comm->members[netId].pictureState = CTVT_PICTURE_SEND_READY;
                }
                func_ov172_021b7b20(sys, camera, netId);
                changed = TRUE;
            }
            count++;
        } else if (comm->members[netId].active == TRUE) {
            comm->members[netId].active = FALSE;
            if (comm->members[netId].isSelf == FALSE) {
                func_0203e8c8(netId);
                comm->pictureBufferMask -= 1 << comm->members[netId].pictureBuffer;
            }
            if (comm->talker == netId) {
                comm->voiceBusy = FALSE;
            }
            changed = TRUE;
            comm->photoTargetMask -= changed << netId;
            func_ov172_021b8614(sys, comm, &comm->members[netId]);
        }
    }
    if (changed == TRUE) {
        func_ov172_021be058(sys, count);
        func_ov172_021be0a8(sys, selfNetId);
    }
    func_ov172_021b7740(sys, func_ov172_021bdfdc(sys), FALSE, TRUE);
}

// pokebw2's CtvtComm_IsInvited
BOOL func_ov172_021b85d0(const CtvtCommBeacon *beacon, const u8 *mac) {
    u8 i, j;

    for (i = 0; i < 3; i++) {
        BOOL same = TRUE;

        for (j = 0; j < 6; j++) {
            if (beacon->inviteMacs[i][j] != mac[j]) {
                same = FALSE;
                break;
            }
        }
        if (same == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's CtvtComm_ClearMember
void func_ov172_021b8614(CommTvtWork *sys, CtvtComm *comm, CtvtCommMember *member) {
    member->isSelf = FALSE;
    member->active = FALSE;
    member->infoReceived = FALSE;
    member->infoApplied = FALSE;
    member->wantsToTalk = FALSE;
    member->pictureBuffer = 0xff;
    member->pictureState = 0;
}

// pokebw2's CtvtComm_UpdateMember
static void func_ov172_021b8628(CommTvtWork *sys, CtvtComm *comm, CtvtCommMember *member, u8 netId) {
    CtvtCamera *camera = func_ov172_021bdfdc(sys);

    if (member->active == FALSE || member->isSelf == TRUE) {
        return;
    }
    if (member->infoApplied == FALSE && member->infoReceived == TRUE) {
        if (!func_ov172_021be0d8(sys)) {
            func_ov172_021be2f0(sys, &member->info);
        }
        member->infoApplied = TRUE;
    }
    switch (member->pictureState) {
    case CTVT_PICTURE_NONE:
        break;
    case CTVT_PICTURE_SEND_READY:
        if (func_ov172_021b8740(sys, comm, CTVT_PACKET_READY, 0) == TRUE) {
            member->pictureState = CTVT_PICTURE_WAIT_START;
        }
        break;
    case CTVT_PICTURE_WAIT_START:
        if (func_0203ea70(netId) == FALSE) {
            func_ov172_021b7724(sys, camera, netId);
            member->pictureState = CTVT_PICTURE_WAIT_END;
        }
        break;
    case CTVT_PICTURE_WAIT_END:
        if (func_0203ea70(netId) == TRUE) {
            void *picture = func_ov172_021b7830(sys, camera, netId);
            s16 size[2];

            size[1] = func_ov172_021b77b8(sys, camera);
            size[0] = func_ov172_021b77e8(sys, camera);
            SSP_StartJpegDecoder(func_0203e96c(netId), func_0203e9dc(netId), picture, &size[1], &size[0], 0);
            func_ov172_021b7714(sys, camera, netId);
            member->pictureState = CTVT_PICTURE_SEND_READY;
        }
        break;
    case CTVT_PICTURE_DONE:
        break;
    }
}

// pokebw2's CtvtComm_SendZoom
void func_ov172_021b8720(CommTvtWork *sys, CtvtComm *comm, BOOL zoomed) {
    if (func_ov172_021b8740(sys, comm, CTVT_PACKET_ZOOM, zoomed) == FALSE) {
        comm->sendZoom = TRUE;
        comm->zoom = zoomed;
    }
}

// pokebw2's CtvtComm_SendPacket
BOOL func_ov172_021b8740(CommTvtWork *sys, CtvtComm *comm, u8 type, u32 value) {
    NetHandle *handle = func_020399a8();
    CtvtCommPacket packet;

    packet.type = type;
    packet.value = value;
    return func_0203d3f8(handle, CTVT_COMM_SEND_ALL, CTVT_COMM_CMD_PACKET, sizeof(CtvtCommPacket), &packet, TRUE, FALSE,
                         FALSE);
}

static void func_ov172_021b8770(int netId, int size, const void *data, void *work, NetHandle *handle) {
    CtvtComm *comm = work;
    const CtvtCommPacket *packet = data;
    u8 selfNetId = func_0203d1a4(func_020399a8());

    switch (packet->type) {
    case CTVT_PACKET_READY:
        if (netId != selfNetId) {
            comm->photoReadyMask |= 1 << netId;
        }
        break;
    case CTVT_PACKET_TALKER: {
        u32 talker = packet->value;

        comm->talker = talker;
        if (talker != CTVT_COMM_NONE) {
            func_ov172_021be210(comm->sys, talker);
        } else {
            CtvtMic *mic = func_ov172_021bdfec(comm->sys);

            func_ov172_021be2e0(comm->sys);
            func_ov172_021ba234(mic);
        }
        break;
    }
    case CTVT_PACKET_DRAW_INDEX:
        func_ov172_021b6ea8(func_ov172_021bdffc(comm->sys), packet->value);
        func_ov172_021be0cc(comm->sys, FALSE);
        break;
    case CTVT_PACKET_ZOOM: {
        CtvtCamera *camera = func_ov172_021bdfdc(comm->sys);
        BOOL zoomed = packet->value;

        if (zoomed != func_ov172_021be078(comm->sys)) {
            func_ov172_021be0a0(comm->sys, zoomed);
            func_ov172_021b7740(comm->sys, camera, TRUE, FALSE);
        }
        break;
    }
    case CTVT_PACKET_UNK4:
        if (netId != selfNetId) {
            comm->unk3c0 = TRUE;
        }
        break;
    case CTVT_PACKET_UNK5:
        if (netId != selfNetId) {
            comm->unk3c8 = TRUE;
        }
        break;
    case CTVT_PACKET_REQUEST_TALK:
        if (selfNetId == 0) {
            comm->members[netId].wantsToTalk = TRUE;
            comm->talkRequestsChanged = TRUE;
        }
        break;
    case CTVT_PACKET_PLAY_VOICE: {
        CtvtMic *mic = func_ov172_021bdfec(comm->sys);

        // The voice was decoded into voiceBuffer, but the packet buffer is what is played
        if (netId == selfNetId) {
            func_ov172_021ba1e8(mic, comm->voicePacket, comm->voiceSize, 0, comm->voiceSpeed);
        } else {
            func_ov172_021ba1e8(mic, comm->voicePacket, comm->voiceSize, 127, comm->voiceSpeed);
        }
        break;
    }
    case CTVT_PACKET_CANCEL_TALK:
        if (selfNetId == 0) {
            comm->members[netId].wantsToTalk = FALSE;
            comm->talkRequestsChanged = TRUE;
        }
        func_ov172_021be2e0(comm->sys);
        break;
    case CTVT_PACKET_TALK_DONE:
        if (netId == comm->talker) {
            comm->talkRequestsChanged = TRUE;
        }
        func_ov172_021be2e0(comm->sys);
        break;
    case CTVT_PACKET_UNK_A:
        func_ov172_021be0d4(comm->sys, TRUE);
        break;
    }
}

// pokebw2's CtvtComm_SendVoice
BOOL func_ov172_021b891c(CommTvtWork *sys, CtvtComm *comm, CtvtVoicePacket *packet) {
    BOOL sent = func_0203d3f8(func_020399a8(), CTVT_COMM_SEND_ALL, CTVT_COMM_CMD_VOICE, packet->size + 8, packet, TRUE,
                              FALSE, TRUE);

    if (sent) {
        comm->voiceBusy = TRUE;
    }
    return sent;
}

// pokebw2's CtvtComm_GetVoiceBuffer
void *func_ov172_021b8954(int netId, void *work, int size) {
    CtvtComm *comm = work;

    comm->voiceBusy = TRUE;
    MIi_CpuClear32(0, comm->voicePacket, sizeof(CtvtVoicePacket));
    return comm->voicePacket;
}

// pokebw2's CtvtComm_RecvVoice
static void func_ov172_021b8978(int netId, int size, const void *data, void *work, NetHandle *handle) {
    CtvtComm *comm = work;
    CtvtMic *mic = func_ov172_021bdfec(comm->sys);
    CtvtVoicePacket *packet = comm->voicePacket;
    s8 *voice = packet->data;

    func_0203d1a4(func_020399a8());
    if (packet->chunk == 0) {
        func_ov172_021b69f0();
        MIi_CpuClear32(0, comm->voiceBuffer, CTVT_COMM_VOICE_BUFFER_SIZE);
    }
    func_ov172_021ba194(mic, voice, (s16 *)((u8 *)comm->voiceBuffer + packet->chunk * CTVT_COMM_VOICE_CHUNK_SIZE),
                   packet->size);
    if (packet->isLast == TRUE) {
        // The playback speeds, 1.0 at 0x8000
        u16 speeds[] = { 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000, 0xa000, 0xb000, 0xc000 };

        comm->voiceSpeed = speeds[packet->speed];
        comm->voiceReady = TRUE;
        comm->voiceSize = packet->playSize;
    }
    comm->voiceBusy = FALSE;
}

// pokebw2's CtvtComm_SendDraw
static BOOL func_ov172_021b8a14(CommTvtWork *sys, CtvtComm *comm) {
    NetHandle *handle = func_020399a8();
    u8 count;
    BOOL sent;

    if (comm->drawReadIndex > comm->drawWriteIndex) {
        count = CTVT_COMM_DRAW_QUEUE_SIZE - comm->drawReadIndex;
    } else {
        count = comm->drawWriteIndex - comm->drawReadIndex;
    }
    if (count > CTVT_COMM_DRAW_SEND_MAX) {
        count = CTVT_COMM_DRAW_SEND_MAX;
    }
    if (count != 0) {
        sent = func_0203d3f8(handle, CTVT_COMM_SEND_ALL, CTVT_COMM_CMD_DRAW, count * sizeof(DrawCommand),
                             &comm->drawQueue[comm->drawReadIndex], TRUE, TRUE, FALSE);
        if (sent) {
            comm->drawReadIndex += count;
            if (comm->drawReadIndex >= CTVT_COMM_DRAW_QUEUE_SIZE) {
                comm->drawReadIndex -= CTVT_COMM_DRAW_QUEUE_SIZE;
            }
        }
        return sent;
    }
    return TRUE;
}

static void func_ov172_021b8a98(int netId, int size, const void *data, void *work, NetHandle *handle) {
    CtvtComm *comm = work;
    DrawSystem *drawSystem = func_ov172_021bdffc(comm->sys);
    u8 count = size / sizeof(DrawCommand);
    u8 i;

    func_0203d1a4(func_020399a8());
    for (i = 0; i < count; i++) {
        func_ov172_021b6e88(drawSystem, (const DrawCommand *)data + i);
    }
}

static BOOL func_ov172_021b8ad4(CommTvtWork *sys, CtvtComm *comm) {
    CtvtCommMemberInfo *info = func_ov172_021b8b9c(sys, comm);

    return func_0203d3f8(func_020399a8(), CTVT_COMM_SEND_ALL, CTVT_COMM_CMD_INFO, sizeof(CtvtCommMemberInfo), info,
                         TRUE, TRUE, TRUE);
}

static void func_ov172_021b8b00(int netId, int size, const void *data, void *work, NetHandle *handle) {
    CtvtComm *comm = work;

    if (comm->members[netId].infoReceived == FALSE) {
        comm->members[netId].infoReceived = TRUE;
        func_ov172_021be1a4(comm->sys);
    }
}

static void *func_ov172_021b8b20(int netId, void *work, int size) {
    CtvtComm *comm = work;

    return func_ov172_021b8b94(comm->sys, comm, netId);
}

// pokebw2's CtvtComm_StartSync
void func_ov172_021b8b34(CommTvtWork *sys, CtvtComm *comm, u32 no) {
    func_02039c68(func_020399a8(), no, 0x20);
}

// pokebw2's CtvtComm_IsSynced
BOOL func_ov172_021b8b48(CommTvtWork *sys, CtvtComm *comm, u32 no) {
    return func_02039cb4(func_020399a8(), no, 0x20);
}

u8 func_ov172_021b8b5c(CommTvtWork *sys, CtvtComm *comm) {
    return func_0203d1a4(func_020399a8());
}





// pokebw2's CtvtComm_GetTalker
u8 func_ov172_021b8b68(CommTvtWork *sys, CtvtComm *comm) {
    return comm->talker;
}

// pokebw2's CtvtComm_IsVoiceBusy
BOOL func_ov172_021b8b74(CommTvtWork *sys, CtvtComm *comm) {
    return comm->voiceBusy;
}

// pokebw2's CtvtComm_GetBeacon
CtvtCommBeacon *func_ov172_021b8b7c(CommTvtWork *sys, CtvtComm *comm) {
    return &comm->beacon;
}

void *func_ov172_021b8b84(CommTvtWork *sys, CtvtComm *comm) {
    return comm->voicePacket;
}


// pokebw2's CtvtComm_IsMemberInfoReceived
BOOL func_ov172_021b8b8c(CommTvtWork *sys, CtvtComm *comm, u8 member) {
    return comm->members[member].infoReceived;
}

// pokebw2's CtvtComm_GetMemberInfo
CtvtCommMemberInfo *func_ov172_021b8b94(CommTvtWork *sys, CtvtComm *comm, u8 member) {
    return &comm->members[member].info;
}

// pokebw2's CtvtComm_GetSelfInfo
CtvtCommMemberInfo *func_ov172_021b8b9c(CommTvtWork *sys, CtvtComm *comm) {
    return &comm->selfInfo;
}

// pokebw2's CtvtComm_IsMemberActive
BOOL func_ov172_021b8ba4(CommTvtWork *sys, CtvtComm *comm, u8 member) {
    return comm->members[member].active;
}

// pokebw2's CtvtComm_HasMemberCamera
BOOL func_ov172_021b8bac(CommTvtWork *sys, CtvtComm *comm, u8 member) {
    return comm->members[member].info.cameraEnabled;
}

// pokebw2's CtvtComm_GetDrawSlot
DrawCommand *func_ov172_021b8bb8(CommTvtWork *sys, CtvtComm *comm, BOOL *full) {
    u8 next = comm->drawWriteIndex + 1;

    if (next >= CTVT_COMM_DRAW_QUEUE_SIZE) {
        next -= CTVT_COMM_DRAW_QUEUE_SIZE;
    }
    if (next == comm->drawReadIndex) {
        *full = TRUE;
        return NULL;
    }
    *full = FALSE;
    return &comm->drawQueue[comm->drawWriteIndex];
}

// pokebw2's CtvtComm_CommitDrawSlot
void func_ov172_021b8bfc(CommTvtWork *sys, CtvtComm *comm) {
    comm->drawWriteIndex++;
    if (comm->drawWriteIndex >= CTVT_COMM_DRAW_QUEUE_SIZE) {
        comm->drawWriteIndex -= CTVT_COMM_DRAW_QUEUE_SIZE;
    }
}

// pokebw2's CtvtComm_StartInviteTimer
void func_ov172_021b8c14(CommTvtWork *sys, CtvtComm *comm) {
    comm->inviteTimer = CTVT_COMM_INVITE_FRAMES;
}

// pokebw2's CtvtComm_IsInviteTimerDone
BOOL func_ov172_021b8c20(CommTvtWork *sys, CtvtComm *comm) {
    if (comm->inviteTimer == 0) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CtvtComm_GetUnk3c8
BOOL func_ov172_021b8c34(CommTvtWork *sys, CtvtComm *comm) {
    return comm->unk3c8;
}
