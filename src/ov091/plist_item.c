// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_item.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "pml/evolution.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "system/bmp_winframe.h"
#include "system/game_beacon.h"
#include "system/game_comm.h"
#include "system/printsys.h"
#include "system/wordset.h"

#define HEAPID_POKELIST 0x17

// Using an item from the bag on a Pokémon of the party list, and the stats shown after a level up. The ROM doesn't
// name this file; it is named for what it does

// What an item does to a Pokémon, from its data: 0 nothing the list uses, 1 revive, 2 level up, 3 to 8 cure one
// status condition (sleep, poison, burn, freeze, paralysis, confusion), 9 cure them all, 10 restore HP, 11 restore
// HP and cure, 12 restore all HP, 13 to 18 raise an effort value, 19 to 24 lower one, 25 restore PP, 26 restore one
// move's PP, 27 restore all moves' PP, 28 raise a move's PP, 29 not usable
static u32 func_ov091_021bbe74(u16 item);
BOOL func_ov091_021bcc2c(PartyPkm *param_1, PkmField param_2, u16 item);
static void func_ov091_021bcc40(PokeListWork *wk, u32 msgId);
static void func_ov091_021bcc80(PokeListWork *wk, u32 msgId, u16 stat);

// The stats in the order the level up shows them
// pokebw2's sStatParams
static const u16 data_ov091_021bece4[] = {
    PKM_PARAM_MAX_HP, PKM_PARAM_ATTACK, PKM_PARAM_DEFENSE, PKM_PARAM_SP_ATTACK, PKM_PARAM_SP_DEFENSE, PKM_PARAM_SPEED,
};

void func_ov091_021bc628(PokeListWork *param_1);
void func_ov091_021bc5cc(PokeListWork *wk);
void func_ov091_021bc6a0(PokeListWork *param_1);
void func_ov091_021bc548(PokeListWork *param_1);

// pokebw2's PokeList_GetItemEffect
static u32 func_ov091_021bbe74(u16 item) {
    void *data = PML_ItemReadDataFile(item, 0, HEAPID_POKELIST);
    s32 cures;
    s32 effort;

    if (PML_ItemGetParam(data, 14) != 1) {
        GFL_HeapFree(data);
        return 29;
    }
    if (PML_ItemGetParam(data, 30) != 0 || PML_ItemGetParam(data, 31) != 0 || PML_ItemGetParam(data, 32) != 0 ||
        PML_ItemGetParam(data, 33) != 0 || PML_ItemGetParam(data, 34) != 0 || PML_ItemGetParam(data, 35) != 0 ||
        PML_ItemGetParam(data, 36) != 0) {
        GFL_HeapFree(data);
        return 0;
    }
    if (PML_ItemGetParam(data, 27) != 0) {
        GFL_HeapFree(data);
        return 1;
    }
    if (PML_ItemGetParam(data, 28) != 0) {
        GFL_HeapFree(data);
        return 2;
    }
    cures = PML_ItemGetParam(data, 18);
    cures += PML_ItemGetParam(data, 19) << 1;
    cures += PML_ItemGetParam(data, 20) << 2;
    cures += PML_ItemGetParam(data, 21) << 3;
    cures += PML_ItemGetParam(data, 22) << 4;
    cures += PML_ItemGetParam(data, 23) << 5;
    switch (cures) {
    case 0x1:
        GFL_HeapFree(data);
        return 3;
    case 0x2:
        GFL_HeapFree(data);
        return 4;
    case 0x4:
        GFL_HeapFree(data);
        return 5;
    case 0x8:
        GFL_HeapFree(data);
        return 6;
    case 0x10:
        GFL_HeapFree(data);
        return 7;
    case 0x20:
        GFL_HeapFree(data);
        return 8;
    case 0x3f:
        if (PML_ItemGetParam(data, 41) != 0) {
            GFL_HeapFree(data);
            return 11;
        }
        GFL_HeapFree(data);
        return 9;
    }
    if (PML_ItemGetParam(data, 24) != 0) {
        GFL_HeapFree(data);
        return 10;
    }
    if (PML_ItemGetParam(data, 41) != 0) {
        GFL_HeapFree(data);
        return 11;
    }
    if (PML_ItemGetParam(data, 26) != 0) {
        GFL_HeapFree(data);
        return 12;
    }
    effort = PML_ItemGetParam(data, 52);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 13;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 19;
    }
    effort = PML_ItemGetParam(data, 53);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 14;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 20;
    }
    effort = PML_ItemGetParam(data, 54);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 15;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 21;
    }
    effort = PML_ItemGetParam(data, 55);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 16;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 22;
    }
    effort = PML_ItemGetParam(data, 56);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 17;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 23;
    }
    effort = PML_ItemGetParam(data, 57);
    if (effort > 0) {
        GFL_HeapFree(data);
        return 18;
    }
    if (effort < 0) {
        GFL_HeapFree(data);
        return 24;
    }
    if (PML_ItemGetParam(data, 29) != 0) {
        GFL_HeapFree(data);
        return 25;
    }
    if (PML_ItemGetParam(data, 37) != 0) {
        GFL_HeapFree(data);
        return 26;
    }
    if (PML_ItemGetParam(data, 38) != 0) {
        GFL_HeapFree(data);
        return 27;
    }
    if (PML_ItemGetParam(data, 39) != 0 || PML_ItemGetParam(data, 40) != 0) {
        GFL_HeapFree(data);
        return 28;
    }
    GFL_HeapFree(data);
    return 29;
}

