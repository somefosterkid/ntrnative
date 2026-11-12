// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_message.c, ported by tools/port_file.py.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/ui/touchbar.h"
#include "app/pokemon_trade_local.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/app_menu_common.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/app_taskmenu.h"
#include "system/bmp_winframe.h"
#include "system/gf_font.h"
#include "system/mcss.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/wordset.h"
#include "text/system/ability_handlers_3.h"
#include "field/beacon_view.h"

// The trade's messages, menus and the panels that describe a Pokémon: its name, level, stats, moves and icons. The
// ROM names no file between pokemontrade_save.c and pokemontrade_3d.c, so the name is descriptive


static void func_ov119_021e0da0(ResSprite *sprite);
static void func_ov119_021e0dbc(ResSprite *sprite, PartyPkm *pkm, int index, ClActUnit *unit, int x, int y,
                                u32 vramType, HeapID heapId);
static void func_ov119_021e0e4c(ResSprite *sprite);
static void func_ov119_021e0e68(ResSprite *sprite, PartyPkm *pkm, ClActUnit *unit, int x, int y, u32 vramType,
                                HeapID heapId, u32 plttOffset);
void func_ov119_021e1430(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, BOOL speciesName, PokemonTradeWork *wk);
void func_ov119_021e14dc(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, PokemonTradeWork *wk);
void func_ov119_021e1594(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, PokemonTradeWork *wk);
void func_ov119_021e15f8(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1670(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, PokemonTradeWork *wk, BOOL light, BOOL speciesName);
void func_ov119_021e1718(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1778(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e17e8(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1880(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e194c(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e198c(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1a44(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1aa8(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk);
void func_ov119_021e1cc8(PokemonTradeWork *wk);
void func_ov119_021e2054(PokemonTradeWork *wk, PartyPkm *pkm, int side, BOOL reload);
void func_ov119_021e2070(PokemonTradeWork *wk, PartyPkm *pkm, int *icons);
void func_ov119_021e2140(PokemonTradeWork *wk, PartyPkm *pkm);
void func_ov119_021e2a98(TCB *tcb, void *work);

static void func_ov119_021e0da0(ResSprite *sprite) {
    if (sprite->actor != NULL) {
        func_0204b3b4(sprite->actor);
        sprite->actor = NULL;
        func_ov067_021b6284(&sprite->res);
    }
}

// A type icon of the Pokémon: its first type (0) or its second (1)
static void func_ov119_021e0dbc(ResSprite *sprite, PartyPkm *pkm, int index, ClActUnit *unit, int x, int y,
                                u32 vramType, HeapID heapId) {
    UIObjResSetup param;
    u8 type = PokeParty_GetParam(pkm, PKM_PARAM_TYPE1 + index, NULL);
    param.vramType = vramType;
    param.flags = 0;
    param.arcId = getUINarcIdx();
    param.paletteFile = func_020275e8();
    param.charFile = func_020275f8(type);
    param.cellFile = func_020275fc(2);
    param.animFile = func_02027600(2);
    param.paletteOffset = 8;
    param.paletteStart = 0;
    param.paletteCount = 3;
    func_ov067_021b6208(&sprite->res, &param, unit, heapId);
    sprite->actor = func_ov067_021b629c(&sprite->res, unit, x, y, 0, heapId);
    func_0204b6f4(sprite->actor, func_020275ec(type), 1);
}

static void func_ov119_021e0e4c(ResSprite *sprite) {
    if (sprite->actor != NULL) {
        func_0204b3b4(sprite->actor);
        sprite->actor = NULL;
        func_ov067_021b6284(&sprite->res);
    }
}

// The icon of the Poké Ball the Pokémon was caught in
static void func_ov119_021e0e68(ResSprite *sprite, PartyPkm *pkm, ClActUnit *unit, int x, int y, u32 vramType,
                                HeapID heapId, u32 plttOffset) {
    UIObjResSetup param;
    u32 ball = PokeParty_GetParam(pkm, PKM_PARAM_POKEBALL, NULL);
    func_ov119_021e0e4c(sprite);
    param.vramType = vramType;
    param.flags = 0;
    param.arcId = getUINarcIdx();
    param.paletteFile = func_02027720(ball);
    param.charFile = func_0202772c(ball);
    param.cellFile = func_02027738(ball, 2);
    param.animFile = func_02027740(ball, 2);
    param.paletteOffset = plttOffset;
    param.paletteStart = 0;
    param.paletteCount = 1;
    func_ov067_021b6208(&sprite->res, &param, unit, heapId);
    sprite->actor = func_ov067_021b629c(&sprite->res, unit, x, y, 0, heapId);
}

void func_ov119_021e0ef4(PokemonTradeWork *wk, BOOL instant, u32 x, u32 y, u32 width, u32 height) {
    BmpWin *window;
    func_ov119_021e106c(wk);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 4, 0x1c0, 0x20, wk->heapId);
    window = BmpWin_CreateDynamic(6, x, y, width, height, 14, 0);
    wk->msgWindow = window;
    GFL_BitmapFill(BmpWin_GetBitmap(window), 15);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    if (!instant) {
        wk->printStream = func_0201cce4(window, 0, 0, wk->strbuf, wk->font, func_02012ffc(), wk->tcbEx, 2,
                                        wk->heapId, 15);
        func_02028470(&wk->printWait, 2);
    } else {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, wk->strbuf, wk->font);
    }
    BmpWin_DrawFrame(window, 1, wk->cursorImage, 15);
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(6);
}


// Opens the message window at the top or the bottom of the lower screen
void func_ov119_021e0ff4(PokemonTradeWork *wk, BOOL instant, BOOL bottom) {
    if (bottom) {
        func_ov119_021e0ef4(wk, instant, 1, 18, 30, 4);
    } else {
        func_ov119_021e0ef4(wk, instant, 1, 1, 30, 4);
    }
}

void func_ov119_021e1024(PokemonTradeWork *wk) {
    func_ov119_021e0ff4(wk, FALSE, FALSE);
}

void func_ov119_021e1030(PokemonTradeWork *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    wk->waitIcon = func_0202ee8c(wk->tcbEx, wk->msgWindow, 15, 16, wk->heapId);
}

void func_ov119_021e106c(PokemonTradeWork *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    if (wk->msgWindow != NULL) {
        BmpWin_Free(wk->msgWindow);
        wk->msgWindow = NULL;
    }
}

void func_ov119_021e1098(PokemonTradeWork *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    if (wk->msgWindow != NULL) {
        func_020456f8(wk->msgWindow);
        GFL_BGSysQueueScrLoad(6);
        func_0201f7a8(wk->msgWindow, 2);
        BmpWin_Free(wk->msgWindow);
        wk->msgWindow = NULL;
    }
}

void func_ov119_021e10d8(PokemonTradeWork *wk) {
    wk->wordSet = GFL_WordSetSystemCreateDefault(wk->heapId);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_ABILITY_HANDLERS_3, wk->heapId);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, wk->heapId);
    wk->drawStr = GFL_StrBufCreate(128, wk->heapId);
    wk->drawTemplate = GFL_StrBufCreate(128, wk->heapId);
    wk->strbuf = GFL_StrBufCreate(128, wk->heapId);
    wk->strbufTemplate = GFL_StrBufCreate(128, wk->heapId);
    wk->printQueue = func_0201c440(wk->heapId);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
    wk->tcbEx = GFL_TCBExMgrCreate(wk->heapId, wk->heapId, 2, 0);
    wk->keyCursor = func_02028590(15, TRUE, FALSE, wk->heapId);
    wk->taskMenuRes = func_02027f60(6, 9, wk->font, wk->printQueue, wk->heapId);
}

void func_ov119_021e11a8(PokemonTradeWork *wk) {
    int i;
    GFL_VBlankResetCallback();
    if (wk->tcbEx != NULL) {
        func_02028604(wk->keyCursor);
        func_ov119_021e106c(wk);
        func_ov119_021e12f8(wk);
        if (wk->printStream != NULL) {
            func_0201ce48(wk->printStream);
            wk->printStream = NULL;
        }
        if (wk->menuWin != NULL) {
            func_02028148(wk->menuWin);
            wk->menuWin = NULL;
        }
        if (wk->taskMenuRes != NULL) {
            func_02027fd8(wk->taskMenuRes);
            wk->taskMenuRes = NULL;
        }
        if (wk->summaryWindow != NULL) {
            BmpWin_Free(wk->summaryWindow);
            wk->summaryWindow = NULL;
        }
        for (i = 0; i < 2; i++) {
            if (wk->unk5BC[i] != NULL) {
                BmpWin_Free(wk->unk5BC[i]);
                wk->unk5BC[i] = NULL;
            }
        }
        GFL_WordSetSystemFree(wk->wordSet);
        GFL_MsgDataFree(wk->msgData);
        GFL_FontFree(wk->font);
        GFL_StrBufFree(wk->drawStr);
        GFL_StrBufFree(wk->drawTemplate);
        GFL_StrBufFree(wk->strbuf);
        GFL_StrBufFree(wk->strbufTemplate);
        func_0201c6ec(wk->printQueue);
        func_0201c4c0(wk->printQueue);
        GFL_TCBExMgrFree(wk->tcbEx);
        wk->tcbEx = NULL;
    }
}



BOOL func_ov119_021e12ac(PokemonTradeWork *wk) {
    BOOL done = TRUE;
    if (wk->printStream != NULL) {
        if (wk->msgWindow != NULL) {
            func_020286c4(wk->keyCursor, wk->printStream, wk->msgWindow);
        }
        done = func_02028484(&wk->printWait, wk->printStream);
        if (done && wk->printStream != NULL) {
            func_0201ce48(wk->printStream);
            wk->printStream = NULL;
        }
    }
    return done;
}





void func_ov119_021e12f8(PokemonTradeWork *wk) {
    if (wk->menu != NULL) {
        G2_BlendNone();
        func_02027858(wk->menu);
        wk->menu = NULL;
    }
}

void func_ov119_021e131c(PokemonTradeWork *wk, const u32 *items, int count, u32 right, u32 bottom) {
    AppTaskMenuInit setup;
    int i;
    setup.heapId = wk->heapId;
    setup.itemCount = count;
    setup.items = wk->menuItems;
    setup.posType = APP_TASKMENU_POS_BOTTOM_RIGHT;
    setup.x = right;
    setup.y = bottom;
    setup.width = 13;
    setup.height = 3;
    for (i = 0; i < count; i++) {
        wk->menuItems[i].str = GFL_StrBufCreate(100, wk->heapId);
        GFL_MsgDataLoadStrbuf(wk->msgData, items[i], wk->menuItems[i].str);
        wk->menuItems[i].color = PRINT_COLOR(14, 15, 3);
        if (items[i] == 5) {
            wk->menuItems[i].type = APP_TASKMENU_ITEM_RETURN;
        } else {
            wk->menuItems[i].type = 0;
        }
    }
    wk->menu = func_02027778(&setup, wk->taskMenuRes);
    for (i = 0; i < count; i++) {
        GFL_StrBufFree(wk->menuItems[i].str);
    }
}



void func_ov119_021e1410(PokemonTradeWork *wk, const u32 *items, int count) {
    if (count == 1) {
        func_ov119_021e131c(wk, items, count, 32, 21);
    } else {
        func_ov119_021e131c(wk, items, count, 32, 24);
    }
}

void func_ov119_021e1430(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, BOOL speciesName,
                                PokemonTradeWork *wk) {
    if (!isEgg) {
        if (speciesName) {
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_028, wk->drawTemplate);
            setPartyPokemonSpeciesNameToStrbuf(wk->wordSet, 0, pkm);
        } else {
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_029, wk->drawTemplate);
            loadPokemonNicknameToStrbuf(wk->wordSet, 0, pkm);
        }
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
    } else {
        GFL_MsgDataLoadStrbuf(wk->msgData, 105, wk->drawStr);
    }
    GFL_TextRendererDrawToBitmap(bitmap, x, y, wk->drawStr, wk->font);
}

void func_ov119_021e14dc(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, PokemonTradeWork *wk) {
    if (!isEgg) {
        u32 number = PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
        if (!wk->nationalDex) {
            u16 *table = PML_PersonalLoadRegionalDexTable(wk->heapId, NULL);
            number = table[number];
            GFL_HeapFree(table);
        }
        if (number == 999) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 162, wk->drawStr);
        } else {
            WordSetNumber(wk->wordSet, 1, number, 3, 1, TRUE);
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_027, wk->drawTemplate);
            GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
        }
        GFL_TextRendererDrawToBitmap(bitmap, x, y, wk->drawStr, wk->font);
    }
}

