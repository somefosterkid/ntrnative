// Black's code at ov071 0x021b7234-0x021b7e60. No name is known: the module and address.

#include "types.h"
#include "app/monolith/monolith_main.h"
#include "app/monolith/monolith_tool.h"
#include "field/beacon_view.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "struct_decls.h"
#include "system/app_printsys_common.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/palanm.h"
#include "system/printsys.h"
#include "gfl/proc.h"
#include "system/wordset.h"
#include "system/game_system.h"
#include "field/intrude_work.h"
#include "gfl/touchpanel.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "constants/sound.h"

static const BGSetup data_ov071_021ba8dc = { 0, 0, 2048, 0, 1, 0, 4, 3, 0x8000, 0, 0, 0, 0 };
static const BGSetup data_ov071_021ba8fc = { 0, 0, 2048, 0, 1, 0, 2, 1, 0x8000, 0, 3, 0, 0 };
void func_ov071_021b7234(MonolithScreenParam *screen, s32 row, int bg);
ClActor *func_ov071_021b6f54(MonolithWork *wk, PassPowerLevel *levels, u32 index, const s32 *pos, u32 res);
static const u32 data_ov071_021ba8a0[1] = { 88 };
ClActor *func_ov071_021b7014(MonolithWork *wk, const s32 *pos, u32 index);
static const u32 data_ov071_021ba8a4[11] = { 24, 168, 24, 64, 56, 192, 56, 88, 88, 168, 88 };
void func_ov071_021b6fe0(ClActor *actor, u8 *table, long index);
static const BGSetup data_ov071_021ba928 = { 0, 0, 2048, 0, 1, 0, 2, 1, 0x8000, 0, 3, 0, 0 };
// Where the six pass power levels' actors are, an (x, y) a mission (MonolithState's order, data_ov071_021baa4c)
static const s32 data_ov071_021ba948[12] = {
#include "asset/ov071_021b7234_data_ov071_021ba948.inc"
};

// The bottom screen's work (0x84 bytes): its message window, the six pass power level actors, then the return button,
// then the actor of the arrows
typedef struct {
    BmpWin *unk_00;
    StrBuf *unk_04;
    PrintStream *unk_08;
    ClActor *levelActors[6];
    MonolithReturnButton returnButton;
    MonolithTextActor texts[2];
    IntrudeMission mission;
    IntrudePlayer player;
    u8 unk_78;
    u8 unk_79[3];
    ClActor *unk_7C;
    u8 unk_80[2];
    u8 unk_82;
    u8 unk_83;
} MonolithBottomWork;

// The top screen's work while the bottom one is up (0x24 bytes): the player's name, then the six pass power level
// actors
typedef struct {
    MonolithOamText text;
    ClActor *levelActors[6];
} MonolithPowerLevelsWork;

// The levels of the pass power bars, cut short
typedef struct {
    u8 unk_00[0xc];
    u16 unk_0C;
    u16 unk_0E;
} MonolithLevels;

void func_ov071_021b6fd8(ClActor *actor);

