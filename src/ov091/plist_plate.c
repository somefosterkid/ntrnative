// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_plate.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "battle/regulation.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "field/musical.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "pml/species_names.h"
#include "system/hp_gauge.h"
#include "system/printsys.h"
#include "system/wordset.h"

ClActor *func_0204c040(ClActUnit *unit, u32 chars, u32 palette, u32 cellAnims, const ClActorSetup *setup, u16 surface, HeapID heapId);

// A Pokémon's plate in the party list: its name, level, HP, icon, ball, item and status

// Black's layout (0x44 bytes): the file's matching functions reach every field at its offset
struct PokeListPlate {
    PartyPkm *pkm;
    u8 index;
    // The HP shown, which moves toward targetHp
    u16 hp;
    u16 targetHp;
    BOOL isEgg;
    BOOL selected;
    BOOL isEmpty;
    // Whether the window waits for its text to be printed, to be copied to the BG
    BOOL nameDirty;
    // The Pokémon's place in the battle's order from 0, POKELIST_ENTRY_* otherwise
    int entry;
    BmpWin *window;
    ClActRenderer *renderer;
    ClActUnit *unit;
    u32 iconChars;
    ClActor *icon;
    ClActor *ball;
    ClActor *hpBar;
    ClActor *itemIcon;
    ClActor *statusIcon;
};

static void func_ov091_021baae4(PokeListWork *wk, PokeListPlate *plate, BOOL numbersOnly);
void func_ov091_021bab04(PokeListWork *wk, PokeListPlate *plate, BOOL numbersOnly);
static void func_ov091_021bae94(PokeListWork *wk, PokeListPlate *plate);
static void func_ov091_021baf28(PokeListWork *wk, PokeListPlate *plate);
void func_ov091_021bb050(PokeListWork *wk, PokeListPlate *plate, u32 palette);
static void func_ov091_021bb2e0(PokeListWork *wk, PokeListPlate *plate);
static void func_ov091_021bb46c(PokeListWork *wk, PokeListPlate *plate);
static u8 func_ov091_021bb554(PokeListPlate *plate);
static void func_ov091_021bb56c(PokeListPlate *plate, int x, int y, ClActorPos *pos);

// Where the plate's screen is in the plates' screen file: the first plate's, then the others'
// pokebw2's sPlateScreenPos
static const u8 data_ov091_021bec6c[2][2] = {
    { 0, 1 },
    { 16, 2 },
};

// The tile that each plate's top left corner is at
static const u8 sPlatePos[POKELIST_PLATE_COUNT][2] = {
    { 0, 1 }, { 16, 2 }, { 0, 7 }, { 16, 8 }, { 0, 13 }, { 16, 14 },
};

u32 func_02019da4(Regulation *regulation, PartyPkm *pkm);
void func_ov091_021baa50(PokeListWork *wk, PokeListPlate *plate);

void func_ov091_021ba954(PokeListWork *wk, PokeListPlate *plate);

