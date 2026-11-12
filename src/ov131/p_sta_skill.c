// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov207/p_sta_skill.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "constants/arc.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/math.h"
#include "p_status_local.h"
#include "pml/hm_check.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/app_menu_common.h"
#include "system/hp_gauge.h"
#include "system/wordset.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
void func_ov131_021d62b4(PStatusWork * wk, BmpWin * window, WordSet * wordSet, u32 msgId, u16 x, u16 y, u16 color);


// The summary screen's skill page: the stats and ability, and the moves as plates of sprites, with the details of the
// move picked, swapping two moves, and, when a new move is learned, picking the move to forget. The names of the
// fields and functions are guesses

#define SKILL_WINDOW_COUNT 13
// Four moves, and the move to learn
#define PLATE_COUNT 5
#define PLATE_NEW 4
#define CURSOR_NONE 0xff
#define SLOT_NONE 5

// What a plate's animation shows, added to its slot times PLATE_COUNT
enum {
    PLATE_ANIM_NORMAL,
    PLATE_ANIM_SELECTED,
    PLATE_ANIM_HELD,
};

// Black 2's, not checked against Black
typedef struct {
    u8 x;
    u8 y;
} PlatePos;

// A move's plate: its frame and type icon, and the bitmap of its name and PP shown as sprites
typedef struct {
    u8 slot;
    BOOL isDirty;
    ClActor *plate;
    ClActor *typeIcon;
    GFLBitmap *bitmap;
    PStaOamActor *oam;
} SkillPlate;

struct PStaSkillWork {
    BOOL isShown;
    BOOL redrawStats;
    BOOL redrawDetail;
    // Not cleared when the work is allocated, so it is read before it is first set
    BOOL redrawMessage;
    BOOL isSwapPending;
    BOOL isSwapping;
    BOOL isConfirming;
    // Not cleared when the work is allocated, so it is read before it is first set
    BOOL isDragging;
    BOOL isExiting;
    BOOL forgetConfirmed;
    u8 exitTimer;
    u8 cursor;
    u8 swapSrc;
    u8 moveCount;
    u32 dragStartX;
    u32 dragStartY;
    PStaScreen screens[5];
    BmpWin *windows[SKILL_WINDOW_COUNT];
    // The stats' frame, the move category, a cursor that is never shown, the held move and the touched move
    ClActor *actors[5];
    PStaOam *oam;
    GFLBitmap *forgetBitmap;
    PStaOamActor *forgetOam;
    SkillPlate plates[PLATE_COUNT];
};

