// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov271/br_net.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Battle Recorder's connection to the Global Link: the requests that its online modes make to the Global Data
// Storage client, which the overlay 201 holds, and the result messages that they end with. The name is the ROM's
// string, which GFL_HeapAllocate is given.

#include "types.h"
#include "app/battle_recorder/br_net.h"
#include "app/gds_rap.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "save/gds_profile.h"
#include "save/player_info.h"
#include "system/game_data.h"

typedef struct BrNetSeq BrNetSeq;

typedef void (*BrNetSeqFunc)(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);

// A step of a request, which runs every frame until it sets the next one
struct BrNetSeq {
    BrNetSeqFunc func;
    u32 seq;
    BrNet *net;
    u32 unkC;
};

struct BrNet {
    // The GDS client's work
    GdsRapWork gdsrap;
    BOOL isOpen;
    GdsProfile *profile;
    BrNetSeq *seq;
    BrNetRequestParam param;
    // Set by the GDS client's callbacks as the requests finish
    BOOL uploadMusicalShotEnd;
    BOOL downloadMusicalShotEnd;
    BOOL uploadVideoEnd;
    BOOL downloadVideoEnd;
    BOOL downloadVideoListEnd;
    u32 unk5cc;
    u32 unk5d0;
    u32 unk5d4;
    BOOL unk5d8End;
    BOOL isFailed;
    GameData *gameData;
};

