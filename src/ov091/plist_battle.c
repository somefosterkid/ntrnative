// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov165/plist_battle.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/pokelist.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "types.h"
#include "struct_decls.h"
#include "gfl/net.h"
#include "gfl/particle.h"
#include "save/pokedex.h"
#include "nitro/rtc.h"
#include "system/rtc.h"
#include "gfl/rtc_cache.h"
#include "constants/species.h"
#include "gfl/sound.h"

// The party list in a battle's team selection: the partners' teams of a multi battle shown beside the player's, the
// battle's messages, and the time left to pick

static void func_ov091_021bd454(PokeListWork *wk, PokeListPartnerPkm *partnerPkm);
void func_ov091_021bce84(PokeListWork *wk);
void func_ov091_021bce9c(PokeListWork *wk);
void func_ov091_021bceac(PokeListWork *wk);
void func_ov091_021bd058(PokeListWork *wk);
PokeListPartner *func_ov091_021bd05c(PokeListWork *wk, PokeListPartnerParam *partnerParam, u8 x, u8 y, u8 spacing);
void func_ov091_021bd504(PokeListWork *param_1);
void func_ov091_021bd59c(PokeListWork *wk);
void func_ov091_021bd61c(PokeListWork *wk);
void func_ov091_021bd634(PokeListWork *wk);

void func_ov091_021bcccc(PokeListWork *wk) {
    func_ov091_021bce84(wk);
    func_ov091_021bceac(wk);
    if (wk->param->showPartners == TRUE) {
        switch (wk->param->partnerCount) {
        case 1:
            wk->partners[1] = func_ov091_021bd05c(wk, &wk->param->partners[1], 10, 0, 2);
            break;
        case 2:
            wk->partners[0] = func_ov091_021bd05c(wk, &wk->param->partners[0], 0, 0, 0);
            wk->partners[1] = func_ov091_021bd05c(wk, &wk->param->partners[1], 11, 0, 0);
            wk->partners[2] = func_ov091_021bd05c(wk, &wk->param->partners[2], 22, 0, 0);
            break;
        }
    }
    wk->shownBattleMsg = wk->param->battleMsg;
    wk->shownTime = 0;
    if (wk->param->timerEnabled == TRUE) {
        func_ov091_021bd59c(wk);
    }
    wk->timerBlink = 0;
}


void func_ov091_021bcd80(PokeListWork *wk) {
    if (wk->param->timerEnabled == TRUE) {
        func_ov091_021bd61c(wk);
    }
    if (wk->param->showPartners == TRUE) {
        switch (wk->param->partnerCount) {
        case 1:
            func_ov091_021bd18c(wk, wk->partners[1]);
            break;
        case 2:
            func_ov091_021bd18c(wk, wk->partners[0]);
            func_ov091_021bd18c(wk, wk->partners[1]);
            func_ov091_021bd18c(wk, wk->partners[2]);
            break;
        }
    }
    func_ov091_021bd058(wk);
    func_ov091_021bce9c(wk);
}

void func_ov091_021bcde8(PokeListWork *wk) {
    if (wk->param->showPartners == TRUE) {
        switch (wk->param->partnerCount) {
        case 1:
            func_ov091_021bd1bc(wk, wk->partners[1]);
            break;
        case 2:
            func_ov091_021bd1bc(wk, wk->partners[0]);
            func_ov091_021bd1bc(wk, wk->partners[1]);
            func_ov091_021bd1bc(wk, wk->partners[2]);
            break;
        }
    }
    if (wk->shownBattleMsg != wk->param->battleMsg && wk->state == 2) {
        func_ov091_021bd504(wk);
        wk->shownBattleMsg = wk->param->battleMsg;
    }
    if (wk->param->timerEnabled == TRUE) {
        func_ov091_021bd634(wk);
    }
    if (wk->param->unk62[1] == 1) {
        func_ov091_021ba4f8(wk);
    }
}



void func_ov091_021bce84(PokeListWork *wk) {
    wk->partnerUnit = func_0204b100(0x24, 8, wk->heapId);
}