void func_ov131_021d9e30(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d9e44(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d9ee8(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021d9ef0(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021da0b0(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021da3fc(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021da488(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021da958(PStatusWork *wk, PStaSkillWork *skill);
BOOL func_ov131_021da998(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021dabcc(PStatusWork *wk, PStaSkillWork *skill);
BOOL func_ov131_021daee4(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021db140(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021db3f0(PStatusWork *wk, PStaSkillWork *skill);
void func_ov131_021db468(PStatusWork *wk, PStaSkillWork *skill, u8 slot, BOOL force);
void func_ov131_021db508(PStatusWork *wk, PStaSkillWork *skill, u8 slotA, u8 slotB);
void func_ov131_021db614(PStatusWork *wk, PStaSkillWork *skill, BOOL show);
void func_ov131_021db688(PStatusWork *wk, PStaSkillWork *skill, BOOL isHm);
u16 func_ov131_021db6d0(PStatusWork *wk, PStaSkillWork *skill, u8 nature, u32 stat);
void func_ov131_021db700(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate, u8 slot);
void func_ov131_021db7b8(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021db7d0(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021db7d4(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021db964(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021db9e8(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021db9fc(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate);
void func_ov131_021dba14(SkillPlate *plate, TouchRect *rect);
void func_ov131_021dba54(SkillPlate *plate, ClActorPos *pos);
void func_ov131_021dba78(SkillPlate *plate, u32 anim);

// The Pokémon's move in a slot
static inline u32 PStaSkill_GetMove(PStatusWork *wk, u32 slot) {
    return PML_PkmGetParam(func_ov131_021d6064(wk), PKM_PARAM_MOVE1 + slot, NULL);
}

// pokebw2's sPlatePos
static const PlatePos data_ov131_021dc2b8[PLATE_COUNT] = { { 1, 2 }, { 1, 6 }, { 1, 10 }, { 1, 14 }, { 1, 20 } };

// pokebw2's sSkillWindows
static const PStaWindowSetup data_ov131_021dc2c2[SKILL_WINDOW_COUNT] = {
#include "asset/p_sta_skill_data_ov131_021dc2c2.inc"
};

PStaSkillWork *func_ov131_021d96b0(PStatusWork *wk) {
    PStaSkillWork *skill;
    PStatusParam *param;
    u8 i;

    skill = GFL_HeapAllocate(wk->heapId, sizeof(PStaSkillWork), 0, "p_sta_skill.c", 0x13B);
    skill->isShown = FALSE;
    skill->redrawStats = FALSE;
    skill->redrawDetail = FALSE;
    skill->isSwapPending = FALSE;
    skill->isSwapping = FALSE;
    skill->isConfirming = FALSE;
    skill->forgetConfirmed = FALSE;
    skill->swapSrc = SLOT_NONE;
    skill->cursor = CURSOR_NONE;
    param = wk->param;
    if (param->mode == PSTATUS_MODE_FORGET_MOVE && param->move != 0) {
        skill->moveCount = PLATE_COUNT;
    } else {
        skill->moveCount = PLATE_COUNT - 1;
    }
    for (i = 0; i < PLATE_COUNT; i++) {
        skill->plates[i].slot = 0xff;
        skill->plates[i].isDirty = FALSE;
    }
    return skill;
}

void func_ov131_021d9734(PStatusWork *wk, PStaSkillWork *skill) {
    GFL_HeapFree(skill);
}

void func_ov131_021d9740(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;
    BOOL done;
    BmpWin *window;

    if (skill->redrawStats == FALSE && skill->redrawDetail == FALSE && skill->isSwapPending == FALSE &&
        skill->isExiting == FALSE) {
        func_ov131_021da958(wk, skill);
    }
    if (skill->redrawStats == TRUE) {
        done = TRUE;
        for (i = 0; i <= 7; i++) {
            if (func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(skill->windows[i])) == TRUE) {
                done = FALSE;
                break;
            }
        }
        if (done == TRUE) {
            for (i = 0; i <= 7; i++) {
                window = skill->windows[i];
                BmpWin_FlushChar(window);
                BmpWin_FlushMap(window);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
            }
            func_ov131_021d9e44(wk, skill);
            skill->redrawStats = FALSE;
        }
    }
    if (skill->redrawDetail == TRUE) {
        done = TRUE;
        for (i = 8; i <= 11; i++) {
            if (func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(skill->windows[i])) == TRUE) {
                done = FALSE;
                break;
            }
        }
        if (done == TRUE) {
            for (i = 8; i <= 11; i++) {
                window = skill->windows[i];
                BmpWin_FlushChar(window);
                BmpWin_FlushMap(window);
                GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
            }
            func_ov131_021d9ef0(wk, skill);
            skill->redrawDetail = FALSE;
        }
    }
    if (skill->redrawMessage == TRUE && func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(skill->windows[12])) == FALSE) {
        window = skill->windows[12];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysQueueScrLoad(BmpWin_GetBGIndex(window));
        skill->redrawMessage = FALSE;
    }
    if (skill->isSwapPending == TRUE && func_0201c6b4(wk->printQueue) == TRUE) {
        skill->isSwapping = FALSE;
        func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
        skill->swapSrc = SLOT_NONE;
        func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        func_0204b3dc(skill->actors[3], FALSE);
        skill->isSwapPending = FALSE;
    }
    for (i = 0; i < PLATE_COUNT; i++) {
        func_ov131_021db7d0(wk, skill, &skill->plates[i]);
    }
    if (skill->isExiting == TRUE) {
        skill->exitTimer++;
        if (skill->exitTimer % 8 < 4) {
            if (skill->forgetConfirmed == TRUE) {
                func_0204b878(skill->plates[skill->cursor].plate, 15);
            } else {
                func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_NORMAL);
            }
        } else if (skill->forgetConfirmed == TRUE) {
            func_0204b878(skill->plates[skill->cursor].plate, 16);
        } else {
            func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_SELECTED);
        }
        if (skill->exitTimer > 16) {
            wk->unk50 = 2;
        }
    }
}


void func_ov131_021d9980(PStatusWork *wk, PStaSkillWork *skill, ArcTool *arc) {
    skill->screens[0].file = GFL_G2DIOReadNSCRArc(arc, 0x48, 0, &skill->screens[0].screen, wk->heapId);
    skill->screens[1].file = GFL_G2DIOReadNSCRArc(arc, 0x49, 0, &skill->screens[1].screen, wk->heapId);
    skill->screens[2].file = GFL_G2DIOReadNSCRArc(arc, 0x4a, 0, &skill->screens[2].screen, wk->heapId);
    skill->screens[3].file = GFL_G2DIOReadNSCRArc(arc, 0x4b, 0, &skill->screens[3].screen, wk->heapId);
    skill->screens[4].file = GFL_G2DIOReadNSCRArc(arc, 0x4c, 0, &skill->screens[4].screen, wk->heapId);
    skill->oam = func_ov131_021dba90(wk->heapId, wk->actorUnit);
}

void func_ov131_021d9a04(PStatusWork *wk, PStaSkillWork *skill) {
    if (skill->isShown == TRUE) {
        if (wk->param->mode == PSTATUS_MODE_FORGET_MOVE) {
            func_ov131_021da84c(wk, skill);
        } else {
            func_ov131_021d9d44(wk, skill);
        }
    }
    func_ov131_021dbac0(skill->oam);
    GFL_HeapFree(skill->screens[0].file);
    GFL_HeapFree(skill->screens[1].file);
    GFL_HeapFree(skill->screens[2].file);
    GFL_HeapFree(skill->screens[3].file);
    GFL_HeapFree(skill->screens[4].file);
}

void func_ov131_021d9a4c(PStatusWork *wk, PStaSkillWork *skill) {
    ClActorSetup setup;
    u8 i;

    setup.x = 159;
    setup.y = 52;
    setup.priority = 10;
    setup.bgPriority = 1;
    setup.sequence = 0;
    skill->actors[0] = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(7)], wk->unk98[PSTA_RES_PLTT(5)],
                                     wk->unk98[PSTA_RES_CELL(6)], &setup, 1, wk->heapId);
    func_0204b3dc(skill->actors[0], FALSE);
    setup.x = 172;
    setup.y = 56;
    skill->actors[1] = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(4)], wk->unk98[PSTA_RES_PLTT(4)],
                                     wk->unk98[PSTA_RES_CELL(4)], &setup, 1, wk->heapId);
    func_0204b3dc(skill->actors[1], FALSE);
    setup.bgPriority = 0;
    skill->actors[2] = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(9)], wk->unk98[PSTA_RES_PLTT(11)],
                                     wk->unk98[PSTA_RES_CELL(8)], &setup, 0, wk->heapId);
    func_0204b3dc(skill->actors[2], FALSE);
    setup.sequence = 5;
    setup.priority = 8;
    skill->actors[3] = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(9)], wk->unk98[PSTA_RES_PLTT(11)],
                                     wk->unk98[PSTA_RES_CELL(8)], &setup, 0, wk->heapId);
    func_0204b3dc(skill->actors[3], FALSE);
    func_0204b92c(skill->actors[3], TRUE);
    setup.sequence = 2;
    setup.priority = 4;
    setup.bgPriority = 0;
    skill->actors[4] = func_0204b294(wk->actorUnit, wk->unk98[PSTA_RES_CHAR(9)], wk->unk98[PSTA_RES_PLTT(11)],
                                     wk->unk98[PSTA_RES_CELL(8)], &setup, 0, wk->heapId);
    func_0204b3dc(skill->actors[4], FALSE);
    func_0204b92c(skill->actors[4], TRUE);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db700(wk, skill, &skill->plates[i], i);
    }
}



void func_ov131_021d9bd4(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db7b8(wk, skill, &skill->plates[i]);
    }
    func_0204b3b4(skill->actors[0]);
    func_0204b3b4(skill->actors[1]);
    func_0204b3b4(skill->actors[2]);
    func_0204b3b4(skill->actors[3]);
    func_0204b3b4(skill->actors[4]);
}

void func_ov131_021d9c3c(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    for (i = 0; i < SKILL_WINDOW_COUNT; i++) {
        skill->windows[i] = BmpWin_CreateDynamic(4, data_ov131_021dc2c2[i].x, data_ov131_021dc2c2[i].y,
                                                 data_ov131_021dc2c2[i].width, data_ov131_021dc2c2[i].height, 14, 1);
    }
    func_02045798(skill->windows[1], 13);
    func_ov131_021d9e30(wk, skill);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db7d4(wk, skill, &skill->plates[i]);
    }
    skill->cursor = CURSOR_NONE;
    skill->isShown = TRUE;
    skill->isExiting = FALSE;
    skill->exitTimer = 0;
}


void func_ov131_021d9cd0(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    GFL_BGSysLoadScrArea(2, 0, 0, 32, 24, skill->screens[0].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(2);
    func_ov131_021d9e44(wk, skill);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db964(wk, skill, &skill->plates[i]);
    }
    func_0204b878(wk->unk178[1], 4);
}

void func_ov131_021d9d44(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db9e8(wk, skill, &skill->plates[i]);
    }
    for (i = 0; i < SKILL_WINDOW_COUNT; i++) {
        BmpWin_Free(skill->windows[i]);
    }
    skill->isShown = FALSE;
}