PokeListPlate *func_ov091_021ba6a0(PokeListWork *wk, u8 index, PartyPkm *pkm) {
    u8 i;
    PokeListPlate *plate;
    ClActSurfaceSetup setup;
    ClActorPos pos;
    u8 x = sPlatePos[index][0];
    u8 y = sPlatePos[index][1];

    plate = GFL_HeapAllocate(wk->heapId, sizeof(PokeListPlate), FALSE, "plist_plate.c", 159);
    plate->index = index;
    plate->pkm = pkm;
    plate->isEmpty = FALSE;
    plate->selected = FALSE;
    plate->nameDirty = FALSE;
    plate->hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    plate->targetHp = plate->hp;
    GFL_BGSysLoadScrArea(2, x + 16, y, 16, 6, wk->plateScreen->rawData,
                         data_ov091_021bec6c[index != 0 ? 1 : 0][0], data_ov091_021bec6c[index != 0 ? 1 : 0][1],
                         32, 32);
    plate->isEgg = PokeParty_GetParam(plate->pkm, PKM_PARAM_IS_EGG, NULL);
    setup.x = 0;
    setup.y = 0;
    setup.w = 256;
    setup.h = 192;
    setup.screen = 0;
    setup.culling = 0;
    plate->renderer = func_0204b034(&setup, 1, wk->heapId);
    plate->unit = func_0204b100(6, 0, wk->heapId);
    func_0204b258(plate->unit, plate->renderer);
    func_ov091_021ba954(wk, plate);
    func_ov091_021baa50(wk, plate);
    pos.x = -x * 8;
    pos.y = -y * 8;
    func_0204b09c(plate->renderer, 0, &pos);
    plate->window = BmpWin_CreateDynamic(1, x + 16, y, 16, 6, 13, TRUE);
    func_ov091_021bb46c(wk, plate);
    if (func_ov091_021b9a84(wk) == TRUE) {
        for (i = 0; i < 6; i++) {
            if (plate->index + 1 == wk->param->picked[i]) {
                plate->entry = i;
            }
        }
    }
    func_ov091_021bafa0(wk, plate, 0);
    func_ov091_021baae4(wk, plate, FALSE);
    return plate;
}

PokeListPlate *func_ov091_021ba81c(PokeListWork *wk, u8 index) {
    PokeListPlate *plate = GFL_HeapAllocate(wk->heapId, sizeof(PokeListPlate), FALSE, "plist_plate.c", 234);
    u8 x = sPlatePos[index][0];
    u8 y = sPlatePos[index][1];

    GFL_BGSysLoadScrArea(2, x + 16, y, 16, 6, wk->plateScreen->rawData, 0, 19, 32, 32);
    plate->index = index;
    plate->pkm = NULL;
    plate->isEmpty = TRUE;
    plate->entry = 8;
    GFL_BGSysQueueScrLoad(2);
    return plate;
}

void func_ov091_021ba890(PokeListWork *wk, PokeListPlate *plate) {
    if (plate->isEmpty == FALSE) {
        BmpWin_Free(plate->window);
        func_0204b3b4(plate->icon);
        func_0204b3b4(plate->ball);
        func_0204b3b4(plate->hpBar);
        func_0204b084(plate->renderer);
        func_0204b1cc(plate->unit);
        func_0204a8d4(plate->iconChars);
    }
    GFL_HeapFree(plate);
}

// pokebw2's PokeListPlate_Draw


// Black 2's PokeListPlate_Update, whose icon hops by a timer of the plate's: Black's by its animation's frame
void func_ov091_021ba8cc(PokeListWork *wk, PokeListPlate *plate) {
    if (plate->isEmpty == FALSE) {
        if (plate->nameDirty == TRUE && func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(plate->window)) == FALSE) {
            plate->nameDirty = FALSE;
            BmpWin_Transfer(plate->window);
        }
        if (plate->selected == TRUE && GetStatusCond(plate->pkm) == 0 && plate->hp != 0) {
            ClActorPos pos;

            func_ov091_021bb56c(plate, 26, 19, &pos);
            if (func_0204b8f4(plate->icon) == 1) {
                pos.y -= 6;
            }
            func_0204b404(plate->icon, &pos, 0);
        }
    }
}