void func_ov091_021bce9c(PokeListWork *wk) {
    func_0204b1cc(wk->partnerUnit);
}

void func_ov091_021bceac(PokeListWork *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(0x4c, wk->heapId);
    ArcTool *commonArc;
    ArcTool *iconArc;

    GFL_G2DIOLoadArcNCLR(arc, 1, 4, 0x20, 0x20, 0x20, wk->heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 0, 4, 0x1a0, 0x20, wk->heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 9, 6, 0, 0, FALSE, wk->heapId);
    if (wk->param->showPartners == TRUE) {
        switch (wk->param->partnerCount) {
        case 1:
            loadBGScrToVramByFileNoReserveNegAlign(arc, 0x11, 6, 0, 0, FALSE, wk->heapId);
            break;
        case 2:
            loadBGScrToVramByFileNoReserveNegAlign(arc, 0x10, 6, 0, 0, FALSE, wk->heapId);
            break;
        }
    }
    GFL_ArcToolFree(arc);

    commonArc = GFL_ArcSysCreateFileHandle(getUINarcIdx(), wk->heapId);
    wk->clResources[CL_RES_PLTT(3)] = func_0204abf0(arc, func_02027694(), 1, 0x60, wk->heapId);
    wk->clResources[CL_RES_CHAR(4)] = func_0204a6c8(arc, func_02027698(), FALSE, 1, wk->heapId);
    wk->clResources[CL_RES_CELL(4)] = func_0204af28(arc, func_0202769c(2), func_020276a0(2), wk->heapId);
    if (wk->param->timerEnabled == TRUE) {
        GFL_G2DIOLoadArcNCLRDefault(arc, func_02027624(), 4, 0x40, 0x20, wk->heapId);
        GFL_BGSysLoadArcNCGRStatic(arc, func_02027628(), 5, 0, 0, FALSE, wk->heapId);
        loadBGScrToVramByFileNoReserveNegAlign(arc, func_0202762c(), 5, 0, 0, FALSE, wk->heapId);
        GFL_BGSysSetScrPaletteNo(5, 0, 0, 32, 32, 2);
        GFL_BGSysQueueScrLoad(5);
    }
    GFL_ArcToolFree(commonArc);

    iconArc = GFL_ArcSysCreateFileHandle(7, wk->heapId);
    wk->clResources[CL_RES_PLTT(7)] = func_0204acec(iconArc, func_0201bc3c(), 1, 0xc0, wk->heapId);
    GFL_ArcToolFree(iconArc);
    MIi_CpuCopy16((u16 *)HW_DB_BG_PLTT + 2 * 16, wk->timerBaseColors, 32);
}


void func_ov091_021bd058(PokeListWork *wk) {
}

static inline PokeListPartnerPkm *PokeListBattle_CreatePartnerPkmAt(PokeListWork *wk, PartyPkm *pkm, u8 col, u8 row,
                                                                    u8 offset) {
    return func_ov091_021bd218(wk, pkm, offset + col * 8, row * 8);
}
PokeListPartner *func_ov091_021bd05c(PokeListWork *wk, PokeListPartnerParam *partnerParam, u8 x, u8 y, u8 spacing) {
    PokeListPartner *partner = GFL_HeapAllocate(wk->heapId, sizeof(PokeListPartner), FALSE, "plist_battle.c", 323);
    u8 i;
    u8 count = PokeParty_GetPkmCount(partnerParam->party);
    StrBuf *name;

    for (i = 0; i < 6; i++) {
        if (i < count) {
            u8 col = x + (i % 2) * (spacing + 5);
            u8 row = y + (i / 2) * 6 + 4;
            int offset = 4;

            if (i % 2 != 0) {
                offset = 0;
            }
            partner->pkms[i] = PokeListBattle_CreatePartnerPkmAt(wk, PokeParty_GetPkm(partnerParam->party, i), col, row, offset);
        } else {
            partner->pkms[i] = NULL;
        }
    }
    partner->window = BmpWin_CreateDynamic(4, x + 1 + spacing / 2, y + 1, 10, 2, 14, TRUE);
    name = GFL_StrBufCreate(16, wk->heapId);
    GFL_StrBufLoadString(name, partnerParam->name);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(partner->window), 0, 0, name, wk->font, 0x3c40);
    GFL_StrBufFree(name);
    partner->dirty = TRUE;
    return partner;
}

