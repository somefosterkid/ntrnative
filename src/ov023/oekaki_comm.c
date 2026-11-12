// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov070/oekaki_comm.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/oekaki.h"
#include "field/union_app.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_system.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "nitro/mi.h"

// The drawing board's net commands. The ROM has no name for the file, which is named after what it holds; pokeplatinum
// has the same commands in comm_command_oekaki.c. Names are ours

static void func_ov023_021841f8(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_021842a4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_021842bc(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_02184320(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_02184368(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_02184454(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_02184474(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_021844b4(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov023_021844d8(OekakiWork *wk, int index, NetHandle *handle);
static void *func_ov023_0218454c(int netId, void *work, int size);

// The commands from OEKAKI_NET_CMD_BASE
// pokebw2's sOekakiCommands
static const NetCommand data_ov023_02186a6c[OEKAKI_NET_CMD_COUNT] = {
    { func_ov023_021841f8, func_ov023_0218454c },
    { func_ov023_021842a4, NULL },
    { func_ov023_021842bc, NULL },
    { func_ov023_02184474, NULL },
    { func_ov023_02184320, NULL },
    { func_ov023_02184368, NULL },
    { func_ov023_021844b4, NULL },
    { func_ov023_02184454, NULL },
};

// pokebw2's OekakiComm_AddCommands
void func_ov023_021841e0(OekakiWork *wk) {
    func_0203a574(OEKAKI_NET_CMD_BASE, data_ov023_02186a6c, OEKAKI_NET_CMD_COUNT, wk);
}

// pokebw2's OekakiComm_RecvPicture
static void func_ov023_021841f8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const OekakiPictureChunk *chunk = data;
    OekakiWork *wk = work;

    if (func_02039338() != 0) {
        // A child puts the chunk into its picture
        if (chunk->index * OEKAKI_CHUNK_SIZE > OEKAKI_PICTURE_SIZE) {
            MIi_CpuCopyFast(chunk, wk->picture + chunk->index * OEKAKI_CHUNK_SIZE, 400);
        } else {
            MIi_CpuCopyFast(chunk, wk->picture + chunk->index * OEKAKI_CHUNK_SIZE, OEKAKI_CHUNK_SIZE);
        }
        MIi_CpuCopyFast(wk->picture, GFL_BitmapGetPixelData(BmpWin_GetBitmap(wk->drawWindow)), OEKAKI_PICTURE_SIZE);
        BmpWin_TransferNow(wk->drawWindow);
    } else {
        // The parent sends the next one
        if (wk->sendChunk * OEKAKI_CHUNK_SIZE < OEKAKI_PICTURE_SIZE) {
            wk->sendChunk++;
            func_ov023_021844d8(wk, wk->sendChunk, handle);
        } else {
            func_ov023_02186860(wk, OEKAKI_NET_CMD_PICTURE_END, 0);
        }
    }
}

// pokebw2's OekakiComm_RecvTouch
static void func_ov023_021842a4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    OekakiWork *wk = work;

    if (netId != 0) {
        func_ov023_02186988((OekakiTouchInfo *)data, &wk->queue, netId);
    }
}

// pokebw2's OekakiComm_RecvAllTouch
static void func_ov023_021842bc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const OekakiTouchInfo *touch = data;
    OekakiWork *wk = work;
    int i;

    if (wk != NULL && netId == 0) {
        for (i = 0; i < OEKAKI_MEMBER_MAX; i++) {
            wk->allTouch[i] = touch[i];
        }
    }
}

// pokebw2's OekakiComm_RecvPictureEnd
static void func_ov023_02184320(int netId, int size, const void *data, void *work, NetHandle *handle) {
    OekakiWork *wk = work;

    func_ov023_02185ab0(wk, OEKAKI_SEQ_NEWCOMER_END, 0);
    MIi_CpuClearFast(0, wk->picture, OEKAKI_PICTURE_SIZE);
    wk->shareNum = func_ov022_021841a4(wk->param->app);
    wk->shareBit = func_ov022_021841a0(wk->param->app);
    wk->banFlag = OEKAKI_BAN_NONE;
}

// pokebw2's OekakiComm_RecvSignal
static void func_ov023_02184368(int netId, int size, const void *data, void *work, NetHandle *handle) {
    const OekakiSignal *signal = data;
    OekakiWork *wk = work;

    if (netId != 0) {
        // A member's signal reaches the parent, which answers it
        if (func_02039338() == 0) {
            wk->signal.netId = signal->netId;
            wk->signal.count = signal->count;
            wk->signal.type = signal->type;
            wk->signal.ok = signal->ok;
            wk->signal.netId = netId;
            wk->signal.count = wk->shareNum;
            switch (signal->type) {
            case OEKAKI_SIGNAL_LEAVE_REQUEST:
                if (wk->shareNum != func_ov022_021841a4(wk->param->app) ||
                    wk->shareNum != MATH_CountPopulation(func_ov022_021841a0(wk->param->app))) {
                    wk->signal.ok = FALSE;
                } else {
                    wk->signal.ok = TRUE;
                    func_ov022_02184134(wk->param->app);
                }
                break;
            case OEKAKI_SIGNAL_LEAVE:
                break;
            }
            func_ov023_02186860(wk, OEKAKI_NET_CMD_SIGNAL, 0);
        }
    } else {
        switch (signal->type) {
        case OEKAKI_SIGNAL_LEAVE_REQUEST: {
            u8 id = signal->netId;

            if (id == func_02039338()) {
                if (!signal->ok) {
                    func_ov023_02185ab0(wk, OEKAKI_SEQ_END_REFUSED, id);
                } else {
                    wk->signalCount = signal->count;
                    func_ov023_02185ab0(wk, OEKAKI_SEQ_END_OK, signal->netId);
                    wk->leaving = TRUE;
                }
            }
            break;
        }
        case OEKAKI_SIGNAL_LEAVE:
            if (!wk->leaving) {
                func_ov023_02185ab0(wk, OEKAKI_SEQ_MEMBER_LEFT, signal->netId);
            }
            break;
        }
    }
}

// pokebw2's OekakiComm_RecvLeaving
static void func_ov023_02184454(int netId, int size, const void *data, void *work, NetHandle *handle) {
    OekakiWork *wk = work;

    if (func_02039338() == 0) {
        wk->leavingBits |= 1 << netId;
    }
}

// pokebw2's OekakiComm_RecvNewcomer
static void func_ov023_02184474(int netId, int size, const void *data, void *work, NetHandle *handle) {
    OekakiWork *wk = work;
    const u8 *id = data;

    if (!wk->parentEnded) {
        func_ov023_02185ab0(wk, OEKAKI_SEQ_NEWCOMER, *id);
        // The parent starts sending the picture
        if (func_02039338() == 0 && wk->procSeq == OEKAKI_PROC_SEQ_MAIN) {
            wk->sendChunk = 0;
            func_ov023_021844d8(wk, 0, handle);
        }
    }
}

// pokebw2's OekakiComm_RecvParentEnd
static void func_ov023_021844b4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    OekakiWork *wk = work;

    wk->parentEnded = TRUE;
    if (func_02039338() != 0) {
        func_ov023_02185ab0(wk, OEKAKI_SEQ_PARENT_END_MSG, 0);
    }
}

// Sends a chunk of the picture to every member
// The original narrows what func_ov022_021841a0 returns, which union_app.h declares as a u8 already
// pokebw2's OekakiComm_SendPicture
static void func_ov023_021844d8(OekakiWork *wk, int index, NetHandle *handle) {
    u8 *pixels = GFL_BitmapGetPixelData(BmpWin_GetBitmap(wk->drawWindow));
    int i;
    u32 *data;
    u32 checksum;
    u8 joined;

    MIi_CpuCopyFast(pixels + index * OEKAKI_CHUNK_SIZE, wk->sendChunkData.data, OEKAKI_CHUNK_SIZE);
    data = wk->sendChunkData.data;
    for (checksum = 0, i = 0; i < OEKAKI_CHUNK_SIZE / 4; i++) {
        checksum ^= data[i];
    }
    wk->sendChunkData.checksum = checksum;
    wk->sendChunkData.index = index;
    joined = func_ov022_021841a0(wk->param->app);
    func_0203aa94(func_0203996c(GFL_NET_NETID_SERVER), joined, OEKAKI_NET_CMD_PICTURE, sizeof(OekakiPictureChunk),
                  &wk->sendChunkData);
}

// pokebw2's OekakiComm_GetPictureRecvBuffer
static void *func_ov023_0218454c(int netId, void *work, int size) {
    OekakiWork *wk = work;

    return &wk->recvChunks[netId];
}