void func_ov119_021e1594(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, BOOL isEgg, PokemonTradeWork *wk) {
    if (!isEgg) {
        GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_028, wk->drawTemplate);
        setPartyPokemonSpeciesNameToStrbuf(wk->wordSet, 0, pkm);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
        GFL_TextRendererDrawToBitmap(bitmap, x, y, wk->drawStr, wk->font);
    }
}

void func_ov119_021e15f8(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, PokemonTradeWork *wk) {
    PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
    GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_018, wk->drawTemplate);
    WordSetNumber(wk->wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL), 3, 1, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
    GFL_TextRendererDrawToBitmap(bitmap, x, y, wk->drawStr, wk->font);
}

void func_ov119_021e1670(PartyPkm *pkm, GFLBitmap *bitmap, int x, int y, PokemonTradeWork *wk, BOOL light,
                                BOOL speciesName) {
    int sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
    u32 showSex = PokeParty_GetParam(pkm, PKM_PARAM_NIDORAN_NICKNAME, NULL);
    u32 species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    u16 color;
    if (sex > GENDER_FEMALE || !showSex) {
        return;
    }
    if (speciesName && species == SPECIES_NIDORAN_F) {
        return;
    }
    if (speciesName && species == SPECIES_NIDORAN_M) {
        return;
    }
    if (light) {
        if (sex == GENDER_FEMALE) {
            color = PRINT_COLOR(12, 13, 0);
        } else {
            color = PRINT_COLOR(14, 15, 0);
        }
    } else {
        if (sex == GENDER_FEMALE) {
            color = PRINT_COLOR(3, 4, 0);
        } else {
            color = PRINT_COLOR(5, 6, 0);
        }
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, sex + 46, wk->drawStr);
    GFL_TextRendererDrawToBitmapEx(bitmap, x, y, wk->drawStr, wk->font, color);
}