void func_ov091_021ba954(PokeListWork *wk, PokeListPlate *plate) {
    ClActorSetup ballSetup;
    ClActor *actor;
    ClActorSetup hpBarSetup;
    ClActorSetup itemSetup;
    ClActorSetup statusSetup;

    ballSetup.x = 16;
    ballSetup.y = 16;
    ballSetup.sequence = 0;
    ballSetup.priority = 1;
    ballSetup.bgPriority = 2;
    actor = func_0204b294(plate->unit, wk->clResources[CL_RES_CHAR(1)], wk->clResources[CL_RES_PLTT(0)],
                          wk->clResources[CL_RES_CELL(1)], &ballSetup, 0, wk->heapId);
    plate->ball = actor;
    func_0204b92c(actor, TRUE);
    hpBarSetup.x = 84;
    hpBarSetup.y = 28;
    hpBarSetup.sequence = 0;
    hpBarSetup.priority = 0;
    hpBarSetup.bgPriority = 2;
    actor = func_0204b294(plate->unit, wk->clResources[CL_RES_CHAR(6)], wk->clResources[CL_RES_PLTT(5)],
                          wk->clResources[CL_RES_CELL(6)], &hpBarSetup, 0, wk->heapId);
    plate->hpBar = actor;
    itemSetup.x = 32;
    itemSetup.y = 24;
    itemSetup.sequence = 0;
    itemSetup.priority = 0;
    itemSetup.bgPriority = 2;
    actor = func_0204b294(plate->unit, wk->clResources[CL_RES_CHAR(3)], wk->clResources[CL_RES_PLTT(2)],
                          wk->clResources[CL_RES_CELL(3)], &itemSetup, 0, wk->heapId);
    plate->itemIcon = actor;
    func_0204b3dc(actor, FALSE);
    statusSetup.x = 32;
    statusSetup.y = 42;
    statusSetup.sequence = 0;
    statusSetup.priority = 0;
    statusSetup.bgPriority = 2;
    actor = func_0204b294(plate->unit, wk->clResources[CL_RES_CHAR(5)], wk->clResources[CL_RES_PLTT(4)],
                          wk->clResources[CL_RES_CELL(5)], &statusSetup, 0, wk->heapId);
    plate->statusIcon = actor;
    func_0204b3dc(actor, FALSE);
}

void func_ov091_021baa50(PokeListWork *wk, PokeListPlate *plate) {
    ArcTool *arc;
    PartyPkm *pkm;
    BoxPkm *boxPkm;
    u32 fileId;
    ClActor *actor;
    ClActorSetup setup;

    arc = GFL_ArcSysCreateFileHandle(7, wk->heapId);
    pkm = PokeParty_GetPkm(wk->param->party, plate->index);
    boxPkm = func_0201d624(pkm);
    fileId = func_0201ba68(boxPkm);
    fileId = func_0204a6c8(arc, fileId, 0, 0, wk->heapId);
    plate->iconChars = fileId;
    GFL_ArcToolFree(arc);
    fileId = func_0201bbe8(boxPkm);
    setup.x = 24;
    setup.y = 16;
    setup.sequence = 1;
    setup.priority = 0;
    setup.bgPriority = 2;
    actor = func_0204b294(plate->unit, plate->iconChars, wk->clResources[CL_RES_PLTT(6)],
                          wk->clResources[CL_RES_CELL(7)], &setup, 0, wk->heapId);
    plate->icon = actor;
    func_0204b6f4(actor, fileId, 1);
    func_0204b92c(plate->icon, TRUE);
}

static void func_ov091_021baae4(PokeListWork *wk, PokeListPlate *plate, BOOL numbersOnly) {
    if (plate->isEgg == FALSE) {
        func_ov091_021bab04(wk, plate, numbersOnly);
        func_ov091_021bae94(wk, plate);
    } else {
        func_ov091_021baf28(wk, plate);
    }
}