void func_ov131_021d9d94(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db9fc(wk, skill, &skill->plates[i]);
    }
    GFL_BGSysFillScrArea(1, 0, 0, 0, 0x13, 0x15, 0x10);
    GFL_BGSysLoadScr(1);
    GFL_BGSysFillScrAsync(4, 0);
    GFL_BGSysQueueScrLoad(4);
    func_0204b3dc(skill->actors[0], FALSE);
    func_0204b3dc(skill->actors[1], FALSE);
    func_0204b3dc(wk->typeIcons[0], FALSE);
    func_0204b3dc(wk->typeIcons[1], FALSE);
    func_0204b878(wk->unk178[1], 1);
}

// pokebw2's PStaSkill_PrintStats
void func_ov131_021d9e30(PStatusWork *wk, PStaSkillWork *skill) {
    func_ov131_021da0b0(wk, skill);
    func_ov131_021da3fc(wk, skill);
}

void func_ov131_021d9e44(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;
    BmpWin *window;

    GFL_BGSysFillScrAsync(4, 0);
    for (i = 0; i <= 7; i++) {
        window = skill->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
    GFL_BGSysBufferScrDefault(6, skill->screens[1].screen->rawData, skill->screens[1].screen->szByte);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysLoadScrArea(5, 0, 0, 32, 3, skill->screens[3].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(5);
    func_0204b3dc(skill->actors[0], TRUE);
    func_0204b3dc(skill->actors[1], FALSE);
    func_0204b3dc(wk->typeIcons[0], FALSE);
    func_0204b3dc(wk->typeIcons[1], FALSE);
}


// pokebw2's PStaSkill_PrintDetail
void func_ov131_021d9ee8(PStatusWork *wk, PStaSkillWork *skill) {
    func_ov131_021da488(wk, skill);
}

void func_ov131_021d9ef0(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;
    BmpWin *window;
    BoxPkm *pkm;
    u32 type1;
    u32 type2;
    ClActorPos pos;
    NNSG2dImageProxy proxy1;
    NNSG2dImageProxy proxy2;

    GFL_BGSysFillScrAsync(4, 0);
    for (i = 8; i <= 11; i++) {
        window = skill->windows[i];
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysLoadScr(BmpWin_GetBGIndex(window));
    }
    GFL_BGSysLoadScrCore(6, skill->screens[2].screen->rawData, skill->screens[2].screen->szByte, 0);
    GFL_BGSysBufferScrDefault(6, skill->screens[2].screen->rawData, skill->screens[2].screen->szByte);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysLoadScrArea(5, 0, 0, 32, 3, skill->screens[4].screen->rawData, 0, 0, 32, 32);
    GFL_BGSysQueueScrLoad(5);
    func_0204b3dc(skill->actors[0], FALSE);
    func_0204b3dc(skill->actors[1], TRUE);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db964(wk, skill, &skill->plates[i]);
    }
    pkm = func_ov131_021d6064(wk);
    type1 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE1, NULL);
    type2 = PML_PkmGetParam(pkm, PKM_PARAM_TYPE2, NULL);
    func_0204ab84(wk->typeIconChars[type1], &proxy1);
    func_0204b784(wk->typeIcons[0], &proxy1);
    func_0204b6f4(wk->typeIcons[0], func_020275ec(type1), 1);
    pos.x = 208;
    pos.y = 120;
    func_0204b404(wk->typeIcons[0], &pos, 0);
    func_0204b3dc(wk->typeIcons[0], TRUE);
    func_0204b84c(wk->typeIcons[0], 0);
    if (type1 != type2) {
        func_0204ab84(wk->typeIconChars[type2], &proxy2);
        func_0204b784(wk->typeIcons[1], &proxy2);
        func_0204b6f4(wk->typeIcons[1], func_020275ec(type2), 1);
        pos.x = 240;
        pos.y = 120;
        func_0204b404(wk->typeIcons[1], &pos, 0);
        func_0204b3dc(wk->typeIcons[1], TRUE);
        func_0204b84c(wk->typeIcons[1], 0);
    } else {
        func_0204b3dc(wk->typeIcons[1], FALSE);
    }
}

void func_ov131_021da0b0(PStatusWork *wk, PStaSkillWork *skill) {
    PartyPkm *pkm = func_ov131_021d60a4(wk);
    u32 nature = PokeParty_GetParam(pkm, PKM_PARAM_NATURE, NULL);
    WordSet *wordSet;
    u32 ability;
    MsgData *msgData;
    StrBuf *str;

    func_ov131_021d618c(wk, skill->windows[0], 0x6e, 25, 1, PRINT_COLOR(15, 2, 0));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[0], wordSet, 0x77, 89, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);
    func_ov131_021d618c(wk, skill->windows[0], 0x75, 93, 1, PRINT_COLOR(1, 2, 0));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[0], wordSet, 0x76, 121, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);

    func_ov131_021d618c(wk, skill->windows[2], 0x6f, 1, 1, func_ov131_021db6d0(wk, skill, nature, 1));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_ATTACK, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[2], wordSet, 0x78, 105, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);
    func_ov131_021d618c(wk, skill->windows[3], 0x70, 1, 1, func_ov131_021db6d0(wk, skill, nature, 2));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_DEFENSE, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[3], wordSet, 0x79, 105, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);
    func_ov131_021d618c(wk, skill->windows[4], 0x71, 1, 1, func_ov131_021db6d0(wk, skill, nature, 3));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_SP_ATTACK, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[4], wordSet, 0x7a, 105, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);
    func_ov131_021d618c(wk, skill->windows[5], 0x72, 1, 1, func_ov131_021db6d0(wk, skill, nature, 4));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_SP_DEFENSE, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[5], wordSet, 0x7b, 105, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);
    func_ov131_021d618c(wk, skill->windows[6], 0x73, 1, 1, func_ov131_021db6d0(wk, skill, nature, 5));
    wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    WordSetNumber(wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_SPEED, NULL), 3, 1, 1);
    func_ov131_021d62b4(wk, skill->windows[6], wordSet, 0x7c, 105, 1, PRINT_COLOR(1, 2, 0));
    GFL_WordSetSystemFree(wordSet);

    func_ov131_021d618c(wk, skill->windows[7], 0x74, 5, 1, PRINT_COLOR(15, 2, 0));
    ability = PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_DESCRIPTIONS, wk->heapId);
    str = GFL_MsgDataLoadStrbufNew(msgData, ability);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(skill->windows[7]), 5, 17, str, wk->font,
                          PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_NAMES, wk->heapId);
    str = GFL_MsgDataLoadStrbufNew(msgData, ability);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(skill->windows[7]), 65, 1, str, wk->font,
                          PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    skill->redrawStats = TRUE;
}