void func_ov119_021e1718(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    int i;
    for (i = 0; i < 6; i++) {
        GFL_MsgDataLoadStrbuf(wk->msgData, i + 30, wk->drawStr);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y + i * 16, wk->drawStr, wk->font);
    }
}

void func_ov119_021e1778(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    u32 item;
    GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_043, wk->drawTemplate);
    item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
    if (item != 0) {
        loadItemNameToStrbuf(wk->wordSet, 0, item);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, wk->drawStr, wk->font);
    }
}

void func_ov119_021e17e8(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    u32 hp, maxHp;
    GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_039, wk->drawTemplate);
    hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
    maxHp = PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL);
    WordSetNumber(wk->wordSet, 0, hp, 3, 1, TRUE);
    WordSetNumber(wk->wordSet, 1, maxHp, 3, 1, TRUE);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, wk->drawStr, wk->font);
}

void func_ov119_021e1880(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    int i = 0;
    u32 params[] = { PKM_PARAM_ATTACK, PKM_PARAM_DEFENSE, PKM_PARAM_SP_ATTACK, PKM_PARAM_SP_DEFENSE, PKM_PARAM_SPEED };
    for (; i < 5; i++) {
        GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_040, wk->drawTemplate);
        WordSetNumber(wk->wordSet, 0, PokeParty_GetParam(pkm, params[i], NULL), 3, 1, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y + i * 16, wk->drawStr, wk->font);
    }
}









