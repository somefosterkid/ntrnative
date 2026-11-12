// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_nego.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "text/system/ability_handlers_btl_main.h"
#include "constants/arc.h"
#include "app/ui/touchbar.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/nhttp_rap.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "pml/item.h"
#include "pml/mail.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/wipe.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "gfl/graphics.h"

// Negotiating a trade: each player offers up to three Pokémon and chooses one of the other's, and a trade over the
// Wi-Fi Club checks the chosen Pokémon with the server first

// The message with the trainer name that a Pokémon gets when the server finds it invalid, which is the version's
#ifdef BLACK2
#define MSG_INVALID_PKM_TRAINER 25
#else
#define MSG_INVALID_PKM_TRAINER 24
#endif

static void func_ov119_021dd080(NetHandle *handle, u32 timing, PokemonTradeWork *wk);
void func_ov119_021dd09c(PokemonTradeWork *wk);
BOOL func_ov119_021dd140(PokemonTradeWork *wk);
int func_ov119_021dd21c(PokemonTradeWork *wk);
int func_ov119_021dd428(PokemonTradeWork *wk, int side, int slot, int box, PartyPkm *pkm);
void func_ov119_021dda50(PokemonTradeWork *wk);
void func_ov119_021ddad4(PokemonTradeWork *wk);
static void func_ov119_021ddb10(PokemonTradeWork *wk);
static void func_ov119_021ddb64(PokemonTradeWork *wk);
void func_ov119_021ddb90(PokemonTradeWork *wk);
void func_ov119_021ddbf0(PokemonTradeWork *wk);
static void func_ov119_021ddcf0(PokemonTradeWork *wk);
void func_ov119_021ddd60(PokemonTradeWork *wk);
void func_ov119_021dddb8(PokemonTradeWork *wk);
void func_ov119_021dddf4(PokemonTradeWork *wk);
void func_ov119_021dde4c(PokemonTradeWork *wk);
void func_ov119_021dde90(PokemonTradeWork *wk);
void func_ov119_021dded4(PokemonTradeWork *wk);
void func_ov119_021ddf28(PokemonTradeWork *wk);
void func_ov119_021ddf74(PokemonTradeWork *wk);
void func_ov119_021de0f0(PokemonTradeWork *wk);
void func_ov119_021de12c(PokemonTradeWork *wk);
void func_ov119_021de1b4(PokemonTradeWork *wk, int index);
BOOL func_ov119_021de208(PokemonTradeWork *wk);
void func_ov119_021de2c4(PokemonTradeWork *wk);
void func_ov119_021de3bc(PokemonTradeWork *wk);
void func_ov119_021de3e8(PokemonTradeWork *wk);
void func_ov119_021de518(PokemonTradeWork *wk);
void func_ov119_021de5c4(PokemonTradeWork *wk);
void func_ov119_021de66c(PokemonTradeWork *wk);
void func_ov119_021de6ac(PokemonTradeWork *wk);
static void func_ov119_021de70c(PokemonTradeWork *wk);
BOOL func_ov119_021de754(PokemonTradeWork *wk, int index);
void func_ov119_021de820(PokemonTradeWork *wk);
static void func_ov119_021de8c0(PokemonTradeWork *wk);
void func_ov119_021de918(PokemonTradeWork *wk);
void func_ov119_021de950(PokemonTradeWork *wk);
void func_ov119_021de988(PokemonTradeWork *wk);
void func_ov119_021de9e8(PokemonTradeWork *wk);
void func_ov119_021dea24(PokemonTradeWork *wk);
void func_ov119_021deb34(PokemonTradeWork *wk);
void func_ov119_021deba0(PokemonTradeWork *wk);
static void func_ov119_021debd4(PokemonTradeWork *wk);
void func_ov119_021dec4c(PokemonTradeWork *wk);
void func_ov119_021deca0(PokemonTradeWork *wk);
static void func_ov119_021ded58(PokemonTradeWork *wk);
void func_ov119_021ded74(PokemonTradeWork *wk);
static void func_ov119_021dedbc(PokemonTradeWork *wk);
void func_ov119_021dee00(PokemonTradeWork *wk);
void func_ov119_021deea8(PokemonTradeWork *wk);
void func_ov119_021deefc(PokemonTradeWork *wk);
void func_ov119_021def1c(PokemonTradeWork *wk);
void func_ov119_021df0b8(PokemonTradeWork *wk);
void func_ov119_021df168(PokemonTradeWork *wk);
void func_ov119_021df1c8(PokemonTradeWork *wk);
void func_ov119_021df210(PokemonTradeWork *wk);
static void func_ov119_021df338(PokemonTradeWork *wk);
void func_ov119_021df37c(PokemonTradeWork *wk);
static void func_ov119_021df3e4(PokemonTradeWork *wk);
void func_ov119_021df404(PokemonTradeWork *wk);
void func_ov119_021df43c(PokemonTradeWork *wk);
static void func_ov119_021df490(PokemonTradeWork *wk);
void func_ov119_021df4cc(PokemonTradeWork *wk);
void func_ov119_021df510(PokemonTradeWork *wk);
void func_ov119_021df548(PokemonTradeWork *wk);
static void func_ov119_021df59c(PokemonTradeWork *wk);
static void func_ov119_021df6ec(PokemonTradeWork *wk);
BOOL func_ov119_021df9e8(PokemonTradeWork *wk, PartyPkm *pkm, int command);

// The six Pokémon on the screen, the three of each player
// pokebw2's sNegoPkmRects
static TouchRect data_ov119_021e80c4[] = {
    { 0x18, 0x44, 0x08, 0x78 }, { 0x48, 0x74, 0x08, 0x78 }, { 0x78, 0xa4, 0x08, 0x78 }, { 0x18, 0x44, 0x88, 0xf8 },
    { 0x48, 0x74, 0x88, 0xf8 }, { 0x78, 0xa4, 0x88, 0xf8 }, { TOUCH_RECT_END },
};

static void func_ov119_021dd080(NetHandle *handle, u32 timing, PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        func_02039c68(handle, timing, 8);
    }
}

void func_ov119_021dd09c(PokemonTradeWork *wk) {
    if (func_020355c4()) {
        func_ov119_021e68f4(wk, -1);
    } else if (wk->unk10A0 != -1) {
        func_ov119_021e68f4(wk, wk->unk10A0);
    }
}

BOOL func_ov119_021dd0cc(PokemonTradeWork *wk, BoxPkm *pkm) {
    static const u32 sBlockedFields[15] = {
#include "asset/pokemontrade_nego_sBlockedFields.inc"
};
    u32 species;
    BOOL wasEncrypted;
    u32 found;
    u32 i;

    species = PML_PkmGetParam(pkm, PKM_PARAM_SPECIES, NULL);
    if (wk->type == 3) {
        wasEncrypted = PML_PkmDecrypt(pkm);
        found = 0;
        for (i = 0; i < 15; i++) {
            found = PML_PkmGetParam(pkm, sBlockedFields[i], NULL);
            if (found == TRUE) {
                break;
            }
        }
        PML_PkmReEncrypt(pkm, wasEncrypted);
        if (found) {
            return TRUE;
        }
        if (PML_PkmIsLegendNational(species)) {
            return TRUE;
        }
    }
    return FALSE;
}