// pokebw2's PStaSkill_DrawHpBar
void func_ov131_021da3fc(PStatusWork *wk, PStaSkillWork *skill) {
    PartyPkm *pkm = func_ov131_021d60a4(wk);
    u16 maxHp = PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL);
    u16 hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    u8 color = func_0202d034(hp, maxHp);
    u8 width = func_0202d01c(hp, maxHp, 48);
    GFLBitmap *bitmap = BmpWin_GetBitmap(skill->windows[1]);
    u8 top;
    u8 bottom;

    if (color == HP_GAUGE_COLOR_RED || color == HP_GAUGE_COLOR_NONE) {
        top = 7;
        bottom = 8;
    } else if (color == HP_GAUGE_COLOR_YELLOW) {
        top = 9;
        bottom = 10;
    } else {
        top = 5;
        bottom = 6;
    }
    GFL_BitmapFillArea(bitmap, 0, 3, width, 1, top);
    GFL_BitmapFillArea(bitmap, 0, 4, width, 1, bottom);
}

void func_ov131_021da488(PStatusWork *wk, PStaSkillWork *skill) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u8 i;
    u32 move;
    s32 category;
    u32 power;
    WordSet *wordSet;
    u32 accuracy;
    MsgData *msgData;
    StrBuf *str;
    NNSG2dImageProxy proxy;

    for (i = 8; i <= 11; i++) {
        GFL_BitmapFill(BmpWin_GetBitmap(skill->windows[i]), 0);
    }
    if (skill->cursor < PLATE_NEW) {
        move = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + skill->cursor, NULL);
    } else if (skill->cursor == CURSOR_NONE) {
        if (wk->param->mode == PSTATUS_MODE_FORGET_MOVE && skill->moveCount == PLATE_COUNT) {
            move = wk->param->move;
        } else {
            move = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1, NULL);
        }
    } else {
        move = wk->param->move;
    }
    func_ov131_021d618c(wk, skill->windows[8], 0x95, 1, 1, PRINT_COLOR(15, 2, 0));
    category = PML_MoveGetCategory(move);
    func_0204ab84(wk->unk98[16 + category], &proxy);
    func_0204b784(skill->actors[1], &proxy);
    func_0204b6f4(skill->actors[1], func_02027604(category), 1);
    func_ov131_021d618c(wk, skill->windows[9], 0x93, 1, 1, PRINT_COLOR(15, 2, 0));
    power = PML_MoveGetBasePower(move);
    if (power <= 1) {
        func_ov131_021d618c(wk, skill->windows[9], 0x9a, 87, 1, PRINT_COLOR(1, 2, 0));
    } else {
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, power, 3, 1, 1);
        func_ov131_021d6214(wk, skill->windows[9], wordSet, 0x96, 81, 1, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);
    }
    func_ov131_021d618c(wk, skill->windows[10], 0x94, 1, 1, PRINT_COLOR(15, 2, 0));
    accuracy = PML_MoveGetParam(move, MOVE_PARAM_ACCURACY);
    if (PML_MoveIsAlwaysHit(move) == TRUE || accuracy == 0) {
        func_ov131_021d618c(wk, skill->windows[10], 0x9a, 87, 1, PRINT_COLOR(1, 2, 0));
    } else {
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, accuracy, 3, 1, 1);
        func_ov131_021d6214(wk, skill->windows[10], wordSet, 0x97, 81, 1, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(wordSet);
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_13, wk->heapId);
    str = GFL_MsgDataLoadStrbufNew(msgData, move);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(skill->windows[11]), 1, 1, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    skill->redrawMessage = FALSE;
    skill->redrawDetail = TRUE;
}


void func_ov131_021da6a4(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;
    struct PStaOamSetup setup;

    for (i = 0; i < SKILL_WINDOW_COUNT; i++) {
        skill->windows[i] = BmpWin_CreateDynamic(4, data_ov131_021dc2c2[i].x, data_ov131_021dc2c2[i].y,
                                                 data_ov131_021dc2c2[i].width, data_ov131_021dc2c2[i].height, 14, 1);
    }
    func_02045798(skill->windows[1], 13);
    func_ov131_021d9ee8(wk, skill);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db7d4(wk, skill, &skill->plates[i]);
    }
    if (skill->moveCount == PLATE_COUNT) {
        skill->forgetBitmap = GFL_BitmapCreate(11, 4, 32, wk->heapId);
        func_ov131_021d614c(wk, skill->forgetBitmap, 0xc1, 5, 9, PRINT_COLOR(1, 2, 0));
        setup.x = 32;
        setup.y = 160;
        setup.palette = wk->unk98[8];
        setup.paletteOffset = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        setup.surface = 0;
        setup.vramType = 0;
        setup.bitmap = skill->forgetBitmap;
        skill->forgetOam = func_ov131_021dbad0(skill->oam, &setup);
        func_ov131_021dbca4(skill->forgetOam, FALSE);
    }
    skill->cursor = CURSOR_NONE;
    skill->isShown = TRUE;
    skill->isExiting = FALSE;
    skill->exitTimer = 0;
}


void func_0204b878(ClActor *actor, u16 seq);
void func_ov131_021d5a9c(PStatusWork *wk, u32 mode);
void GFL_BGSysLoadScrArea(u8 bg, u8 x, u8 y, u8 width, u8 height, const void *src, u8 srcX, u8 srcY, u8 srcWidth, u8 srcHeight);
void GFL_BGSysQueueScrLoad(u8 bg);


void func_ov131_021da7a4(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    GFL_BGSysLoadScrArea(2, 0, 0, 0x20, 0x18, skill->screens[0].screen->rawData, 0, 0, 0x20, 0x20);
    GFL_BGSysQueueScrLoad(2);
    func_ov131_021d9ef0(wk, skill);
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db964(wk, skill, &skill->plates[i]);
    }
    func_0204b878(wk->unk178[1], 4);
    if (skill->moveCount == PLATE_COUNT) {
        skill->cursor = PLATE_NEW;
    } else {
        skill->cursor = 0;
    }
    func_ov131_021d5a9c(wk, FALSE);
    func_ov131_021db468(wk, skill, skill->cursor, FALSE);
}

void func_ov131_021da84c(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    if (skill->moveCount == PLATE_COUNT) {
        GFL_BitmapFree(skill->forgetBitmap);
        func_ov131_021dbc3c(skill->forgetOam);
    }
    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db9e8(wk, skill, &skill->plates[i]);
    }
    for (i = 0; i < SKILL_WINDOW_COUNT; i++) {
        BmpWin_Free(skill->windows[i]);
    }
    skill->isShown = FALSE;
}



void func_ov131_021da8bc(PStatusWork *wk, PStaSkillWork *skill) {
    u8 i;

    for (i = 0; i < skill->moveCount; i++) {
        func_ov131_021db9fc(wk, skill, &skill->plates[i]);
    }
    GFL_BGSysFillScrArea(1, 0, 0, 0, 0x13, 0x15, 0x10);
    GFL_BGSysLoadScr(1);
    GFL_BGSysFillScrAsync(4, 0);
    GFL_BGSysQueueScrLoad(4);
    func_0204b3dc(skill->actors[0], FALSE);
    func_0204b3dc(skill->actors[1], FALSE);
    func_0204b3dc(wk->typeIcons[0], FALSE);
    func_0204b3dc(wk->typeIcons[1], FALSE);
    func_0204b878(wk->unk178[1], 1);
}

