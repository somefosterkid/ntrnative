// Black's code at ov174 0x021b6100-0x021b6994. No name is known: the module and address.

#include "types.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/random.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "struct_decls.h"
#include "system/app_keycursor.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "gfl/arc_util.h"
#include "local_tvt_chara.h"
#include "ov174_021b6100.h"
#include "gfl/tcb.h"
#include "gfl/arc.h"
#include "system/app_printsys_common.h"
#include "system/bmp_winframe.h"
#include "gfl/bmp.h"
#include "nitro/rtc.h"
#include "ov174_021b6100.h"
#include "system/game_data.h"
#include "system/rtc.h"
#include "gfl/rtc_cache.h"
#include "gfl/net.h"
#include "system/wipe.h"
#include "nitro/mi.h"
#include "save/player_info.h"

void func_ov174_021b6824(Ov174Work *work);
void func_ov174_021b6508(Ov174Work *work);
void func_ov174_021b63dc(Ov174Work *work);
void func_ov174_021b6594(Ov174Work *work);
void func_ov174_021b655c(Ov174Work *work);
void func_ov174_021b6760(Ov174Work *work);
void func_ov174_021b63b0(TCB *tcb, void *data);
void func_ov174_021b6538(const BGSetup *param_1, u8 param_2, u8 param_3);
void func_ov174_021b687c(Ov174Work *work);


void func_ov174_021b6100(Ov174Work *work) {
    u8 i;
    RTCTime time;
    u32 season;
    u32 hour1;
    u32 hour2;
    u32 hour3;

    func_ov174_021b63dc(work);
    work->arc = GFL_ArcSysCreateFileHandle(0x83, work->heapId);
    func_ov174_021b655c(work);
    func_ov174_021b6594(work);
    func_ov174_021b6760(work);
    season = GameData_GetSeason(work->unk_30->gameData);
    hour1 = GetLightChangeHoursForSeasons(season, 0);
    hour2 = GetLightChangeHoursForSeasons(season, 2);
    hour3 = GetLightChangeHoursForSeasons(season, 3);
    RTC_GetCachedTime(&time);
    if (time.hour >= hour1 && time.hour < hour2) {
        work->unk_10 = 0;
    } else if (time.hour >= hour2 && time.hour < hour3) {
        work->unk_10 = 1;
    } else {
        work->unk_10 = 2;
    }
    for (i = 0; i < work->unk_08; i++) {
        work->unk_1c[i] = func_ov174_021b6c70(work, i);
    }
    work->unk_04 = GFL_VBlankTCBAdd(func_ov174_021b63b0, work, 8);
    work->unk_0d = 1;
    work->unk_0e = 0;
    work->unk_34 = 1;
    work->unk_89 = 0;
    work->unk_14 = 0;
    work->unk_1c[4] = NULL;
    work->unk_0c = 0;
    func_0203d34c(0, work->heapId);
}

void func_ov174_021b61c4(Ov174Work *work) {
    u8 i;

    GFL_TCBRemove(work->unk_04);
    func_ov174_021b6824(work);
    i = 0;
    if (work->unk_08 > 0) {
        do {
            if (work->unk_1c[i] != NULL) {
                func_ov174_021b6d00(work, work->unk_1c[i]);
            }
            i++;
        } while ((s32)i < work->unk_08);
    }
    func_0204b3b4(work->unk_54);
    func_0204ada4(work->unk_44);
    func_0204a8d4(work->unk_48);
    func_0204afd8(work->unk_4c);
    func_0204b1cc(work->unk_50);
    GFL_ArcToolFree(work->arc);
    func_ov174_021b6508(work);
    GFL_HeapFree(work->file);
}