BOOL func_ov119_021dd140(PokemonTradeWork *wk) {
    if (wk->unk11F1 != 0) {
        if (func_ov119_021d8adc(wk)) {
            wk->unk11F1--;
        } else {
            wk->unk11F1 = 0;
        }
    }
    if (wk->unk11F1 == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021dd174(PokemonTradeWork *wk) {
    int side, i;

    for (side = 0; side < 2; side++) {
        for (i = 0; i < 3; i++) {
            if (wk->negoPkm[side][i] != NULL) {
                GFL_HeapFree(wk->negoPkm[side][i]);
                wk->negoPkm[side][i] = NULL;
            }
        }
    }
}

void func_ov119_021dd1b8(PokemonTradeWork *wk) {
    int side, i;

    for (side = 0; side < 2; side++) {
        for (i = 0; i < 3; i++) {
            wk->negoSlot[side][i] = -1;
            wk->negoBox[side][i] = -1;
            if (wk->negoPkm[side][i] != NULL) {
                MI_CpuFill8(wk->negoPkm[side][i], 0, PokeParty_GetPkmRawSize());
            }
        }
    }
}



int func_ov119_021dd21c(PokemonTradeWork *wk) {
    int i;
    int count = 0;

    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[0][i] != -1 && wk->negoBox[0][i] != -1) {
            count++;
        }
    }
    return count;
}

BOOL func_ov119_021dd24c(PokemonTradeWork *wk) {
    if (wk->type == 3) {
        if (wk->menuWin == NULL) {
            func_ov119_021e2984(wk, 0x9a);
        }
        if (func_ov119_021e2a80(wk)) {
            func_ov119_021d89cc(wk);
            func_ov119_021d8e84(wk, func_ov119_021d9434);
            return TRUE;
        }
    }
    return FALSE;
}





void func_ov119_021dd294(PokemonTradeWork *wk, u32 a1, u32 a2) {
    if (wk->type == 3) {
        func_ov119_021e2a1c(wk, a1, a2);
    }
}

BOOL func_ov119_021dd2a8(PokemonTradeWork *wk) {
    BoxSaveAccessor *boxes = GameData_GetBoxSaveAccessor(wk->gameData);
    int offered = func_ov119_021dd21c(wk);
    int count = PokeParty_GetPkmCount(GameData_GetParty(wk->gameData)) + howManyTotalPokesAreInBoxes(boxes);

    if (count == 2 && offered >= 1) {
        return TRUE;
    }
    if (count > 2 && offered >= 2) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov119_021dd2f4(PokemonTradeWork *wk) {
    int i;

    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 3; i++) {
            if (wk->negoSlot[0][i] != -1 && wk->negoBox[0][i] != -1) {
                return TRUE;
            }
        }
    } else if (wk->selectSlot != -1 && wk->selectBox != -1) {
        return TRUE;
    }
    return FALSE;
}



int func_ov119_021dd34c(PokemonTradeWork *wk, int box, int slot) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[0][i] == slot && wk->negoBox[0][i] == box) {
            return i;
        }
    }
    return -1;
}

void func_ov119_021dd380(PokemonTradeWork *wk, int index, PartyPkm *pkm) {
    wk->negoSlot[1][index] = index;
    wk->negoBox[1][index] = index;
    if (wk->negoPkm[1][index] == NULL) {
        wk->negoPkm[1][index] =
            GFL_HeapAllocate(wk->heapId, PokeParty_GetPkmRawSize(), TRUE, "pokemontrade_nego.c", 290);
    }
    MI_CpuCopy8(pkm, wk->negoPkm[1][index], PokeParty_GetPkmRawSize());
}

int func_ov119_021dd3d4(PokemonTradeWork *wk, int side, int slot, int box) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[side][i] == slot && wk->negoBox[side][i] == box) {
            return -1;
        }
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[side][i] == -1) {
            return i;
        }
    }
    return 2;
}

int func_ov119_021dd428(PokemonTradeWork *wk, int side, int slot, int box, PartyPkm *pkm) {
    int i;

    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[side][i] == slot && wk->negoBox[side][i] == box) {
            return -1;
        }
    }
    for (i = 0; i < 3; i++) {
        if (wk->negoSlot[side][i] == -1 || i == 2) {
            wk->negoSlot[side][i] = slot;
            wk->negoBox[side][i] = box;
            if (wk->negoPkm[side][i] == NULL) {
                wk->negoPkm[side][i] =
                    GFL_HeapAllocate(wk->heapId, PokeParty_GetPkmRawSize(), TRUE, "pokemontrade_nego.c", 332);
            }
            MI_CpuCopy8(pkm, wk->negoPkm[side][i], PokeParty_GetPkmRawSize());
            func_ov119_021df918(wk, side, pkm);
            return i;
        }
    }
    return -1;
}




void func_ov119_021dd4f4(PokemonTradeWork *wk, int side, int index) {
    wk->negoSlot[side][index] = -1;
    wk->negoBox[side][index] = -1;
    if (wk->negoPkm[side][index] != NULL) {
        GFL_HeapFree(wk->negoPkm[side][index]);
    }
    wk->negoPkm[side][index] = NULL;
}



void func_ov119_021dd528(PokemonTradeWork *wk, u32 vram) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(7, wk->heapId);
    u16 offset = vram == 1 ? 0x60 : 0;
    wk->objRes[TRADE_OBJRES_PLTT_NEGO] = func_0204acec(arc, func_0201bc3c(), vram, offset, wk->heapId);
    wk->objRes[TRADE_OBJRES_CELL_NEGO] = func_0204af28(arc, func_0201bc78(), getOBJTileMapping_MainEng(), wk->heapId);
    GFL_ArcToolFree(arc);
}

void func_ov119_021dd588(PokemonTradeWork *wk) {
    if (wk->objRes[TRADE_OBJRES_PLTT_NEGO] != 0) {
        func_0204ada4(wk->objRes[TRADE_OBJRES_PLTT_NEGO]);
        wk->objRes[TRADE_OBJRES_PLTT_NEGO] = 0;
    }
    if (wk->objRes[TRADE_OBJRES_CELL_NEGO] != 0) {
        func_0204afd8(wk->objRes[TRADE_OBJRES_CELL_NEGO]);
        wk->objRes[TRADE_OBJRES_CELL_NEGO] = 0;
    }
}

BOOL func_ov119_021dd5b4(PokemonTradeWork *wk, int slot, int box) {
    PartyPkm *pkm;
    int index;
    u32 i;

    func_020399a8();
    pkm = func_ov119_021d8d4c(wk->boxes, wk->selectBox, wk->selectSlot, wk);
    if (func_ov119_021d8a24(wk)) {
        index = func_ov119_021dd428(wk, 0, slot, box, pkm);
        if (index != -1) {
            if (!func_ov119_021df9e8(wk, wk->negoPkm[0][index], TRADE_NET_CMD_OFFER + index)) {
                func_ov119_021dd4f4(wk, 0, index);
                GFL_HeapFree(pkm);
                return FALSE;
            }
            for (i = 0; i < PokeParty_GetPkmRawSize(); i++) {
            }
        }
    } else {
        index = func_ov119_021dd428(wk, 0, slot, box, pkm);
    }
    if (index == -1) {
        GFL_HeapFree(pkm);
        return TRUE;
    }
    func_ov119_021e6334(wk, 0, index, pkm, TRUE, FALSE);
    GFL_HeapFree(pkm);
    return TRUE;
}