const u8 data_ov071_021ba85c[6] = { 0x00, 0x01, 0x03, 0x05, 0x04, 0x02 };
const u8 data_ov071_021ba862[26] = {
#include "asset/ov071_021b7234_data_ov071_021ba862.inc"
};
const TouchRect data_ov071_021ba87c[9] = {
#include "asset/ov071_021b7234_data_ov071_021ba87c.inc"
};
void func_ov071_021b7e24(MonolithPowerLevelsWork *top);
void func_ov071_021b7d98(void);
void func_ov071_021b7e3c(MonolithPowerLevelsWork *top, MonolithScreenParam *screen);
void func_ov071_021b7068(ClActor *actor);
void func_ov071_021b7be4(MonolithBottomWork *bottom);
void func_ov071_021b7b9c(MonolithBottomWork *bottom);
void func_ov071_021b7aac(MonolithBottomWork *bottom);
void func_ov071_021b7994(MonolithBottomWork *bottom);
void func_ov071_021b78d4(void);
void func_ov071_021b7d6c(void);
void func_ov071_021b7dac(MonolithScreenParam *screen, MonolithWork *work);
void func_ov071_021b7de0(MonolithPowerLevelsWork *top, MonolithScreenParam *screen);
void func_ov071_021b7880(void);
void func_ov071_021b78f4(MonolithScreenParam *screen, MonolithWork *work);
void func_ov071_021b7924(MonolithBottomWork *bottom, MonolithWork *work);
void func_ov071_021b7a68(MonolithBottomWork *bottom, MonolithScreenParam *screen);
void func_ov071_021b7b10(MonolithScreenParam *screen, MonolithBottomWork *bottom);
void func_ov071_021b7bd8(MonolithScreenParam *param, MonolithBottomWork *bottom);
void func_ov071_021b7ac4(MonolithBottomWork *bottom, MonolithScreenParam *screen);
void func_ov071_021b7ae8(MonolithBottomWork *bottom, s32 index);
void func_ov071_021b7bb4(MonolithScreenParam *screen, MonolithBottomWork *bottom);
void func_ov071_021b7bf0(MonolithBottomWork *bottom);
void func_ov071_021b7bfc(MonolithScreenParam *screen, MonolithBottomWork *bottom, s32 flag);
void func_ov071_021b79a0(MonolithBottomWork *bottom, MonolithWork *work, u32 msgId);
BOOL func_ov071_021b7a08(MonolithWork *work, MonolithBottomWork *bottom);
void func_ov071_021b7a14(MonolithBottomWork *bottom);



void func_ov071_021b7234(MonolithScreenParam *screen, s32 row, int bg) {
    MonolithLevels *levels;
    s32 white;
    s32 black;
    s32 whiteWidth;
    s32 blackWidth;
    u16 *area;
    int i;
    int y;

    levels = func_ov071_021b6f40(screen);
    y = 0x14;
    if (row != 0) {
        y = 0xe;
    }
    white = levels->unk_0C;
    black = levels->unk_0E;
    if (white == black) {
        whiteWidth = 0x78;
        blackWidth = 0x78;
    } else {
        whiteWidth = 0xf0 * white / (white + black);
        if (black > 0) {
            blackWidth = 0xf0 - whiteWidth;
        } else {
            blackWidth = 0;
            whiteWidth = 0xf0;
        }
    }
    if (white > 0 && whiteWidth == 0) {
        blackWidth--;
    } else if (black > 0 && blackWidth == 0) {
        blackWidth++;
    }
    area = GFL_HeapAllocate(0x61, 0x3c, TRUE, "monolith_tool.c", 0x474);
    for (i = 0; blackWidth > 0; i++) {
        if (blackWidth >= 8) {
            area[i] = 1;
            blackWidth -= 8;
        } else {
            area[i] = 9 - blackWidth;
            blackWidth = 0;
        }
    }
    for (; i < 0x1e; i++) {
        area[i] = 9;
    }
    for (i = 0; i < 0x1e; i++) {
        area[i] = area[i] | 0x1000;
    }
    GFL_BGSysLoadScrAreaAll(bg, area, 1, y, 0x1e, 1);
    GFL_HeapFree(area);
}