void func_ov119_021e194c(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_044, wk->drawStr);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, wk->drawStr, wk->font);
}

void func_ov119_021e198c(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    int i;
    for (i = 0; i < 4; i++) {
        u32 move = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);
        if (move != 0) {
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_045, wk->drawTemplate);
            loadMoveNameToStrbuf(wk->wordSet, 0, move);
            GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y + i * 16, wk->drawStr, wk->font);
        }
    }
}

void func_ov119_021e1a44(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_NATURES, wk->heapId);
    GFL_MsgDataLoadStrbuf(msgData, PokeParty_GetNature(pkm), wk->drawStr);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, wk->drawStr, wk->font);
    GFL_MsgDataFree(msgData);
}

void func_ov119_021e1aa8(PartyPkm *pkm, BmpWin *window, int x, int y, PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_042, wk->drawTemplate);
    loadAbilityNameToStrbuf(wk->wordSet, 0, PokeParty_GetParam(pkm, PKM_PARAM_ABILITY, NULL));
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, y, wk->drawStr, wk->font);
}

void func_ov119_021e1b14(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    int windowX[] = { 1, 18 };
    int ballX[] = { 0, 136 };
    int ballPalette[] = { 12, 5 };
    BOOL isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1c0, 0x20, wk->heapId);
    if (wk->unk5BC[side] != NULL) {
        BmpWin_Free(wk->unk5BC[side]);
    }
    wk->unk5BC[side] = BmpWin_CreateDynamic(3, windowX[side], 1, 14, 20, 14, 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov119_021e1430(pkm, BmpWin_GetBitmap(wk->unk5BC[side]), 16, 0, isEgg, func_ov119_021d8adc(wk), wk);
    if (!isEgg) {
        func_ov119_021e1670(pkm, BmpWin_GetBitmap(wk->unk5BC[side]), 80, 0, wk, FALSE, func_ov119_021d8adc(wk));
        GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
        func_ov119_021e15f8(pkm, BmpWin_GetBitmap(wk->unk5BC[side]), 16, 19, wk);
        func_ov119_021e1778(pkm, wk->unk5BC[side], 16, 128, wk);
        func_ov119_021e0e68(&wk->ballIcons[side + 1], pkm, wk->clactUnit, ballX[side] + 16, 16, 0, wk->heapId,
                            ballPalette[side]);
        func_0204b84c(wk->ballIcons[side + 1].actor, 1);
    } else {
        func_ov119_021e0e4c(&wk->ballIcons[side + 1]);
    }
    BmpWin_FlushChar(wk->unk5BC[side]);
    BmpWin_FlushMap(wk->unk5BC[side]);
    GFL_BGSysQueueScrLoad(3);
}






void func_ov119_021e1ca8(PokemonTradeWork *wk) {
    int i;
    for (i = 0; i < 3; i++) {
        func_ov119_021e0e4c(&wk->ballIcons[i]);
    }
}


void func_ov119_021e1cc8(PokemonTradeWork *wk) {
    int i;
    for (i = 0; i < 4; i++) {
        func_ov119_021e0da0(&wk->typeIcons[i]);
    }
}