// pokebw2's PokeListBattle_FreePartner
void func_ov091_021bd18c(PokeListWork *wk, PokeListPartner *partner) {
    u8 i;

    for (i = 0; i < 6; i++) {
        if (partner->pkms[i] != NULL) {
            func_ov091_021bd454(wk, partner->pkms[i]);
        }
    }
    BmpWin_Free(partner->window);
    GFL_HeapFree(partner);
}

// pokebw2's PokeListBattle_UpdatePartner
void func_ov091_021bd1bc(PokeListWork *wk, PokeListPartner *partner) {
    u8 i;

    for (i = 0; i < 6; i++) {
        if (partner->pkms[i] != NULL) {
            func_ov091_021bd480(wk, partner->pkms[i]);
        }
    }
    if (partner->dirty == TRUE && func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(partner->window)) == FALSE) {
        partner->dirty = FALSE;
        BmpWin_Transfer(partner->window);
    }
}

// pokebw2's PokeListBattle_CreatePartnerPkm
static inline void PokeListBattle_CreateSexWindow(PokeListWork *wk, PokeListPartnerPkm *partnerPkm, BOOL isEgg, u32 sex,
                                                  u8 x, u8 y) {
    u8 subX = x % 8;
    u8 tileY = y / 8;
    u8 tileX = x / 8;

    partnerPkm->sexWindow = BmpWin_CreateDynamic(4, tileX + 3, tileY, 3, 2, 13, TRUE);
    if (isEgg == FALSE) {
        if (sex == GENDER_MALE) {
            func_ov091_021b9c1c(wk, partnerPkm->sexWindow, 7, subX + 4, 0, 0x14c0);
        } else if (sex == GENDER_FEMALE) {
            func_ov091_021b9c1c(wk, partnerPkm->sexWindow, 8, subX + 4, 0, 0xc80);
        }
    }
}

static inline void PokeListBattle_CreateLevelWindow(PokeListWork *wk, PokeListPartnerPkm *partnerPkm, BOOL isEgg,
                                                    u32 level, u8 x, u8 y) {
    u8 subX = x % 8;
    u8 tileY = y / 8;
    u8 tileX = x / 8;

    partnerPkm->levelWindow = BmpWin_CreateDynamic(4, tileX, tileY + 3, 6, 1, 14, TRUE);
    if (isEgg == FALSE) {
        WordSet *wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);

        WordSetNumber(wordSet, 0, level, 3, 0, TRUE);
        func_ov091_021b9d58(wk, partnerPkm->levelWindow, wordSet, 2, subX, 0, 0x3c40);
        GFL_WordSetSystemFree(wordSet);
    }
}
PokeListPartnerPkm *func_ov091_021bd218(PokeListWork *wk, PartyPkm *pkm, u8 x, u8 y) {
    PokeListPartnerPkm *partnerPkm =
        GFL_HeapAllocate(wk->heapId, sizeof(PokeListPartnerPkm), FALSE, "plist_battle.c", 409);
    BOOL isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    ArcTool *arc = GFL_ArcSysCreateFileHandle(7, wk->heapId);
    BoxPkm *boxPkm = func_0201d624(pkm);
    u32 palette;
    u32 item;
    u32 sex;
    u32 level;
    ClActorSetup iconSetup;
    ClActorSetup itemSetup;

    partnerPkm->iconChars = func_0204a6c8(arc, func_0201ba68(boxPkm), FALSE, 1, wk->heapId);
    GFL_ArcToolFree(arc);
    palette = func_0201bbe8(boxPkm);
    iconSetup.x = x + 16;
    iconSetup.y = y + 8;
    iconSetup.sequence = 1;
    iconSetup.priority = 16;
    iconSetup.bgPriority = 2;
    partnerPkm->icon = func_0204b294(wk->partnerUnit, partnerPkm->iconChars, wk->clResources[CL_RES_PLTT(7)],
                                     wk->clResources[CL_RES_CELL(7)], &iconSetup, 1, wk->heapId);
    func_0204b6f4(partnerPkm->icon, palette, 1);
    func_0204b92c(partnerPkm->icon, TRUE);
    item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    itemSetup.x = x + 32;
    itemSetup.y = y + 16;
    itemSetup.sequence = 0;
    itemSetup.priority = 8;
    itemSetup.bgPriority = 2;
    partnerPkm->itemIcon =
        func_0204b294(wk->partnerUnit, wk->clResources[CL_RES_CHAR(4)], wk->clResources[CL_RES_PLTT(3)],
                      wk->clResources[CL_RES_CELL(4)], &itemSetup, 1, wk->heapId);
    if (item != 0) {
        func_0204b3dc(partnerPkm->itemIcon, TRUE);
        if (PML_ItemIsMail(item) == TRUE) {
            func_0204b878(partnerPkm->itemIcon, 1);
        } else {
            func_0204b878(partnerPkm->itemIcon, 0);
        }
    } else {
        func_0204b3dc(partnerPkm->itemIcon, FALSE);
    }
    sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
    PokeListBattle_CreateSexWindow(wk, partnerPkm, isEgg, sex, x, y);
    level = PokeParty_GetLevel(pkm);
    PokeListBattle_CreateLevelWindow(wk, partnerPkm, isEgg, level, x, y);
    partnerPkm->dirty = TRUE;
    return partnerPkm;
}