void func_ov091_021bab04(PokeListWork *wk, PokeListPlate *plate, BOOL numbersOnly) {
    BOOL showLearn = FALSE;
    u32 item;

    if (wk->param->mode == 6 || wk->wasMode18 == TRUE) {
        showLearn = TRUE;
    }
    if (numbersOnly == FALSE) {
        WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);

        loadPokemonNicknameToStrbuf(wordSet, 0, plate->pkm);
        func_ov091_021b9cf4(wk, plate->window, wordSet, 3, 41, 9, 0x440);
        GFL_WordSetSystemFree(wordSet);
    }
    if (numbersOnly == FALSE && PokeParty_GetParam(plate->pkm, PKM_PARAM_SHOW_SEX, NULL) == TRUE) {
        u32 sex = PokeParty_GetParam(plate->pkm, PKM_PARAM_SEX, NULL);

        if (sex == GENDER_MALE) {
            func_ov091_021b9c1c(wk, plate->window, 7, 105, 9, 0x14c0);
        } else if (sex == GENDER_FEMALE) {
            func_ov091_021b9c1c(wk, plate->window, 8, 105, 9, 0xc80);
        }
    }
    if (showLearn == FALSE) {
        if ((GetStatusCond(plate->pkm) == 0 && plate->hp != 0) || plate->entry != POKELIST_ENTRY_EMPTY) {
            WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);

            WordSetNumber(wordSet, 0, PokeParty_GetLevel(plate->pkm), 3, 0, TRUE);
            if (numbersOnly == FALSE) {
                func_ov091_021b9d58(wk, plate->window, wordSet, 2, 9, 35, 0x440);
            } else {
                func_ov091_021b9dc0(wk, plate->window, wordSet, 2, 9, 35, 0x440);
            }
            GFL_WordSetSystemFree(wordSet);
            func_0204b3dc(plate->statusIcon, FALSE);
        } else {
            if (plate->hp == 0) {
                func_0204b878(plate->statusIcon, 6);
            } else {
                switch (GetStatusCond(plate->pkm)) {
                case 1:
                    func_0204b878(plate->statusIcon, 1);
                    break;
                case 2:
                    func_0204b878(plate->statusIcon, 3);
                    break;
                case 3:
                    func_0204b878(plate->statusIcon, 2);
                    break;
                case 4:
                    func_0204b878(plate->statusIcon, 5);
                    break;
                case 5:
                    func_0204b878(plate->statusIcon, 4);
                    break;
                }
            }
            func_0204b3dc(plate->statusIcon, TRUE);
        }
    }
    if (plate->entry != POKELIST_ENTRY_EMPTY) {
        u32 msgId;

        if (plate->entry == POKELIST_ENTRY_ABLE) {
            msgId = 0x81;
        } else if (plate->entry == POKELIST_ENTRY_UNABLE) {
            msgId = 0x82;
        } else {
            msgId = plate->entry + 0x83;
        }
        func_ov091_021b9c1c(wk, plate->window, msgId, 46, 31, 0x440);
        func_0204b3dc(plate->hpBar, FALSE);
    } else if (showLearn == TRUE) {
        u32 msgId;

        switch (func_ov091_021b9ac8(wk, plate->pkm, plate->index)) {
        case 0:
        case 1:
            msgId = 0x89;
            break;
        case 2:
            msgId = 0x8a;
            break;
        case 3:
            msgId = 0x8b;
            break;
        }
        func_ov091_021b9c1c(wk, plate->window, msgId, 29, 31, 0x440);
        func_0204b3dc(plate->hpBar, FALSE);
    } else if (wk->param->mode == 16) {
        u32 msgId = !func_ov091_021b9bf4(wk, plate->pkm, wk->param->item) ? 0x8d : 0x8c;

        func_ov091_021b9c1c(wk, plate->window, msgId, 46, 31, 0x440);
        func_0204b3dc(plate->hpBar, FALSE);
    } else {
        WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);

        WordSetNumber(wordSet, 0, PokeParty_GetParam(plate->pkm, PKM_PARAM_MAX_HP, NULL), 3, 0, TRUE);
        if (numbersOnly == FALSE) {
            func_ov091_021b9d58(wk, plate->window, wordSet, 4, 91, 35, 0x440);
        } else {
            func_ov091_021b9dc0(wk, plate->window, wordSet, 4, 91, 35, 0x440);
        }
        GFL_WordSetSystemFree(wordSet);
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        WordSetNumber(wordSet, 0, plate->hp, 3, 1, TRUE);
        if (numbersOnly == FALSE) {
            func_ov091_021b9d58(wk, plate->window, wordSet, 5, 57, 35, 0x440);
        } else {
            func_ov091_021b9dc0(wk, plate->window, wordSet, 5, 57, 35, 0x440);
        }
        GFL_WordSetSystemFree(wordSet);
        if (numbersOnly == FALSE) {
            func_ov091_021b9c64(wk, plate->window, 6, 82, 35, 0x440);
        } else {
            func_ov091_021b9cb0(wk, plate->window, 6, 82, 35, 0x440);
        }
    }
    item = PokeParty_GetParam(plate->pkm, PKM_PARAM_ITEM, NULL);
    if (item != 0) {
        func_0204b3dc(plate->itemIcon, TRUE);
        if (PML_ItemIsMail(item) == TRUE) {
            func_0204b878(plate->itemIcon, 1);
        } else {
            func_0204b878(plate->itemIcon, 0);
        }
    } else {
        func_0204b3dc(plate->itemIcon, FALSE);
    }
    {
        u8 color = func_ov091_021bb554(plate);
        u16 sequence = 1;

        if (color == HP_GAUGE_COLOR_NONE) {
            sequence = 0;
        } else if (GetStatusCond(plate->pkm) != 0) {
            sequence = 5;
        } else if (color == HP_GAUGE_COLOR_RED) {
            sequence = 4;
        } else if (color == HP_GAUGE_COLOR_YELLOW) {
            sequence = 3;
        } else if (color == HP_GAUGE_COLOR_GREEN) {
            sequence = 2;
        }
        func_0204b878(plate->icon, sequence);
    }
    plate->nameDirty = TRUE;
}