// Whether the item is used on one of the Pokémon's moves, which the menu picks
// pokebw2's PokeList_IsItemForMove
BOOL func_ov091_021bc17c(PokeListWork *wk, u16 item) {
    switch (func_ov091_021bbe74(item)) {
    case 26:
        return TRUE;
    case 27:
        return TRUE;
    case 28:
        if (GetItemParam(item, 39, wk->heapId)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Whether the item is used on the whole party at once, as Sacred Ash is
// pokebw2's PokeList_IsItemForParty
BOOL func_ov091_021bc1b4(PokeListWork *wk, u16 item) {
    if (GetItemParam(item, 27, wk->heapId)) {
        return TRUE;
    }
    return FALSE;
}

// The first Pokémon of the party that the item can be used on, or -1
// pokebw2's PokeList_FindItemTarget
int func_ov091_021bc1d0(PokeListWork *wk) {
    u8 count = PokeParty_GetPkmCount(wk->param->party);
    u8 i;

    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(wk->param->party, i);

        if (func_ov091_021bdd78(pkm, wk->param->item, 0, wk->heapId) == TRUE) {
            return i;
        }
    }
    return -1;
}

// The message of the menu that picks a move for the item
// pokebw2's PokeList_GetItemMenuMessage
u32 func_ov091_021bc228(PokeListWork *wk, u16 item) {
    switch (func_ov091_021bbe74(item)) {
    case 26:
    case 27:
        return 0x16;
    case 28:
        return 0x15;
    }
    return 0x15;
}

// Changes Arceus's type to the plate it holds
// pokebw2's PokeList_UpdateArceusForm
void func_ov091_021bc24c(PokeListWork *wk, PartyPkm *pkm, u16 item) {
    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_ARCEUS) {
        u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        u16 type = _getTypeForPlate(item);

        if (form != type) {
            PokeParty_ChangeForme(pkm, type);
        }
    }
}

// Changes Genesect's form to the drive it holds
// pokebw2's PokeList_UpdateGenesectForm
void func_ov091_021bc288(PokeListWork *wk, PartyPkm *pkm, u16 item) {
    if (PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL) == SPECIES_GENESECT) {
        u32 form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        u32 driveForm = func_02019d00(item);

        if (form != driveForm) {
            PokeParty_ChangeForme(pkm, driveForm);
        }
    }
}

void func_ov091_021bc2c8(PokeListWork *wk) {
    wk->param->result = 10;
    func_ov091_021bb97c(wk, wk->message);
    func_ov091_021bb9ac(wk, wk->message, 0, wk->param->item);
    func_ov091_021b9788(wk, 0x51, 1, (void *)func_ov091_021b9e3c);
    func_ov091_021bb98c(wk, wk->message);
}