// pokebw2's PokeListBattle_FreePartnerPkm
static void func_ov091_021bd454(PokeListWork *wk, PokeListPartnerPkm *partnerPkm) {
    BmpWin_Free(partnerPkm->sexWindow);
    BmpWin_Free(partnerPkm->levelWindow);
    func_0204b3b4(partnerPkm->itemIcon);
    func_0204b3b4(partnerPkm->icon);
    func_0204a8d4(partnerPkm->iconChars);
    GFL_HeapFree(partnerPkm);
}

// pokebw2's PokeListBattle_UpdatePartnerPkm
void func_ov091_021bd480(PokeListWork *wk, PokeListPartnerPkm *partnerPkm) {
    if (partnerPkm->dirty == TRUE && func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(partnerPkm->sexWindow)) == FALSE &&
        func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(partnerPkm->levelWindow)) == FALSE) {
        partnerPkm->dirty = FALSE;
        BmpWin_Transfer(partnerPkm->sexWindow);
        BmpWin_Transfer(partnerPkm->levelWindow);
    }
}

void func_ov091_021bd4ec(PokeListWork *wk) {
    func_ov091_021bd504(wk);
    wk->shownBattleMsg = wk->param->battleMsg;
}

void func_ov091_021bd504(PokeListWork *param_1) {
    BOOL BVar1;
    StrBuf *strbuf;

    if (param_1->param->battleMsg != 0) {
        BVar1 = func_ov091_021bb7d8(param_1, param_1->message);
        if (BVar1 == 0) {
            func_ov091_021bb714(param_1, param_1->message, 1);
        }
        if (param_1->param->partnerCount == 1) {
            strbuf = GFL_StrBufCreate(0x10, param_1->heapId);
            GFL_StrBufLoadString(strbuf, param_1->param->partners[1].name);
            func_ov091_021bb97c(param_1, param_1->message);
            func_ov091_021bba0c(param_1, param_1->message, 0, strbuf, (u32)param_1->param->partners[1].gender
                                                  );
            func_ov091_021bb7e8(param_1, param_1->message, 0xa9);
            func_ov091_021bb98c(param_1, param_1->message);
            GFL_StrBufFree(strbuf);
            return;
        }
        func_ov091_021bb7e8(param_1, param_1->message, param_1->param->battleMsg + 0xa9);
    }
}