// pokebw2's PokeListPlate_DrawHpBar
static void func_ov091_021bae94(PokeListWork *wk, PokeListPlate *plate) {
    if (func_ov091_021b9a84(wk) == FALSE && wk->param->mode != 6) {
        u8 color = func_ov091_021bb554(plate);
        u8 width = func_0202d01c(plate->hp, PokeParty_GetParam(plate->pkm, PKM_PARAM_MAX_HP, NULL), 48);
        GFLBitmap *bitmap = BmpWin_GetBitmap(plate->window);
        u8 topColor;
        u8 bottomColor;

        if (color == HP_GAUGE_COLOR_RED || color == HP_GAUGE_COLOR_NONE) {
            topColor = 14;
            bottomColor = 15;
        } else if (color == HP_GAUGE_COLOR_YELLOW) {
            topColor = 12;
            bottomColor = 13;
        } else {
            topColor = 10;
            bottomColor = 11;
        }
        GFL_BitmapFillArea(bitmap, 61, 27, width, 1, topColor);
        GFL_BitmapFillArea(bitmap, 61, 28, width, 1, bottomColor);
        func_0204b3dc(plate->hpBar, TRUE);
    }
}

// pokebw2's PokeListPlate_DrawEgg
static void func_ov091_021baf28(PokeListWork *wk, PokeListPlate *plate) {
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    StrBuf *name = GFL_MsgDataLoadStrbufNew(g_PMLSpeciesNamesResident, SPECIES_EGG);

    func_0201ee0c(wordSet, 0, name, 0, 1, 2);
    func_ov091_021b9cf4(wk, plate->window, wordSet, 3, 41, 9, 0x440);
    GFL_StrBufFree(name);
    GFL_WordSetSystemFree(wordSet);
    func_0204b3dc(plate->hpBar, FALSE);
    func_0204b878(plate->icon, 1);
    plate->nameDirty = TRUE;
}

