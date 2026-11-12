// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btlv_core.c, ported by tools/port_file.py.
#include "types.h"
#include "work_area.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "app/bag.h"
#include "battle/b_bag_main.h"
#include "battle/b_plist_main.h"
#include "battle/btl_client.h"
#include "battle/btl_main.h"
#include "battle/btl_poke_select.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_string.h"
#include "battle/btl_tables.h"
#include "battle/btlv.h"
#include "battle/btlv_clact.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_scd.h"
#include "battle/tr_ai.h"
#include "battle/trainer_data.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/waza.h"
#include "system/gf_font.h"
#include "text/system/btlv_core.h"

// The procedure each display command starts
typedef struct BtlvCmdEntry {
    u32 cmd;
    BtlvMainProc proc;
} BtlvCmdEntry;

typedef struct BtlvViewParam {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
} BtlvViewParam;

// The size of BtlvCore's work, the running procedure's (func_ov093_021e9100: BtlvCaptureDemoWork and the others below),
// which they still fit on a PC target
#define BTLV_CORE_WORK_SIZE 0x80

struct BtlvCore {
    BtlMainModule *mainModule;
    BtlClient *client;
    BtlPokeCon *pokeCon;
    u8 clientId;
    u32 cmd;
    BtlvMainProc mainProc;
    s32 seq;
    BtlvSubProc subProc;
    u8 work[BTLV_CORE_WORK_SIZE];
    StrBuf *strbuf;
    Font *font;
    Font *font2;
    void *unkB8;
    BattleMon *mon;
    u32 monId;
    u32 result;
    BBagParam ov286;
    BPlistParam ov287;
    BtlvPokeSelectParam *selectParam;
    BtlvViewParam unk160;
    BtlvSelectTargetParam *unk170;
    u8 subSeq;
    u8 unk175;
    u8 unk176[2];
    TCBExManager *tcbManager;
    BtlvScu *scu;
    BtlvScd *scd;
    HeapID heapId;
};

// The work of the capture demo's procedure
typedef struct BtlvCaptureDemoWork {
    MsgData *msgData;
    s32 wait;
    u8 savedKeys;
} BtlvCaptureDemoWork;

WORK_AREA_CHECK(BtlvCaptureDemoWork, BTLV_CORE_WORK_SIZE);

typedef struct BtlvPosEffectWork {
    u16 unk00;
    u32 type;
    u16 unk08;
    u8 pos;
} BtlvPosEffectWork;

typedef struct BtlvMultiEffectWork {
    u8 pos[6];
    u8 count;
    u16 seq;
    u32 type;
    u16 unk10;
} BtlvMultiEffectWork;

typedef struct BtlvMonEffectWork {
    u8 pos;
    u8 unk01;
    u8 unk02;
    u8 monId;
} BtlvMonEffectWork;

typedef struct BtlvSwapWork {
    u8 clientId;
    u8 unk01;
    u8 unk02;
    u8 slot1;
    u8 slot2;
    u8 pos1;
    u8 pos2;
} BtlvSwapWork;

typedef struct BtlvRotateWork {
    u8 clientId;
    u8 unk01;
    u8 viewPos[3];
    u8 pos[3];
} BtlvRotateWork;

typedef struct BtlvTwoStringWork {
    StrBuf *first;
    StrBuf *second;
    u32 unk08;
} BtlvTwoStringWork;

WORK_AREA_CHECK(BtlvTwoStringWork, BTLV_CORE_WORK_SIZE);

BOOL func_ov093_021e9104(BtlvCore *core, s32 *seq, void *work);
BOOL func_ov093_021e91e8(BtlvCore *core, s32 *seq, void *arg);

const BtlvCmdEntry data_ov093_021f3840[2] = {
    { 0, func_ov093_021e9104 },
    { 1, func_ov093_021e91e8 },
};

const BGSysLCDConfig data_ov093_021f3850 = { GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BGMODE_3, GX_BG0_AS_3D };

const ClActSysSetup data_ov093_021f3860 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 56, 48, 48, 48, 16, 16 };

const BGSysVRAMConfig data_ov093_021f387c = {
    GX_VRAM_BG_128_D,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_64_E,  GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_16_I,        GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_01_AB, GX_VRAM_TEXPLTT_01_FG,   GX_OBJVRAMMODE_CHAR_1D_64K,  GX_OBJVRAMMODE_CHAR_1D_32K,
};

static inline void BtlvCore_SetSubProc(BtlvCore *core, BtlvSubProcFn main) {
    core->subProc.init = NULL;
    core->subProc.main = main;
    core->subProc.arg = core;
    core->subProc.seq = 0;
}