u32 func_ov174_021b622c(Ov174Work *work) {
    u8 idx;
    u8 i;
    GXBGCharBase bg;
    GXBGCharBase cnt;

    switch (work->unk_0c) {
    case 0:
        GFL_WipeSet(0, 1, 1, 0, 6, 1, work->heapId);
        work->unk_0c = 1;
        break;
    case 1:
        if (GFL_WipeIsFinished() == 1) {
            work->unk_0c = 4;
        }
        break;
    case 2:
        GFL_WipeSet(0, 0, 0, 0, 6, 1, work->heapId);
        work->unk_0c = 3;
        break;
    case 3:
        if (GFL_WipeIsFinished() == 1) {
            return 1;
        }
        break;
    case 4:
        func_ov174_021b6a90(work);
        break;
    }
    if (work->unk_0e < 0x14) {
        work->unk_0e++;
    }
    if (work->unk_1c[4] == NULL && work->unk_14 == 4) {
        if (work->unk_0d == 0) {
            bg = (GXBGCharBase)GX_BG_CHARBASE_0x10000;
            cnt = (GXBGCharBase)GX_BG_CHARBASE_0x28000;
        } else {
            bg = (GXBGCharBase)GX_BG_CHARBASE_0x1c000;
            cnt = (GXBGCharBase)GX_BG_CHARBASE_0x34000;
        }
        G2_SetBG2Control256x16Pltt(GX_BG_SCRSIZE_256x16PLTT_256x256, GX_BG_AREAOVER_REPEAT, (GXBGScrBase)GX_BG_SCRBASE_0x6800, bg);
        G2_SetBG3Control256x16Pltt(GX_BG_SCRSIZE_256x16PLTT_256x256, GX_BG_AREAOVER_REPEAT, (GXBGScrBase)GX_BG_SCRBASE_0x7000, cnt);
        work->unk_0d = work->unk_0d == 0;
        work->unk_14 = 0;
        work->unk_0e = 0;
    }
    if (work->unk_1c[4] != NULL && func_ov174_021b6f2c(work, work->unk_1c[4]) == 1) {
        work->unk_1c[4] = NULL;
    }
    if (work->unk_1c[4] == NULL) {
        switch (work->unk_14) {
        case 0:
        case 1:
        case 2:
        case 3:
            idx = work->unk_14;
            if (work->unk_1c[idx] != NULL) {
                func_ov174_021b6f24(work, work->unk_1c[idx], work->unk_0d);
                work->unk_1c[4] = work->unk_1c[idx];
                if (work->unk_08 == 2 && work->unk_14 == 1) {
                    work->unk_14 = 4;
                } else {
                    work->unk_14++;
                }
            } else {
                work->unk_14++;
            }
            break;
        }
    }
    i = 0;
    if (work->unk_08 > 0) {
        do {
            if (work->unk_1c[i] != NULL) {
                func_ov174_021b6d3c(work, work->unk_1c[i]);
            }
            i++;
        } while ((s32)i < work->unk_08);
    }
    func_ov174_021b687c(work);
    func_0204a600();
    return 0;
}


void func_ov174_021b63b0(TCB *tcb, void *data) {
    u32 value;

    func_0204a648();
    value = GFL_RandomLCAlt(0x100);
    GFL_BGSysMoveBG(1, 0, value);
    value = GFL_RandomLCAlt(0x100);
    GFL_BGSysMoveBG(1, 3, value);
}

void func_ov174_021b63dc(Ov174Work *work) {
    ClActSysSetup setup;
    ClActUnit *unit;

    GFL_BGSysDisableAllA();
    GFL_BGSysDisableAllB();
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    func_0202223c(0, 0);
    func_0202223c(1, 0);
    func_02022224(0);
    func_02022224(1);
    G2_BlendNone();
    G2S_BlendNone();
    GX_SetDispSelect(GX_DISP_SELECT_MAIN_SUB);
    GFL_BGSysSetVRAMBanks(&data_ov174_021b7450);
    GFL_BGSysCreate(work->heapId);
    BmpWin_InitAllocator(work->heapId);
    GFL_BGSysSetLCDConfig(&data_ov174_021b7388);
    func_ov174_021b6538(&data_ov174_021b73d0, 0, 0);
    func_ov174_021b6538(&data_ov174_021b73b0, 1, 0);
    func_ov174_021b6538(&data_ov174_021b7410, 2, 2);
    func_ov174_021b6538(&data_ov174_021b7430, 3, 2);
    func_ov174_021b6538(&data_ov174_021b73f0, 7, 0);
    MIi_CpuClear32(0, (void *)HW_BG_VRAM, 0x40000);
    setup = data_020a1448;
    ClActSys_Create(&setup, &data_ov174_021b7450, work->heapId);
    GFL_BGSysSetBGEnabledA(0x10, 1);
    GFL_BGSysSetBGEnabledB(0x10, 1);
    unit = func_0204b100(0x10, 0, work->heapId);
    work->unk_50 = unit;
    func_0204b270(unit);
    G2_SetBlendAlpha(1, 0x1e, 0x10, 4);
}


const LocalTvtNamePos data_ov174_021b7380[4] = {
    { 0, 0 },
    { 16, 0 },
    { 0, 12 },
    { 16, 12 },
};

const BGSysLCDConfig data_ov174_021b7388 = { 1, 5, 0, 0 };

const Ov174StateFuncs data_ov174_021b7398[] = {
    { NULL, NULL },
    { func_ov174_021b6af8, func_ov174_021b6b5c },
    { func_ov174_021b6b8c, func_ov174_021b6b94 },
};