// pokebw2's PokeListPlate_SetSelected
void func_ov091_021bafa0(PokeListWork *wk, PokeListPlate *plate, BOOL selected) {
    if (selected == TRUE) {
        ClActorPos pos;

        if ((u8)(wk->state + 0xfd) <= 1) {
            func_ov091_021bb050(wk, plate, 8);
        } else if (func_ov091_021b9a84(wk) == TRUE && plate->entry < POKELIST_ENTRY_ABLE) {
            func_ov091_021bb050(wk, plate, 8);
        } else {
            func_ov091_021bb050(wk, plate, 6);
        }
        func_0204b878(plate->ball, 1);
        func_ov091_021bb56c(plate, 26, 19, &pos);
        func_0204b404(plate->icon, &pos, 0);
    } else {
        ClActorPos pos;

        if ((wk->state == 3 && wk->selectPos == plate->index) || (wk->state == 4 && wk->selectPos2 == plate->index)) {
            func_ov091_021bb050(wk, plate, 5);
        } else if (func_ov091_021b9a84(wk) == TRUE && plate->entry < POKELIST_ENTRY_ABLE) {
            func_ov091_021bb050(wk, plate, 5);
        } else {
            func_ov091_021bb050(wk, plate, 3);
        }
        func_0204b878(plate->ball, 0);
        func_ov091_021bb56c(plate, 24, 16, &pos);
        func_0204b404(plate->icon, &pos, 0);
    }
    plate->selected = selected;
}

// The palette of a plate: the fainted ones' palette follows each of 3 and 6
// pokebw2's PokeListPlate_SetPalette
void func_ov091_021bb050(PokeListWork *wk, PokeListPlate *plate, u32 palette) {
    if (palette == 3 && func_ov091_021bb554(plate) == HP_GAUGE_COLOR_NONE) {
        GFL_BGSysSetScrPaletteNo(2, sPlatePos[plate->index][0] + 16, sPlatePos[plate->index][1], 16, 6, 4);
    } else if (palette == 6 && func_ov091_021bb554(plate) == HP_GAUGE_COLOR_NONE) {
        GFL_BGSysSetScrPaletteNo(2, sPlatePos[plate->index][0] + 16, sPlatePos[plate->index][1], 16, 6, 7);
    } else {
        GFL_BGSysSetScrPaletteNo(2, sPlatePos[plate->index][0] + 16, sPlatePos[plate->index][1], 16, 6, palette);
    }
    GFL_BGSysQueueScrLoad(2);
}

// Draws the plate offset by a step of its slide, to the left for the left column and to the right for the right
// pokebw2's PokeListPlate_DrawSlid
void func_ov091_021bb0c4(PokeListWork *wk, PokeListPlate *plate, int step) {
    u8 x = sPlatePos[plate->index][0];
    u8 y = sPlatePos[plate->index][1];
    ClActorPos pos;

    if (plate->index % 2 == 0) {
        step = -step;
    }
    func_02045780(plate->window, step + (x + 16));
    pos.x = -(x + step) * 8;
    pos.y = -y * 8;
    func_0204b09c(plate->renderer, 0, &pos);
    GFL_BGSysLoadScrArea(2, x + 16 + step, y, 16, 6, wk->plateScreen->rawData,
                         data_ov091_021bec6c[plate->index != 0 ? 1 : 0][0], data_ov091_021bec6c[plate->index != 0 ? 1 : 0][1],
                         32, 32);
    GFL_BGSysSetScrPaletteNo(2, x + 16 + step, y, 16, 6, 5);
    BmpWin_FlushMap(plate->window);
    GFL_BGSysQueueScrLoad(1);
    GFL_BGSysQueueScrLoad(2);
}

// Clears where the plate is drawn at a step of its slide
// pokebw2's PokeListPlate_ClearSlid
void func_ov091_021bb1a0(PokeListWork *wk, PokeListPlate *plate, int step) {
    u8 x = sPlatePos[plate->index][0];
    u8 y = sPlatePos[plate->index][1];
    u32 left;

    if (plate->index % 2 == 0) {
        step = -step;
    }
    left = x + 16 + step;
    GFL_BGSysFillScrArea(2, 0, left, y, 16, 6, 5);
    GFL_BGSysFillScrArea(1, 0, left, y, 16, 6, 5);
}