void func_ov119_021dd68c(PokemonTradeWork *wk) {
    u32 xs[] = { 1, 17 };
    PlayerInfo *infos[2];
    BmpWin *window;
    int i;

    infos[0] = wk->myInfo;
    infos[1] = wk->partnerInfo;
    GFL_BGSysLoadNCLRDefault(getUINarcIdx(), 31, 0, 0x1a0, 0x20, wk->heapId);
    if (wk->unk5D4 != NULL) {
        BmpWin_Free(wk->unk5D4);
        BmpWin_Free(wk->unk5D8);
    }
    window = BmpWin_CreateDynamic(3, 0, 0, 0x20, 2, 13, FALSE);
    wk->unk5D4 = window;
    GFL_TextRndUpdateColorIndexLUT(14, 15, 0);
    if (wk->type == 3 && wk->unk1050[0] != -1) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x85, wk->drawStr);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 8, 2, wk->drawStr, wk->font);
        GFL_MsgDataLoadStrbuf(wk->msgData, wk->unk1050[0] + 0x7b, wk->drawStr);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0x88, 2, wk->drawStr, wk->font);
    }
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    window = BmpWin_CreateDynamic(3, 0, 3, 0x20, 8, 5, FALSE);
    wk->unk5D8 = window;
    for (i = 0; i < 2; i++) {
        if (infos[i] != NULL) {
            int msg;
            int x;

            if (wk->type == 3 && wk->unk1048[i] != -1) {
                msg = 0x75;
            } else {
                msg = 0x80;
            }
            GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
            GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->drawTemplate);
            copyVarForText(wk->wordSet, 0, infos[i]);
            GFL_WordSetFormatStrbuf(wk->wordSet, wk->drawStr, wk->drawTemplate);
            x = xs[i] * 8;
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x, 2, wk->drawStr, wk->font);
            if (wk->type == 3 && wk->unk1048[i] != -1) {
                GFL_TextRndUpdateColorIndexLUT(3, 4, 0);
                GFL_MsgDataLoadStrbuf(wk->msgData, wk->unk1048[i] + 0x76, wk->drawStr);
                GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), x + 4, 0x22, wk->drawStr, wk->font);
            }
        }
    }
    BmpWin_FlushChar(window);
    BmpWin_FlushMap(window);
    GFL_BGSysQueueScrLoad(3);
}


void func_ov119_021dd90c(PokemonTradeWork *wk, int side, u32 msg, BOOL force) {
    u32 xs[] = { 2, 19 };
    BmpWin *window;

    if (force || (wk->unk11E8[side] != 3 && wk->unk11EA && func_ov119_021d8aa8(wk))) {
        if (wk->unk5BC[side] != NULL) {
            BmpWin_Free(wk->unk5BC[side]);
        }
        wk->unk5BC[side] = BmpWin_CreateDynamic(3, xs[side], 16, 12, 2, 5, FALSE);
        window = wk->unk5BC[side];
        GFL_TextRndUpdateColorIndexLUT(5, 6, 0);
        msg += 114;
        GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->drawStr);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 0, 0, wk->drawStr, wk->font);
        BmpWin_FlushChar(window);
        BmpWin_FlushMap(window);
        GFL_BGSysQueueScrLoad(3);
        wk->unk11E8[side] = 3;
    }
}


void func_ov119_021dd9dc(PokemonTradeWork *wk) {
    if (wk->unk5BC[0] != NULL) {
        func_020456f8(wk->unk5BC[0]);
        BmpWin_Free(wk->unk5BC[0]);
        wk->unk5BC[0] = NULL;
    }
    if (wk->unk5BC[1] != NULL) {
        func_020456f8(wk->unk5BC[1]);
        BmpWin_Free(wk->unk5BC[1]);
        wk->unk5BC[1] = NULL;
    }
    if (wk->unk5D4 != NULL) {
        func_020456f8(wk->unk5D4);
        BmpWin_Free(wk->unk5D4);
        wk->unk5D4 = NULL;
    }
    if (wk->unk5D8 != NULL) {
        func_020456f8(wk->unk5D8);
        BmpWin_Free(wk->unk5D8);
        wk->unk5D8 = NULL;
    }
    GFL_BGSysQueueScrLoad(3);
}

void func_ov119_021dda50(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        switch (choice) {
        case 0:
            GFL_WipeSet(3, 0, 0, 0, 6, 1, wk->heapId);
            func_ov119_021d8e84(wk, func_ov119_021de518);
            break;
        case 1:
            func_ov119_021d8980(wk);
            func_ov119_021debd4(wk);
            func_ov119_021dd09c(wk);
            func_ov119_021d8e84(wk, func_ov119_021dea24);
            break;
        }
    }
}




void func_ov119_021ddad4(PokemonTradeWork *wk) {
    u32 items[] = { 3, 5 };

    func_ov119_021e1410(wk, items, NELEMS(items));
    func_ov067_021b6558(wk->touchBar, 1, FALSE);
    func_ov119_021d8e84(wk, func_ov119_021dda50);
}


static void func_ov119_021ddb10(PokemonTradeWork *wk) {
    func_020399a8();
    if (func_ov119_021d8a24(wk) && !func_02039cb4(func_020399a8(), 0x19, 8)) {
        func_ov119_021dd24c(wk);
        return;
    }
    func_ov119_021e1098(wk);
    func_ov119_021dd294(wk, 0, 0);
    func_ov119_021d8980(wk);
    func_ov119_021dd09c(wk);
    func_ov119_021d8e84(wk, func_ov119_021dea24);
}

static void func_ov119_021ddb64(PokemonTradeWork *wk) {
    func_020399a8();
    if (func_ov119_021e12ac(wk)) {
        func_ov119_021dd080(func_020399a8(), 0x19, wk);
        func_ov119_021d8e84(wk, func_ov119_021ddb10);
    }
}

void func_ov119_021ddb90(PokemonTradeWork *wk) {
    u32 keys;

    func_020399a8();
    if (func_ov119_021e12ac(wk)) {
        keys = GCTX_HIDGetPressedKeys();
        if (keys) {
            func_020355d8(FALSE);
        }
        if (keys == 0) {
            keys = func_02035c60();
            if (keys) {
                func_020355d8(TRUE);
            }
            if (keys == 0) {
                return;
            }
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021e1030(wk);
        func_ov119_021d8e84(wk, func_ov119_021ddb64);
    }
}

void func_ov119_021ddbf0(PokemonTradeWork *wk) {
    int myId = func_0203d1a4(func_020399a8());
    int other = 1 - myId;
    int msg;

    if (func_ov119_021e12ac(wk) && func_ov119_021dd140(wk)) {
        if (func_ov119_021d8a24(wk)) {
            if (!func_02039cb4(func_020399a8(), 0x1d, 8)) {
                func_ov119_021dd24c(wk);
                return;
            }
        }
        func_ov119_021dd294(wk, 0, 0);
        if (wk->command[myId] == 13 && wk->command[other] == 13) {
            func_ov119_021dd080(func_020399a8(), 13, wk);
            func_ov119_021d8e84(wk, func_ov119_021da76c);
            return;
        }
        if (wk->command[other] == 5) {
            msg = 97;
        } else if (wk->command[myId] == 5) {
            msg = 98;
        } else if (wk->command[other] == 7) {
            msg = 99;
        } else if (wk->command[myId] == 7) {
            msg = 100;
        } else if (wk->command[other] == 8) {
            msg = 100;
        } else if (wk->command[myId] == 8) {
            msg = 99;
        } else if (wk->command[other] == 6) {
            msg = 97;
        } else if (wk->command[myId] == 6) {
            msg = 111;
        } else if (wk->command[other] == 14) {
            msg = 151;
        } else if (wk->command[myId] == 14) {
            msg = 150;
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
        func_ov119_021e1024(wk);
        wk->command[0] = 0;
        wk->command[1] = 0;
        func_ov119_021d8e84(wk, func_ov119_021ddb90);
    }
}



static void func_ov119_021ddcf0(PokemonTradeWork *wk) {
    u8 command = 13;
    if (func_ov119_021d94d4(wk)) {
        command = 5;
    } else if (func_ov119_021d95d0(wk)) {
        command = 7;
    } else if (func_ov119_021d95f0(wk)) {
        command = 8;
    } else if (func_ov119_021d9610(wk)) {
        command = 6;
    }
    if (func_0203d3a8(func_020399a8(), TRADE_NET_CMD_SELECT, 1, &command)) {
        func_ov119_021dd080(func_020399a8(), 0x1d, wk);
        func_ov119_021d8e84(wk, func_ov119_021ddbf0);
    }
}

void func_ov119_021ddd60(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk)) {
        wk->command[0] = 13;
        wk->command[1] = 13;
        func_ov119_021d8e84(wk, func_ov119_021ddbf0);
        return;
    }
    if (!func_02039cb4(func_020399a8(), 0x1e, 8)) {
        func_ov119_021dd24c(wk);
        return;
    }
    func_ov119_021dd294(wk, 0, 0);
    func_ov119_021d8e84(wk, func_ov119_021ddcf0);
}