static inline BOOL BtlvCore_RunSubProc(BtlvCore *core) {
    if (core->subProc.init != NULL) {
        if (core->subProc.init(&core->subProc.seq, core->subProc.arg)) {
            core->subProc.init = NULL;
            core->subProc.seq = 0;
        }
        return FALSE;
    }
    if (core->subProc.main != NULL) {
        if (core->subProc.main(&core->subProc.seq, core->subProc.arg)) {
            core->subProc.main = NULL;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void func_ov093_021e8dc4(HeapID heapId) {
    func_ov093_021e8e38(heapId);
    GFL_OvlLoad(OVERLAY_ID(94));
    GX_SetBankForLCDC(0x80);
    GFL_OvlLoad(OVERLAY_ID(95));
    if (func_0203d29c() == FALSE) {
        GFL_OvlLoad(OVERLAY_ID(96));
    }
}

void func_ov093_021e8df8(void) {
    func_ov093_021e8f08();
    GFL_OvlUnload(OVERLAY_ID(94));
    GFL_OvlUnload(OVERLAY_ID(95));
    if (func_0203d29c() == FALSE) {
        GFL_OvlUnload(OVERLAY_ID(96));
    }
}

void func_ov093_021e8e28(HeapID heapId) {
    func_ov093_021e8f08();
    func_ov093_021e8e38(heapId);
}

void func_ov093_021e8e38(HeapID heapId) {
    GFL_BGSysCreate(heapId);
    BmpWin_InitAllocator(heapId);
    func_0201dd60();
    GFL_BGSysSetVRAMBanks(&data_ov093_021f387c);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetLCDConfig(&data_ov093_021f3850);
    GFL_G3DSysCreate(FALSE, 1, 0, 1, 0, heapId, NULL);
    GFL_G3DSysSetSwapBufferParams(1, 0);
    G3X_AlphaBlend(TRUE);
    G3X_EdgeMarking(FALSE);
    G3X_AntiAlias(TRUE);
    G3X_SetFog(FALSE, 0, 0, 0);
    G2_SetBG0Priority(1);
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG1,
                     GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ |
                         GX_BLEND_PLANEMASK_BD,
                     0x1f, 3);
    ClActSys_Create(&data_ov093_021f3860, &data_ov093_021f387c, heapId);
}

void func_ov093_021e8f08(void) {
    func_ov094_021f7808();
    func_0204a5ac();
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_G3DSysFree();
}

BtlvCore *BtlvCore_Create(BtlMainModule *mainModule, BtlClient *client, BtlPokeCon *pokeCon, u32 arg3, HeapID heapId) {
    BtlvCore *core;

    core = GFL_HeapAllocate(heapId, sizeof(BtlvCore), FALSE, "btlv_core.c", 301);
    core->mainModule = mainModule;
    core->client = client;
    core->pokeCon = pokeCon;
    core->clientId = func_ov093_021d3d70(client);
    core->cmd = 4;
    core->heapId = heapId;
    core->strbuf = GFL_StrBufCreate(0x200, heapId);
    core->font = GFL_FontCreate(0x17, 0, 0, 0, heapId);
    core->font2 = GFL_FontCreate(0x17, 2, 0, 0, heapId);
    core->tcbManager = GFL_TCBExMgrCreate(heapId, heapId, 0x40, 0x80);
    core->scu = func_ov093_021eaf7c(core, core->mainModule, pokeCon, core->tcbManager, core->font, core->font2,
                                    core->clientId, heapId);
    core->scd = data_ov095_06899af1(core, core->mainModule, pokeCon, core->tcbManager, core->font, core->client,
                                    arg3, heapId);
    core->mainProc = NULL;
    core->seq = 0;
    core->subSeq = 0;
    func_ov093_021eec78(mainModule, core->clientId, pokeCon, heapId);
    return core;
}

void func_ov093_021e9034(BtlvCore *core) {
    func_ov093_021eecd8();
    data_ov095_06899dd9(core->scd);
    func_ov093_021eb234(core->scu);
    GFL_TCBExMgrFree(core->tcbManager);
    GFL_StrBufFree(core->strbuf);
    GFL_FontFree(core->font);
    GFL_FontFree(core->font2);
    GFL_HeapFree(core);
}

void func_ov093_021e908c(BtlvCore *core) {
    GFL_TCBExMgrUpdate(core->tcbManager);
    func_ov094_021f78e4();
}

void func_ov093_021e90a0(BtlvCore *core, u32 cmd) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (cmd == data_ov093_021f3840[i].cmd) {
            core->cmd = cmd;
            core->mainProc = data_ov093_021f3840[i].proc;
            core->seq = 0;
            return;
        }
    }
}