const BGSetup data_ov174_021b73b0 = { 0, 0, 2048, 0, 1, 0, 12, 0, 0x6000, 0, 3, 0, 0 };

const BGSetup data_ov174_021b73d0 = { 0, 0, 2048, 0, 1, 0, 15, 2, 0x8000, 0, 0, 0, 0 };

const BGSetup data_ov174_021b73f0 = { 0, 0, 2048, 0, 1, 0, 14, 0, 0x6000, 1, 3, 0, 0 };

const BGSetup data_ov174_021b7410 = { 0, 0, 2048, 0, 1, 1, 13, 4, 0xc000, 1, 1, 1, 0 };

const BGSetup data_ov174_021b7430 = { 0, 0, 2048, 0, 1, 1, 14, 10, 0xc000, 1, 2, 0, 0 };

const BGSysVRAMConfig data_ov174_021b7450 = {
#include "asset/ov174_021b6100_data_ov174_021b7450.inc"
};

void func_ov174_021b6508(Ov174Work *param_1) {
    func_0204a5ac();
    GFL_BGSysReleaseBG(7);
    GFL_BGSysReleaseBG(3);
    GFL_BGSysReleaseBG(2);
    GFL_BGSysReleaseBG(1);
    GFL_BGSysReleaseBG(0);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

void func_ov174_021b6538(const BGSetup *param_1, u8 param_2, u8 param_3) {
    GFL_BGSysCreateBG(param_2, param_1, param_3);
    GFL_BGSysSetBGEnabled(param_2, 1);
    GFL_BGSysClearBG(param_2);
    GFL_BGSysLoadScr(param_2);
}

void func_ov174_021b655c(Ov174Work *work) {
    void *file;

    file = GFL_ArcToolReadHeapNewLZ(work->arc, work->unk_30->unk_00 + 0x3e, FALSE, work->heapId);
    work->file = file;
    work->header = file;
    work->unk_40 = (Ov174ScriptCmd *)((u8 *)file + sizeof(LocalTvtScriptHeader));
    if (work->header->persons[2] == 0xff && work->header->persons[3] == 0xff) {
        work->unk_08 = 2;
        return;
    }
    work->unk_08 = 4;
}

void func_ov174_021b6594(Ov174Work *work) {
    PlayerInfo *info;
    ArcTool *arc;
    ClActorSetup setup;
    u32 fileId;

    if (work->unk_08 == 2) {
        fileId = 0x25;
    } else {
        fileId = 0x26;
    }
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, fileId, 2, 0, 0, 0, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, fileId, 3, 0, 0, 0, work->heapId);
    info = GetGameDataPlayerInfo(work->unk_30->gameData);
    arc = GFL_ArcSysCreateFileHandle(0xac, work->heapId);
    work->unk_44 = func_0204acec(arc, 3, 0, 0, work->heapId);
    work->unk_48 = func_0204a6c8(arc, 0x18, 0, 0, work->heapId);
    work->unk_4c = func_0204af28(arc, 0x14, 0x16, work->heapId);
    if (getTrainerGender(info) == 0) {
        GFL_G2DIOLoadArcNCLRDefault(arc, 7, 4, 0, 0x60, work->heapId);
    } else {
        GFL_G2DIOLoadArcNCLRDefault(arc, 6, 4, 0, 0x60, work->heapId);
    }
    GFL_BGSysLoadArcNCGRStatic(arc, 0xb, 7, 0, 0, 0, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0xe, 7, 0, 0, 0, work->heapId);
    GFL_BGSysLoadScr(7);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 0, 0x160, 0x20, work->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 8, 1, 0, 0, 0, work->heapId);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 0xd, 1, 0, 0, 0, work->heapId);
    GFL_BGSysSetScrPaletteNo(1, 0, 0, 0x20, 0x20, 0xb);
    GFL_BGSysLoadScr(1);
    GFL_ArcToolFree(arc);
    setup.priority = 0xa;
    setup.bgPriority = 1;
    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0;
    work->unk_54 = func_0204b294(work->unk_50, work->unk_48, work->unk_44, work->unk_4c, &setup, 0, work->heapId);
    func_0204b3dc(work->unk_54, 0);
    func_0204b92c(work->unk_54, 1);
}


void func_ov174_021b6704(Ov174Work *work, u32 idx) {
    ClActorPos pos;
    u32 val;

    val = func_ov174_021b7360(work->unk_1c[idx]);
    pos.x = (val >> 1) + (data_ov174_021b7380[idx].x * 8 + 0x50);
    pos.y = data_ov174_021b7380[idx].y * 8 + 0x10;
    func_0204b404(work->unk_54, &pos, 0);
    func_0204b3dc(work->unk_54, 1);
    func_0204b9a0(work->unk_54);
}