void func_ov119_021dddb8(PokemonTradeWork *wk) {
    u8 command;

    command = 14;
    if (func_0203d3a8(func_020399a8(), 3 << 10, 1, &command)) {
        func_ov119_021dd080(func_020399a8(), 0x1d, wk);
        func_ov119_021d8e84(wk, func_ov119_021ddbf0);
    }
}

void func_ov119_021dddf4(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk)) {
        wk->command[0] = 14;
        wk->command[1] = 14;
        func_ov119_021d8e84(wk, func_ov119_021ddbf0);
        return;
    }
    if (!func_02039cb4(func_020399a8(), 0x1e, 8)) {
        func_ov119_021dd24c(wk);
        return;
    }
    func_ov119_021dd294(wk, 0, 0);
    func_ov119_021d8e84(wk, func_ov119_021dddb8);
}


void func_ov119_021dde4c(PokemonTradeWork *wk) {
    func_ov119_021dd080(func_020399a8(), 0x1e, wk);
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021ddd60);
}

void func_ov119_021dde90(PokemonTradeWork *wk) {
    func_ov119_021dd080(func_020399a8(), 0x1e, wk);
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021dddf4);
}

void func_ov119_021dded4(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        switch (choice) {
        case 0:
            func_ov119_021d8e84(wk, func_ov119_021dde4c);
            break;
        case 1:
            func_ov119_021d8e84(wk, func_ov119_021dde90);
            break;
        }
    }
}


void func_ov119_021ddf28(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[] = { 4, 5 };

        func_ov119_021e1410(wk, items, NELEMS(items));
        GFL_BGSysSetEnabledBGsA(0x1f);
        func_ov067_021b6558(wk->touchBar, 1, FALSE);
        func_ov119_021d8e84(wk, func_ov119_021dded4);
    }
}

void func_ov119_021ddf74(PokemonTradeWork *wk) {
    int myId = func_0203d1a4(func_020399a8());
    int other = 1 - myId;
    int msg;
    int i;

    if (func_ov119_021e12ac(wk) && func_ov119_021dd140(wk)) {
        if (func_ov119_021d8a24(wk)) {
            if (!func_02039cb4(func_020399a8(), 0x1c, 8)) {
                func_ov119_021dd24c(wk);
                return;
            }
        }
        func_ov119_021dd294(wk, 0, 0);
        if (wk->command[myId] == 11 && wk->command[other] == 11) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 146, wk->strbufTemplate);
            for (i = 0; i < 2; i++) {
                loadPokemonNicknameToStrbuf(wk->wordSet, i, func_ov119_021d8fa4(wk, i));
            }
            GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
            func_ov119_021e1024(wk);
            func_ov119_021d8e84(wk, func_ov119_021ddf28);
            return;
        }
        if (wk->command[other] == 12) {
            wk->unk11F1 = 0x78;
            msg = 147;
            if (wk->type == 3 && wk->unk11FA >= 3) {
                func_ov119_021d89cc(wk);
                func_ov119_021d8e84(wk, func_ov119_021d9434);
                return;
            }
        } else if (wk->command[myId] == 12) {
            wk->unk11F1 = 0x78;
            msg = 149;
            if (wk->type == 3 && wk->unk11FA >= 3) {
                func_ov119_021d89cc(wk);
                func_ov119_021d8e84(wk, func_ov119_021d9434);
                return;
            }
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
        func_ov119_021e1024(wk);
        wk->command[0] = 0;
        wk->command[1] = 0;
        if (func_ov119_021d8a24(wk)) {
            func_02039c68(func_020399a8(), 0x19, 8);
        }
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021de8c0);
    }
}


void func_ov119_021de0f0(PokemonTradeWork *wk) {
    u8 command;

    command = 11;
    if (func_0203d3a8(func_020399a8(), 3 << 10, 1, &command)) {
        func_02039c68(func_020399a8(), 0x1c, 8);
        func_ov119_021d8e84(wk, func_ov119_021ddf74);
    }
}

void func_ov119_021de12c(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    u32 size;

    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    if (!func_ov119_021d8a24(wk)) {
        wk->command[0] = 11;
        wk->command[1] = 11;
        pkm = wk->negoPkm[0][0];
        func_ov119_021d9468(wk, 1, pkm);
        size = PokeParty_GetPkmRawSize();
        MI_CpuCopy8(pkm, func_ov119_021d8fa4(wk, 1), size);
        func_ov119_021d8e84(wk, func_ov119_021ddf74);
    } else {
        func_ov119_021d8e84(wk, func_ov119_021de0f0);
    }
}


void func_ov119_021de1b4(PokemonTradeWork *wk, int index) {
    int slot = index % 3;
    int side = index / 3;
    int other = 1 - side;
    PartyPkm *pkm = wk->negoPkm[other][slot];

    if (func_ov119_021d89ec((u8 *)pkm)) {
        func_ov119_021e219c(wk, pkm, other, TRUE);
        func_ov119_021e6440(wk, side, slot);
    }
}

BOOL func_ov119_021de208(PokemonTradeWork *wk) {
    BOOL moved = FALSE;
    u32 keys;

    keys = GCTX_HIDGetPressedKeys();
    if (keys) {
        func_020355d8(FALSE);
    }
    if (keys == 0x10) {
        wk->unk10A0 = (wk->unk10A0 + 3) % 6;
        moved = TRUE;
    }
    keys = GCTX_HIDGetPressedKeys();
    if (keys) {
        func_020355d8(FALSE);
    }
    if (keys == 0x20) {
        wk->unk10A0 = (wk->unk10A0 - 3) % 6;
        if (wk->unk10A0 < 0) {
            wk->unk10A0 += 6;
        }
        moved = TRUE;
    }
    keys = GCTX_HIDGetPressedKeys();
    if (keys) {
        func_020355d8(FALSE);
    }
    if (keys == 0x40) {
        if (wk->unk10A0 == 0 || wk->unk10A0 == 3) {
            wk->unk10A0 += 3;
        }
        wk->unk10A0--;
        moved = TRUE;
    }
    keys = GCTX_HIDGetPressedKeys();
    if (keys) {
        func_020355d8(FALSE);
    }
    if (keys == 0x80) {
        if (wk->unk10A0 == 2 || wk->unk10A0 == 5) {
            wk->unk10A0 -= 3;
        }
        wk->unk10A0++;
        moved = TRUE;
    }
    return moved;
}