BOOL func_ov071_021b730c(GameProc *proc, u32 *state, MonolithScreenParam *screen) {
    MonolithBottomWork *bottom;

    switch (*state) {
    case 0:
        bottom = GFL_ProcInitSubsystem(proc, sizeof(MonolithBottomWork), 0x61);
        MI_CpuFill8(bottom, 0, sizeof(MonolithBottomWork));
        bottom->unk_82 = 0;
        func_ov071_021b7880();
        func_ov071_021b78f4(screen, screen->work);
        func_ov071_021b7924(bottom, screen->work);
        func_ov071_021b7a68(bottom, screen);
        func_ov071_021b7b10(screen, bottom);
        func_ov071_021b7bd8(screen, bottom);
        bottom->unk_7C = func_ov071_021b7014(screen->work, &data_ov071_021ba8a0[screen->state->mission * 2], 1);
        *state = *state + 1;
        break;
    case 1:
        GFL_BGSysSetBGEnabled(6, 1);
        GFL_BGSysSetBGEnabledB(0x10, 1);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov071_021b739c(GameProc *proc, u32 *state, void *param, void *work) {
    MonolithScreenParam *screen = param;
    MonolithBottomWork *bottom = work;
    IntrudeComm *intrude;
    int keys;
    int touch;
    int i;
    u8 sel;
    u8 mission;
    u32 kind;
    u8 result;

    intrude = func_ov010_02158344(GSYS_GetGameCommSystem(screen->param->gsys));
    func_ov071_021b7ac4(bottom, screen);
    func_ov071_021b7ae8(bottom, screen->state->mission);
    func_ov071_021b7bb4(screen, bottom);
    func_ov071_021b7bf0(bottom);
    if (screen->exiting == 1 && *state == 3) {
        return TRUE;
    }
    switch (*state) {
    case 0:
        if (intrude != NULL && func_ov011_02175b14(&intrude->unk_1AC) == TRUE) {
            func_ov071_021b7bfc(screen, bottom, 1);
        }
        *state = 1;
        break;
    case 1:
        if (func_ov010_02158590(GSYS_GetGameData(screen->param->gsys)) == FALSE) {
            func_ov071_021b79a0(bottom, screen->work, 0x16);
        } else {
            func_ov071_021b79a0(bottom, screen->work, 0x12);
        }
        (*state)++;
        break;
    case 2:
        if (func_ov071_021b7a08(screen->work, bottom) == TRUE) {
            if (func_02035c60() != FALSE || (GCTX_HIDGetPressedKeys() & 3) != 0) {
                func_ov071_021b7a14(bottom);
                GFL_SndSEPlay(0x547);
                if (func_02035c60() != FALSE) {
                    bottom->unk_83 = 1;
                    func_ov071_021b6b98(screen, bottom->texts, 1, 0xff, 3);
                } else {
                    bottom->unk_83 = 0;
                    func_ov071_021b6b98(screen, bottom->texts, 1, 0, 3);
                }
                if (bottom->unk_82 == 0) {
                    *state = 3;
                } else {
                    *state = 4;
                }
            }
        }
        break;
    case 3:
        keys = GCTX_HIDGetPressedKeys();
        touch = func_02035c08(data_ov071_021ba87c);
        if (intrude != NULL && func_ov011_02175b14(&intrude->unk_1AC) == TRUE) {
            func_ov071_021b7bfc(screen, bottom, 1);
            bottom->unk_80[0] = 1;
            *state = 4;
            break;
        }
        if (keys > 0 && bottom->unk_83 == 1) {
            bottom->unk_83 = 0;
            func_ov071_021b6b98(screen, bottom->texts, 1, 0, 3);
            GFL_SndSEPlay(0x548);
            break;
        }
        if (keys & 0xf0) {
            sel = 0xff;
            for (i = 0; i < NELEMS(data_ov071_021ba85c) && data_ov071_021ba85c[i] != screen->state->mission; i++) {
            }
            mission = screen->state->mission;
            if (keys & 0x40) {
                sel = data_ov071_021ba862[mission * 4];
            } else if (keys & 0x80) {
                sel = data_ov071_021ba862[mission * 4 + 1];
            } else if (keys & 0x20) {
                sel = data_ov071_021ba862[mission * 4 + 2];
            } else if (keys & 0x10) {
                sel = data_ov071_021ba862[mission * 4 + 3];
            }
            if (sel != 0xff) {
                mission = sel;
            }
            screen->state->mission = mission;
            GFL_SndSEPlay(0x548);
        }
        if (touch >= 0 && touch < 6) {
            screen->state->mission = touch;
            bottom->unk_83 = 1;
            func_ov071_021b6b98(screen, bottom->texts, 1, 0xff, 3);
            GFL_SndSEPlay(0x548);
            break;
        }
        if (touch == 6 || (keys & 1)) {
            func_020355d8(touch == 6);
            if (func_ov010_02158590(GSYS_GetGameData(screen->param->gsys)) == FALSE) {
                kind = data_ov071_021baa4c[screen->state->mission];
                if (kind != 2 && kind != 1) {
                    *state = 9;
                    break;
                }
            }
            func_ov071_021b6bec(screen, bottom->texts, 1, 0, 3);
            GFL_SndSEPlay(0x736);
            *state = 5;
            break;
        }
        if (touch == 7 || (keys & 2)) {
            func_ov071_021b7108(&bottom->returnButton);
            func_020355d8(touch == 7);
            GFL_SndSEPlay(0x551);
            *state = 11;
        }
        break;
    case 4:
        keys = GCTX_HIDGetPressedKeys();
        touch = func_02035c08(data_ov071_021ba87c);
        if (touch == 7 || (keys & 2)) {
            func_020355d8(touch == 7);
            GFL_SndSEPlay(0x551);
            *state = 11;
        }
        return FALSE;
    case 5:
        bottom->mission = screen->work->missions[data_ov071_021baa4c[screen->state->mission]];
        bottom->unk_78 = screen->param->commPlayer.unk_11;
        bottom->player = screen->param->commPlayer.player;
        if (intrude == NULL || func_ov011_02173e2c(intrude, (const IntrudeMissionEntry *)&bottom->mission) == TRUE) {
            *state = 6;
        }
        break;
    case 6:
        if (intrude == NULL) {
            *state = 3;
            break;
        }
        result = func_ov011_021761ec(&intrude->unk_1AC);
        if (result == 1) {
            func_ov071_021b79a0(bottom, screen->work, 0x10);
            *state = 7;
        } else if (result == 2) {
            func_ov071_021b79a0(bottom, screen->work, 0x11);
            *state = 8;
        } else if (result == 3) {
            func_ov071_021b79a0(bottom, screen->work, 0x15);
            *state = 8;
        } else if (result == 4) {
            func_ov071_021b79a0(bottom, screen->work, 0x14);
            *state = 8;
        }
        break;
    case 7:
    case 8:
        if (func_ov071_021b7a08(screen->work, bottom) == TRUE) {
            if (*state == 7 || (*state == 8 && (func_02035c60() != FALSE || (GCTX_HIDGetPressedKeys() & 3) != 0))) {
                if (*state == 7 && intrude != NULL && func_ov011_02175b14(&intrude->unk_1AC) == FALSE) {
                    break;
                }
                func_ov071_021b7a14(bottom);
                GFL_SndSEPlay(0x547);
                if (*state == 7) {
                    if (intrude != NULL) {
                        func_ov011_02175b4c(intrude, &intrude->unk_1AC);
                    }
                    bottom->unk_80[1] = 1;
                    *state = 11;
                } else {
                    *state = 11;
                }
            }
        }
        break;
    case 9:
        func_ov071_021b79a0(bottom, screen->work, 0x17);
        (*state)++;
        break;
    case 10:
        if (func_ov071_021b7a08(screen->work, bottom) == TRUE) {
            if (func_02035c60() != FALSE || (GCTX_HIDGetPressedKeys() & 3) != 0) {
                func_ov071_021b7a14(bottom);
                *state = 3;
            }
        }
        break;
    case 11:
        if (func_ov071_021b6c74(screen, 3) != 2 && func_ov071_021b7114(&bottom->returnButton) == FALSE) {
            screen->next = bottom->unk_80[1] == 1 ? 4 : 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}



BOOL func_ov071_021b7824(GameProc *proc, u32 *state, MonolithScreenParam *screen, MonolithBottomWork *bottom) {
    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_BG3);
    if (bottom->unk_08 != NULL) {
        func_0201ce48(bottom->unk_08);
    }
    if (bottom->unk_04 != NULL) {
        GFL_StrBufFree(bottom->unk_04);
    }
    func_ov071_021b7068(bottom->unk_7C);
    func_ov071_021b7be4(bottom);
    func_ov071_021b7b9c(bottom);
    func_ov071_021b7aac(bottom);
    func_ov071_021b7994(bottom);
    func_ov071_021b78d4();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// The bottom screen's proc
const GameProcFunctions data_ov071_021ba8d0 = {
    (GameProcFunc)func_ov071_021b730c,
    func_ov071_021b739c,
    (GameProcFunc)func_ov071_021b7824,
};

void func_ov071_021b7880(void) {
    GFL_BGSysCreateBG(5, &data_ov071_021ba8dc, 0);
    GFL_BGSysCreateBG(6, &data_ov071_021ba8fc, 0);
    GFL_BGSysFillScrArea(5, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysFillScrArea(6, 0, 0, 0, 0x20, 0x20, 0x11);
    GFL_BGSysSetBGEnabled(5, 0);
}

void func_ov071_021b78d4(void) {
    GFL_BGSysSetBGEnabled(5, 0);
    GFL_BGSysSetBGEnabled(6, 0);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
}

void func_ov071_021b78f4(MonolithScreenParam *screen, MonolithWork *work) {
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 6, 6, 0, 0, 1, 0x61);
    func_ov071_021b7234(screen, 1, 6);
    GFL_BGSysLoadScr(6);
}

void func_ov071_021b7924(MonolithBottomWork *bottom, MonolithWork *work) {
    LoadSysMsgBoxBGChar(5, 1, 0, 0x61);
    func_020215ec(work->fade, 5, GetSysMsgBoxPaletteDatID(0), 0x61, 1, 0x20, 0xb0, 0);
    bottom->unk_00 = BmpWin_CreateDynamic(5, 1, 0x13, 0x1e, 4, 0xd, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(bottom->unk_00), 0xff);
    BmpWin_FlushMap(bottom->unk_00);
    BmpWin_DrawFrame(bottom->unk_00, 0, 1, 0xb);
}

void func_ov071_021b7994(MonolithBottomWork *bottom) {
    BmpWin_Free(bottom->unk_00);
}

void func_ov071_021b79a0(MonolithBottomWork *bottom, MonolithWork *work, u32 msgId) {
    StrBuf *strbuf;
    s32 wait;
    PrintStream *stream;

    if (bottom->unk_08 != NULL) {
        GFL_DebugAssertFail("", 0, "");
    }
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0xf);
    strbuf = GFL_MsgDataLoadStrbufNew(work->msgMonolith, msgId);
    bottom->unk_04 = strbuf;
    wait = func_02012ffc();
    stream = func_0201cce4(bottom->unk_00, 0, 0, bottom->unk_04, work->font, wait, work->tcbMgr, 10, 0x61, 0xf);
    bottom->unk_08 = stream;
    GFL_BGSysSetBGEnabled(5, 1);
}

BOOL func_ov071_021b7a08(MonolithWork *work, MonolithBottomWork *bottom) {
    return func_02028484(&work->printsys, bottom->unk_08);
}

void func_ov071_021b7a14(MonolithBottomWork *bottom) {
    GFLBitmap *bitmap;

    bitmap = BmpWin_GetBitmap(bottom->unk_00);
    if (bottom->unk_08 == NULL) {
        GFL_DebugAssertFail("", 0, "");
    }
    func_0201ce48(bottom->unk_08);
    bottom->unk_08 = NULL;
    GFL_StrBufFree(bottom->unk_04);
    bottom->unk_04 = NULL;
    GFL_BGSysSetBGEnabled(5, 0);
    GFL_BitmapFill(bitmap, 0xff);
    BmpWin_FlushChar(bottom->unk_00);
    GFL_TextRndUpdateColorIndexLUT(0xf, 2, 0);
}

void func_ov071_021b7a68(MonolithBottomWork *bottom, MonolithScreenParam *screen) {
    void *levels;
    int i;

    levels = func_ov071_021b6f40(screen);
    for (i = 0; i < 6; i++) {
        bottom->levelActors[i] = func_ov071_021b6f54(screen->work, levels, i, &data_ov071_021ba8a0[data_ov071_021baa4c[i] * 2], 1);
    }
}

void func_ov071_021b7aac(MonolithBottomWork *bottom) {
    int i;

    for (i = 0; i < 6; i++) {
        func_ov071_021b6fd8(bottom->levelActors[i]);
    }
}


void func_ov071_021b7ac4(MonolithBottomWork *bottom, MonolithScreenParam *screen) {
    void *levels;
    int i;

    levels = func_ov071_021b6f40(screen);
    for (i = 0; i < 6; i++) {
        func_ov071_021b6fe0(bottom->levelActors[i], levels, i);
    }
}

void func_ov071_021b7ae8(MonolithBottomWork *bottom, s32 index) {
    ClActorPos pos;

    pos.x = data_ov071_021ba8a0[index * 2];
    pos.y = data_ov071_021ba8a4[index * 2];
    func_0204b404(bottom->unk_7C, &pos, 1);
}


void func_ov071_021b7b10(MonolithScreenParam *screen, MonolithBottomWork *bottom) {
    func_ov071_021b6c54(screen);
    func_ov071_021b6a0c(screen->work, &bottom->texts[0], 1, 1, 0x80, 0x90, 0xe, NULL);
    if (func_020355c4() == 0) {
        func_ov071_021b6b98(screen, &bottom->texts[0], 1, 0, 3);
    } else {
        func_ov071_021b6b98(screen, &bottom->texts[0], 1, 0xff, 3);
        bottom->unk_83 = 1;
    }
    func_ov071_021b6a0c(screen->work, &bottom->texts[1], 1, 1, 0x80, 0x90, 0xf, NULL);
    func_ov071_021b6b58(&bottom->texts[1], FALSE);
}

void func_ov071_021b7b9c(MonolithBottomWork *bottom) {
    int i;

    for (i = 0; i < 2; i++) {
        func_ov071_021b6b38(&bottom->texts[i]);
    }
}

void func_ov071_021b7bb4(MonolithScreenParam *screen, MonolithBottomWork *bottom) {
    int i;

    func_ov071_021b6c7c(screen, 3);
    for (i = 0; i < 2; i++) {
        func_ov071_021b6b70(screen->work, &bottom->texts[i]);
    }
}

void func_ov071_021b7bd8(MonolithScreenParam *param, MonolithBottomWork *bottom) {
    func_ov071_021b7070(param->work, &bottom->returnButton);
}


void func_ov071_021b7be4(MonolithBottomWork *bottom) {
    func_ov071_021b70b8(&bottom->returnButton);
}

void func_ov071_021b7bf0(MonolithBottomWork *bottom) {
    func_ov071_021b70c4(&bottom->returnButton);
}



void func_ov071_021b7bfc(MonolithScreenParam *screen, MonolithBottomWork *bottom, s32 flag) {
    if (flag == 0) {
        func_ov071_021b6b58(&bottom->texts[0], TRUE);
        func_ov071_021b6b58(&bottom->texts[1], FALSE);
    } else {
        screen->state->mission = 6;
        func_ov071_021b6b58(&bottom->texts[0], FALSE);
        func_ov071_021b6b58(&bottom->texts[1], TRUE);
    }
    bottom->unk_82 = flag;
}

BOOL func_ov071_021b7c3c(GameProc *proc, u32 *state, MonolithScreenParam *screen) {
    MonolithPowerLevelsWork *work;
    PlayerInfo *info;
    StrBuf *strbuf;
    u16 *name;
    u32 gender;

    switch (*state) {
    case 0:
        work = GFL_ProcInitSubsystem(proc, sizeof(MonolithPowerLevelsWork), 0x61);
        MI_CpuFill8(work, 0, sizeof(MonolithPowerLevelsWork));
        func_ov071_021b7d6c();
        func_ov071_021b7dac(screen, screen->work);
        info = func_ov071_021b6f2c(screen);
        strbuf = GFL_StrBufCreate(8, 0x61);
        name = GetPlayerName(info);
        GFL_StrBufLoadFixedString(strbuf, name, 8);
        gender = getTrainerGender(info);
        func_0201ee0c(screen->work->wordSet, 0, strbuf, gender, 1, 2);
        GFL_StrBufFree(strbuf);
        func_ov071_021b6e1c(screen->work, &work->text, 0, 0x80, 0xc, 0x1e, 2, 0, screen->work->wordSet);
        func_ov071_021b7de0(work, screen);
        *state = *state + 1;
        break;
    case 1:
        GFL_BGSysSetBGEnabled(2, 1);
        GFL_BGSysSetBGEnabledA(0x10, 1);
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov071_021b7d00(GameProc *proc, u32 *state, MonolithScreenParam *screen, void *work) {
    MonolithPowerLevelsWork *top = work;
    s64 flag;
    if (screen->endTop == 1) {
        return TRUE;
    }
    func_ov071_021b6f04(screen->work, &top->text);
    func_ov071_021b7e3c(top, screen);
    flag = FALSE;
    if (*state == 0) {
        return FALSE;
    }
    return flag;
}

BOOL func_ov071_021b7d34(GameProc *proc, u32 *state, MonolithScreenParam *screen, void *work) {
    MonolithPowerLevelsWork *top = work;

    if (func_0201c6b4(screen->work->printQueue) == FALSE) {
        return FALSE;
    }
    GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_BG3);
    func_ov071_021b7e24(top);
    func_ov071_021b6eec(&top->text);
    func_ov071_021b7d98();
    GFL_ProcReleaseSubsystem(proc);
    return TRUE;
}

// The top screen's proc of the pass power levels
const GameProcFunctions data_ov071_021ba91c = {
    (GameProcFunc)func_ov071_021b7c3c,
    (GameProcFunc)func_ov071_021b7d00,
    (GameProcFunc)func_ov071_021b7d34,
};



void func_ov071_021b7d6c(void) {
    GFL_BGSysCreateBG(2, &data_ov071_021ba928, 0);
    GFL_BGSysFillScrArea(2, 0, 0, 0, 0x20, 0x20, 0x11);
}

void func_ov071_021b7d98(void) {
    GFL_BGSysSetBGEnabled(2, 0);
    GFL_BGSysReleaseBG(2);
}

void func_ov071_021b7dac(MonolithScreenParam *screen, MonolithWork *work) {
    loadBGScrToVramByFileNoReserveNegAlign(work->arc, 0xb, 2, 0, 0, 1, 0x61);
    func_ov071_021b7234(screen, 0, 2);
    GFL_BGSysLoadScr(2);
}

void func_ov071_021b7de0(MonolithPowerLevelsWork *top, MonolithScreenParam *screen) {
    void *levels;
    int i;

    levels = func_ov071_021b6f40(screen);
    for (i = 0; i < 6; i++) {
        top->levelActors[i] = func_ov071_021b6f54(screen->work, levels, i, &data_ov071_021ba948[data_ov071_021baa4c[i] * 2], 0);
    }
}

void func_ov071_021b7e24(MonolithPowerLevelsWork *top) {
    int i;

    for (i = 0; i < 6; i++) {
        func_ov071_021b6fd8(top->levelActors[i]);
    }
}


void func_ov071_021b7e3c(MonolithPowerLevelsWork *top, MonolithScreenParam *screen) {
    void *levels;
    int i;

    levels = func_ov071_021b6f40(screen);
    for (i = 0; i < 6; i++) {
        func_ov071_021b6fe0(top->levelActors[i], levels, i);
    }
}