void func_ov174_021b6754(Ov174Work *work) {
    func_0204b3dc(work->unk_54, 0);
}

void func_ov174_021b6760(Ov174Work *work) {
    work->unk_60 = GFL_FontCreate(0x17, 0, 0, 0, work->heapId);
    work->unk_68 = GFL_MsgSysLoadData(0, 2, 0xc6, work->heapId);
    work->unk_6c = GFL_MsgSysLoadData(0, 2, work->unk_30->unk_00 + 0xbf, work->heapId);
    LoadSysMsgBox(0, 1, 0xd, 0, work->heapId);
    GFL_G2DIOLoadNCLR(5, GetSysMsgBoxPaletteDatID(0), 0, 0x20, 0x1a0, 0x20, work->heapId);
    GFL_BGSysLoadNCLRDefault(0x17, 7, 0, 0x180, 0x20, work->heapId);
    func_0201dd68();
    work->unk_58 = GFL_TCBExMgrCreate(work->heapId, work->heapId, 3, 0x100);
    work->unk_64 = NULL;
    work->unk_5c = NULL;
    work->unk_70 = NULL;
    work->unk_74 = NULL;
    work->unk_78 = func_0201c440(work->heapId);
    work->unk_7c = func_02028590(0xf, 1, 1, work->heapId);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0xf);
}


void func_ov174_021b6824(Ov174Work *param_1) {
    func_02028604(param_1->unk_7c);
    func_0201c4c0(param_1->unk_78);
    if (param_1->unk_64 != (PrintStream *)0x0) {
        func_0201ce48(param_1->unk_64);
    }
    if (param_1->unk_70 != (StrBuf *)0x0) {
        GFL_StrBufFree(param_1->unk_70);
    }
    if (param_1->unk_74 != (WordSet *)0x0) {
        GFL_WordSetSystemFree(param_1->unk_74);
    }
    GFL_MsgDataFree(param_1->unk_6c);
    GFL_MsgDataFree(param_1->unk_68);
    if (param_1->unk_5c != (BmpWin *)0x0) {
        BmpWin_Free(param_1->unk_5c);
    }
    GFL_FontFree(param_1->unk_60);
    GFL_TCBExMgrFree(param_1->unk_58);
    func_0201dd68();
}

void func_ov174_021b687c(Ov174Work *work) {
    u32 ret;

    GFL_TCBExMgrUpdate(work->unk_58);
    if (work->unk_64 != NULL) {
        func_020286c4(work->unk_7c, work->unk_64, work->unk_5c);
        ret = func_0201ce30(work->unk_64);
        if (ret != 0) {
            func_ov174_021b7364(work->unk_1c[work->unk_88], 0);
        } else {
            func_ov174_021b7364(work->unk_1c[work->unk_88], 1);
        }
        if (func_02028484(&work->unk_80, work->unk_64) == 1) {
            func_0201ce48(work->unk_64);
            work->unk_64 = NULL;
            GFL_StrBufFree(work->unk_70);
            work->unk_70 = NULL;
        }
    }
    func_0201c4e4(work->unk_78);
}


u32 func_ov174_021b68e8(Ov174Work *work) {
    if (work->unk_64 == NULL) {
        return 1;
    }
    return 0;
}

void func_ov174_021b68f8(Ov174Work *work, u32 param_2) {
    GFLBitmap *bitmap;
    BmpWin *win;

    if (work->unk_5c != NULL) {
        func_0201f7a8(work->unk_5c, 1);
        win = work->unk_5c;
        func_020456f8(win);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(win));
        BmpWin_Free(work->unk_5c);
        work->unk_5c = NULL;
    }
    if (param_2 == 0) {
        work->unk_5c = BmpWin_CreateDynamic(0, 1, 1, 0x1e, 4, 0xc, 1);
    } else if (param_2 == 1) {
        work->unk_5c = BmpWin_CreateDynamic(0, 1, 0x13, 0x1e, 4, 0xc, 1);
    }
    BmpWin_DrawFrame(work->unk_5c, 1, 1, 0xd);
    bitmap = BmpWin_GetBitmap(work->unk_5c);
    GFL_BitmapFill(bitmap, 0xf);
    BmpWin_FlushChar(work->unk_5c);
    BmpWin_FlushMap(work->unk_5c);
    GFL_BGSysQueueScrLoad(0);
    func_02028470(&work->unk_80, 6);
}