static void func_ov186_021ff8e0(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ff8e4(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ff900(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ff938(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ff968(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
void func_ov186_021ff99c(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
void func_ov186_021ff9d0(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ffa14(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ffa40(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ffa6c(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
void func_ov186_021ffa98(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net);
static void func_ov186_021ffacc(void *work, const GdsRapResult *result);
static void func_ov186_021ffad8(void *work, const GdsRapResult *result);
static void func_ov186_021ffae4(void *work, const GdsRapResult *result);
static void func_ov186_021ffaf0(void *work, const GdsRapResult *result);
static void func_ov186_021ffafc(void *work, const GdsRapResult *result);
static void func_ov186_021ffb08(void *work, const GdsRapResult *result);
static BrNetSeq *func_ov186_021ffb14(BrNet *net, BrNetSeqFunc func, HeapID heapId);
static void func_ov186_021ffb50(BrNetSeq *p_wk);
static void func_ov186_021ffb58(BrNetSeq *p_wk);
static void func_ov186_021ffb68(BrNetSeq *p_wk, BrNetSeqFunc func);
static BOOL func_ov186_021ffb70(const BrNetSeq *cp_wk, BrNetSeqFunc func);
static u32 func_ov186_021ffb80(void *work, int a1, int event, int a3);


BrNet *func_ov186_021ff408(GameData *gameData, void *loginBuffer, HeapID heapId) {
    GdsRapInit init;
    BrNet *net;

    GFL_OvlLoad(OVERLAY_ID(125));
    net = GFL_HeapAllocate(heapId, sizeof(BrNet), FALSE, "br_net.c", 151);
    MI_CpuFill8(net, 0, sizeof(BrNet));
    net->gameData = gameData;

    MI_CpuFill8(&init, 0, sizeof(GdsRapInit));
    init.gameData = gameData;
    init.profileId = func_0200853c(GetGameDataPlayerInfo(gameData));
    init.loginBuffer = loginBuffer;
    init.callbacks.work = net;
    init.callbacks.onUploadMusicalShotEnd = func_ov186_021ffacc;
    init.callbacks.onDownloadMusicalShotEnd = func_ov186_021ffad8;
    init.callbacks.onUploadVideoEnd = func_ov186_021ffae4;
    init.callbacks.onDownloadVideoListEnd = func_ov186_021ffaf0;
    init.callbacks.onDownloadVideoEnd = func_ov186_021ffafc;
    init.callbacks.onFavoriteVideoEnd = func_ov186_021ffb08;
    func_ov125_021d6660(&net->gdsrap, &init);

    net->isOpen = TRUE;
    GameData_GetSaveControl(gameData);
    net->profile = func_0200d59c(heapId);
    func_0200d5d0(net->profile, gameData);
    net->seq = func_ov186_021ffb14(net, func_ov186_021ff8e0, heapId);
    func_ov009_02156de0(func_ov186_021ffb80, net);
    return net;
}

void func_ov186_021ff4e4(BrNet *net) {
    func_ov009_02156de0(NULL, NULL);
    func_ov186_021ffb50(net->seq);
    func_0200d5c8(net->profile);
    if (net->isOpen) {
        func_ov125_021d6770(&net->gdsrap);
        net->isOpen = FALSE;
    }
    GFL_HeapFree(net);
    GFL_OvlUnload(OVERLAY_ID(125));
}

// pokebw2's BrNet_Main
void func_ov186_021ff52c(BrNet *net) {
    if (net->isOpen) {
        func_ov186_021ffb58(net->seq);
        func_ov125_021d6a34(&net->gdsrap);
    }
}

void func_ov186_021ff54c(BrNet *net, s32 type, const BrNetRequestParam *param) {
    GFL_ASSERT(type < 9);
    if (param != NULL) {
        net->param = *param;
    }
    MI_CpuFill8(&net->uploadMusicalShotEnd, 0, 0x24);
    switch (type) {
    case 0:
        func_ov186_021ffb68(net->seq, func_ov186_021ff900);
        break;
    case 1:
        func_ov186_021ffb68(net->seq, func_ov186_021ff938);
        break;
    case 2:
        func_ov186_021ffb68(net->seq, func_ov186_021ff968);
        break;
    case 3:
        func_ov186_021ffb68(net->seq, func_ov186_021ff99c);
        break;
    case 4:
        func_ov186_021ffb68(net->seq, func_ov186_021ff9d0);
        break;
    case 5:
        func_ov186_021ffb68(net->seq, func_ov186_021ffa14);
        break;
    case 6:
        func_ov186_021ffb68(net->seq, func_ov186_021ffa40);
        break;
    case 7:
        func_ov186_021ffb68(net->seq, func_ov186_021ffa6c);
        break;
    case 8:
        func_ov186_021ffb68(net->seq, func_ov186_021ffa98);
        break;
    default:
        GFL_ASSERT(FALSE);
        break;
    }
}


// pokebw2's BrNet_IsRequestEnd
BOOL func_ov186_021ff648(BrNet *net) {
    return func_ov186_021ffb70(net->seq, func_ov186_021ff8e0);
}

// pokebw2's BrNet_GetDownloadMusicalShot
BOOL func_ov186_021ff660(BrNet *net, BrMusicalShotRecv **tbl, int tblMax, int *recvNum) {
    BOOL isEnd = net->downloadMusicalShotEnd;

    if (isEnd) {
        *recvNum = func_ov125_021d7374(&net->gdsrap, (POKE_NET_GDS_MUSICALSHOT_SEND **)tbl, tblMax);
    }
    return isEnd;
}

u64 func_ov125_021d73e0(GdsRapWork *gdsrap);
BOOL func_ov186_021ff67c(BrNet *net, u64 *number) {
    BOOL isEnd = net->uploadVideoEnd;

    if (isEnd) {
        *number = func_ov125_021d73e0(&net->gdsrap);
    }
    return isEnd;
}


// pokebw2's BrNet_GetDownloadVideo
BOOL func_ov186_021ff698(BrNet *net, void **video, u32 *value) {
    BOOL isEnd = net->downloadVideoEnd;

    if (isEnd) {
        *value = func_ov125_021d74d8(&net->gdsrap, (BattleRecVideo **)video);
    }
    return isEnd;
}

// pokebw2's BrNet_GetDownloadVideoListA
BOOL func_ov186_021ff6b4(BrNet *net, void *list, int listMax, int *num) {
    BOOL isEnd = net->downloadVideoListEnd;

    if (isEnd) {
        *num = func_ov125_021d7414(&net->gdsrap, list, listMax);
    }
    return *num != 0 && isEnd;
}

// pokebw2's BrNet_GetDownloadVideoListB
BOOL func_ov186_021ff6dc(BrNet *net, void *list, int listMax, int *num) {
    BOOL isEnd = net->downloadVideoListEnd;

    if (isEnd) {
        *num = func_ov125_021d7414(&net->gdsrap, list, listMax);
    }
    return *num != 0 && isEnd;
}

// pokebw2's BrNet_GetDownloadVideoListC
BOOL func_ov186_021ff704(BrNet *net, void *list, int listMax, int *num) {
    BOOL isEnd = net->downloadVideoListEnd;

    if (isEnd) {
        *num = func_ov125_021d7414(&net->gdsrap, list, listMax);
    }
    return *num != 0 && isEnd;
}

// pokebw2's BrNet_GetResultMsg
BOOL func_ov186_021ff72c(BrNet *net, u32 *msgID) {
    GdsRapResult *result;
    BOOL ret;

    if (func_ov125_021d72ec(&net->gdsrap, &result)) {
        ret = TRUE;
        if (result->type == 0) {
            if (result->detail != 0) {
                if (msgID != NULL) {
                    *msgID = result->detail + 0x11b;
                }
                ret = TRUE;
            }
        } else if (result->type == 1) {
            if (result->detail == 7) {
                if (msgID != NULL) {
                    *msgID = result->detail + 0x12a;
                }
                ret = TRUE;
            } else {
                ret = TRUE;
            }
        } else if (result->type == 2) {
            switch (result->code) {
            case 0x5208:
                if (result->detail != 0) {
                    if (msgID != NULL) {
                        *msgID = result->detail + 0xe6;
                    }
                    ret = 2;
                }
                break;
            case 0x5209:
                if (result->detail != 0) {
                    if (msgID != NULL) {
                        *msgID = result->detail + 0xed;
                    }
                    ret = 2;
                }
                break;
            case 0x55f0:
                if (result->detail != 0) {
                    switch (result->detail) {
                    case 0x96:
                        GFL_SndStop();
                        func_0200fb4c(0x41);
                        ret = 3;
                        break;
                    case 0x97:
                        GFL_SndStop();
                        func_0200fb4c(0x42);
                        ret = 3;
                        break;
                    case 0x98:
                        GFL_SndStop();
                        func_0200fb4c(0x44);
                        ret = 3;
                        break;
                    case 0x99:
                        GFL_SndStop();
                        func_0200fb4c(0x43);
                        ret = 3;
                        break;
                    default:
                        if (msgID != NULL) {
                            *msgID = result->detail + 0x105;
                        }
                        ret = 2;
                        break;
                    }
                }
                break;
            case 0x55f1:
                if (result->detail != 0) {
                    if (msgID != NULL) {
                        *msgID = result->detail + 0x10d;
                    }
                    ret = 2;
                }
                break;
            case 0x55f2:
                if (result->detail != 0) {
                    if (msgID != NULL) {
                        *msgID = result->detail + 0x112;
                    }
                    ret = 1;
                }
                break;
            case 0x55f3:
                ret = 0;
                break;
            }
        }

        func_ov125_021d72f8(&net->gdsrap);
        if (ret == 3) {
            net->isFailed = TRUE;
        }
        return ret;
    }
    return FALSE;
}

// pokebw2's BrNet_CheckError
u32 func_ov186_021ff88c(BrNet *net) {
    BOOL ret;

    if (net == NULL) {
        return FALSE;
    }

    ret = TRUE;
    switch (func_ov009_02157170(TRUE, FALSE)) {
    case 1:
        GFL_SndStop();
        return ret;
    case 2:
        GFL_SndStop();
        net->isFailed = ret;
        return 2;
    case 3:
        GFL_SndStop();
        func_0200f7dc();
        break;
    }
    return FALSE;
}

// pokebw2's BrNet_IsFailed
BOOL func_ov186_021ff8d4(BrNet *net) {
    return net->isFailed;
}

// pokebw2's BrNet_SeqIdle
static void func_ov186_021ff8e0(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
}

// pokebw2's BrNet_SeqWaitEnd
static void func_ov186_021ff8e4(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    if (func_ov125_021d730c(&net->gdsrap)) {
        func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e0);
    }
}

// pokebw2's BrNet_SeqUploadMusicalShot
static void func_ov186_021ff900(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d67c8(&net->gdsrap, net->profile, net->param.data);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

// pokebw2's BrNet_SeqDownloadMusicalShot
static void func_ov186_021ff938(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d6814(&net->gdsrap, net->param.species);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

// pokebw2's BrNet_Seq2
static void func_ov186_021ff968(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d683c(&net->gdsrap, net->profile);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

void func_ov186_021ff99c(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d69dc(&net->gdsrap, net->param.dataNumber);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

void func_ov186_021ff9d0(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d68ec(&net->gdsrap, net->param.species, net->param.unk4, net->param.unk2, net->param.unk3);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}



// pokebw2's BrNet_Seq5
static void func_ov186_021ffa14(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d6968(&net->gdsrap);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

// pokebw2's BrNet_Seq6
static void func_ov186_021ffa40(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d69b4(&net->gdsrap);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

// pokebw2's BrNet_Seq7
static void func_ov186_021ffa6c(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d698c(&net->gdsrap);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}

void func_ov186_021ffa98(BrNetSeq *p_seqwk, u32 *p_seq, BrNet *net) {
    BOOL ret = func_ov125_021d6a08(&net->gdsrap, net->param.dataNumber);

    GFL_ASSERT(ret);
    func_ov186_021ffb68(p_seqwk, func_ov186_021ff8e4);
}


// pokebw2's BrNet_OnUploadMusicalShotEnd
void func_ov186_021ffacc(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->uploadMusicalShotEnd = TRUE;
}

// pokebw2's BrNet_OnDownloadMusicalShotEnd
static void func_ov186_021ffad8(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->downloadMusicalShotEnd = TRUE;
}

// pokebw2's BrNet_OnUploadVideoEnd
static void func_ov186_021ffae4(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->uploadVideoEnd = TRUE;
}

// pokebw2's BrNet_OnDownloadVideoListEnd
static void func_ov186_021ffaf0(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->downloadVideoListEnd = TRUE;
}

// pokebw2's BrNet_OnDownloadVideoEnd
static void func_ov186_021ffafc(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->downloadVideoEnd = TRUE;
}

// pokebw2's BrNet_OnUnk5d8End
static void func_ov186_021ffb08(void *work, const GdsRapResult *result) {
    BrNet *net = work;

    net->unk5d8End = TRUE;
}

// pokebw2's BrNetSeq_Init
static BrNetSeq *func_ov186_021ffb14(BrNet *net, BrNetSeqFunc func, HeapID heapId) {
    BrNetSeq *p_wk = GFL_HeapAllocate(heapId, sizeof(BrNetSeq), FALSE, "br_net.c", 1103);

    MI_CpuFill8(p_wk, 0, sizeof(BrNetSeq));
    p_wk->net = net;
    func_ov186_021ffb68(p_wk, func);
    return p_wk;
}

// pokebw2's BrNetSeq_Exit
static void func_ov186_021ffb50(BrNetSeq *p_wk) {
    GFL_HeapFree(p_wk);
}

// pokebw2's BrNetSeq_Main
static void func_ov186_021ffb58(BrNetSeq *p_wk) {
    if (p_wk->func != NULL) {
        p_wk->func(p_wk, &p_wk->seq, p_wk->net);
    }
}

// pokebw2's BrNetSeq_SetNext
static void func_ov186_021ffb68(BrNetSeq *p_wk, BrNetSeqFunc func) {
    p_wk->func = func;
    p_wk->seq = 0;
}

// pokebw2's BrNetSeq_IsComp
static BOOL func_ov186_021ffb70(const BrNetSeq *cp_wk, BrNetSeqFunc func) {
    return cp_wk->func == func;
}

// pokebw2's BrNet_EventFunc
static u32 func_ov186_021ffb80(void *work, int a1, int event, int a3) {
    BrNet *net = work;

    if (GameData_CheckEventsPaused(net->gameData)) {
        return TRUE;
    }

    func_ov125_021d7124(&net->gdsrap, a1, event, a3);
    if (event == 3 || event == 6) {
        if (net->isOpen) {
            func_ov125_021d6770(&net->gdsrap);
            net->isOpen = FALSE;
        }
        func_ov009_02156de0(NULL, NULL);
    }
    return FALSE;
}