// pokebw2's PStaSkill_HandleInput
void func_ov131_021da958(PStatusWork *wk, PStaSkillWork *skill) {
    if (wk->param->mode != PSTATUS_MODE_FORGET_MOVE) {
        if (func_ov131_021da998(wk, skill) == FALSE) {
            func_ov131_021dabcc(wk, skill);
        }
    } else if (func_ov131_021daee4(wk, skill) == FALSE) {
        func_ov131_021db140(wk, skill);
    }
    if (func_02035c38() == FALSE) {
        skill->isDragging = FALSE;
    }
}

BOOL func_ov131_021da998(PStatusWork *wk, PStaSkillWork *skill) {
    ClActorPos pos;

    if (skill->isDragging == TRUE) {
        return FALSE;
    }
    if (wk->unk44 == TRUE) {
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
            skill->cursor = 0;
            wk->unk1C = FALSE;
            func_ov131_021d5a9c(wk, FALSE);
            func_ov131_021db468(wk, skill, skill->cursor, FALSE);
            func_ov131_021d9ee8(wk, skill);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            return TRUE;
        }
    } else if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B && skill->isSwapping == FALSE) {
        func_ov131_021d5a9c(wk, TRUE);
        func_0204b878(wk->unk178[7], 9);
        func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_NORMAL);
        func_0204b3dc(skill->actors[2], FALSE);
        func_ov131_021d9e30(wk, skill);
        wk->unk1C = FALSE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return TRUE;
    } else if (wk->unk1C == TRUE) {
        if (GCTX_HIDGetPressedKeys() != 0) {
            wk->unk1C = FALSE;
            skill->swapSrc = SLOT_NONE;
            func_ov131_021db468(wk, skill, skill->cursor, FALSE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            return TRUE;
        }
    } else if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
        if (wk->param->mode != PSTATUS_MODE_LOCK_MARKINGS) {
            if (skill->isSwapping == FALSE) {
                skill->isSwapping = TRUE;
                skill->swapSrc = skill->cursor;
                func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_HELD);
                func_ov131_021dba54(&skill->plates[skill->swapSrc], &pos);
                func_0204b404(skill->actors[3], &pos, 0);
                func_0204b3dc(skill->actors[3], TRUE);
                func_0204b9a0(skill->actors[3]);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            } else if (skill->swapSrc != skill->cursor) {
                func_ov131_021db508(wk, skill, skill->swapSrc, skill->cursor);
                GFL_SndSEPlay(SEQ_SE_DECIDE2);
            } else {
                skill->isSwapping = FALSE;
                func_ov131_021db468(wk, skill, skill->cursor, FALSE);
                func_0204b3dc(skill->actors[3], FALSE);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
            }
            return TRUE;
        }
    } else if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B && skill->isSwapping == TRUE) {
        func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
        func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        func_0204b878(wk->unk178[7], 9);
        func_0204b3dc(skill->actors[3], FALSE);
        skill->isSwapping = FALSE;
        skill->swapSrc = SLOT_NONE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return TRUE;
    } else if (GCTX_HIDGetPressedKeys() == PAD_KEY_UP || GCTX_HIDGetPressedKeys() == PAD_KEY_DOWN) {
        func_ov131_021db3f0(wk, skill);
        return TRUE;
    }
    return FALSE;
}

void func_ov131_021dabcc(PStatusWork *wk, PStaSkillWork *skill) {
    ClActorPos touchPos;
    ClActorPos heldPos;
    TouchRect rects[PLATE_COUNT + 1];
    u8 i;
    s32 hit;

    if (wk->touchHit == 7 && wk->unk44 == 0) {
        func_ov131_021d5a9c(wk, TRUE);
        func_0204b878(wk->unk178[7], 9);
        func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_NORMAL);
        func_0204b3dc(skill->actors[2], FALSE);
        func_ov131_021d9e30(wk, skill);
        if (skill->isSwapping == TRUE) {
            func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
            func_0204b3dc(skill->actors[3], FALSE);
            skill->isSwapping = FALSE;
            skill->swapSrc = SLOT_NONE;
        }
        wk->unk1C = TRUE;
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return;
    }
    for (i = 0; i < PLATE_COUNT; i++) {
        func_ov131_021dba14(&skill->plates[i], &rects[i]);
    }
    rects[PLATE_COUNT].top = TOUCH_RECT_END;
    if (func_02035c60() == TRUE) {
        hit = func_02035c08(rects);
        if (hit == TOUCH_RECT_NONE) {
            return;
        }
        if (PML_PkmGetParam(func_ov131_021d6064(wk), PKM_PARAM_MOVE1 + hit, NULL) == MOVE_NONE) {
            return;
        }
        if (wk->unk44 == 1) {
            func_ov131_021d5a9c(wk, FALSE);
        }
        if (wk->param->mode != PSTATUS_MODE_LOCK_MARKINGS) {
            skill->isDragging = TRUE;
            if (skill->isSwapping != TRUE) {
                skill->swapSrc = hit;
                func_ov131_021dba54(&skill->plates[hit], &heldPos);
                func_0204b404(skill->actors[4], &heldPos, 0);
                func_0204b3dc(skill->actors[4], TRUE);
                if (hit == 0) {
                    func_0204b878(skill->actors[4], 3);
                } else if (hit == 3) {
                    func_0204b878(skill->actors[4], 4);
                } else {
                    func_0204b878(skill->actors[4], 2);
                }
            }
            skill->dragStartX = wk->unk28;
            skill->dragStartY = wk->unk2C;
            func_ov131_021dba54(&skill->plates[hit], &touchPos);
            func_0204b404(skill->actors[3], &touchPos, 0);
            func_0204b3dc(skill->actors[3], TRUE);
            func_0204b9a0(skill->actors[3]);
        }
        wk->unk1C = TRUE;
        func_ov131_021db468(wk, skill, hit, TRUE);
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    } else if (func_02035c38() == TRUE) {
        if (skill->isDragging != TRUE) {
            return;
        }
        hit = func_02035ba4(rects);
        if (skill->isSwapping == FALSE) {
            if (hit == TOUCH_RECT_NONE) {
                skill->isDragging = FALSE;
                func_0204b3dc(skill->actors[4], FALSE);
                func_0204b3dc(skill->actors[3], FALSE);
            } else if (MATH_ABS((int)(skill->dragStartY - wk->unk2C)) > 8) {
                func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_HELD);
                skill->isSwapping = TRUE;
                func_0204b3dc(skill->actors[4], FALSE);
            }
        }
        if (skill->isSwapping == TRUE) {
            if (hit != TOUCH_RECT_NONE) {
                if (PML_PkmGetParam(func_ov131_021d6064(wk), PKM_PARAM_MOVE1 + hit, NULL) == MOVE_NONE) {
                    return;
                }
                func_ov131_021db468(wk, skill, hit, FALSE);
                return;
            }
            func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
            func_0204b3dc(skill->actors[4], FALSE);
            func_0204b3dc(skill->actors[3], FALSE);
            skill->isSwapping = FALSE;
            skill->isDragging = FALSE;
            func_ov131_021db468(wk, skill, skill->cursor, FALSE);
            skill->swapSrc = SLOT_NONE;
        }
    } else if (skill->isDragging == TRUE) {
        func_0204b3dc(skill->actors[4], FALSE);
        func_0204b3dc(skill->actors[3], FALSE);
        if (skill->isSwapping == TRUE) {
            if (skill->swapSrc != skill->cursor) {
                func_ov131_021db508(wk, skill, skill->swapSrc, skill->cursor);
                GFL_SndSEPlay(SEQ_SE_DECIDE2);
                return;
            }
            skill->isSwapping = FALSE;
            func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        }
    }
}