// pokebw2's PokeListBattle_CreateTimer
void func_ov091_021bd59c(PokeListWork *wk) {
    StrBuf *label;

    wk->timerWindows[0] = BmpWin_CreateDynamic(4, 7, 21, 11, 3, 14, TRUE);
    wk->timerWindows[1] = BmpWin_CreateDynamic(4, 20, 21, 5, 3, 14, TRUE);
    label = GFL_MsgDataLoadStrbufNew(wk->msgData, 0xad);
    func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->timerWindows[0]), 0, 4, label, wk->font, 0x3c40);
    GFL_StrBufFree(label);
    wk->timerWindowDirty[0] = TRUE;
}

// pokebw2's PokeListBattle_FreeTimer
void func_ov091_021bd61c(PokeListWork *wk) {
    BmpWin_Free(wk->timerWindows[0]);
    BmpWin_Free(wk->timerWindows[1]);
}

// pokebw2's PokeListBattle_UpdateTimer
void func_ov091_021bd634(PokeListWork *wk) {
    u16 timeLeft;
    u8 minutes;
    u8 seconds;
    StrBuf *str;
    StrBuf *format;
    WordSet *wordSet;
    u32 width;
    u8 level;
    u8 i;

    timeLeft = wk->param->timeLeft;
    if (wk->shownTime != timeLeft) {
        minutes = timeLeft / 60;
        seconds = timeLeft % 60;
        str = GFL_StrBufCreate(0x20, wk->heapId);
        wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
        if (minutes == 0) {
            format = GFL_MsgDataLoadStrbufNew(wk->msgData, 0xaf);
            WordSetNumber(wordSet, 1, seconds, 2, 1, TRUE);
        } else {
            format = GFL_MsgDataLoadStrbufNew(wk->msgData, 0xae);
            WordSetNumber(wordSet, 0, minutes, 2, 1, TRUE);
            WordSetNumber(wordSet, 1, seconds, 2, 2, TRUE);
        }
        GFL_BitmapFill(BmpWin_GetBitmap(wk->timerWindows[1]), 0);
        GFL_WordSetFormatStrbuf(wordSet, str, format);
        width = GFL_FontGetBlockWidth(str, wk->font, 0);
        func_0201c724(wk->printQueue, BmpWin_GetBitmap(wk->timerWindows[1]), 36 - width, 4, str, wk->font, 0x3c40);
        GFL_WordSetSystemFree(wordSet);
        GFL_StrBufFree(str);
        GFL_StrBufFree(format);
        wk->timerWindowDirty[1] = TRUE;
        wk->shownTime = wk->param->timeLeft;
    }
    if (wk->param->timeLeft == 0) {
        func_ov091_021ba4f8(wk);
    }
    if (wk->timerWindowDirty[1] == TRUE &&
        func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->timerWindows[1])) == FALSE) {
        wk->timerWindowDirty[1] = FALSE;
        BmpWin_Transfer(wk->timerWindows[1]);
    }
    if (wk->timerWindowDirty[0] == TRUE &&
        func_0201c6c4(wk->printQueue, BmpWin_GetBitmap(wk->timerWindows[0])) == FALSE) {
        wk->timerWindowDirty[0] = FALSE;
        BmpWin_Transfer(wk->timerWindows[0]);
    }
    if (wk->param->timeLeft <= 10) {
        level = ((FX_CosIdx(wk->timerBlink * 0x10000 / 60) + FX32_ONE) * 32) / (FX32_ONE * 2);
        for (i = 0; i < 16; i++) {
            u16 color = wk->timerBaseColors[i];
            u8 g = (color & 0x3e0) >> 5;
            u8 b = (color & 0x7c00) >> 10;
            u8 r = color & 0x1f;

            r = (r + level > 31) ? 31 : r + level;
            wk->timerColors[i] = GX_RGB(r, g, b);
        }
        NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_SUB, 0x40, wk->timerColors, sizeof(wk->timerColors));
        wk->timerBlink++;
        if (wk->timerBlink >= 60) {
            wk->timerBlink = 0;
        }
    }
}