void func_ov119_021e1ce8(PokemonTradeWork *wk, BOOL hideWindows) {
    func_ov119_021e5ef8(&wk->infoIcons[1]);
    func_ov119_021e5ef8(&wk->infoIcons[2]);
    func_ov119_021e625c(wk);
    if (hideWindows) {
        GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    }
    func_ov119_021e1ca8(wk);
    func_ov119_021e1cc8(wk);
    func_ov119_021e5ef8(&wk->infoIcons[0]);
    func_ov119_021e5be4(wk);
    func_ov119_021e36d8(wk, 0);
    func_ov119_021e36d8(wk, 1);
    func_020456f8(wk->summaryWindow);
    BmpWin_Free(wk->summaryWindow);
    wk->summaryWindow = NULL;
    func_ov119_021e3f70(wk);
}

void func_ov119_021e1d68(PokemonTradeWork *wk, PartyPkm *pkm) {
    GFL_BGSysSetBGEnabled(3, FALSE);
    func_ov119_021e3230(wk);
    func_ov119_021e1098(wk);
    func_ov119_021e2438(wk, 0);
    func_ov119_021e2438(wk, 1);
    func_ov119_021e5958(wk);
    func_ov119_021e219c(wk, pkm, 0, TRUE);
    if (!func_ov119_021d8aa8(wk)) {
        func_ov119_021e5998(wk);
        func_ov119_021e3f30(wk);
        GFL_BGSysSetEnabledBGsA(GX_PLANEMASK_BG0 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3 | GX_PLANEMASK_OBJ);
        GFL_BGSysSetEnabledBGsB(GX_PLANEMASK_ALL);
        G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -8);
        wk->unk11F9 = 1;
        GFL_VBlankTCBAdd(func_ov119_021e2a98, wk, 0);
    }
    func_ov067_021b6558(wk->touchBar, 1, TRUE);
}

void func_ov119_021e1e00(PokemonTradeWork *wk, int page, PartyPkm *pkm) {
    int windowX[] = { 0, 16 };
    int ballX[] = { 0, 128 };
    int side = 0;
    BOOL isEgg;
    if (wk->touchX < 128) {
        side = 1;
    }
    func_ov119_021e6294(wk, 11, 4);
    if (wk->summaryWindow != NULL) {
        BmpWin_Free(wk->summaryWindow);
    }
    wk->summaryWindow = BmpWin_CreateDynamic(6, windowX[side], 0, 16, 18, 11, 0);
    GFL_TextRndUpdateColorIndexLUT(14, 15, 0);
    isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    if (page == 0) {
        func_ov119_021e1430(pkm, BmpWin_GetBitmap(wk->summaryWindow), 32, 0, isEgg, FALSE, wk);
        if (!isEgg) {
            func_ov119_021e15f8(pkm, BmpWin_GetBitmap(wk->summaryWindow), 32, 16, wk);
            func_ov119_021e1718(pkm, wk->summaryWindow, 8, 32, wk);
            func_ov119_021e1778(pkm, wk->summaryWindow, 24, 128, wk);
            func_ov119_021e17e8(pkm, wk->summaryWindow, 64, 32, wk);
            func_ov119_021e1880(pkm, wk->summaryWindow, 80, 48, wk);
            func_ov119_021e1670(pkm, BmpWin_GetBitmap(wk->summaryWindow), 96, 0, wk, FALSE, FALSE);
            func_ov119_021e5dfc(wk, side, pkm);
            func_ov119_021e5f14(wk, side, FALSE, pkm);
            func_ov119_021e0e68(&wk->ballIcons[0], pkm, wk->clactUnit, ballX[side] + 16, 16, 1, wk->heapId, 13);
        } else {
            func_ov119_021e0e4c(&wk->ballIcons[0]);
        }
    } else {
        func_ov119_021e0e4c(&wk->ballIcons[0]);
        func_ov119_021e5ef8(&wk->infoIcons[0]);
        func_ov119_021e5ef8(&wk->infoIcons[1]);
        func_ov119_021e5ef8(&wk->infoIcons[2]);
        if (!isEgg) {
            func_ov119_021e194c(pkm, wk->summaryWindow, 8, 0, wk);
            func_ov119_021e198c(pkm, wk->summaryWindow, 16, 16, wk);
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_036, wk->drawStr);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->summaryWindow), 8, 80, wk->drawStr, wk->font);
            func_ov119_021e1a44(pkm, wk->summaryWindow, 16, 96, wk);
            GFL_MsgDataLoadStrbuf(wk->msgData, AbilityHandlers3_Text_037, wk->drawStr);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->summaryWindow), 8, 112, wk->drawStr, wk->font);
            func_ov119_021e1aa8(pkm, wk->summaryWindow, 16, 128, wk);
        }
    }
    func_ov119_021e40ec(wk, side, page);
    BmpWin_FlushMap(wk->summaryWindow);
    BmpWin_FlushChar(wk->summaryWindow);
    GFL_BGSysQueueScrLoad(6);
}