BOOL func_ov131_021daee4(PStatusWork *wk, PStaSkillWork *skill) {
    BOOL isHm;
    ClActorPos pos;

    if (wk->unk1C == TRUE && GCTX_HIDGetPressedKeys() != 0) {
        wk->unk1C = FALSE;
        if (skill->isConfirming == FALSE) {
            skill->swapSrc = SLOT_NONE;
            func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        }
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B) {
        if (skill->isConfirming == FALSE) {
            wk->unk1C = FALSE;
            wk->unk54 = PSTATUS_RESULT_BACK;
            wk->param->result = PSTATUS_RESULT_BACK;
            wk->unk50 = 2;
            func_0204b878(wk->unk178[7], 9);
            wk->unk1A0 = wk->unk178[7];
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            return TRUE;
        }
        wk->unk1C = FALSE;
        func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
        func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        func_0204b3dc(skill->actors[3], FALSE);
        skill->cursor = skill->swapSrc;
        skill->swapSrc = SLOT_NONE;
        skill->isConfirming = FALSE;
        func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        func_0204b878(wk->unk178[7], 9);
        func_ov131_021db614(wk, skill, FALSE);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
    } else if (skill->isConfirming) {
        if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
            wk->unk1C = FALSE;
            wk->unk54 = PSTATUS_RESULT_BACK;
            wk->param->slot = skill->swapSrc;
            wk->param->result = PSTATUS_RESULT_FORGET;
            skill->isExiting = TRUE;
            skill->forgetConfirmed = TRUE;
            wk->unk1A0 = NULL;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
    } else if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_A) {
        wk->unk1C = FALSE;
        if (skill->cursor < PLATE_NEW) {
            BoxPkm *pkm = func_ov131_021d6064(wk);

            isHm = isPkmMoveHmMove(wk->param->gameData, PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + skill->cursor, NULL),
                                   wk->heapId);
            if (isHm == FALSE || wk->unk14 == TRUE) {
                if (wk->param->move == MOVE_NONE) {
                    wk->unk54 = PSTATUS_RESULT_BACK;
                    wk->param->result = PSTATUS_RESULT_FORGET;
                    wk->param->slot = skill->cursor;
                    skill->isExiting = TRUE;
                    wk->unk1A0 = NULL;
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                } else {
                    skill->isConfirming = TRUE;
                    skill->swapSrc = skill->cursor;
                    func_ov131_021dba78(&skill->plates[skill->cursor], PLATE_ANIM_HELD);
                    skill->cursor = PLATE_NEW;
                    func_ov131_021db468(wk, skill, skill->cursor, FALSE);
                    func_ov131_021dba54(&skill->plates[skill->swapSrc], &pos);
                    func_0204b404(skill->actors[3], &pos, 0);
                    func_0204b3dc(skill->actors[3], TRUE);
                    func_0204b9a0(skill->actors[3]);
                    func_ov131_021db614(wk, skill, TRUE);
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                }
            } else {
                func_ov131_021db688(wk, skill, isHm);
            }
        } else {
            wk->unk54 = PSTATUS_RESULT_BACK;
            wk->param->result = PSTATUS_RESULT_BACK;
            skill->isExiting = TRUE;
            wk->unk1A0 = NULL;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
        }
        return TRUE;
    } else if (GCTX_HIDGetPressedKeys() == PAD_KEY_UP || GCTX_HIDGetPressedKeys() == PAD_KEY_DOWN) {
        func_ov131_021db3f0(wk, skill);
        return TRUE;
    }
    return FALSE;
}

void func_ov131_021db140(PStatusWork *wk, PStaSkillWork *skill) {
    ClActorPos pos;
    TouchRect rects[PLATE_COUNT + 1];
    u8 i;
    s32 hit;
    BOOL isHm;

    if (wk->touchHit == 7) {
        if (skill->isConfirming == FALSE) {
            wk->unk54 = PSTATUS_RESULT_BACK;
            wk->param->result = PSTATUS_RESULT_BACK;
            wk->unk50 = 2;
            func_0204b878(wk->unk178[7], 9);
            wk->unk1A0 = wk->unk178[7];
            GFL_SndSEPlay(SEQ_SE_CANCEL1);
            return;
        }
        wk->unk1C = TRUE;
        func_ov131_021dba78(&skill->plates[skill->swapSrc], PLATE_ANIM_NORMAL);
        func_ov131_021db468(wk, skill, skill->cursor, FALSE);
        func_0204b3dc(skill->actors[3], FALSE);
        skill->swapSrc = SLOT_NONE;
        skill->isConfirming = FALSE;
        if (skill->moveCount == PLATE_COUNT) {
            func_ov131_021db468(wk, skill, PLATE_NEW, FALSE);
            func_ov131_021d9ee8(wk, skill);
        } else {
            skill->cursor = CURSOR_NONE;
            func_ov131_021db468(wk, skill, CURSOR_NONE, FALSE);
        }
        func_ov131_021db614(wk, skill, FALSE);
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        return;
    }
    for (i = 0; i < PLATE_COUNT; i++) {
        func_ov131_021dba14(&skill->plates[i], &rects[i]);
    }
    rects[PLATE_COUNT].top = TOUCH_RECT_END;
    hit = func_02035c08(rects);
    if (hit == TOUCH_RECT_NONE) {
        return;
    }
    if (skill->isConfirming == FALSE) {
        if (hit < PLATE_NEW) {
            if (PStaSkill_GetMove(wk, hit) == MOVE_NONE) {
                return;
            }
            isHm = isPkmMoveHmMove(wk->param->gameData,
                                   PML_PkmGetParam(func_ov131_021d6064(wk), PKM_PARAM_MOVE1 + hit, NULL), wk->heapId);
            if (isHm == FALSE || wk->unk14 == TRUE) {
                if (wk->param->move == MOVE_NONE) {
                    wk->unk54 = PSTATUS_RESULT_BACK;
                    wk->param->result = PSTATUS_RESULT_FORGET;
                    wk->param->slot = hit;
                    skill->isExiting = TRUE;
                    wk->unk1A0 = NULL;
                    GFL_SndSEPlay(SEQ_SE_DECIDE1);
                    func_ov131_021db468(wk, skill, hit, FALSE);
                    func_ov131_021dba78(&skill->plates[hit], PLATE_ANIM_HELD);
                    return;
                }
                wk->unk1C = TRUE;
                skill->isConfirming = TRUE;
                func_ov131_021db468(wk, skill, hit, FALSE);
                skill->swapSrc = hit;
                func_ov131_021dba78(&skill->plates[hit], PLATE_ANIM_HELD);
                skill->cursor = PLATE_NEW;
                func_ov131_021db468(wk, skill, skill->cursor, FALSE);
                func_ov131_021dba54(&skill->plates[skill->swapSrc], &pos);
                func_0204b404(skill->actors[3], &pos, 0);
                func_0204b3dc(skill->actors[3], TRUE);
                func_0204b9a0(skill->actors[3]);
                func_ov131_021db614(wk, skill, TRUE);
                GFL_SndSEPlay(SEQ_SE_DECIDE1);
                return;
            }
            func_ov131_021db468(wk, skill, hit, FALSE);
            skill->swapSrc = hit;
            func_ov131_021dba78(&skill->plates[hit], PLATE_ANIM_HELD);
            func_ov131_021db688(wk, skill, isHm);
            return;
        }
        func_ov131_021db468(wk, skill, hit, FALSE);
        wk->unk1C = TRUE;
        wk->unk54 = PSTATUS_RESULT_BACK;
        wk->param->result = PSTATUS_RESULT_BACK;
        skill->isExiting = TRUE;
        wk->unk1A0 = NULL;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    } else if (hit == PLATE_NEW) {
        wk->unk1C = TRUE;
        wk->unk54 = PSTATUS_RESULT_BACK;
        wk->param->slot = skill->swapSrc;
        wk->param->result = PSTATUS_RESULT_FORGET;
        skill->isExiting = TRUE;
        skill->forgetConfirmed = TRUE;
        wk->unk1A0 = NULL;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    }
}