BOOL func_ov093_021e90d0(BtlvCore *core) {
    if (core->cmd != 4) {
        if (core->mainProc(core, &core->seq, core->work)) {
            core->cmd = 4;
            core->mainProc = NULL;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void *func_ov093_021e9100(BtlvCore *core, u32 size) {
    return core->work;
}

BOOL func_ov093_021e9104(BtlvCore *core, s32 *seq, void *work) {
    BtlvEffectSetup *setup;

    switch (*seq) {
    case 0:
        setup = func_ov094_021f745c(core->mainModule, core->scu, core->heapId);
        func_ov094_021f7544(setup, core->font2, core->heapId);
        GFL_HeapFree(setup);
        func_ov093_021eb028(core->scu);
        data_ov095_06899be1(core->scd);
        (*seq)++;
        break;
    case 1:
        func_ov093_021eb32c(core->scu, func_ov093_021cd9e0(core->client));
        (*seq)++;
        break;
    case 2:
        if (func_ov093_021eb52c(core->scu)) {
            if (func_ov093_021cd9c8(core->client)) {
                core->unk160.unk00 = 1;
                core->unk160.unk04 = 1;
                core->unk160.unk08 = func_ov093_021cd9dc(core->client);
                core->unk160.unk0C = 0;
                if (!func_ov093_021cd9e0(core->client)) {
                    data_ov095_0689b781(core->scd, &core->unk160);
                }
            }
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021e91e8(BtlvCore *core, s32 *seq, void *arg) {
    BtlvCaptureDemoWork *work = arg;
    BtlvEffectSetup *setup;
    BtlvStringParam param1;
    BtlvStringParam param2;
    BtlvStringParam param3;
    BattleMon *mon;

    switch (*seq) {
    case 0:
        setup = func_ov094_021f745c(core->mainModule, core->scu, core->heapId);
        func_ov094_021f7544(setup, core->font2, core->heapId);
        GFL_HeapFree(setup);
        func_ov093_021eb028(core->scu);
        data_ov095_06899be1(core->scd);
        work->msgData = GFL_MsgSysLoadData(0, 2, 0x14, core->heapId);
        work->wait = 0;
        (*seq)++;
        break;
    case 1:
        func_ov093_021eb32c(core->scu, 0);
        (*seq)++;
        break;
    case 2:
        if (func_ov093_021eb52c(core->scu)) {
            mon = func_ov093_021e9634(core, 0);
            Btlv_StringParam_Setup(&param1, 1, 0x45);
            Btlv_StringParam_AddArg(&param1, GetMonID(mon));
            func_ov093_021eaf78(&param1, 0);
            func_ov093_021ea65c(core, &param1);
            (*seq)++;
        }
        break;
    case 3:
        core->mon = func_ov093_021e9634(core, 0);
        core->monId = GetMonID(core->mon);
        core->unkB8 = NULL;
        core->result = 0;
        core->unk175 = 0;
        data_ov095_06899f01(core->scd, core->mon, core->unk175, NULL);
        (*seq)++;
        break;
    case 4:
        if (data_ov095_06899f29(core->scd)) {
            data_ov095_06899fe9(core->scd, core->mon, core->unkB8);
            (*seq)++;
        }
        break;
    case 5:
        if (data_ov095_06899f29(core->scd)) {
            func_ov093_021ea6f4(core, GetMonID(func_ov093_021e9634(core, 0)), 1);
            data_ov095_06899d65(core->scd);
            (*seq)++;
        }
        break;
    case 6:
        if (func_ov093_021ea710(core)) {
            if (data_ov095_06899db1(core->scd)) {
                func_ov093_021ed118(core->scu, 0, 1, 1, PML_MoveGetParam(1, 0x1b), 0, 0);
                (*seq)++;
            }
        }
        break;
    case 7:
        if (func_ov093_021ed1b0(core->scu)) {
            func_ov094_021f80fc(1, (s32)GetBattleMonStat(func_ov093_021e9634(core, 1), 0xe) / 6);
            func_ov094_021f7c2c(1, 1);
            func_ov093_021eaf00(core, 0x564);
            (*seq)++;
        }
        break;
    case 8:
        if (func_ov093_021ed250(core->scu)) {
            func_ov093_021ea6f4(core, GetMonID(func_ov093_021e9634(core, 1)), 0x2b);
            (*seq)++;
        }
        break;
    case 9:
        if (func_ov093_021ea710(core)) {
            func_ov093_021ed118(core->scu, 1, 0, 0x2b, PML_MoveGetParam(0x2b, 0x1b), 0, 0);
            (*seq)++;
        }
        break;
    case 10:
        if (func_ov093_021ed1b0(core->scu)) {
            func_ov093_021ea7c8(core, 0);
            (*seq)++;
        }
        break;
    case 11:
        if (func_ov093_021ea7e8(core, 0)) {
            mon = func_ov093_021e9634(core, 0);
            Btlv_StringParam_Setup(&param2, 2, 0x5d);
            Btlv_StringParam_AddArg(&param2, GetMonID(mon));
            func_ov093_021eaf78(&param2, 0);
            func_ov093_021ea614(core, &param2);
            (*seq)++;
        }
        break;
    case 12:
        if (func_ov093_021ea710(core)) {
            GFL_MsgDataLoadStrbuf(work->msgData, 0, core->strbuf);
            func_ov093_021ea730(core, core->strbuf, 0, 0);
            work->wait = 0x3c;
            (*seq)++;
        }
        break;
    case 13:
        if (func_ov093_021ea710(core)) {
            work->wait--;
            if (work->wait == 0) {
                data_ov095_06899f01(core->scd, core->mon, core->unk175, core->unkB8);
                (*seq)++;
            }
        }
        break;
    case 14:
        if (data_ov095_06899f29(core->scd)) {
            BattleClientCmd_StartItemSelect(core, 2, 0, 0, 1, 0);
            (*seq)++;
        }
        break;
    case 15:
        if (func_ov093_021e9e80(core)) {
            GFL_MsgDataLoadStrbuf(work->msgData, 1, core->strbuf);
            func_ov093_021ea730(core, core->strbuf, 0, 0);
            data_ov095_06899d65(core->scd);
            (*seq)++;
        }
        break;
    case 16:
        if (func_ov093_021ea710(core)) {
            if (!func_ov094_021f7eb4()) {
                func_ov094_021f7cfc(1, 4, 3, 1, 0);
                (*seq)++;
            }
        }
        break;
    case 17:
        if (!func_ov094_021f7eb4()) {
            mon = func_ov093_021e9634(core, 1);
            Btlv_StringParam_Setup(&param3, 1, 0x41);
            Btlv_StringParam_AddArg(&param3, GetMonID(mon));
            func_ov093_021ea614(core, &param3);
            (*seq)++;
        }
        break;
    case 18:
        if (func_ov093_021ea720(core)) {
            GFL_SndBGMPlay(0x518, 0xffff);
        }
        if (func_ov093_021ea710(core)) {
            (*seq)++;
        }
        break;
    case 19:
        if (!GFL_SndBGMIsPlaying()) {
            func_ov094_021f7c0c();
            GFL_MsgDataFree(work->msgData);
            (*seq)++;
        }
        break;
    case 20:
        return TRUE;
    }
    return FALSE;
}

BattleMon *func_ov093_021e9634(BtlvCore *core, u32 index) {
    return func_ov093_021b98ac(core->pokeCon, func_ov093_021b8ed8(core->mainModule, index));
}

BOOL func_ov093_021e964c(BtlvCore *core, s32 *seq, void *work) {
    switch (*seq) {
    case 0:
        data_ov095_06899ed1(core->scd, core->mon, core->unk175, func_ov093_021d3d90(core->client),
                            core->unkB8);
        (*seq)++;
    case 1:
        core->result = data_ov095_06899f29(core->scd);
        if (core->result != 0) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

BOOL func_ov093_021e96c4(BtlvCore *core, s32 *seq, void *work) {
    switch (*seq) {
    case 0:
        data_ov095_0689a009(core->scd, core->mon, core->unkB8);
        (*seq)++;
        break;
    case 1:
        if (data_ov095_0689a021(core->scd)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021e9728(BtlvCore *core, BtlvMainProc proc) {
    core->mainProc = proc;
    core->seq = 0;
}

void func_ov093_021e9730(BtlvCore *core) {
    core->mainProc = NULL;
    core->seq = 0;
}

BOOL func_ov093_021e9738(BtlvCore *core) {
    if (core->mainProc != NULL) {
        if (core->mainProc(core, &core->seq, core->work)) {
            core->mainProc = NULL;
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

void func_ov093_021e9760(BtlvCore *core, BattleMon *mon, u32 arg2, void *arg3) {
    core->mon = mon;
    core->monId = GetMonID(mon);
    core->unkB8 = arg3;
    core->result = 0;
    core->unk175 = arg2;
    func_ov093_021e9728(core, func_ov093_021e964c);
}

void func_ov093_021e979c(BtlvCore *core) {
    func_ov093_021e9928(core);
}

u32 func_ov093_021e97a4(BtlvCore *core) {
    if (func_ov093_021e9738(core)) {
        return core->result;
    }
    return 0;
}

void func_ov093_021e97bc(BtlvCore *core, BattleMon *mon, void *arg2) {
    core->mon = mon;
    core->monId = GetMonID(mon);
    core->unkB8 = arg2;
    data_ov095_06899f41(core->scd, core->mon, arg2);
    func_ov093_021e9728(core, func_ov093_021e9884);
}

void func_ov093_021e9808(BtlvCore *core, BtlvSelectTargetParam *param, void *arg2) {
    BattleMon *mon;

    mon = param->mons[0].mon;
    core->mon = mon;
    core->monId = GetMonID(mon);
    core->unk170 = param;
    core->unkB8 = arg2;
    data_ov095_06899f65(core->scd, param, arg2);
    func_ov093_021e9728(core, func_ov093_021e9884);
}

void func_ov093_021e9858(BtlvCore *core) {
    data_ov095_06899fdd(core->scd);
    func_ov093_021e9728(core, func_ov093_021e9884);
}

BOOL func_ov093_021e9884(BtlvCore *core, s32 *seq, void *work) {
    if (data_ov095_06899fd5(core->scd)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021e98ac(BtlvCore *core) {
    func_ov093_021e9928(core);
}

BOOL func_ov093_021e98b4(BtlvCore *core) {
    return func_ov093_021e9738(core);
}

void func_ov093_021e98bc(BtlvCore *core, BattleMon *mon, void *arg2) {
    core->mon = mon;
    core->monId = GetMonID(mon);
    core->unkB8 = arg2;
    func_ov093_021e9728(core, func_ov093_021e96c4);
}

u32 func_ov093_021e98e8(BtlvCore *core) {
    if (func_ov093_021e9738(core)) {
        if (data_ov095_0689b0cd(core->scd)) {
            return 1;
        }
        return 2;
    }
    return 0;
}

void func_ov093_021e9920(BtlvCore *core) {
    func_ov093_021e9928(core);
}

void func_ov093_021e9928(BtlvCore *core) {
    func_ov094_021f8a78();
    data_ov095_0689a029(core->scd);
    func_ov093_021e9730(core);
}

void func_ov093_021e9954(BtlvCore *core) {
    data_ov095_06899d3d(core->scd);
}

void func_ov093_021e9964(BtlvCore *core) {
    data_ov095_06899d65(core->scd);
}

BOOL func_ov093_021e9974(BtlvCore *core) {
    return data_ov095_06899db1(core->scd);
}

void func_ov093_021e9984(BtlvCore *core, BPlistParam *param, u32 mode, u8 partyIndex, u32 move) {
    u8 clientId;
    u8 unk1C;
    BOOL multi;

    clientId = func_ov093_021d3d70(core->client);
    param->party = func_ov093_021b8f5c(core->mainModule, clientId);
    if (mode != 3) {
        multi = func_ov093_021b8f38(core->mainModule);
    } else {
        multi = FALSE;
    }
    param->unk18 = multi;
    if (multi) {
        param->unk8 = func_ov093_021b8f7c(core->mainModule, clientId);
        unk1C = func_ov093_021b8fc0(core->mainModule, clientId);
    } else {
        param->unk8 = NULL;
        unk1C = 0;
    }
    param->unk1F = mode;
    param->unk1C = unk1C;
    if (BtlSetup_GetBattleStyle(core->mainModule) != 3) {
        param->unk22 = func_ov093_021b99b8(core->mainModule, clientId);
    } else {
        param->unk22 = 3;
    }
    param->partyIndex = (mode == 4 || mode == 5) ? partyIndex : 0;
    param->unk21 = partyIndex;
    param->move = move;
    param->slot = 0;
    param->heapId = core->heapId;
    param->font = core->font;
    param->unk14 = BtlSetup_GetBattleStyle(core->mainModule);
    param->usingKeys = data_ov095_0689b66d(core->scd);
    param->tcbManager = func_ov094_021f86d4();
    param->paletteFade = func_ov094_021f86e0();
    param->gameData = func_ov093_021b8748(core->mainModule);
    param->unk30 = 0;
    param->unk40 = BtlSetup_GetBattleType(core->mainModule) != 3;
    param->unk34 = 0;
    param->done = 0;
}

void BattleClientCmd_StartPokeList(BtlvCore *core, const BtlvPokeListCmd *cmd, s32 partyIndex, u16 move,
                                   BtlvPokeSelectParam *select) {
    u32 i;
    u32 count;

    if (partyIndex < 0) {
        partyIndex = 0;
    }
    func_ov093_021e9984(core, &core->ov287, cmd->mode, partyIndex, move);
    count = data_ov095_0689cb19(select);
    for (i = 0; i < count; i++) {
        if (i >= 2) {
            break;
        }
        core->ov287.unk1D[i] = data_ov095_0689cb2d(select, i);
    }
    for (; i < 2; i++) {
        core->ov287.unk1D[i] = 6;
    }
    select->selecting = 1;
    core->selectParam = select;
    core->subSeq = 0;
}

void BattleClientCmd_QuitPokeSelect(BtlvCore *core) {
    core->ov287.unk30 = 1;
}

BOOL BattleClientCmd_WaitPokeSelect(BtlvCore *core) {
    u32 i;

    switch (core->subSeq) {
    case 0:
        if (core->ov287.unk1F == 1 || core->ov287.unk1F == 2) {
            data_ov095_06899ca5(core->scd, 0);
        } else {
            data_ov095_06899ca5(core->scd, 1);
        }
        GFL_OvlLoad(OVERLAY_ID(196));
        core->subSeq++;
        break;
    case 1:
        if (data_ov095_06899d05(core->scd)) {
            data_ov095_06899d2d(core->scd);
            GFL_OvlLoad(OVERLAY_ID(198));
            func_ov198_0220f3e8(&core->ov287);
            core->subSeq++;
        }
        break;
    case 2:
        if (core->ov287.done) {
            if (core->ov287.unk1F != 5 && core->ov287.unk30 == 0) {
                for (i = 0; i < 3; i++) {
                    if (core->ov287.unk48[i] != 0xff) {
                        data_ov095_0689caed(core->selectParam, i, core->ov287.unk48[i]);
                        data_ov095_0689cb19(core->selectParam);
                        core->selectParam->selecting = 0;
                    }
                }
            }
            GFL_OvlUnload(OVERLAY_ID(198));
            data_ov095_06899cdd(core->scd);
            data_ov095_06899d31(core->scd);
            core->subSeq++;
        }
        break;
    case 3:
        if (data_ov095_06899d05(core->scd)) {
            core->subSeq++;
        }
        break;
    default:
        GFL_OvlUnload(OVERLAY_ID(196));
        core->subSeq = 0;
        return TRUE;
    }
    return FALSE;
}

void BattleClientCmd_StartMoveInfoView(BtlvCore *core, u8 partyIndex, u8 slot) {
    func_ov093_021e9984(core, &core->ov287, 5, partyIndex, 0);
    core->ov287.slot = slot;
    core->subSeq = 0;
}

void BattleClientCmd_StartItemSelect(BtlvCore *core, u32 mode, u8 arg2, u8 arg3, u8 canUseItems, u8 arg5) {
    u32 kind;
    BattleParty *party;
    u32 count;
    u32 i;

    if (core->subSeq == 0) {
        core->ov286.bag = func_ov093_021b90e8(core->mainModule);
        core->ov286.bagCursor = func_ov093_021b90f0(core->mainModule);
        core->ov286.mode = mode;
        core->ov286.font = core->font;
        core->ov286.heapId = core->heapId;
        core->ov286.shooterEnergy = arg2;
        core->ov286.shooterSpent = arg3;
        core->ov286.item = 0;
        core->ov286.usingKeys = data_ov095_0689b66d(core->scd);
        core->ov286.quit = 0;
        core->ov286.shooterDisabled = func_ov093_021ba1e4(core->mainModule);
        kind = 0;
        if (BtlSetup_GetBattleType(core->mainModule) != 3) {
            kind = 1;
        }
        core->ov286.playSound = kind;
        kind = 0;
        core->ov286.abort = 0;
        if (BtlSetup_GetBattleType(core->mainModule) == 0) {
            kind = 1;
        }
        core->ov286.isWild = kind;
        kind = 0;
        core->ov286.done = FALSE;
        if (!canUseItems) {
            kind = 3;
        } else if (BtlSetup_IsBattleType(core->mainModule, 0x40)) {
            kind = 1;
        } else if (BtlSetup_GetBattleType(core->mainModule) == 0) {
            if (func_ov093_021b9b04(core->pokeCon, 1) > 1) {
                kind = 2;
            } else if (arg5) {
                kind = 4;
            }
        }
        core->ov286.ballError = kind;
        func_ov093_021e9984(core, &core->ov287, 3, 0, 0);
        party = func_ov093_021d3d7c(core->client);
        count = GetNumMonsInParty(party);
        for (i = 0; i < count; i++) {
            core->ov287.unk38[i] = CheckCondition(GetBattleMonFromParty(party, i), 0x13);
        }
        for (; i < 6; i++) {
            core->ov287.unk38[i] = 0;
        }
        core->subSeq = 1;
    }
}

void func_ov093_021e9e70(BtlvCore *core) {
    core->ov286.quit = 1;
    core->ov287.unk30 = 1;
}

BOOL func_ov093_021e9e80(BtlvCore *core) {
    switch (core->subSeq) {
    case 1:
        data_ov095_06899ca5(core->scd, 1);
        GFL_OvlLoad(OVERLAY_ID(196));
        core->subSeq = 2;
        break;
    case 2:
        if (data_ov095_06899d05(core->scd)) {
            data_ov095_06899d2d(core->scd);
            GFL_OvlLoad(OVERLAY_ID(197));
            func_ov197_0220c834(&core->ov286);
            core->subSeq = 3;
        }
        break;
    case 3:
        if (core->ov286.done) {
            GFL_OvlUnload(OVERLAY_ID(197));
            if (core->ov286.quit == 0) {
                core->subSeq = 4;
            } else {
                core->subSeq = 6;
            }
        }
        break;
    case 4:
        if (core->ov286.item != 0 && core->ov286.pocket != 2 && !data_ov095_0689c90d(core->ov286.item)) {
            core->ov287.item = core->ov286.item;
            GFL_OvlLoad(OVERLAY_ID(198));
            func_ov198_0220f3e8(&core->ov287);
            core->subSeq = 5;
        } else {
            core->subSeq = 6;
        }
        break;
    case 5:
        if (core->ov287.done) {
            GFL_OvlUnload(OVERLAY_ID(198));
            core->subSeq = 6;
        }
        break;
    case 6:
        data_ov095_06899cdd(core->scd);
        data_ov095_06899d31(core->scd);
        core->subSeq = 7;
        break;
    case 7:
        if (data_ov095_06899d05(core->scd)) {
            GFL_OvlUnload(OVERLAY_ID(196));
            core->subSeq = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

u16 func_ov093_021ea01c(BtlvCore *core) {
    if (core->ov286.done) {
        return core->ov286.item;
    }
    return 0;
}

u8 func_ov093_021ea030(BtlvCore *core) {
    if (core->ov286.done) {
        return core->ov286.cost;
    }
    return 0;
}

u8 func_ov093_021ea044(BtlvCore *core) {
    return core->ov287.partyIndex;
}

u8 func_ov093_021ea04c(BtlvCore *core) {
    return core->ov287.slot;
}

void func_ov093_021ea058(BtlvCore *core) {
    if (core->ov286.item != 0 && core->ov287.partyIndex != 6 && core->ov286.mode != 1) {
        func_020082b8(core->ov286.bagCursor, core->ov286.rows, core->ov286.pages);
        func_020082d4(core->ov286.bagCursor, core->ov286.item, core->ov286.pocket);
    }
}

void func_ov093_021ea0a0(BtlvCore *core, u8 attackerPos, u8 targetPos, u16 move, s32 arg4, u32 arg5, u8 arg6) {
    u8 attacker;
    u8 target;

    attacker = func_ov093_021b8e70(core->mainModule, attackerPos);
    if (targetPos != 6) {
        target = func_ov093_021b8e70(core->mainModule, targetPos);
    } else {
        target = 0xff;
    }
    func_ov093_021ed118(core->scu, attacker, target, move, arg4, arg5, arg6);
}

BOOL func_ov093_021ea0e4(BtlvCore *core) {
    return func_ov093_021ed1b0(core->scu);
}

void func_ov093_021ea0f4(BtlvCore *core, u16 arg1, u8 pos, u8 type) {
    BtlvPosEffectWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvPosEffectWork));
    work->type = type;
    work->pos = pos;
    work->unk08 = 0;
    work->unk00 = arg1;
    BtlvCore_SetSubProc(core, func_ov093_021ea160);
}

BOOL func_ov093_021ea120(BtlvCore *core) {
    return BtlvCore_RunSubProc(core);
}

BOOL func_ov093_021ea160(s32 *seq, void *arg) {
    BtlvCore *core = arg;
    BtlvPosEffectWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvPosEffectWork));
    switch (*seq) {
    case 0:
        func_ov093_021ed208(core->scu, work->pos, work->unk00, func_ov093_021cd9e0(core->client));
        func_ov093_021ea32c(core, work->type);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed250(core->scu)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021ea1b8(BtlvCore *core, u16 arg1, u8 pos, u32 type) {
    BtlvPosEffectWork *work;

    if (!func_ov093_021cd9e0(core->client)) {
        work = func_ov093_021e9100(core, sizeof(BtlvPosEffectWork));
        work->type = type;
        work->pos = pos;
        work->unk08 = 0;
        work->unk00 = arg1;
        BtlvCore_SetSubProc(core, func_ov093_021ea230);
    }
}

BOOL func_ov093_021ea1f0(BtlvCore *core) {
    return BtlvCore_RunSubProc(core);
}

BOOL func_ov093_021ea230(s32 *seq, void *arg) {
    BtlvCore *core = arg;
    BtlvPosEffectWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvPosEffectWork));
    switch (*seq) {
    case 0:
        func_ov093_021ed268(core->scu, work->pos, work->unk00);
        func_ov093_021ea32c(core, work->type);
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed280(core->scu)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021ea280(BtlvCore *core, u16 count, u32 type, const u8 *monIds, u16 arg4) {
    BtlvMultiEffectWork *work;
    u32 i;

    work = func_ov093_021e9100(core, sizeof(BtlvMultiEffectWork));
    work->type = type;
    work->count = count;
    work->unk10 = arg4;
    work->seq = 0;
    for (i = 0; i < count; i++) {
        work->pos[i] = MonIDToBattlePos(core->mainModule, core->pokeCon, monIds[i]);
    }
}

BOOL func_ov093_021ea2bc(BtlvCore *core) {
    BtlvMultiEffectWork *work;
    BOOL flag;
    u32 i;

    work = func_ov093_021e9100(core, sizeof(BtlvMultiEffectWork));
    switch (work->seq) {
    case 0:
        flag = func_ov093_021cd9e0(core->client);
        for (i = 0; i < work->count; i++) {
            func_ov093_021ed208(core->scu, work->pos[i], work->unk10, flag);
        }
        func_ov093_021ea32c(core, work->type);
        work->seq++;
        break;
    case 1:
        if (func_ov093_021ed250(core->scu) && func_ov093_021ecf68(core->scu)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021ea32c(BtlvCore *core, u32 type) {
    u32 se;

    switch (type) {
    case 2:
        se = 0x563;
        break;
    case 3:
        se = 0x565;
        break;
    default:
        se = 0x564;
        break;
    }
    func_ov093_021eaf00(core, se);
}

void func_ov093_021ea354(BtlvCore *core, u8 pos, u32 visible) {
    if (func_ov094_021f7f1c(pos) && visible != func_ov094_021f853c(pos)) {
        if (visible == 1) {
            func_ov094_021f7e74(pos);
        } else {
            func_ov094_021f7e84(pos);
        }
    }
}

void func_ov093_021ea384(BtlvCore *core, u8 pos) {
    func_ov093_021ed494(core->scu, pos, func_ov093_021cd9e0(core->client));
}

BOOL func_ov093_021ea3a0(BtlvCore *core) {
    if (func_ov093_021ed4d0(core->scu)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ea3b8(BtlvCore *core, u32 arg1, u16 arg2) {
    if (!func_ov093_021cd9e0(core->client)) {
        func_ov094_021f7a28(arg1, arg2);
    }
}

BOOL func_ov093_021ea3d4(BtlvCore *core) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ea3e8(BtlvCore *core, u32 arg1, u32 arg2, u16 arg3) {
    func_ov094_021f7aac(arg1, arg2, arg3);
}

BOOL func_ov093_021ea3f8(BtlvCore *core) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ea40c(BtlvCore *core, u8 pos) {
    func_ov093_021ed294(core->scu, pos, func_ov093_021cd9e0(core->client));
}

BOOL func_ov093_021ea428(BtlvCore *core) {
    return func_ov093_021ed2bc(core->scu);
}

void func_ov093_021ea438(BtlvCore *core, u8 pos) {
    func_ov093_021ed2d0(core->scu, pos);
}

BOOL func_ov093_021ea448(BtlvCore *core) {
    return func_ov093_021ed304(core->scu);
}

void func_ov093_021ea458(BtlvCore *core, u8 pos, u16 arg2) {
    func_ov093_021ed318(core->scu, pos, arg2, func_ov093_021cd9e0(core->client));
}

BOOL func_ov093_021ea478(BtlvCore *core) {
    return func_ov093_021ed374(core->scu);
}

void func_ov093_021ea488(BtlvCore *core, u8 pos, u8 arg2, u8 arg3) {
    BtlvMonEffectWork *work;
    BattleMon *mon;

    work = func_ov093_021e9100(core, sizeof(BtlvMonEffectWork));
    mon = func_ov093_021b98ac(core->pokeCon, pos);
    work->unk01 = arg2;
    work->unk02 = arg3;
    work->pos = pos;
    work->monId = GetMonID(mon);
    BtlvCore_SetSubProc(core, func_ov093_021ea504);
}

BOOL func_ov093_021ea4c4(BtlvCore *core) {
    return BtlvCore_RunSubProc(core);
}

BOOL func_ov093_021ea504(s32 *seq, void *arg) {
    BtlvCore *core = arg;
    BtlvMonEffectWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvMonEffectWork));
    switch (*seq) {
    case 0:
        func_ov093_021ed3d4(core->scu, work->pos, work->unk01, work->unk02, func_ov093_021cd9e0(core->client));
        (*seq)++;
        break;
    case 1:
        if (func_ov093_021ed450(core->scu)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021ea55c(const BtlvStringParam *param, StrBuf *strbuf) {
    switch (param->type) {
    case 1:
        func_ov093_021eee80(strbuf, param->message, param->args);
        break;
    case 2:
        func_ov093_021eeef4(strbuf, param->message, param->args);
        break;
    case 3:
        func_ov093_021ef694(strbuf, param->message);
        break;
    case 5:
        func_ov093_021ef6a8(strbuf, param->message, param->args);
        break;
    case 6:
        func_ov093_021ef6e8(strbuf, param->message, param->args);
        break;
    case 4:
        func_ov093_021ef5e8(strbuf, param->args[0], param->args[1]);
        break;
    case 7:
        func_ov093_021ef664(strbuf, param->args[0], param->args[1]);
        break;
    }
}

void func_ov093_021ea5e4(BtlvCore *core, const StrBuf *src) {
    GFL_StrBufCopy(core->strbuf, src);
}

void func_ov093_021ea5f0(BtlvCore *core, const BtlvStringParam *param) {
    func_ov093_021ea55c(param, core->strbuf);
}

void func_ov093_021ea600(BtlvCore *core) {
    func_ov093_021ea730(core, core->strbuf, 0x50, NULL);
}

void func_ov093_021ea614(BtlvCore *core, const BtlvStringParam *param) {
    func_ov093_021ea55c(param, core->strbuf);
    func_ov093_021ea730(core, core->strbuf, param->mode, 0);
}

void func_ov093_021ea638(BtlvCore *core, const BtlvStringParam *param, BtlvMsgCallback callback) {
    func_ov093_021ea55c(param, core->strbuf);
    func_ov093_021ea730(core, core->strbuf, param->mode, callback);
}

void func_ov093_021ea65c(BtlvCore *core, const BtlvStringParam *param) {
    func_ov093_021ea55c(param, core->strbuf);
    func_ov093_021ea758(core, core->strbuf);
}

void func_ov093_021ea678(BtlvCore *core, u16 message, const u32 *args) {
    func_ov093_021eee80(core->strbuf, message, args);
    func_ov093_021ea730(core, core->strbuf, 0x50, NULL);
}

void func_ov093_021ea694(BtlvCore *core, u16 message, const u32 *args) {
    func_ov093_021eeef4(core->strbuf, message, args);
    func_ov093_021ea730(core, core->strbuf, 0x50, NULL);
}

BOOL func_ov093_021ea6b0(BtlvCore *core, u16 trainerId, u16 msgId) {
    if (func_0202a298(trainerId, msgId, core->heapId)) {
        func_0202a310(trainerId, msgId, core->strbuf, core->heapId);
        func_ov093_021ea730(core, core->strbuf, 0x50, NULL);
    } else {
        return FALSE;
    }
    return TRUE;
}

void func_ov093_021ea6f4(BtlvCore *core, u8 monId, u16 move) {
    func_ov093_021ef5e8(core->strbuf, monId, move);
    func_ov093_021ea730(core, core->strbuf, 0x50, NULL);
}

BOOL func_ov093_021ea710(BtlvCore *core) {
    return func_ov093_021ecf68(core->scu);
}

BOOL func_ov093_021ea720(BtlvCore *core) {
    return func_ov093_021ecf50(core->scu);
}

void func_ov093_021ea730(BtlvCore *core, const StrBuf *strbuf, u16 wait, BtlvMsgCallback callback) {
    if (!func_ov093_021cd9e0(core->client)) {
        func_ov093_021eceb8(core->scu, strbuf, wait, callback);
    }
}

void func_ov093_021ea758(BtlvCore *core, const StrBuf *strbuf) {
    func_ov093_021ece60(core->scu, strbuf);
}

void func_ov093_021ea768(BtlvCore *core, u8 pos, BOOL flash) {
    func_ov093_021eda5c(core->scu, pos, flash);
}

BOOL func_ov093_021ea778(BtlvCore *core, u8 pos) {
    return func_ov093_021eda94(core->scu, pos);
}

void func_ov093_021ea788(BtlvCore *core, u8 pos) {
    func_ov093_021edab0(core->scu, pos);
}

BOOL func_ov093_021ea798(BtlvCore *core, u8 pos) {
    return func_ov093_021edae0(core->scu, pos);
}

void func_ov093_021ea7a8(BtlvCore *core, u8 pos) {
    func_ov093_021edb00(core->scu, pos);
}

BOOL func_ov093_021ea7b8(BtlvCore *core, u8 pos) {
    return func_ov093_021edb20(core->scu, pos);
}

void func_ov093_021ea7c8(BtlvCore *core, u32 arg1) {
    func_ov094_021f7a28(arg1, 0x262);
}

void func_ov093_021ea7d8(BtlvCore *core, u32 arg1) {
    func_ov094_021f7a28(arg1, 0x261);
}

BOOL func_ov093_021ea7e8(BtlvCore *core, u32 arg1) {
    if (!func_ov094_021f7eb4()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021ea7fc(BtlvCore *core) {
    func_ov094_021f8a40(3);
    func_ov093_021ee200(core->scu);
}

BOOL func_ov093_021ea814(BtlvCore *core) {
    return func_ov093_021ee21c(core->scu);
}

void func_ov093_021ea824(BtlvCore *core) {
    func_ov094_021f8a78();
    func_ov093_021ee224(core->scu);
}

void func_ov093_021ea838(BtlvCore *core, u8 pos) {
    func_ov093_021ed56c(core->scu, func_ov093_021b8e70(core->mainModule, pos));
}

BOOL func_ov093_021ea850(BtlvCore *core, u8 pos) {
    return func_ov093_021ed57c(core->scu, func_ov093_021b8e70(core->mainModule, pos));
}

void func_ov093_021ea868(BtlvCore *core, u8 pos, u32 arg2) {
    func_ov093_021ed590(core->scu, pos, arg2);
}

BOOL func_ov093_021ea878(BtlvCore *core) {
    return func_ov093_021ed5d8(core->scu);
}

void func_ov093_021ea888(BtlvCore *core, u32 arg1) {
    func_ov093_021ed6bc(core->scu, arg1, func_ov093_021cd9e0(core->client));
}

BOOL func_ov093_021ea8a4(BtlvCore *core) {
    return func_ov093_021ed714(core->scu);
}

void func_ov093_021ea8b4(BtlvCore *core, u32 arg1, u32 arg2) {
    func_ov093_021ed660(core->scu, arg1, arg2, func_ov093_021cd9e0(core->client));
}

BOOL func_ov093_021ea8d4(BtlvCore *core) {
    return func_ov093_021ed714(core->scu);
}

void func_ov093_021ea8e4(BtlvCore *core) {
    func_ov093_021ed8ac(core->scu);
}

BOOL func_ov093_021ea8f4(BtlvCore *core) {
    return func_ov093_021ed8d8(core->scu);
}

void func_ov093_021ea904(BtlvCore *core, u8 clientId, u8 arg2, u8 arg3, u8 slot1, u8 slot2) {
    BtlvSwapWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvSwapWork));
    work->clientId = clientId;
    work->unk01 = arg2;
    work->unk02 = arg3;
    work->slot1 = slot1;
    work->slot2 = slot2;
    work->pos1 = func_ov093_021b8bec(core->mainModule, clientId, slot1);
    work->pos2 = func_ov093_021b8bec(core->mainModule, clientId, slot2);
    BtlvCore_SetSubProc(core, func_ov093_021ea990);
}

BOOL func_ov093_021ea950(BtlvCore *core) {
    return BtlvCore_RunSubProc(core);
}

BOOL func_ov093_021ea990(s32 *seq, void *arg) {
    BtlvCore *core = arg;
    BtlvSwapWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvSwapWork));
    switch (*seq) {
    case 0:
        func_ov094_021f84b4(work->unk01, work->unk02);
        func_ov093_021eaf00(core, 0x57b);
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ed4e4(core->scu, work->pos1, work->pos2);
            (*seq)++;
        }
        break;
    case 2:
        if (func_ov093_021ed510(core->scu, work->pos1, work->pos2)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021eaa00(BtlvCore *core, u8 arg1) {
    void *work;

    work = func_ov093_021e9100(core, 0x3c);
    func_ov093_021b9af8(core->pokeCon, core->clientId);
    data_ov095_0689b515(core->scd, work);
}

BOOL func_ov093_021eaa34(BtlvCore *core, u8 *out) {
    if (data_ov095_0689b525(core->scd, out)) {
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021eaa5c(BtlvCore *core) {
    data_ov095_0689b559(core->scd);
}

void func_ov093_021eaa6c(BtlvCore *core, u8 clientId, u8 arg2) {
    BtlvRotateWork *work;

    if (arg2 != 1) {
        work = func_ov093_021e9100(core, sizeof(BtlvRotateWork));
        work->clientId = clientId;
        work->unk01 = arg2;
        work->pos[0] = func_ov093_021b8bec(core->mainModule, clientId, 0);
        work->pos[1] = func_ov093_021b8bec(core->mainModule, clientId, 1);
        work->pos[2] = func_ov093_021b8bec(core->mainModule, clientId, 2);
        work->viewPos[0] = func_ov093_021b8e70(core->mainModule, work->pos[0]);
        work->viewPos[1] = func_ov093_021b8e70(core->mainModule, work->pos[1]);
        work->viewPos[2] = func_ov093_021b8e70(core->mainModule, work->pos[2]);
        BtlvCore_SetSubProc(core, func_ov093_021eab18);
    }
}

BOOL func_ov093_021eaad8(BtlvCore *core) {
    return BtlvCore_RunSubProc(core);
}

BOOL func_ov093_021eab18(s32 *seq, void *arg) {
    BtlvCore *core = arg;
    BtlvRotateWork *work;
    BOOL done;

    work = func_ov093_021e9100(core, sizeof(BtlvRotateWork));
    switch (*seq) {
    case 0:
        func_ov094_021f8414(work->unk01, work->viewPos[0] & 1, !func_ov093_021cd9e0(core->client));
        (*seq)++;
        break;
    case 1:
        if (!func_ov094_021f7eb4()) {
            func_ov093_021ed53c(core->scu, work->pos[0]);
            func_ov093_021ed53c(core->scu, work->pos[1]);
            func_ov093_021ed53c(core->scu, work->pos[2]);
            (*seq)++;
        }
        break;
    case 2:
        done = TRUE;
        if (!func_ov093_021ed554(core->scu, work->pos[0])) {
            done = FALSE;
        }
        if (!func_ov093_021ed554(core->scu, work->pos[1])) {
            done = FALSE;
        }
        if (!func_ov093_021ed554(core->scu, work->pos[2])) {
            done = FALSE;
        }
        if (done) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void func_ov093_021eabc4(BtlvCore *core, const BtlvStringParam *param1, const BtlvStringParam *param2, u32 arg3) {
    BtlvTwoStringWork *work;

    work = func_ov093_021e9100(core, sizeof(BtlvTwoStringWork));
    work->first = GFL_StrBufCreate(0x20, HEAPID_TAIL(core->heapId));
    work->second = GFL_StrBufCreate(0x20, HEAPID_TAIL(core->heapId));
    work->unk08 = arg3;
    func_ov093_021ea55c(param1, work->first);
    func_ov093_021ea55c(param2, work->second);
    data_ov095_0689b5a5(core->scd, work);
    GFL_HeapFree(work->first);
    GFL_HeapFree(work->second);
}

BOOL func_ov093_021eac54(BtlvCore *core, u32 *answer) {
    return data_ov095_0689b5cd(core->scd, answer);
}

void func_ov093_021eac64(BtlvCore *core, u8 partyIndex, u16 move) {
    func_ov093_021e9984(core, &core->ov287, 4, partyIndex, move);
    core->subSeq = 0;
}

BOOL func_ov093_021eac80(BtlvCore *core, u8 *slot) {
    switch (core->subSeq) {
    case 0:
        data_ov095_06899ca5(core->scd, 0);
        GFL_OvlLoad(OVERLAY_ID(196));
        core->subSeq++;
        break;
    case 1:
        if (data_ov095_06899d05(core->scd)) {
            data_ov095_06899d2d(core->scd);
            GFL_OvlLoad(OVERLAY_ID(198));
            func_ov198_0220f3e8(&core->ov287);
            core->subSeq++;
        }
        break;
    case 2:
        if (core->ov287.done) {
            GFL_OvlUnload(OVERLAY_ID(198));
            data_ov095_06899cdd(core->scd);
            data_ov095_06899d31(core->scd);
            core->subSeq++;
        }
        break;
    case 3:
        if (data_ov095_06899d05(core->scd)) {
            core->subSeq++;
        }
        break;
    default:
        *slot = core->ov287.slot;
        GFL_OvlUnload(OVERLAY_ID(196));
        core->subSeq = 0;
        return TRUE;
    }
    return FALSE;
}

void func_ov093_021eada4(BtlvCore *core, BattleMon *mon, const BattleMonLevelUp *levelUp) {
    func_ov093_021ee264(core->scu, mon, levelUp);
}

BOOL func_ov093_021eadb4(BtlvCore *core) {
    return func_ov093_021ee2a0(core->scu);
}

void func_ov093_021eadc4(BtlvCore *core) {
    func_ov093_021ee2f8(core->scu);
}

BOOL func_ov093_021eadd4(BtlvCore *core) {
    return func_ov093_021ee318(core->scu);
}

void func_ov093_021eade4(BtlvCore *core) {
    func_ov093_021ee370(core->scu);
}

BOOL func_ov093_021eadf4(BtlvCore *core) {
    return func_ov093_021ee390(core->scu);
}

void func_ov093_021eae04(BtlvCore *core) {
    func_ov093_021ee228(core->scu);
}

BOOL func_ov093_021eae14(BtlvCore *core) {
    return func_ov093_021ee25c(core->scu);
}

void func_ov093_021eae24(BtlvCore *core) {
    func_ov093_021ee23c(core->scu);
}

BOOL func_ov093_021eae34(BtlvCore *core) {
    return func_ov093_021ee25c(core->scu);
}

void BattleClientCmd_ForceQuitInputNotify(BtlvCore *core) {
    data_ov095_0689b671(core->scd);
}

BOOL BattleClientCmd_ForceQuitInputWait(BtlvCore *core) {
    return data_ov095_0689b69d(core->scd);
}

u32 func_ov093_021eae64(BtlvCore *core) {
    return data_ov095_0689b791(core->scd);
}

void func_ov093_021eae74(BtlvCore *core, u16 arg1, u16 arg2) {
    core->unk160.unk00 = arg1;
    core->unk160.unk04 = arg2;
    core->unk160.unk0C = 0;
    data_ov095_0689b781(core->scd, &core->unk160);
}

void func_ov093_021eaea8(BtlvCore *core, u16 arg1) {
    core->unk160.unk00 = arg1;
    core->unk160.unk04 = arg1;
    core->unk160.unk0C = 3;
    data_ov095_0689b781(core->scd, &core->unk160);
}

void func_ov093_021eaecc(BtlvCore *core, u32 arg1, u32 arg2) {
    core->unk160.unk00 = arg1;
    core->unk160.unk04 = arg1;
    core->unk160.unk0C = arg2;
    data_ov095_0689b781(core->scd, &core->unk160);
}

void func_ov093_021eaf00(BtlvCore *core, u32 se) {
    if (!func_ov093_021cd9e0(core->client)) {
        GFL_SndSEPlay(se);
    }
}

// Function names from swan.
void Btlv_StringParam_Setup(BtlvStringParam *param, u32 type, u16 message) {
    u32 i;

    for (i = 0; i < 8; i++) {
        param->args[i] = 0;
    }
    param->count = 0;
    param->message = message;
    param->type = type;
    param->mode = 0x50;
}

void Btlv_StringParam_AddArg(BtlvStringParam *param, u32 arg) {
    u8 count;

    count = param->count;
    if (count < 8) {
        param->count = count + 1;
        param->args[count] = arg;
    }
}

void func_ov093_021eaf78(BtlvStringParam *param, u8 mode) {
    param->mode = mode;
}