void func_ov119_021e2054(PokemonTradeWork *wk, PartyPkm *pkm, int side, BOOL reload) {
    func_ov119_021e36d8(wk, side);
    if (wk->summaryWindow != NULL) {
        BmpWin_Free(wk->summaryWindow);
    }
}

void func_ov119_021e2070(PokemonTradeWork *wk, PartyPkm *pkm, int *icons) {
    func_ov119_021e0da0(&wk->typeIcons[icons[0]]);
    func_ov119_021e0da0(&wk->typeIcons[icons[1]]);
    func_ov119_021e0dbc(&wk->typeIcons[icons[0]], pkm, 0, wk->clactUnit, 176, 95, 0, wk->heapId);
    if (PokeParty_GetParam(pkm, PKM_PARAM_TYPE1, NULL) != PokeParty_GetParam(pkm, PKM_PARAM_TYPE2, NULL)) {
        func_ov119_021e0dbc(&wk->typeIcons[icons[1]], pkm, 1, wk->clactUnit, 210, 95, 0, wk->heapId);
    }
    if (wk->typeIcons[icons[2]].actor != NULL) {
        func_0204b3dc(wk->typeIcons[icons[2]].actor, FALSE);
    }
    if (wk->typeIcons[icons[3]].actor != NULL) {
        func_0204b3dc(wk->typeIcons[icons[3]].actor, FALSE);
    }
}


void func_ov119_021e2140(PokemonTradeWork *wk, PartyPkm *pkm) {
    if (wk->typeIconPage != 0) {
        int icons[4] = { 0, 1, 2, 3 };
        func_ov119_021e2070(wk, pkm, icons);
        wk->typeIconPage = 0;
    } else {
        int icons[4] = { 2, 3, 0, 1 };
        func_ov119_021e2070(wk, pkm, icons);
        wk->typeIconPage = 1;
    }
}

void func_ov119_021e219c(PokemonTradeWork *wk, PartyPkm *pkm, int side, BOOL reload) {
    int i;
    VecFx32 pos;
    BOOL isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    func_ov119_021e2054(wk, pkm, side, reload);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1c0, 0x20, wk->heapId);
    if (wk->mcss[side] == NULL) {
        func_ov119_021e35a0(wk, side, TRUE, pkm, FALSE);
    } else if (reload) {
        func_ov119_021e35a0(wk, side, TRUE, pkm, FALSE);
    }
    pos.x = FX32_CONST(56);
    pos.y = FX32_CONST(4);
    pos.z = 0;
    func_02015c48(wk->mcss[side], &pos);
    func_02015eb0(wk->mcss[side]);
    if (wk->mcss[1 - side] != NULL) {
        func_02015ea0(wk->mcss[1 - side]);
    }
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    wk->summaryWindow = BmpWin_CreateDynamic(3, 1, 0, 31, 24, 14, 0);
    func_ov119_021e1430(pkm, BmpWin_GetBitmap(wk->summaryWindow), 16, 0, isEgg, FALSE, wk);
    if (!isEgg) {
        func_ov119_021e14dc(pkm, BmpWin_GetBitmap(wk->summaryWindow), 0, 16, isEgg, wk);
        func_ov119_021e1594(pkm, BmpWin_GetBitmap(wk->summaryWindow), 56, 16, isEgg, wk);
        func_ov119_021e15f8(pkm, BmpWin_GetBitmap(wk->summaryWindow), 96, 0, wk);
        func_ov119_021e1670(pkm, BmpWin_GetBitmap(wk->summaryWindow), 80, 0, wk, FALSE, FALSE);
        func_ov119_021e1718(pkm, wk->summaryWindow, 0, 32, wk);
        for (i = 0; i < 2; i++) {
            GFL_MsgDataLoadStrbuf(wk->msgData, i + 36, wk->drawStr);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->summaryWindow), 0, i * 16 + 136, wk->drawStr, wk->font);
        }
        func_ov119_021e17e8(pkm, wk->summaryWindow, 56, 32, wk);
        func_ov119_021e1880(pkm, wk->summaryWindow, 72, 48, wk);
        func_ov119_021e1a44(pkm, wk->summaryWindow, 56, 136, wk);
        func_ov119_021e1aa8(pkm, wk->summaryWindow, 56, 152, wk);
        func_ov119_021e1778(pkm, wk->summaryWindow, 40, 168, wk);
        func_ov119_021e194c(pkm, wk->summaryWindow, 152, 113, wk);
        func_ov119_021e198c(pkm, wk->summaryWindow, 152, 128, wk);
        func_ov119_021e2140(wk, pkm);
        func_ov119_021e5dfc(wk, 2, pkm);
        func_ov119_021e0e68(&wk->ballIcons[0], pkm, wk->clactUnit, 12, 7, 0, wk->heapId, 12);
    } else {
        if (wk->infoIcons[0].actor != NULL) {
            func_0204b3dc(wk->infoIcons[0].actor, FALSE);
        }
        func_ov119_021e1cc8(wk);
        func_ov119_021e1ca8(wk);
    }
    func_ov119_021e60bc(wk, pkm, isEgg);
    func_ov119_021e5f14(wk, 0, TRUE, pkm);
    BmpWin_FlushMap(wk->summaryWindow);
    BmpWin_FlushChar(wk->summaryWindow);
    GFL_BGSysQueueScrLoad(3);
}