void func_ov119_021de2c4(PokemonTradeWork *wk) {
    ArcTool *arc;

    if (GFL_WipeIsFinished()) {
        GFL_BGSysSetBGEnabled(3, FALSE);
        func_ov119_021e6440(wk, -1, -1);
        func_ov119_021e1ce8(wk, FALSE);
        arc = GFL_ArcSysCreateFileHandle(0x66, wk->heapId);
        GFL_G2DIOLoadNSCRSync(arc, 11, 2, 0, (u16)wk->unk86C, 0, FALSE, wk->heapId);
        GFL_ArcToolFree(arc);
        func_ov119_021e3208(wk, 0);
        GFL_BGSysQueueScrLoad(3);
        wk->unk11F3 = 0;
        if (func_ov119_021d89ec((u8 *)func_ov119_021d8fa4(wk, 1))) {
            func_ov119_021d9468(wk, 1, func_ov119_021d8fa4(wk, 1));
        }
        if (func_ov119_021d89ec((u8 *)func_ov119_021d8fa4(wk, 0))) {
            func_ov119_021d9468(wk, 0, func_ov119_021d8fa4(wk, 0));
        }
        func_ov119_021debd4(wk);
        func_ov119_021dd09c(wk);
        func_ov119_021d8e84(wk, func_ov119_021dea24);
        func_ov119_021d8980(wk);
        GFL_WipeSet(3, 1, 1, 0, 6, 1, wk->heapId);
    }
}









void func_ov119_021de3bc(PokemonTradeWork *wk) {
    int wait = 3;

    if (func_0203d29c()) {
        wait = 10;
    }
    if (wk->timer > wait) {
        func_ov119_021d8e84(wk, func_ov119_021de3e8);
    }
}

void func_ov119_021de3e8(PokemonTradeWork *wk) {
    int touched;
    u32 keys;

    GFL_BGSysSetBGEnabled(3, TRUE);
    if (!GFL_WipeIsFinished()) {
        return;
    }
    func_ov067_021b63d0(wk->touchBar);
    if (func_ov067_021b6448(wk->touchBar) == 1) {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021de2c4);
        return;
    }
    if (func_ov067_021b6470(wk->touchBar) != -1) {
        return;
    }
    touched = func_02035c08(data_ov119_021e80c4);
    if (touched != -1) {
        func_020355d8(TRUE);
        wk->unk10A0 = touched;
        func_ov119_021e68f4(wk, -1);
        func_ov119_021de1b4(wk, wk->unk10A0);
        GFL_SndSEPlay(0x548);
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021de3bc);
        return;
    }
    if (func_020355c4()) {
        keys = GCTX_HIDGetPressedKeys();
        if (keys) {
            func_020355d8(FALSE);
        }
        if (keys != 0) {
            func_020355d8(FALSE);
            func_ov119_021e68f4(wk, wk->unk10A0);
            GFL_SndSEPlay(0x548);
            wk->timer = 0;
            func_ov119_021d8e84(wk, func_ov119_021de3bc);
            return;
        }
    }
    if (func_ov119_021de208(wk)) {
        GFL_SndSEPlay(0x548);
        func_ov119_021e68f4(wk, wk->unk10A0);
        func_ov119_021de1b4(wk, wk->unk10A0);
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021de3bc);
    }
}


void func_ov119_021de518(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        u16 cursor = wk->unk10A0;
        int slot = cursor % 3;
        int side = cursor / 3;
        PartyPkm *pkm = wk->negoPkm[1 - side][slot];

        wk->unk11F3 = 1;
        if (pkm != NULL && PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            func_ov119_021e1d68(wk, pkm);
            func_ov119_021e6440(wk, side, slot);
        }
        func_ov119_021debd4(wk);
        GFL_WipeSet(3, 1, 1, 0, 6, 1, wk->heapId);
        func_ov119_021d8980(wk);
        func_ov119_021dd09c(wk);
        func_ov119_021d8e84(wk, func_ov119_021de3e8);
    }
}

void func_ov119_021de5c4(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        switch (choice) {
        case 0:
            func_ov119_021d8e84(wk, func_ov119_021de12c);
            break;
        case 1:
            GFL_WipeSet(3, 0, 0, 0, 6, 1, wk->heapId);
            func_ov067_021b6548(wk->touchBar, 1, FALSE);
            func_ov119_021d8e84(wk, func_ov119_021de518);
            break;
        case 2:
            func_ov119_021debd4(wk);
            func_ov119_021d8980(wk);
            func_ov119_021dd09c(wk);
            func_ov119_021d8e84(wk, func_ov119_021dea24);
            break;
        }
    }
}


void func_ov119_021de66c(PokemonTradeWork *wk) {
    u32 items[] = { 4, 3, 5 };

    func_ov119_021e1410(wk, items, NELEMS(items));
    func_ov067_021b6558(wk->touchBar, 1, FALSE);
    func_ov119_021d8e84(wk, func_ov119_021de5c4);
}

void func_ov119_021de6ac(PokemonTradeWork *wk) {
    if (wk->unk11FB_0 == 1) {
        return;
    }
    if (func_ov119_021d8a24(wk)) {
        if (!func_0203d3a8(func_020399a8(), 0xc01, PokeParty_GetPkmRawSize(), wk->unk106C)) {
            return;
        }
    }
    wk->unk11FB_0 = 1;
    func_ov119_021d8e84(wk, func_ov119_021de66c);
}




static void func_ov119_021de70c(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        u8 inParty = func_ov119_021d8f20(wk, 0);
        // BUG: The command's receiver reads one byte, but a Pokémon's size is sent from the one-byte local
#ifdef BUGFIX
        if (!func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNK16, sizeof(inParty), &inParty)) {
#else
        if (!func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNK16, PokeParty_GetPkmRawSize(), &inParty)) {
#endif
            return;
        }
    }
    func_ov119_021d8e84(wk, func_ov119_021de6ac);
}

BOOL func_ov119_021de754(PokemonTradeWork *wk, int index) {
    int side = index / 3;
    int slot = index % 3;
    PartyPkm *pkm;
    u32 value;

    if (index != -1 && wk->negoSlot[1 - side][slot] != -1) {
        pkm = wk->negoPkm[1 - side][slot];
        value = wk->unkFF0[1 - side][slot];
        if (pkm != NULL && PokeParty_GetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            wk->unk10A0 = index;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov067_021b6558(wk->touchBar, 1, FALSE);
            if (side != 0) {
                func_ov119_021d8e84(wk, func_ov119_021ddad4);
            } else {
                func_ov119_021d9468(wk, side, pkm);
                MI_CpuCopy8(pkm, wk->unk106C, PokeParty_GetPkmRawSize());
                func_ov119_021d8f60(wk, 0, value);
                func_ov119_021d8e84(wk, func_ov119_021de70c);
            }
            return TRUE;
        }
    }
    return FALSE;
}


void func_ov119_021de820(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov119_021dc720(wk);
        func_ov119_021e57e8(0);
        func_ov119_021e5680(wk);
        wk->clactUnit = func_0204b100(0x55 << 2, 0, wk->heapId);
        func_ov119_021e10d8(wk);
        func_ov119_021e58a0(wk);
        func_ov119_021e467c(wk);
        func_ov119_021e3e80(wk);
        func_ov119_021e3fe4(wk);
        func_ov119_021e3f74(wk);
        func_ov119_021e3208(wk, 0);
        GFL_BGSysSetEnabledBGsA(0x1f);
        GFL_BGSysSetEnabledBGsB(0x1e);
        func_ov119_021e3c20(wk);
        func_0203d34c(FALSE, wk->heapId);
        GXS_SetVisibleWnd(GX_WNDMASK_NONE);
        func_ov119_021d8e84(wk, func_ov119_021dace4);
    }
}

static void func_ov119_021de8c0(PokemonTradeWork *wk) {
    func_020399a8();
    if (func_ov119_021e12ac(wk) && func_ov119_021dd140(wk) && wk->timer >= 120) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021de820);
    }
}