// Shows another Pokémon on the plate, as swapping two does
// pokebw2's PokeListPlate_SetPkm
void func_ov091_021bb200(PokeListWork *wk, PokeListPlate *plate, PartyPkm *pkm, int step) {
    u8 x = sPlatePos[plate->index][0];
    u8 y = sPlatePos[plate->index][1];
    ClActorPos origin;
    ClActorPos pos;

    if (plate->index % 2 == 0) {
        step = -step;
    }
    plate->pkm = pkm;
    plate->nameDirty = FALSE;
    plate->isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    plate->hp = PokeParty_GetParam(plate->pkm, PKM_PARAM_HP, NULL);
    plate->targetHp = plate->hp;
    func_0204b3b4(plate->icon);
    func_0204a8d4(plate->iconChars);
    GFL_BitmapFill(BmpWin_GetBitmap(plate->window), 0);
    func_0204b3dc(plate->itemIcon, FALSE);
    func_0204b3dc(plate->statusIcon, FALSE);
    origin.x = 0;
    origin.y = 0;
    func_0204b09c(plate->renderer, 0, &origin);
    func_ov091_021baa50(wk, plate);
    pos.x = -(x + step) * 8;
    pos.y = -y * 8;
    func_0204b09c(plate->renderer, 0, &pos);
    func_ov091_021baae4(wk, plate, FALSE);
}

// pokebw2's PokeListPlate_Redraw
void func_ov091_021bb2c0(PokeListWork *wk, PokeListPlate *plate) {
    GFL_BitmapFill(BmpWin_GetBitmap(plate->window), 0);
    func_ov091_021baae4(wk, plate, FALSE);
}

// pokebw2's PokeListPlate_RedrawNumbers
static void func_ov091_021bb2e0(PokeListWork *wk, PokeListPlate *plate) {
    GFL_BitmapFillArea(BmpWin_GetBitmap(plate->window), 0, 26, 128, 22, 0);
    func_ov091_021baae4(wk, plate, TRUE);
}

// Starts moving the HP shown toward the Pokémon's HP, by a hundredth of the change each frame
// pokebw2's PokeListPlate_StartHpChange
void func_ov091_021bb30c(PokeListWork *wk, PokeListPlate *plate) {
    plate->targetHp = PokeParty_GetParam(plate->pkm, PKM_PARAM_HP, NULL);
    wk->hpStep = (plate->targetHp - plate->hp) / 100;
    if (plate->targetHp > plate->hp) {
        wk->hpStep++;
    } else {
        wk->hpStep--;
    }
}

// Moves the HP shown, and returns TRUE once it is the Pokémon's
// pokebw2's PokeListPlate_UpdateHpChange
BOOL func_ov091_021bb358(PokeListWork *wk, PokeListPlate *plate) {
    if (plate->hp < plate->targetHp) {
        if (plate->hp == 0) {
            func_ov091_021bb050(wk, plate, 6);
        }
        if (plate->hp + wk->hpStep > plate->targetHp) {
            plate->hp = plate->targetHp;
        } else {
            plate->hp += wk->hpStep;
        }
        func_ov091_021bb2e0(wk, plate);
        return FALSE;
    }
    if (plate->hp > plate->targetHp) {
        if (plate->hp + wk->hpStep < plate->targetHp) {
            plate->hp = plate->targetHp;
        } else {
            plate->hp += wk->hpStep;
        }
        func_ov091_021bb2e0(wk, plate);
        return FALSE;
    }
    return TRUE;
}

// pokebw2's PokeListPlate_IsValid
BOOL func_ov091_021bb3c4(PokeListWork *wk, PokeListPlate *plate) {
    if (plate->isEmpty == TRUE) {
        return FALSE;
    }
    return TRUE;
}

// Where the cursor is shown on the plate
// pokebw2's PokeListPlate_GetCursorPos
void func_ov091_021bb3d4(PokeListWork *wk, PokeListPlate *plate, ClActorPos *pos) {
    pos->x = (sPlatePos[plate->index][0] + 8) * 8;
    pos->y = (sPlatePos[plate->index][1] + 3) * 8;
}