void func_ov119_021e2438(PokemonTradeWork *wk, int side) {
    if (wk->unk5BC[side] != NULL) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->unk5BC[side]), 0);
        func_020456f8(wk->unk5BC[side]);
        BmpWin_Free(wk->unk5BC[side]);
        GFL_BGSysQueueScrLoad(3);
        wk->unk5BC[side] = NULL;
        func_ov119_021e0e4c(&wk->ballIcons[side + 1]);
    }
}

void func_ov119_021e2484(PokemonTradeWork *wk, BOOL clear) {
    func_ov119_021e0e4c(&wk->ballIcons[0]);
    func_ov119_021e5ef8(&wk->infoIcons[0]);
    func_ov119_021e5ef8(&wk->infoIcons[1]);
    func_ov119_021e5ef8(&wk->infoIcons[2]);
    if (wk->summaryWindow != NULL) {
        GFL_BitmapFill(BmpWin_GetBitmap(wk->summaryWindow), 0);
        if (clear) {
            func_020456f8(wk->summaryWindow);
            BmpWin_FlushChar(wk->summaryWindow);
        }
        BmpWin_Free(wk->summaryWindow);
        wk->summaryWindow = NULL;
    }
}

void func_ov119_021e24e8(PokemonTradeWork *wk, u32 a1) {
    int i, j;
    PlayerInfo *infos[2];
    BOOL isEgg;
    func_ov119_021e6cb8(wk);
    infos[0] = wk->partnerInfo;
    infos[1] = wk->myInfo;
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    for (i = 0; i < 2; i++) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 128, wk->drawTemplate);
        copyVarForText(wk->wordSet, 0, infos[i]);
        GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
        GFL_TextRendererDrawToBitmap(wk->negoBitmaps[i * 4], 0, 0, wk->drawStr, wk->font);
    }
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    for (i = 0; i < 2; i++) {
        GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
        for (j = 0; j < 3; j++) {
            PartyPkm *pkm = wk->negoPkm[1 - i][j];
            if (wk->negoSlot[1 - i][j] != -1) {
                isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
                func_ov119_021e6334(wk, i, j, pkm, FALSE, TRUE);
                func_ov119_021e1430(pkm, wk->negoBitmaps[i * 4 + 1 + j], 0, 0, isEgg, func_ov119_021d8adc(wk), wk);
                if (!isEgg) {
                    func_ov119_021e1670(pkm, wk->negoBitmaps[i * 4 + 1 + j], 56, 16, wk, FALSE, func_ov119_021d8adc(wk));
                    func_ov119_021e15f8(pkm, wk->negoBitmaps[i * 4 + 1 + j], 0, 16, wk);
                }
            }
        }
    }
    func_ov119_021e6e7c(wk);
}





void func_ov119_021e2680(PokemonTradeWork *wk) {
    int i;
    for (i = 0; i < 4; i++) {
        if (wk->unk1038[i] != NULL) {
            BmpWin_Free(wk->unk1038[i]);
            wk->unk1038[i] = NULL;
        }
    }
    func_ov119_021e6dec(wk);
}

void func_ov119_021e26ac(PokemonTradeWork *wk) {
    G2S_SetBlendBrightness(GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, -8);
    G2S_SetWnd0InsidePlane(GX_PLANEMASK_ALL, TRUE);
    G2S_SetWnd0Position(0, 0, 255, 192);
    G2S_SetWnd1InsidePlane(GX_PLANEMASK_ALL, TRUE);
    G2S_SetWnd1Position(255, 0, 0, 192);
    G2S_SetWndOutsidePlane(GX_PLANEMASK_ALL, FALSE);
    GXS_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
}