void func_ov119_021de918(PokemonTradeWork *wk) {
    u8 command;

    command = 12;
    if (func_0203d3a8(func_020399a8(), 12 << 8, 1, &command)) {
        func_02039c68(func_020399a8(), 0x1c, 8);
        func_ov119_021d8e84(wk, func_ov119_021ddf74);
    }
}

void func_ov119_021de950(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021de918);
}

void func_ov119_021de988(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        if (choice == 0) {
            func_ov119_021d8e84(wk, func_ov119_021de950);
        } else {
            func_ov119_021e1098(wk);
            func_ov119_021d8980(wk);
            func_ov119_021dd09c(wk);
            func_ov119_021d8e84(wk, func_ov119_021dea24);
        }
    }
}




void func_ov119_021de9e8(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[] = { 24, 25 };

        func_ov119_021e131c(wk, items, NELEMS(items), 32, 12);
        func_ov119_021d8e84(wk, func_ov119_021de988);
    }
}

void func_ov119_021dea24(PokemonTradeWork *wk) {
    u32 keys;
    int touched;

    func_ov067_021b63d0(wk->touchBar);
    if (func_ov067_021b6448(wk->touchBar) == 1) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x94, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov067_021b6548(wk->touchBar, 1, FALSE);
        func_ov119_021dd09c(wk);
        func_ov119_021d8e84(wk, func_ov119_021de9e8);
        return;
    }
    if (func_ov067_021b6470(wk->touchBar) == -1) {
        touched = func_02035c08(data_ov119_021e80c4);
        if (touched != -1) {
            func_020355d8(TRUE);
            GFL_SndSEPlay(0x548);
            func_ov119_021e68f4(wk, -1);
            if (func_ov119_021de754(wk, touched)) {
                return;
            }
        }
        if (func_020355c4()) {
            keys = GCTX_HIDGetPressedKeys();
            if (keys) {
                func_020355d8(FALSE);
            }
            if (keys != 0) {
                wk->unk10A0 %= 6;
                if (wk->unk10A0 < 0) {
                    wk->unk10A0 = 0;
                }
                func_ov119_021e68f4(wk, wk->unk10A0);
                GFL_SndSEPlay(0x548);
                return;
            }
        }
        keys = GCTX_HIDGetPressedKeys();
        if (keys) {
            func_020355d8(FALSE);
        }
        if (keys != 1 || !func_ov119_021de754(wk, wk->unk10A0)) {
            if (func_ov119_021de208(wk)) {
                func_ov119_021e68f4(wk, wk->unk10A0);
                GFL_SndSEPlay(0x548);
            }
        }
    }
}


void func_ov119_021deb34(PokemonTradeWork *wk) {
    u32 keys;

    if (func_ov119_021dd140(wk) && func_ov119_021e12ac(wk)) {
        keys = GCTX_HIDGetPressedKeys();
        if (keys) {
            func_020355d8(FALSE);
        }
        if (keys == 0) {
            keys = func_02035c60();
            if (keys) {
                func_020355d8(TRUE);
            }
            if (keys == 0) {
                return;
            }
        }
        if (func_020355c4()) {
            func_ov119_021e68f4(wk, -1);
        } else {
            func_ov119_021e68f4(wk, 0);
        }
        func_ov119_021e1098(wk);
        func_ov119_021d8980(wk);
        func_ov119_021d8e84(wk, func_ov119_021dea24);
    }
}


void func_ov119_021deba0(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x91, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021deb34);
}


// Shows the upper screen's window over the other player's side
static void func_ov119_021debd4(PokemonTradeWork *wk) {
    G2S_SetBlendBrightness(0x1b, -8);
    G2S_SetWnd0InsidePlane(0x1f, TRUE);
    G2S_SetWnd0Position(128, 0, 255, 169);
    G2S_SetWndOutsidePlane(0x1f, FALSE);
    GXS_SetVisibleWnd(GX_WNDMASK_W0);
    GFL_BGSysSetBGEnabled(3, TRUE);
    GFL_BGSysSetBGEnabled(5, TRUE);
    GFL_BGSysSetBGEnabled(4, TRUE);
}

void func_ov119_021dec4c(PokemonTradeWork *wk) {
    if (func_ov119_021e69c0(wk)) {
        if (func_020355c4()) {
            func_ov119_021e68f4(wk, -1);
            wk->unk10A0 = 0;
        } else {
            func_ov119_021e68f4(wk, 0);
            wk->unk10A0 = 0;
        }
        if (GFL_WipeIsFinished()) {
            func_ov119_021debd4(wk);
            func_ov119_021d8e84(wk, func_ov119_021deba0);
        }
    }
}

void func_ov119_021deca0(PokemonTradeWork *wk) {
    func_ov119_021e4000(wk);
    func_ov119_021df7d0(wk);
    func_ov119_021df8e0(wk);
    func_ov119_021e64b8(wk);
    func_ov119_021dd588(wk);
    func_ov119_021e4570(wk);
    func_ov119_021dd9dc(wk);
    func_ov119_021e4888(wk);
    func_ov119_021dd528(wk, 1);
    GFL_BGSysFillScrAsync(6, 0);
    func_0204b3dc(wk->actors[2], FALSE);
    func_ov067_021b6558(wk->touchBar, 1, TRUE);
    func_ov067_021b6548(wk->touchBar, 1, FALSE);
    func_ov119_021e24e8(wk, 6);
    wk->unk108C = 0;
    wk->unk1084 = 1;
    func_ov119_021e6b90(wk);
    GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
    func_ov119_021d8e84(wk, func_ov119_021dec4c);
}



static void func_ov119_021ded58(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov119_021d8e84(wk, func_ov119_021deca0);
    }
}

void func_ov119_021ded74(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk) || wk->unk1060 != 0) {
        GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021ded58);
    }
}


static void func_ov119_021dedbc(PokemonTradeWork *wk) {
    NetHandle *handle = func_020399a8();
    if (func_ov119_021d8a24(wk)) {
        u8 data = 1;
        if (func_0203d3a8(handle, TRADE_NET_CMD_UNKA, 1, &data)) {
            func_ov119_021d8e84(wk, func_ov119_021ded74);
        }
    } else {
        func_ov119_021d8e84(wk, func_ov119_021ded74);
    }
}

void func_ov119_021dee00(PokemonTradeWork *wk) {
    if (func_02039cb4(func_020399a8(), 0x15, 8)) {
        int msg;
        u8 result = wk->unk11E6[0];

        if (result == 0 && wk->unk11E6[1] == 0) {
            func_ov119_021d8e84(wk, func_ov119_021dedbc);
            return;
        }
        if (result == 0xf0) {
            msg = 158;
        } else if (result == 0xf1) {
            msg = 159;
        } else if (result == 0xf2) {
            msg = 160;
        } else if (result == 0xff) {
            msg = 161;
        } else if ((wk->unk11E6[1] & 0xf0) == 0xf0) {
            msg = 161;
        } else if (wk->unk11E6[1] != 0) {
            msg = 143;
        } else {
            msg = 144;
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
        func_ov119_021e1024(wk);
        wk->unk11F1 = 0x78;
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021de8c0);
    }
}


void func_ov119_021deea8(PokemonTradeWork *wk) {
    NetHandle *handle = func_020399a8();

    wk->unk109A = 0;
    if (func_0203d3a8(handle, 0xc11, 1, &wk->unk11E6[0])) {
        func_02039c68(func_020399a8(), 0x15, 8);
        if (wk->type == 2) {
            func_ov009_0215642c(FALSE);
        }
        func_ov119_021d8e84(wk, func_ov119_021dee00);
    }
}


void func_ov119_021deefc(PokemonTradeWork *wk) {
    if (wk->unk0 != NULL) {
        func_ov114_021b9774(wk->unk0);
        func_ov114_021b9838(wk->unk0);
        wk->unk0 = NULL;
        func_ov009_02156ed0();
    }
}