void func_ov091_021bc30c(PokeListWork *wk) {
    wk->param->result = 10;
    func_ov091_021bb97c(wk, wk->message);
    func_ov091_021bb9ac(wk, wk->message, 0, wk->param->item);
    func_ov091_021b9788(wk, 0x51, 1, (void *)func_ov091_021b9e20);
    func_ov091_021bb98c(wk, wk->message);
}

int func_ov091_021bc350(PokeListWork *wk, u32 move) {
    int effect = func_ov091_021bbe74(wk->param->item);

    switch (effect) {
    case 1:
        wk->state = 10;
        wk->prevHp = func_ov091_021bb464(wk, wk->plates[wk->cursorPos]);
        wk->hpDoneFunc = func_ov091_021bc5cc;
        break;
    case 2: {
        u32 level = PokeParty_GetLevel(wk->pkm);
        StrBuf *name;
        u8 i;

        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
        func_ov091_021bb9ec(wk, wk->message, 1, level + 1, 3);
        func_ov091_021b9788(wk, 0x9d, 1, func_ov091_021bc6a0);
        func_ov091_021bb98c(wk, wk->message);
        name = GFL_StrBufCreate(12, wk->heapId);
        PokeParty_GetParam(wk->pkm, PKM_PARAM_NICKNAME, name);
        func_02027268(name);
        GFL_StrBufFree(name);
        for (i = 0; i < 6; i++) {
            wk->prevStats[i] = PokeParty_GetParam(wk->pkm, data_ov091_021bece4[i], NULL);
        }
        break;
    }
    case 3:
        func_ov091_021bcc40(wk, 0x47);
        break;
    case 4:
        func_ov091_021bcc40(wk, 0x2d);
        break;
    case 5:
        func_ov091_021bcc40(wk, 0x2f);
        break;
    case 6:
        func_ov091_021bcc40(wk, 0x30);
        break;
    case 7:
        func_ov091_021bcc40(wk, 0x2e);
        break;
    case 9:
        func_ov091_021bcc40(wk, 0x33);
        break;
    case 11:
    case 12:
        wk->state = 10;
        wk->prevHp = func_ov091_021bb464(wk, wk->plates[wk->cursorPos]);
        wk->hpDoneFunc = func_ov091_021bc548;
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        func_ov091_021bcc80(wk, 0x37, effect - 13);
        break;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24: {
        u16 stat = effect - 19;

        if (func_ov091_021bcc2c(wk->pkm, PKM_PARAM_EV_HP + stat, wk->param->item) == FALSE) {
            func_ov091_021bcc80(wk, 0x55, stat);
        } else if (PokeParty_GetParam(wk->pkm, PKM_PARAM_HAPPINESS, NULL) == 255) {
            func_ov091_021bcc80(wk, 0x54, stat);
        } else {
            func_ov091_021bcc80(wk, 0x53, stat);
        }
        break;
    }
    case 26:
    case 27:
        func_ov091_021bb97c(wk, wk->message);
        func_ov091_021bb9cc(wk, wk->message, 0, move);
        func_ov091_021b9788(wk, 0x34, 1, func_ov091_021b9e44);
        func_ov091_021bb98c(wk, wk->message);
        break;
    case 28:
        func_ov091_021bcc40(wk, 0x31);
        break;
    case 0:
    case 8:
    case 10:
    case 25:
    case 29:
        break;
    }
    return effect;
}


void func_ov091_021bc548(PokeListWork *param_1) {
    u16 result;
    result = func_ov091_021bb464(param_1, param_1->plates[param_1->cursorPos]);
    param_1->param->result = 10;
    func_ov091_021bb97c(param_1, param_1->message);
    func_ov091_021bb99c(param_1, param_1->message, 0, param_1->pkm);
    if (param_1->prevHp != 0) {
        func_ov091_021bb9ec(param_1, param_1->message, 1, result - param_1->prevHp, 3);
        func_ov091_021b9788(param_1, 0x2c, 1, (void *)func_ov091_021b9e44);
    } else {
        func_ov091_021b9788(param_1, 0x32, 1, (void *)func_ov091_021b9e44);
    }
    func_ov091_021bb98c(param_1, param_1->message);
}