void func_ov119_021e272c(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    int windowX[] = { 1, 18 };
    int ballX[] = { 0, 136 };
    int colors[] = { 9, 8, 11, 10 };
    int ballPalette[] = { 12, 5 };
    BOOL isEgg = PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL);
    if (wk->unk5C4[side] != NULL) {
        BmpWin_Free(wk->unk5C4[side]);
        BmpWin_Free(wk->unk5CC[side]);
    }
    wk->unk5C4[side] = BmpWin_CreateDynamic(1, windowX[side], 0, 14, 5, 5, 0);
    wk->unk5CC[side] = BmpWin_CreateDynamic(1, windowX[side], 17, 13, 2, 5, 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->unk5C4[side]), colors[side * 2] | (colors[side * 2] << 4));
    GFL_BitmapFill(BmpWin_GetBitmap(wk->unk5CC[side]), colors[side * 2 + 1] | (colors[side * 2 + 1] << 4));
    GFL_TextRndUpdateColorIndexLUT(1, 2, colors[side * 2]);
    func_ov119_021e1430(pkm, BmpWin_GetBitmap(wk->unk5C4[side]), 16, 4, isEgg, func_ov119_021d8adc(wk), wk);
    if (!isEgg) {
        func_ov119_021e1670(pkm, BmpWin_GetBitmap(wk->unk5C4[side]), 80, 4, wk, TRUE, func_ov119_021d8adc(wk));
        func_ov119_021e15f8(pkm, BmpWin_GetBitmap(wk->unk5C4[side]), 16, 19, wk);
        GFL_TextRndUpdateColorIndexLUT(1, 2, colors[side * 2 + 1]);
        func_ov119_021e1778(pkm, wk->unk5CC[side], 0, 0, wk);
        func_ov119_021e0e68(&wk->ballIcons[side + 1], pkm, wk->clactUnit, ballX[side] + 16, 16, 0, wk->heapId,
                            ballPalette[side]);
    } else {
        func_ov119_021e1ca8(wk);
    }
    BmpWin_FlushChar(wk->unk5C4[side]);
    BmpWin_FlushChar(wk->unk5CC[side]);
    BmpWin_FlushMap(wk->unk5C4[side]);
    BmpWin_FlushMap(wk->unk5CC[side]);
    GFL_BGSysQueueScrLoad(1);
    wk->unk11E2[side] = 120;
}



void func_ov119_021e293c(PokemonTradeWork *wk, int side) {
    if (wk->unk5C4[side] != NULL) {
        BmpWin_Free(wk->unk5C4[side]);
        BmpWin_Free(wk->unk5CC[side]);
        func_ov119_021e0e4c(&wk->ballIcons[side + 1]);
        wk->unk5C4[side] = NULL;
        wk->unk5CC[side] = NULL;
    }
}

AppTaskMenuWin *func_ov119_021e2984(PokemonTradeWork *wk, u32 msg) {
    wk->menuItems[0].str = GFL_StrBufCreate(100, wk->heapId);
    GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->menuItems[0].str);
    wk->menuItems[0].color = PRINT_COLOR(14, 15, 3);
    wk->menuItems[0].type = 0;
    wk->menuWin = func_0202800c(wk->taskMenuRes, &wk->menuItems[0], 8, 21, 16, 3, 0, 1, wk->heapId);
    GFL_StrBufFree(wk->menuItems[0].str);
    func_0204b3dc(wk->actors[2], FALSE);
    func_ov067_021b6558(wk->touchBar, 8, FALSE);
    return wk->menuWin;
}


void func_ov119_021e2a1c(PokemonTradeWork *wk, BOOL showActor, BOOL showButton) {
    if (!func_0201c6b4(wk->printQueue)) {
        func_0201c6ec(wk->printQueue);
    }
    if (showActor) {
        func_0204b3dc(wk->actors[2], TRUE);
    }
    if (showButton) {
        func_ov067_021b6558(wk->touchBar, 8, func_ov119_021dd2f4(wk));
    }
    if (wk->menuWin != NULL) {
        func_02028148(wk->menuWin);
        wk->menuWin = NULL;
        GFL_BGSysQueueScrLoad(6);
    }
}



BOOL func_ov119_021e2a80(PokemonTradeWork *wk) {
    static const TouchRect data_ov119_021e792c[] = {
        { 160, 192, 64, 192 },
        { TOUCH_RECT_END, 0, 0, 0 },
    };
    if (func_02035c08(data_ov119_021e792c) == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021e2a98(TCB *tcb, void *work) {
    PokemonTradeWork *wk = work;
    G2S_SetWnd1InsidePlane(GX_PLANEMASK_OBJ, FALSE);
    G2S_SetWnd1Position(48, 0, 208, 32);
    G2S_SetWnd0InsidePlane(GX_PLANEMASK_BG0 | GX_PLANEMASK_OBJ, FALSE);
    G2S_SetWnd0Position(224, 169, 255, 192);
    G2S_SetWndOutsidePlane(GX_PLANEMASK_ALL, TRUE);
    GXS_SetVisibleWnd(GX_WNDMASK_W0 | GX_WNDMASK_W1);
    wk->unk11F9 = 0;
    GFL_TCBRemove(tcb);
}