// pokebw2's PokeListPlate_GetTouchRect
void func_ov091_021bb3fc(PokeListWork *wk, PokeListPlate *plate, TouchRect *rect) {
    rect->left = sPlatePos[plate->index][0] * 8;
    rect->top = sPlatePos[plate->index][1] * 8;
    rect->right = (sPlatePos[plate->index][0] + 16) * 8;
    rect->bottom = (sPlatePos[plate->index][1] + 6) * 8;
}

// pokebw2's PokeListPlate_GetEntry
int func_ov091_021bb43c(PokeListPlate *plate) {
    return plate->entry;
}

// pokebw2's PokeListPlate_SetEntry
void func_ov091_021bb440(PokeListWork *wk, PokeListPlate *plate, int entry) {
    if (plate->entry != entry) {
        if (entry < POKELIST_ENTRY_ABLE) {
            plate->entry = entry;
        } else {
            func_ov091_021bb46c(wk, plate);
        }
        func_ov091_021bb2c0(wk, plate);
    }
}

// pokebw2's PokeListPlate_GetHp
u16 func_ov091_021bb464(PokeListWork *wk, PokeListPlate *plate) {
    return plate->hp;
}

// pokebw2's PokeListPlate_IsEgg
BOOL func_ov091_021bb468(PokeListWork *wk, PokeListPlate *plate) {
    return plate->isEgg;
}

static void func_ov091_021bb46c(PokeListWork *wk, PokeListPlate *plate) {
    BOOL BVar1;

    BVar1 = func_ov091_021b9a84(wk);
    if (BVar1 == 0) {
        plate->entry = 8;
        return;
    }
    if (func_02019da4(wk->param->regulation, (PartyPkm *)plate->pkm) == 0) {
        plate->entry = 6;
        return;
    }
    plate->entry = 7;
}

// Whether the Pokémon can join the battle's order: 0 if it can, 1 if the regulation bars it, 2 for a species and 3
// for an item already in the order, and 4 for a full order
// pokebw2's PokeListPlate_CheckEntry
u32 func_ov091_021bb4a0(PokeListWork *wk, PokeListPlate *plate) {
    u32 species;
    u32 item;
    u8 i;
    Regulation *regulation;

    if (plate->entry != POKELIST_ENTRY_ABLE) {
        return 1;
    }
    species = PokeParty_GetParam(plate->pkm, PKM_PARAM_SPECIES, NULL);
    item = PokeParty_GetParam(plate->pkm, PKM_PARAM_ITEM, NULL);
    regulation = wk->param->regulation;
    for (i = 0; i < 6; i++) {
        u8 picked = wk->param->picked[i];

        if (picked >= 1) {
            PartyPkm *other = wk->plates[picked - 1]->pkm;
            u32 otherSpecies = PokeParty_GetParam(other, PKM_PARAM_SPECIES, NULL);
            u32 otherItem = PokeParty_GetParam(other, PKM_PARAM_ITEM, NULL);

            if (species == otherSpecies && regulation->unk8 == 0) {
                return 2;
            }
            if (item == otherItem && item != 0 && regulation->unk9 == 0) {
                return 3;
            }
        }
    }
    if (wk->enteredCount == regulation->unk3) {
        return 4;
    }
    return 0;
}

// pokebw2's PokeListPlate_GetHpColor
static u8 func_ov091_021bb554(PokeListPlate *plate) {
    return func_0202d034(plate->hp, PokeParty_GetParam(plate->pkm, PKM_PARAM_MAX_HP, NULL));
}

// Where the icon goes for a point of the plate
// pokebw2's PokeListPlate_GetIconPos
static void func_ov091_021bb56c(PokeListPlate *plate, int x, int y, ClActorPos *pos) {
    ClActorPos surfacePos;

    func_0204b0c8(plate->renderer, 0, &surfacePos);
    pos->x = x - surfacePos.x;
    pos->y = y - surfacePos.y;
}