// pokebw2's PStaSkill_MoveCursor
void func_ov131_021db3f0(PStatusWork *wk, PStaSkillWork *skill) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    int cursor = skill->cursor;
    int dir;
    u16 move;

    if (GCTX_HIDGetPressedKeys() == PAD_KEY_UP) {
        dir = -1;
    } else {
        dir = 1;
    }
    GFL_SndSEPlay(SEQ_SE_SELECT1);
    do {
        cursor += dir;
        if (cursor < 0) {
            cursor = skill->moveCount - 1;
        } else if (cursor > skill->moveCount - 1) {
            cursor = 0;
        }
        if (cursor < PLATE_NEW) {
            move = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + cursor, NULL);
        } else {
            move = wk->param->move;
        }
    } while (move == MOVE_NONE);
    func_ov131_021db468(wk, skill, cursor, FALSE);
}

void func_ov131_021db468(PStatusWork *wk, PStaSkillWork *skill, u8 slot, BOOL force) {
    ClActorPos pos;

    if (wk->unk1C == 0) {
        func_ov131_021dba54(&skill->plates[slot], &pos);
        func_0204b404(skill->actors[2], &pos, 0);
        func_0204b3dc(skill->actors[2], 0);
    } else {
        func_0204b3dc(skill->actors[2], 0);
    }
    if (skill->cursor != skill->swapSrc || skill->isSwapping == FALSE) {
        if (skill->cursor != CURSOR_NONE) {
            func_ov131_021dba78(&skill->plates[skill->cursor], 0);
        }
    } else {
        func_ov131_021dba78(&skill->plates[skill->cursor], 2);
    }
    func_ov131_021dba78(&skill->plates[slot], 1);
    if (skill->cursor != slot || force == TRUE) {
        skill->cursor = slot;
        func_ov131_021d9ee8(wk, skill);
    }
}


// pokebw2's PStaSkill_SwapMoves
void func_ov131_021db508(PStatusWork *wk, PStaSkillWork *skill, u8 slotA, u8 slotB) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u32 moveA;
    u32 ppA;
    u32 ppUpA;
    u32 moveB;
    u32 ppB;
    u32 ppUpB;

    func_ov131_021d60d4(wk, TRUE);
    moveA = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + slotA, NULL);
    ppA = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_PP + slotA, NULL);
    ppUpA = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_PP_UP + slotA, NULL);
    moveB = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + slotB, NULL);
    ppB = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_PP + slotB, NULL);
    ppUpB = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_PP_UP + slotB, NULL);
    PML_PkmSetMove(pkm, moveB, slotA);
    PML_PkmSetMove(pkm, moveA, slotB);
    PML_PkmSetParam(pkm, PKM_PARAM_MOVE1_PP + slotA, ppB);
    PML_PkmSetParam(pkm, PKM_PARAM_MOVE1_PP_UP + slotA, ppUpB);
    PML_PkmSetParam(pkm, PKM_PARAM_MOVE1_PP + slotB, ppA);
    PML_PkmSetParam(pkm, PKM_PARAM_MOVE1_PP_UP + slotB, ppUpA);
    func_ov131_021d60d4(wk, FALSE);
    func_ov131_021dbe98(skill->plates[slotA].oam, skill->plates[slotB].oam);
    skill->plates[slotA].isDirty = TRUE;
    skill->plates[slotB].isDirty = TRUE;
    func_ov131_021d9ee8(wk, skill);
    skill->isSwapPending = TRUE;
}

// pokebw2's PStaSkill_ShowForgetPrompt
void func_ov131_021db614(PStatusWork *wk, PStaSkillWork *skill, BOOL show) {
    SkillPlate *plate = &skill->plates[PLATE_NEW];

    if (show == TRUE) {
        func_0204b878(plate->plate, 16);
        func_ov131_021dbca4(plate->oam, FALSE);
        func_0204b3dc(plate->typeIcon, FALSE);
        func_ov131_021dbcf4(skill->forgetOam);
        func_ov131_021dbca4(skill->forgetOam, TRUE);
    } else {
        func_ov131_021dbca4(skill->forgetOam, FALSE);
        func_0204b3dc(plate->typeIcon, TRUE);
        if (skill->cursor == PLATE_NEW) {
            func_ov131_021dba78(plate, PLATE_ANIM_SELECTED);
        } else {
            func_ov131_021dba78(plate, PLATE_ANIM_NORMAL);
        }
        func_ov131_021dbca4(plate->oam, TRUE);
    }
}

// pokebw2's PStaSkill_ShowHmMessage
void func_ov131_021db688(PStatusWork *wk, PStaSkillWork *skill, BOOL isHm) {
    skill->redrawMessage = TRUE;
    GFL_BitmapFill(BmpWin_GetBitmap(skill->windows[12]), 0);
    if (isHm == TRUE) {
        func_ov131_021d618c(wk, skill->windows[12], 0x9c, 0, 0, PRINT_COLOR(15, 2, 0));
    }
    GFL_SndSEPlay(SEQ_SE_BEEP);
}