void func_ov091_021bc5cc(PokeListWork *wk) {
    wk->param->result = 10;
    func_ov091_021bb97c(wk, wk->message);
    func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
    if (func_ov091_021bc1d0(wk) != -1) {
        func_ov091_021b9788(wk, 0x32, 1, (void *)func_ov091_021bc628);
    } else {
        func_ov091_021b9788(wk, 0x32, 1, (void *)func_ov091_021b9e3c);
    }
    func_ov091_021bb98c(wk, wk->message);
}


void func_ov091_021bc628(PokeListWork *param_1) {
    u32 index;
    PartyPkm *pPVar1;

    index = func_ov091_021bc1d0(param_1);
    func_ov091_021bb7a4(param_1, param_1->message);
    func_ov091_021bb050(param_1, param_1->plates[param_1->cursorPos], 3);
    param_1->cursorPos = index;
    pPVar1 = PokeParty_GetPkm(param_1->param->party, index);
    param_1->pkm = pPVar1;
    func_ov091_021bc350(param_1, 0);
    func_ov091_021be2b4(param_1->pkm, param_1->param->item, 0, param_1->param->zoneId, (u32)param_1->heapId);
    func_ov091_021bb2c0(param_1, param_1->plates[param_1->cursorPos]);
    GFL_SndSEPlay(0x56f);
}

void func_ov091_021bc6a0(PokeListWork *param_1) {
    param_1->state = 0xd;
    param_1->subState = 0;
    param_1->param->learnIndex = 0;
    func_ov091_021bba70(param_1, param_1->message);
}

// Called once the message of a move learned is read: on to the next move
// pokebw2's PokeList_LearnMessageDone
void func_ov091_021bc6c0(PokeListWork *wk) {
    wk->state = 13;
    wk->subState = 6;
}