void func_ov119_021def1c(PokemonTradeWork *wk) {
    int i;
    u8 count;
    void *response;
    int status;
    int result;

    count = 0;
    status = func_ov114_021b99dc(wk->unk0);
    result = func_ov114_021b9790(wk->unk0);
    if (result == 0 && status == 200) {
        response = func_ov114_021b97f4(wk->unk0);
        wk->unk11E6[0] = func_ov114_021b99f0(response);
        if (wk->unk11E6[0] == 1 && wk->unk109A == 0) {
            for (i = 0; i < 3; i++) {
                if (func_ov119_021d89ec((u8 *)wk->negoPkm[1][i])) {
                    PartyPkm *pkm = wk->negoPkm[1][i];

                    if (func_ov114_021b99f4(response, count)) {
                        u32 item = PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL);
                        MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE,
                                                              TEXT_BANK_ABILITY_HANDLERS_BTL_MAIN, wk->heapId);
                        StrBuf *name = GFL_MsgDataLoadStrbufNew(msgData, AbilityHandlersBtlMain_Text_VersionName);

                        GFL_MsgDataFree(msgData);
                        if (PML_ItemIsMail(item)) {
                            MailData *mail = CreateMailData(wk->heapId);

                            PokeParty_GetParam(pkm, PKM_PARAM_MAIL, mail);
                            func_02008f88(mail, GFL_StrBufGetStringPtr(name));
                            PokeParty_SetParam(pkm, PKM_PARAM_MAIL, (uintptr_t)mail);
                            GFL_HeapFree(mail);
                        }
                        setNicknameToNick(pkm);
                        PokeParty_SetParam(pkm, PKM_PARAM_OT_NAME_RAW, (uintptr_t)GFL_StrBufGetStringPtr(name));
                        GFL_StrBufFree(name);
                    }
                    count++;
                }
            }
            wk->unk109A = 1;
            func_ov119_021d8e84(wk, func_ov119_021df0b8);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021deea8);
        }
    } else {
        u8 error;

        if (result == 15 || status == 200) {
            return;
        }
        switch (status) {
        case 400:
            error = 0xf0;
            break;
        case 401:
            error = 0xf1;
            break;
        case 408:
            error = 0xf2;
            break;
        default:
            error = 0xff;
            break;
        }
        wk->unk11E6[0] = error;
        func_ov114_021b9774(wk->unk0);
        func_ov119_021d8e84(wk, func_ov119_021deea8);
    }
    if (wk->unk0 != NULL) {
        func_ov114_021b99b8(wk->unk0);
        func_ov114_021b9838(wk->unk0);
        wk->unk0 = NULL;
        func_ov009_02156ed0();
        func_ov009_02156dc4(NULL, NULL);
    }
}


void func_ov119_021df0b8(PokemonTradeWork *wk) {
    int i;
    int count = 0;

    func_ov009_02156e24(0xd, 0x10000, wk->heapId);
    wk->unk0 = func_ov114_021b9800(wk->heapId, func_0200853c(wk->myInfo), wk->param->buffer);
    func_ov009_02156dc4((DWCRapErrorFunc)func_ov119_021deefc, wk);
    for (i = 0; i < 3; i++) {
        if (func_ov119_021d89ec((u8 *)wk->negoPkm[1][i])) {
            count++;
        }
    }
    func_ov114_021b98a0(wk->unk0, wk->heapId, PokeParty_GetPkmRawSize() * count, 2);
    for (i = 0; i < 3; i++) {
        if (func_ov119_021d89ec((u8 *)wk->negoPkm[1][i])) {
            func_ov114_021b98d8(wk->unk0, wk->negoPkm[1][i], PokeParty_GetPkmRawSize());
        }
    }
    func_ov114_021b98f8(wk->unk0);
    func_ov114_021b9748(wk->unk0);
    func_ov119_021d8e84(wk, func_ov119_021def1c);
}



void func_ov119_021df168(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov119_021e4888(wk);
        GFL_BGSysSetEnabledBGsB(4);
        killBrightnessEitherEngine(4);
        wk->unk11FA++;
        if (wk->unk11FA >= 3) {
            func_ov119_021d89cc(wk);
            func_ov119_021d8e84(wk, func_ov119_021d9434);
            return;
        }
        func_02039c68(func_020399a8(), 0x1f, 8);
        func_ov119_021d8e84(wk, func_ov119_021df0b8);
    }
}



void func_ov119_021df1c8(PokemonTradeWork *wk) {
    GFL_WipeSet(4, 0, 0, 0, 6, 1, wk->heapId);
    if (wk->type == 2) {
        func_ov009_0215642c(TRUE);
    }
    func_ov119_021d8e84(wk, func_ov119_021df168);
}

void func_ov119_021df210(PokemonTradeWork *wk) {
    int msg;
    int myId = func_0203d1a4(func_020399a8());
    int other = 1 - myId;

    if (!func_ov119_021d8a24(wk)) {
        myId = 0;
        other = 1;
    }
    if (func_ov119_021d8a24(wk)) {
        if (!func_02039cb4(func_020399a8(), 0x1a, 8)) {
            func_ov119_021dd24c(wk);
            return;
        }
    }
    func_ov119_021dd294(wk, 0, 0);
    wk->unk11EA = 0;
    wk->unk11E8[0] = 3;
    wk->unk11E8[1] = 3;
    if (wk->command[myId] == 10 && wk->command[other] == 10) {
        func_ov119_021dd90c(wk, 0, 1, TRUE);
        func_ov119_021dd90c(wk, 1, 1, TRUE);
        if (wk->type == 3) {
            func_ov119_021d8e84(wk, func_ov119_021df1c8);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021dedbc);
        }
        return;
    }
    if (wk->command[myId] == 4 && wk->command[other] == 4) {
        wk->param->next = TRADE_NEXT_END;
        func_ov119_021d8e84(wk, func_ov119_021d9434);
        return;
    }
    if (wk->command[other] == 4) {
        msg = 97;
    } else if (wk->command[myId] == 4) {
        msg = 102;
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->command[0] = 0;
    wk->command[1] = 0;
    wk->unk11E8[0] = 0;
    wk->unk11E8[1] = 0;
    func_ov119_021d8e84(wk, func_ov119_021daef0);
}


static void func_ov119_021df338(PokemonTradeWork *wk) {
    NetHandle *handle = func_020399a8();
    if (func_ov119_021d8a24(wk)) {
        u8 data = 1;
        if (func_0203d3a8(handle, TRADE_NET_CMD_UNKF, 1, &data)) {
            func_ov119_021d8e84(wk, func_ov119_021df210);
        }
    } else {
        func_ov119_021d8e84(wk, func_ov119_021df210);
    }
}

void func_ov119_021df37c(PokemonTradeWork *wk) {
    NetHandle *handle = func_020399a8();

    if (func_ov119_021e12ac(wk)) {
        if (func_ov119_021d8a24(wk)) {
            u8 command;

            command = 10;
            if (func_0203d3a8(handle, 3 << 10, 1, &command)) {
                func_02039c68(func_020399a8(), 0x1a, 8);
                func_ov119_021d8e84(wk, func_ov119_021df338);
            }
        } else {
            wk->command[0] = 10;
            wk->command[1] = 10;
            func_ov119_021d8e84(wk, func_ov119_021df338);
        }
    }
}


static void func_ov119_021df3e4(PokemonTradeWork *wk) {
    func_020399a8();
    if (func_ov119_021e12ac(wk)) {
        func_ov119_021d8e84(wk, func_ov119_021dbbd0);
    }
}