// pokebw2's PStaSkill_GetStatColor
u16 func_ov131_021db6d0(PStatusWork *wk, PStaSkillWork *skill, u8 nature, u32 stat) {
    switch (statAffectedByNature(nature, stat)) {
    case 1:
        return PRINT_COLOR(15, 9, 0);
    case -1:
        return PRINT_COLOR(15, 10, 0);
    case 0:
    default:
        return PRINT_COLOR(15, 2, 0);
    }
}

void func_ov131_021db700(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate, u8 slot) {
    ClActorSetup setup;

    plate->isDirty = FALSE;
    plate->slot = slot;
    setup.x = data_ov131_021dc2b8[slot].x * 8;
    setup.y = data_ov131_021dc2b8[slot].y * 8;
    setup.priority = 10;
    setup.bgPriority = 0;
    setup.sequence = plate->slot;
    plate->plate = func_0204b294(wk->actorUnit, wk->unk98[20], wk->unk98[6], wk->unk98[32], &setup, 0, wk->heapId);
    func_0204b3dc(plate->plate, FALSE);
    setup.x = data_ov131_021dc2b8[plate->slot].x * 8 + 26;
    setup.y = data_ov131_021dc2b8[plate->slot].y * 8 + 9;
    setup.priority = 8;
    setup.bgPriority = 0;
    setup.sequence = 0;
    plate->typeIcon = func_0204b294(wk->actorUnit, wk->typeIconChars[0], wk->unk98[3], wk->unk98[29], &setup, 0, wk->heapId);
    func_0204b3dc(plate->typeIcon, FALSE);
}


// pokebw2's PStaSkill_FreePlate
void func_ov131_021db7b8(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
    func_0204b3b4(plate->plate);
    func_0204b3b4(plate->typeIcon);
    plate->slot = CURSOR_NONE;
}

// pokebw2's PStaSkill_UpdatePlate
void func_ov131_021db7d0(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
}

void func_ov131_021db7d4(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u32 move;
    u32 pp;
    u32 maxPp;
    MsgData *msgData;
    StrBuf *str;
    WordSet *ppWordSet;
    WordSet *maxPpWordSet;
    struct PStaOamSetup setup;

    if (plate->slot < PLATE_NEW) {
        move = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + plate->slot, NULL);
        pp = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_PP + plate->slot, NULL);
        maxPp = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1_MAX_PP + plate->slot, NULL);
    } else {
        move = wk->param->move;
        maxPp = pp = PML_MoveGetParam(move, MOVE_PARAM_PP);
    }
    plate->bitmap = GFL_BitmapCreate(11, 4, 32, wk->heapId);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_MOVE_NAMES, wk->heapId);
    str = GFL_MsgDataLoadStrbufNew(msgData, move);
    func_0201c724(wk->printQueue, plate->bitmap, 3, 2, str, wk->font, PRINT_COLOR(1, 2, 0));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    if (move != MOVE_NONE) {
        func_ov131_021d614c(wk, plate->bitmap, 0x87, 13, 17, PRINT_COLOR(1, 2, 0));
        ppWordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(ppWordSet, 0, pp, 3, 1, 1);
        func_ov131_021d6244(wk, plate->bitmap, ppWordSet, 0x88, 57, 17, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(ppWordSet);
        func_ov131_021d614c(wk, plate->bitmap, 0x75, 57, 17, PRINT_COLOR(1, 2, 0));
        maxPpWordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(maxPpWordSet, 0, maxPp, 3, 1, 1);
        func_ov131_021d61b8(wk, plate->bitmap, maxPpWordSet, 0x8d, 61, 17, PRINT_COLOR(1, 2, 0));
        GFL_WordSetSystemFree(maxPpWordSet);
    } else {
        func_ov131_021d614c(wk, plate->bitmap, 0x99, 49, 17, PRINT_COLOR(1, 2, 0));
    }
    setup.x = (data_ov131_021dc2b8[plate->slot].x + 5) * 8;
    setup.y = data_ov131_021dc2b8[plate->slot].y * 8;
    setup.palette = wk->unk98[8];
    setup.paletteOffset = 0;
    setup.priority = 6;
    setup.bgPriority = 0;
    setup.surface = 0;
    setup.vramType = 0;
    setup.bitmap = plate->bitmap;
    plate->oam = func_ov131_021dbad0(skill->oam, &setup);
    plate->isDirty = TRUE;
}


void func_ov131_021db964(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
    BoxPkm *pkm = func_ov131_021d6064(wk);
    u32 move;
    u8 type;
    NNSG2dImageProxy proxy;

    if (plate->slot < PLATE_NEW) {
        move = PML_PkmGetParam(pkm, PKM_PARAM_MOVE1 + plate->slot, NULL);
    } else {
        move = wk->param->move;
    }
    if (plate->isDirty == TRUE) {
        if (move != MOVE_NONE) {
            type = PML_MoveGetType(move);
            func_0204ab84(wk->typeIconChars[type], &proxy);
            func_0204b784(plate->typeIcon, &proxy);
            func_0204b6f4(plate->typeIcon, func_020275ec(type), 1);
            func_0204b3dc(plate->typeIcon, TRUE);
        } else {
            func_0204b3dc(plate->typeIcon, FALSE);
        }
        func_ov131_021dbcf4(plate->oam);
        func_0204b3dc(plate->plate, TRUE);
        plate->isDirty = FALSE;
    }
}


// pokebw2's PStaSkill_UnloadPlate
void func_ov131_021db9e8(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
    GFL_BitmapFree(plate->bitmap);
    func_ov131_021dbc3c(plate->oam);
}

// pokebw2's PStaSkill_HidePlate
void func_ov131_021db9fc(PStatusWork *wk, PStaSkillWork *skill, SkillPlate *plate) {
    func_0204b3dc(plate->plate, FALSE);
    func_0204b3dc(plate->typeIcon, FALSE);
}

// pokebw2's PStaSkill_GetPlateRect
void func_ov131_021dba14(SkillPlate *plate, TouchRect *rect) {
    if (plate->slot < PLATE_COUNT) {
        rect->top = data_ov131_021dc2b8[plate->slot].y * 8;
        rect->bottom = rect->top + 32;
        rect->left = data_ov131_021dc2b8[plate->slot].x * 8;
        rect->right = rect->left + 136;
    } else {
        rect->top = 0;
        rect->bottom = 0;
        rect->left = 0;
        rect->right = 0;
    }
}

// pokebw2's PStaSkill_GetPlatePos
void func_ov131_021dba54(SkillPlate *plate, ClActorPos *pos) {
    pos->x = data_ov131_021dc2b8[plate->slot].x * 8;
    pos->y = data_ov131_021dc2b8[plate->slot].y * 8;
}

// pokebw2's PStaSkill_SetPlateAnim
void func_ov131_021dba78(SkillPlate *plate, u32 anim) {
    func_0204b878(plate->plate, plate->slot + anim * PLATE_COUNT);
}