void func_ov091_021bc6cc(PokeListWork *wk) {
    switch (wk->subState) {
    case 0: {
        BOOL bigGain = FALSE;
        u8 i;
        u8 row;

        wk->statsWindow = BmpWin_CreateDynamic(0, 1, 1, 14, 12, 14, TRUE);
        BmpWin_DrawFrame(wk->statsWindow, WINFRAME_TRANSFER_VBLANK, 1, 12);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->statsWindow), 15);
        BmpWin_Transfer(wk->statsWindow);
        for (i = 0; i < 6; i++) {
            u32 gain = PokeParty_GetParam(wk->pkm, data_ov091_021bece4[i], NULL) - wk->prevStats[i];
            StrBuf *name;
            StrBuf *format;
            StrBuf *str;
            WordSet *wordSet;
            u16 width;

            if (gain >= 10) {
                bigGain = TRUE;
            }
            name = GFL_MsgDataLoadStrbufNew(wk->msgData, 0x95 + i);
            func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow), 0, i * 16, name, wk->font, 0x440);
            GFL_StrBufFree(name);
            format = GFL_MsgDataLoadStrbufNew(wk->msgData, 0x9c);
            str = GFL_StrBufCreate(32, wk->heapId);
            wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
            WordSetNumber(wordSet, 0, gain, 3, 0, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, format);
            width = GFL_FontGetBlockWidth(str, wk->font, 0);
            func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow), 104 - width, i * 16, str, wk->font, 0x440);
            GFL_StrBufFree(format);
            GFL_StrBufFree(str);
            GFL_WordSetSystemFree(wordSet);
        }
        for (row = 0; row < 6; row++) {
            u8 x = bigGain == TRUE ? 72 : 80;
            StrBuf *plus = GFL_MsgDataLoadStrbufNew(wk->msgData, 0x9b);

            func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow), x, row * 16, plus, wk->font, 0x440);
            GFL_StrBufFree(plus);
        }
        wk->subState = 1;
        break;
    }
    case 1:
        if (func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow)) == FALSE) {
            BmpWin_Transfer(wk->statsWindow);
            wk->subState = 2;
        }
        break;
    case 2:
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) ||
            func_02035c60() == TRUE) {
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            wk->subState = 3;
        }
        break;
    case 3: {
        u8 i;

        GFL_BitmapFillArea(BmpWin_GetBitmap(wk->statsWindow), 80, 0, 32, 96, 15);
        for (i = 0; i < 6; i++) {
            u32 stat = PokeParty_GetParam(wk->pkm, data_ov091_021bece4[i], NULL);
            StrBuf *format = GFL_MsgDataLoadStrbufNew(wk->msgData, 0x9c);
            StrBuf *str = GFL_StrBufCreate(32, wk->heapId);
            WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
            u16 width;

            WordSetNumber(wordSet, 0, stat, 3, 0, TRUE);
            GFL_WordSetFormatStrbuf(wordSet, str, format);
            width = GFL_FontGetBlockWidth(str, wk->font, 0);
            func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow), 104 - width, i * 16, str, wk->font, 0x440);
            GFL_StrBufFree(format);
            GFL_StrBufFree(str);
            GFL_WordSetSystemFree(wordSet);
        }
        wk->subState = 4;
        break;
    }
    case 4:
        if (func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->statsWindow)) == FALSE) {
            BmpWin_Transfer(wk->statsWindow);
            wk->subState = 5;
        }
        break;
    case 5:
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_A) || (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) ||
            func_02035c60() == TRUE) {
            wk->subState = 6;
            GFL_SndSEPlay(SEQ_SE_MESSAGE);
            func_020456f8(wk->statsWindow);
            func_0201f7a8(wk->statsWindow, WINFRAME_TRANSFER_VBLANK);
            BmpWin_Free(wk->statsWindow);
            wk->statsWindow = NULL;
        }
        break;
    case 6: {
        u16 move = func_02018328(wk->pkm, &wk->param->learnIndex, wk->heapId);

        func_ov091_021bb7a4(wk, wk->message);
        if (move == 0) {
            wk->subState = 7;
        } else if (move == LEARN_MOVE_KNOWN) {
            wk->subState = 7;
        } else if (move & LEARN_MOVE_NO_SLOT) {
            wk->param->move = move - LEARN_MOVE_NO_SLOT;
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, wk->param->move);
            func_ov091_021b9788(wk, 0x20, FALSE, func_ov091_021b9eb0);
            func_ov091_021bb98c(wk, wk->message);
        } else {
            func_ov091_021bb97c(wk, wk->message);
            func_ov091_021bb99c(wk, wk->message, 0, wk->pkm);
            func_ov091_021bb9cc(wk, wk->message, 1, move);
            func_ov091_021b9788(wk, 0x29, TRUE, func_ov091_021bc6c0);
            func_ov091_021bb98c(wk, wk->message);
        }
        break;
    }
    case 7: {
        PokeParty_GetParam(wk->pkm, PKM_PARAM_SPECIES, NULL);

        if (CheckEvolveSpecies(wk->param->party, wk->pkm, 0, wk->param->zoneId, wk->param->unk5F, NULL, wk->heapId) != 0) {
            wk->state = 19;
            wk->param->index = wk->cursorPos;
            wk->param->result = 9;
        } else {
            wk->param->mode = 5;
            wk->param->index = wk->cursorPos;
            wk->param->result = 10;
            func_ov091_021b9e44(wk);
        }
        break;
    }
    }
}

// pokebw2's PokeList_HasEffort
BOOL func_ov091_021bcc2c(PartyPkm *param_1, PkmField param_2, u16 item) {
    return (u32)(PokeParty_GetParam(param_1, param_2, (void *)0x0) != 0);
}

static void func_ov091_021bcc40(PokeListWork *param_1, u32 param_2) {
    param_1->param->result = 10;
    func_ov091_021bb97c(param_1, param_1->message);
    func_ov091_021bb99c(param_1, param_1->message, 0, param_1->pkm);
    func_ov091_021b9788(param_1, param_2, 1, (void *)func_ov091_021b9e44);
    func_ov091_021bb98c(param_1, param_1->message);
}

static void func_ov091_021bcc80(PokeListWork *param_1, u32 param_2, u16 param_3) {
    param_1->param->result = 10;
    func_ov091_021bb97c(param_1, param_1->message);
    func_ov091_021bb99c(param_1, param_1->message, 0, param_1->pkm);
    func_ov091_021bb9dc(param_1, param_1->message, 1, param_3);
    func_ov091_021b9788(param_1, param_2, 1, (void *)func_ov091_021b9e44);
    func_ov091_021bb98c(param_1, param_1->message);
}