void func_ov119_021df404(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021df37c);
}

void func_ov119_021df43c(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        if (choice == 0) {
            func_ov119_021d8e84(wk, func_ov119_021df404);
        } else {
            func_ov119_021e1098(wk);
            func_ov119_021d8e84(wk, func_ov119_021df3e4);
        }
    }
}



static void func_ov119_021df490(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[] = { 24, 25 };
        func_ov119_021e131c(wk, items, NELEMS(items), 32, 12);
        func_ov119_021d8e84(wk, func_ov119_021df43c);
    }
}

void func_ov119_021df4cc(PokemonTradeWork *wk) {
    func_0203d1a4(func_020399a8());
    if (func_ov119_021e12ac(wk) && func_ov119_021dd140(wk)) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x89, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021d8e84(wk, func_ov119_021df490);
    }
}


void func_ov119_021df510(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    wk->unk11F1 = 0x78;
    func_ov119_021d8e84(wk, func_ov119_021df4cc);
}

void func_ov119_021df548(PokemonTradeWork *wk) {
    if (func_020279e8(wk->menu)) {
        u8 choice = func_02027a04(wk->menu);

        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        if (choice == 0) {
            func_ov119_021d8e84(wk, func_ov119_021df510);
        } else {
            func_ov119_021e1098(wk);
            func_ov119_021d8e84(wk, func_ov119_021dbbd0);
        }
    }
}



static void func_ov119_021df59c(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[] = { 24, 25 };
        func_ov119_021e131c(wk, items, NELEMS(items), 32, 12);
        func_ov119_021d8e84(wk, func_ov119_021df548);
    }
}

void func_ov119_021df5d8(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x87, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov067_021b6548(wk->touchBar, 1, FALSE);
    func_ov119_021d8e84(wk, func_ov119_021df59c);
    func_ov119_021e26ac(wk);
}

BOOL func_ov119_021df614(PokemonTradeWork *wk, int a, int b) {
    int i;
    int box = func_ov119_021d8af4(a, wk);
    int slot = func_ov119_021d8b0c(a, b);

    if (wk->heldSlot == slot && wk->heldBox == box) {
        return TRUE;
    }
    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 3; i++) {
            if (wk->negoSlot[0][i] != -1 && slot == wk->negoSlot[0][i] && box == wk->negoBox[0][i]) {
                return TRUE;
            }
        }
    } else if (wk->selectSlot == slot && wk->selectBox == box && wk->heldSlot == -1 && wk->heldBox == -1) {
        return TRUE;
    }
    return FALSE;
}



BOOL func_ov119_021df6b4(PokemonTradeWork *wk, int a, int b) {
    int box = func_ov119_021d8af4(a, wk);
    int slot = func_ov119_021d8b0c(a, b);

    if (wk->unkF94 == slot && wk->unkF98 == box) {
        return TRUE;
    }
    return FALSE;
}


static void func_ov119_021df6ec(PokemonTradeWork *wk) {
    if (!func_02035c38()) {
        func_ov119_021d8e84(wk, func_ov119_021dbbd0);
    }
}

void func_ov119_021df708(PokemonTradeWork *wk) {
    int index = func_ov119_021dd34c(wk, wk->unkF98, wk->unkF94);

    if (func_ov119_021d8a24(wk)) {
        if (!func_0203d3a8(func_020399a8(), 0xc0b, 4, &index)) {
            return;
        }
    }
    wk->selectSlot = -1;
    wk->selectBox = -1;
    wk->heldSlot = -1;
    wk->heldBox = -1;
    wk->unkF94 = -1;
    wk->unkF98 = -1;
    if (index != -1) {
        func_ov119_021dd4f4(wk, 0, index);
        func_ov119_021e62d4(wk, 0, index);
    }
    func_ov119_021d8e84(wk, func_ov119_021df6ec);
}


void func_ov119_021df78c(PokemonTradeWork *wk) {
    int i;
    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 4; i++) {
            func_ov119_021e6718(wk, i);
        }
    }
}

void func_ov119_021df7ac(PokemonTradeWork *wk, BOOL a1) {
    int i;
    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 4; i++) {
            func_ov119_021e6790(wk, i, a1);
        }
    }
}

void func_ov119_021df7d0(PokemonTradeWork *wk) {
    int i;
    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 4; i++) {
            func_ov119_021e67a8(wk, i);
        }
    }
}

void func_ov119_021df7f0(PokemonTradeWork *wk) {
    u32 x, y;
    u8 index;
    if (func_ov119_021d8aa8(wk) && func_02035d20(&x, &y) && x >= 8 && x < 104 && y >= 144 && y < 168) {
        index = (x - 8) / 24;
        if (index >= 4) {
            index = 3;
        }
        func_ov119_021e67c4(wk, index);
        func_ov119_021df8a0(wk, index, 0);
        if (func_ov119_021d8a24(wk)) {
            func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNKD, 1, &index);
        }
    }
}

void func_ov119_021df868(PokemonTradeWork *wk) {
    int i;

    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 2; i++) {
            if (wk->unk11E4[i] != 0) {
                wk->unk11E4[i]--;
                if (wk->unk11E4[i] == 0) {
                    func_ov119_021e68b4(wk, i);
                }
            }
        }
    }
}

void func_ov119_021df8a0(PokemonTradeWork *wk, u8 index, int side) {
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021e67e0(wk, index, side);
        wk->unk11E4[side] = 60;
        if (!GFL_SndIsPlaying(SEQ_SE_SELECT3)) {
            GFL_SndSEPlay(SEQ_SE_SELECT3);
        }
    }
}



void func_ov119_021df8e0(PokemonTradeWork *wk) {
    int i;
    if (func_ov119_021d8aa8(wk)) {
        for (i = 0; i < 2; i++) {
            func_ov119_021e68b4(wk, i);
        }
        func_ov119_021e5868(wk);
    }
}

void func_ov119_021df904(PokemonTradeWork *wk) {
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021e57fc(wk);
    }
}

void func_ov119_021df918(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    func_ov119_021e6308(wk, side, FALSE);
    if (wk->unk11E2[side] != 0) {
        func_ov119_021e293c(wk, side);
        func_ov119_021e36d8(wk, side);
    }
    func_ov119_021e6f0c(wk, side, pkm);
    func_ov119_021e272c(wk, side, pkm);
    GFL_BGSysQueueScrLoad(1);
    func_ov119_021e36a8(wk, side, 1 - side, pkm, 1 - side, TRUE);
}

void func_ov119_021df978(PokemonTradeWork *wk) {
    int i;

    for (i = 0; i < 2; i++) {
        if (wk->unk11E2[i] != 0) {
            wk->unk11E2[i]--;
            func_ov119_021e6308(wk, i, FALSE);
            if (wk->unk11E2[i] == 0) {
                func_ov119_021e293c(wk, i);
                GFL_BGSysFillScrArea(1, 0, i * 16, 0, 16, 24, 16);
                GFL_BGSysQueueScrLoad(1);
                func_ov119_021e36d8(wk, i);
                func_ov119_021e6308(wk, i, TRUE);
            }
        }
    }
}

BOOL func_ov119_021df9e8(PokemonTradeWork *wk, PartyPkm *pkm, int command) {
    NetHandle *handle = func_020399a8();

    if (wk->unk11FB_0 != 0) {
        return FALSE;
    }
    MI_CpuCopy8(pkm, wk->unk106C, PokeParty_GetPkmRawSize());
    if (!func_0203d3f8(handle, 0xff, command, PokeParty_GetPkmRawSize(), wk->unk106C, 0, FALSE, TRUE)) {
        return FALSE;
    }
    wk->unk11FB_0 = 1;
    return TRUE;
}




