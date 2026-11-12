// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov194/pokemontrade_proc.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/ui_scene.h"
#include "app/ui/touchbar.h"
#include "app/pokemon_trade_local.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "field/unity_tower.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/fade.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_command.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/net_system.h"
#include "gfl/nhttp_rap.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/math.h"
#include "pml/hm_check.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/pokedex.h"
#include "save/save_control.h"
#include "system/app_menu_common.h"
#include "system/app_taskmenu.h"
#include "system/country_region.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/mcss.h"
#include "system/printsys.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "app/gtsnego.h"
#include "app/pokemon_trade.h"
#include "work_area.h"

// The trade's procs and the steps of the trade: choosing a Pokémon, the menus, the messages between the machines

BOOL func_ov119_021d89a8(PokemonTradeWork *wk);
void func_ov119_021d8a50(PokemonTradeWork *wk);
void func_ov119_021d8b38(PokemonTradeWork *wk);
void func_ov119_021d8b54(PokemonTradeWork *wk);
void func_ov119_021d8c0c(PokemonTradeWork *wk);
void func_ov119_021d8c6c(PokemonTradeWork *wk);
void func_ov119_021d8c84(PokemonTradeWork *wk);
void func_ov119_021d8ccc(PokemonTradeWork *wk, ClActor *icon, int a2, int a3, BoxPkm *pkm);
void func_ov119_021d8e80(PokemonTradeWork *wk);
void func_ov119_021d9038(int netId, int size, const void *data, void *work, NetHandle *handle, int mode);
void *func_ov119_021d8e90(int netId, void *work, int size);
void *func_ov119_021d8eac(int netId, void *work, int size);
void *func_ov119_021d8ec8(int netId, void *work, int size);
void func_ov119_021d8eec(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d8fe4(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9038(int netId, int size, const void *data, void *work, NetHandle *handle, int index);
void func_ov119_021d90ac(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d90c0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d90d4(int netId, int size, const void *data, void *work, NetHandle *handle);
BOOL func_02027a20(AppTaskMenu *menu);
void func_ov119_021d90e8(u32 value, int index, int netId, PokemonTradeWork *wk, NetHandle *handle);
void func_ov119_021d9124(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d913c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9154(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d916c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9190(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d91bc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d91d8(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d921c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9248(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d92dc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d92fc(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9328(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d935c(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9384(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d93ac(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d93d4(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d93e0(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9404(int netId, int size, const void *data, void *work, NetHandle *handle);
void func_ov119_021d9410(int netId, int size, const void *data, void *work, NetHandle *handle);
static void func_ov119_021d941c(PokemonTradeWork *wk);
static void func_ov119_021d94a0(PokemonTradeWork *wk, int side);
BOOL func_ov119_021d9548(PokemonTradeWork *wk, int box, int slot);
static BOOL func_ov119_021d95a0(PartyPkm *pkm);
void func_ov119_021d9670(PokemonTradeWork *wk);
void func_ov119_021d9750(PokemonTradeWork *wk);
void func_ov119_021d9780(PokemonTradeWork *wk, int side, BOOL reload);
void func_ov119_021d9810(PokemonTradeWork *wk);
void func_ov119_021d98ac(PokemonTradeWork *wk);
void func_ov119_021d99f4(PokemonTradeWork *wk);
void func_ov119_021d9a50(PokemonTradeWork *wk);
static void func_ov119_021d9ad8(PokemonTradeWork *wk);
void func_ov119_021d9b34(PokemonTradeWork *wk);
static void func_ov119_021d9b84(PokemonTradeWork *wk);
static void func_ov119_021d9bc4(PokemonTradeWork *wk);
void func_ov119_021d9c00(PokemonTradeWork *wk);
void func_ov119_021d9cfc(PokemonTradeWork *wk);
void func_ov119_021d9d60(PokemonTradeWork *wk);
static void func_ov119_021d9dc4(PokemonTradeWork *wk);
void func_ov119_021d9e0c(PokemonTradeWork *wk);
void func_ov119_021d9e54(PokemonTradeWork *wk);
void func_ov119_021d9ea8(PokemonTradeWork *wk, u32 a1, u32 a2);
int func_ov119_021d9f24(PokemonTradeWork *wk, BOOL a1);
static void func_ov119_021da064(PokemonTradeWork *wk);
void func_ov119_021da158(PokemonTradeWork *wk);
void func_ov119_021da1b4(PokemonTradeWork *wk);
void func_ov119_021da1dc(PokemonTradeWork *wk);
void func_ov119_021da254(PokemonTradeWork *wk);
void func_ov119_021da27c(PokemonTradeWork *wk);
static void func_ov119_021da2f4(PokemonTradeWork *wk);
BOOL func_ov119_021d9ebc(PokemonTradeWork *wk, int *box, int *slot, int *a3, int *a4);
void func_ov119_021da318(PokemonTradeWork *wk);
void func_ov119_021da640(PokemonTradeWork *wk);
void func_ov119_021da700(PokemonTradeWork *wk);
void func_ov119_021da798(PokemonTradeWork *wk);
static void func_ov119_021da898(PokemonTradeWork *wk);
void func_ov119_021da8f8(PokemonTradeWork *wk);
static void func_ov119_021da9f0(PokemonTradeWork *wk);
static void func_ov119_021daa18(PokemonTradeWork *wk);
static void func_ov119_021daa4c(PokemonTradeWork *wk);
void func_ov119_021daa7c(PokemonTradeWork *wk);
static void func_ov119_021daae4(PokemonTradeWork *wk);
static void func_ov119_021dab0c(PokemonTradeWork *wk);
void func_ov119_021dab34(PokemonTradeWork *wk);
void func_ov119_021dabb0(PokemonTradeWork *wk);
static void func_ov119_021dabe8(PokemonTradeWork *wk);
static void func_ov119_021dac20(PokemonTradeWork *wk);
static void func_ov119_021dac60(PokemonTradeWork *wk);
static void func_ov119_021dad84(PokemonTradeWork *wk);
void func_ov119_021dae2c(PokemonTradeWork *wk);
static void func_ov119_021daec8(PokemonTradeWork *wk);
static void func_ov119_021daf68(PokemonTradeWork *wk);
void func_ov119_021dafb8(PokemonTradeWork *wk);
void func_ov119_021db030(PokemonTradeWork *wk);
void func_ov119_021db10c(PokemonTradeWork *wk);
void func_ov119_021db164(PokemonTradeWork *wk);
static void func_ov119_021db198(PokemonTradeWork *wk);
BOOL func_ov119_021db1bc(PokemonTradeWork *wk, u32 a1);
void func_ov119_021db278(PokemonTradeWork *wk);
void func_ov119_021db290(PokemonTradeWork *wk);
void func_ov119_021db2e8(PokemonTradeWork *wk);
void func_ov119_021db304(PokemonTradeWork *wk);
static void func_ov119_021db41c(PokemonTradeWork *wk);
void func_ov119_021db45c(PokemonTradeWork *wk);
static int func_ov119_021db4b4(int column);
BOOL func_ov119_021db4ec(PokemonTradeWork *wk, int column);
static void func_ov119_021dadac(PokemonTradeWork *wk);
BOOL func_ov119_021db0a0(PokemonTradeWork *wk);
void func_ov119_021db52c(PokemonTradeWork *wk);
void func_ov119_021db7ac(PokemonTradeWork *wk, BOOL playSound, BOOL send);
void func_ov119_021db89c(PokemonTradeWork *wk);
static void func_ov119_021db998(PokemonTradeWork *wk);
void func_ov119_021db9b4(PokemonTradeWork *wk);
static void func_ov119_021dba38(PokemonTradeWork *wk);
void func_ov119_021dba80(PokemonTradeWork *wk);
void func_ov119_021dbaf8(PokemonTradeWork *wk);
void func_ov119_021dbb6c(PokemonTradeWork *wk);
void func_ov119_021dbc3c(PokemonTradeWork *wk);
void func_ov119_021dbc98(PokemonTradeWork *wk);
void func_ov119_021dbd3c(PokemonTradeWork *wk);
BOOL func_ov119_021dbd70(PokemonTradeWork *wk, int box, int slot);
void func_ov119_021dc2b0(PokemonTradeWork *wk);
void func_ov119_021dc370(PokemonTradeWork *wk);
void func_ov119_021dc3ec(PokemonTradeWork *wk);
void func_ov119_021dc428(PokemonTradeWork *wk);
void func_ov119_021dc458(PokemonTradeWork *wk);
void func_ov119_021dc484(PokemonTradeWork *wk);
void func_ov119_021dc494(PokemonTradeWork *wk);
void func_ov119_021dd018(PokemonTradeWork *wk, BOOL moved);
void func_ov119_021dc500(TCB *tcb, void *work);
void func_ov119_021dc538(PokemonTradeWork *wk, GameData *gameData, u16 friendIndex);
void func_ov119_021dc5d0(PokemonTradeWork *wk, BOOL flag);
void func_ov119_021dc5f0(PokemonTradeWork *wk);
void func_ov119_021dc62c(PokemonTradeWork *wk, int box);
void func_ov119_021dc818(PokemonTradeWork *wk);
PokemonTradeWork *func_ov119_021dc854(GameProc *proc, u32 heapSize);
BOOL func_ov119_021dc894(GameProc *proc, u32 *state, PokemonTradeParam *trade, PokemonTradeWork *wk, int type);
static BOOL func_ov119_021dca84(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov119_021dcaac(GameProc *proc, u32 *state, void *param, void *work);
int func_ov119_021dcad4(GameProc *proc, u32 *state, void *param, void *work);
int func_ov119_021dcb30(GameProc *proc, u32 *state, void *param, void *work);
void func_ov119_021dcba4(PokemonTradeDemoParam *demo, PokemonTradeWork *wk);
static BOOL func_ov119_021dcc38(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov119_021dcc78(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov119_021dccb8(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov119_021dccf8(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov119_021dcd38(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov119_021dcf24(GameProc *proc, u32 *state, void *param, void *work);
BOOL func_ov119_021dbdb4(PokemonTradeWork *wk);
static void func_ov119_021dbe70(PokemonTradeWork *wk);

// pokebw2's sSideButtonRects, defined before func_ov119_021da640 as pokebw2 has it, which places it in MWCC's sorted
// .rodata where Black has it
static const TouchRect data_ov119_021e75f0[3];

// pokebw2's sNetCommands
static const NetCommand data_ov119_021e7614[] = {
    { func_ov119_021d92dc, NULL },
    { func_ov119_021d8fe4, func_ov119_021d8e90 },
    { func_ov119_021d9410, NULL },
    { func_ov119_021d93e0, NULL },
    { func_ov119_021d93d4, NULL },
    { func_ov119_021d9404, NULL },
    { func_ov119_021d935c, NULL },
    { func_ov119_021d90ac, func_ov119_021d8eac },
    { func_ov119_021d90c0, func_ov119_021d8eac },
    { func_ov119_021d90d4, func_ov119_021d8eac },
    { func_ov119_021d92fc, NULL },
    { func_ov119_021d9328, NULL },
    { func_ov119_021d9384, NULL },
    { func_ov119_021d93ac, NULL },
    { func_ov119_021d91bc, func_ov119_021d8ec8 },
    { func_ov119_021d8eec, NULL },
    { func_ov119_021d91d8, NULL },
    { func_ov119_021d921c, NULL },
    { func_ov119_021d9248, NULL },
    { func_ov119_021d9124, NULL },
    { func_ov119_021d913c, NULL },
    { func_ov119_021d9154, NULL },
    { func_ov119_021d916c, NULL },
    { func_ov119_021d9190, NULL },
};

// The memory of the trade's heap (defined at the end of the file)
// pokebw2's sHeapMemory
extern u8 data_ov119_021e8560[WORK_AREA_SIZE(0xc000)];


void func_ov119_021d8980(PokemonTradeWork *wk) {
    func_ov067_021b65b4(wk->touchBar, 1);
    func_ov067_021b6558(wk->touchBar, 1, TRUE);
    func_ov067_021b6548(wk->touchBar, 1, TRUE);
}

BOOL func_ov119_021d89a8(PokemonTradeWork *wk) {
    void *save = func_02008fec(wk->gameData);
    BOOL ret = FALSE;
    if (func_02009020(save, 0) == -1) {
        ret = TRUE;
    }
    return ret;
}

void func_ov119_021d89cc(PokemonTradeWork *wk) {
    func_0203d7b0(0);
    func_0203d7bc(0);
    func_0203c554(0);
    wk->param->next = 3;
}



BOOL func_ov119_021d89ec(const u8 *data) {
    int size = PokeParty_GetPkmRawSize();
    int i;
    if (data == NULL) {
        return FALSE;
    }
    for (i = 0; i < size; i++) {
        if (data[i] != 0) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 func_ov119_021d8a18(PokemonTradeWork *wk) {
    return wk->unk11F2;
}

BOOL func_ov119_021d8a24(PokemonTradeWork *wk) {
    if (wk->param == NULL) {
        return FALSE;
    }
    if (wk->type >= 4) {
        return FALSE;
    }
    if (func_0203cd98()) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021d8a50(PokemonTradeWork *wk) {
    if (wk->bgmTimer != 0) {
        wk->bgmTimer++;
        if (wk->bgmTimer == 2) {
            GFL_SndBGMFadeOut(6);
        } else if (wk->bgmTimer == 8) {
            GFL_SndBGMSetPaused(TRUE);
            GFL_SndBGMPush();
            wk->unk11F8++;
            GFL_SndBGMPlay(SEQ_BGM_KOUKAN, 0xffff);
            GFL_SndBGMFadeIn(1);
            wk->bgmTimer = 0;
        }
    }
}




// pokebw2's PokemonTrade_IsNegoType
BOOL func_ov119_021d8aa8(PokemonTradeWork *wk) {
    BOOL ret = FALSE;
    switch (wk->type) {
    case 0:
    case 4:
    case 6:
    case 7:
    case 8:
        break;
    default:
        ret = TRUE;
        break;
    }
    return ret;
}

BOOL func_ov119_021d8adc(PokemonTradeWork *wk) {
    BOOL ret = FALSE;
    if (wk->type == 3 || wk->type == 9) {
        ret = TRUE;
    }
    return ret;
}

int func_ov119_021d8af4(int a, PokemonTradeWork *wk) {
    if (a >= 2) {
        return (a - 2) / 6;
    }
    return wk->unk630;
}


// pokebw2's PokemonTrade_GetColumnSlot
int func_ov119_021d8b0c(int a, int b) {
    if (a >= 2) {
        return (a - 2) % 6 + b * 6;
    }
    if (b < 3) {
        return a + b * 2;
    }
    return -1;
}

void func_ov119_021d8b38(PokemonTradeWork *wk) {
    wk->firstColumn++;
    wk->unk1084 = 1;
    func_ov119_021db7ac(wk, 0, 0);
}

void func_ov119_021d8b54(PokemonTradeWork *wk) {
    u32 x, y;
    if (func_02035cb8(&x, &y)) {
        if (wk->unkF60 == NULL) {
            wk->unkF5C = 0;
        }
        if (wk->unkF60 != NULL) {
            if (MATH_ABS(wk->unkF64.x - wk->unkF68.x - (int)x) > 2) {
                wk->unkF5C = 1;
            }
            if (MATH_ABS(wk->unkF64.y - wk->unkF68.y - (int)y) > 2) {
                wk->unkF5C = 1;
            }
            if (MATH_ABS(wk->unkF64.x - (int)x) > 6 && MATH_ABS(wk->unkF64.y - (int)y) > 6) {
                wk->unkF70 = 0;
            }
        }
        if (y >= 16 && y < 144) {
            wk->touchX = x;
            wk->unkF58 = y;
        }
    }
}


void func_ov119_021d8c0c(PokemonTradeWork *wk) {
    u32 x, y;
    ClActorPos pos;
    if (wk->unkF60 != NULL) {
        if (func_02035cb8(&x, &y)) {
            func_0204b454(wk->unkF60, &wk->unkF6C, 1);
            pos.x = x + wk->unkF68.x;
            pos.y = y + wk->unkF68.y;
            if (pos.y > 150) {
                pos.y = 150;
            }
            func_0204b404(wk->unkF60, &pos, 1);
        }
    }
}


void func_ov119_021d8c6c(PokemonTradeWork *wk) {
    if (wk->unkF60 != NULL) {
        wk->unkF60 = NULL;
    }
    func_ov119_021e66e8(wk);
}

void func_ov119_021d8c84(PokemonTradeWork *wk) {
    if (wk->unkF60 != NULL) {
        func_0204b800(wk->unkF60, 16);
        func_0204b404(wk->unkF60, &wk->unkF64, 1);
        func_0204b3dc(wk->unkF60, 1);
        wk->heldSlot = -1;
        wk->heldBox = -1;
        func_ov119_021d8c6c(wk);
        MI_CpuFill8(&wk->unk4, 0, 2);
    }
}

void func_ov119_021d8ccc(PokemonTradeWork *wk, ClActor *icon, int a2, int a3, BoxPkm *pkm) {
    u32 x, y;
    wk->unkF60 = icon;
    func_0204b454(icon, &wk->unkF64, 1);
    func_ov119_021e4ab8(wk, wk->unkF60, 1);
    if (func_02035cb8(&x, &y)) {
        wk->unkF68.x = wk->unkF64.x - x;
        wk->unkF68.y = wk->unkF64.y - y;
        func_ov119_021e64dc(wk, a2, a3, x + wk->unkF68.x, y + wk->unkF68.y, pkm);
        wk->unkF60 = wk->actors[9];
    }
}

PartyPkm *func_ov119_021d8d4c(BoxSaveAccessor *boxes, int box, int slot, PokemonTradeWork *wk) {
    PartyPkm *pkm = NULL;
    if (box != -1) {
        if (box > wk->unk630) {
            return NULL;
        }
        if (box != wk->unk630 && slot != -1) {
            pkm = boxPkmRegenToPartyPkm(BoxSaveAccessor_GetPkm(boxes, box, slot), wk->heapId);
        } else {
            PokeParty *party = wk->party;
            if (slot < PokeParty_GetPkmCount(party) && slot != -1) {
                u32 size;
                pkm = PokeParty_NewTempPkm(1, 1, 1, wk->heapId);
                size = PokeParty_GetPkmRawSize();
                MI_CpuCopy8(PokeParty_GetPkm(party, slot), pkm, size);
            }
        }
    }
    return pkm;
}



BoxPkm *func_ov119_021d8dd0(BoxSaveAccessor *boxes, int box, int slot, PokemonTradeWork *wk) {
    PokeParty *party;
    if (box != -1) {
        if (box > wk->unk630) {
            return NULL;
        }
        if (box != wk->unk630 && slot != -1) {
            return BoxSaveAccessor_GetPkm(boxes, box, slot);
        }
        party = wk->party;
        if (slot < PokeParty_GetPkmCount(party) && slot != -1) {
            return func_0201d624(PokeParty_GetPkm(party, slot));
        }
    }
    return NULL;
}


TradeBoxEntry *func_ov119_021d8e24(int box, int slot, PokemonTradeWork *wk) {
    if (box != -1) {
        if (box > wk->unk630) {
            return NULL;
        }
        if (box != wk->unk630 && slot != -1) {
            return &wk->boxEntries[box * 30 + 6 + slot];
        }
        if (slot >= 0 && slot < 6) {
            return &wk->boxEntries[slot];
        }
        return NULL;
    }
    return NULL;
}





void func_ov119_021d8e80(PokemonTradeWork *wk) {
}

void func_ov119_021d8e84(PokemonTradeWork *wk, PokemonTradeState state) {
    wk->state = state;
}

void *func_ov119_021d8e90(int netId, void *work, int size) {
    PokemonTradeWork *wk = work;
    if (netId >= 0 && netId < 2) {
        return wk->unk1064[netId];
    }
    return NULL;
}

void *func_ov119_021d8eac(int netId, void *work, int size) {
    PokemonTradeWork *wk = work;
    if (netId >= 0 && netId < 2) {
        return wk->unk1064[netId];
    }
    return NULL;
}

void *func_ov119_021d8ec8(int netId, void *work, int size) {
    PokemonTradeWork *wk = work;
    if (netId == func_0203d1a4(func_020399a8())) {
        return wk->unk10;
    }
    return wk->unk2E6;
}

void func_ov119_021d8eec(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        wk->unk11E8[netId != func_0203d1a4(func_020399a8()) ? 1 : 0] = *(const u8 *)data;
    }
}




u32 func_ov119_021d8f20(PokemonTradeWork *wk, int side) {
    if (!func_ov119_021d8a24(wk)) {
        return wk->unk5E4[side];
    }
    if (func_0203d1a4(func_020399a8()) == 0) {
        return wk->unk5E4[side];
    }
    return wk->unk5E4[1 - side];
}

void func_ov119_021d8f60(PokemonTradeWork *wk, int side, u32 value) {
    if (!func_ov119_021d8a24(wk)) {
        wk->unk5E4[side] = value;
        return;
    }
    if (func_0203d1a4(func_020399a8()) == 0) {
        wk->unk5E4[side] = value;
        return;
    }
    wk->unk5E4[1 - side] = value;
}



PartyPkm *func_ov119_021d8fa4(PokemonTradeWork *wk, int side) {
    if (!func_ov119_021d8a24(wk)) {
        return wk->unk5DC[side];
    }
    if (func_0203d1a4(func_020399a8()) == 0) {
        return wk->unk5DC[side];
    }
    return wk->unk5DC[1 - side];
}

void func_ov119_021d8fe4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        u32 pkmSize = PokeParty_GetPkmRawSize();
        MI_CpuCopy8(wk->unk1064[netId], wk->unk5DC[netId], pkmSize);
        if (netId != func_0203d1a4(func_020399a8())) {
            wk->unkF9C = netId + 1;
            wk->unk11FB_4 = 1;
        }
    }
}

void func_ov119_021d9038(int netId, int size, const void *data, void *work, NetHandle *handle, int mode) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        if (PokeParty_GetParam(wk->unk1064[netId], PKM_PARAM_SPECIES_VALID, NULL)) {
            func_ov119_021dd380(wk, mode, wk->unk1064[netId]);
            func_ov119_021e6334(wk, 1, mode, wk->unk1064[netId], TRUE, FALSE);
            func_ov119_021df918(wk, 1, wk->unk1064[netId]);
        }
        wk->unk11FB_4 = 1;
    }
}


void func_ov119_021d90ac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov119_021d9038(netId, size, data, work, handle, 0);
}

void func_ov119_021d90c0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov119_021d9038(netId, size, data, work, handle, 1);
}

void func_ov119_021d90d4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    func_ov119_021d9038(netId, size, data, work, handle, 2);
}

void func_ov119_021d90e8(u32 value, int index, int netId, PokemonTradeWork *wk, NetHandle *handle) {
    if (handle == func_020399a8()) {
        if (netId == func_0203d1a4(func_020399a8())) {
            wk->unkFF0[0][index] = value;
        } else {
            wk->unkFF0[1][index] = value;
        }
    }
}


void func_ov119_021d9124(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    func_ov119_021d90e8(*(const u8 *)data, 0, netId, wk, handle);
}

void func_ov119_021d913c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    func_ov119_021d90e8(*(const u8 *)data, 1, netId, wk, handle);
}

void func_ov119_021d9154(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    func_ov119_021d90e8(*(const u8 *)data, 2, netId, wk, handle);
}

void func_ov119_021d916c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        wk->unk5E4[netId] = *(const u8 *)data;
    }
}

void func_ov119_021d9190(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        if (netId != func_0203d1a4(func_020399a8())) {
            wk->unk11FB_0 = 0;
        }
    }
}














void func_ov119_021d91bc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (handle != func_020399a8()) {
        return;
    }
    if (netId == func_0203d1a4(func_020399a8())) {
        return;
    }
}

void func_ov119_021d91d8(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    const u8 *bytes = data;
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        wk->unk11F2 = bytes[0];
        wk->unk11F4 = bytes[1];
        wk->unk107E = wk->unk11F2 * 160 + 16;
    }
}


void func_ov119_021d921c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        if (netId != func_0203d1a4(func_020399a8())) {
            wk->unk11E6[1] = *(const u8 *)data;
        }
    }
}

void func_ov119_021d9248(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    UnityTowerVisitor profile;
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        UnityTowerSurveySave *survey = getUnityTower_SurveySaveBlkAddrress(GameData_GetSaveControl(wk->gameData));
        u8 country, province, validCountry, validProvince;
        MI_CpuCopy8(data, &profile, sizeof(UnityTowerVisitor));
        country = UnityTowerVisitor_GetCountry(&profile.info);
        province = UnityTowerVisitor_GetProvince(&profile.info);
        validCountry = func_02025c1c(country, province, TrainerInfo_GetRegion(&profile.info));
        validProvince = func_02025c30(country, province, TrainerInfo_GetRegion(&profile.info));
        if (country == validCountry && province == validProvince) {
            func_02008574(&profile.info, country, province);
        } else {
            func_02008574(&profile.info, 0, 0);
        }
        func_0202eb7c(survey, &profile);
    }
}


void func_ov119_021d92dc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        wk->command[netId] = *(const u8 *)data;
    }
}

void func_ov119_021d92fc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8()) {
        if (netId != func_0203d1a4(func_020399a8())) {
            wk->unk1060 = *(const u8 *)data;
        }
    }
}

void func_ov119_021d9328(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        func_ov119_021dd4f4(wk, 1, *(const u32 *)data);
        func_ov119_021e62d4(wk, 1, *(const u32 *)data);
    }
}

void func_ov119_021d935c(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_0203d1a4(func_020399a8())) {
        func_ov119_021d94a0(wk, 1);
    }
}

void func_ov119_021d9384(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_02039338()) {
        wk->unk107E = *(const s16 *)data;
    }
}

void func_ov119_021d93ac(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_02039338()) {
        func_ov119_021df8a0(wk, *(const u8 *)data, 1);
    }
}

void func_ov119_021d93d4(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (handle != func_020399a8()) {
        return;
    }
}

void func_ov119_021d93e0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    PokemonTradeWork *wk = work;
    if (handle == func_020399a8() && netId != func_02039338()) {
        wk->unkFA0 = *(const u8 *)data;
    }
}











void func_ov119_021d9404(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (handle != func_020399a8()) {
        return;
    }
}

void func_ov119_021d9410(int netId, int size, const void *data, void *work, NetHandle *handle) {
    if (handle != func_020399a8()) {
        return;
    }
}

// pokebw2's PokemonTrade_WaitFadeOutToEnd
static void func_ov119_021d941c(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        func_ov119_021d8e84(wk, NULL);
    }
}

// pokebw2's PokemonTrade_FadeOutToEnd
void func_ov119_021d9434(PokemonTradeWork *wk) {
    GFL_WipeSet(0, 0, 0, 0, 6, 1, wk->heapId);
    func_ov119_021d8e84(wk, func_ov119_021d941c);
}

void func_ov119_021d9468(PokemonTradeWork *wk, int side, PartyPkm *pkm) {
    if (pkm != NULL) {
        func_ov119_021e36d8(wk, side);
        func_ov119_021e36a8(wk, side, 1 - side, pkm, 1, func_ov119_021d8aa8(wk));
        func_ov119_021e1b14(wk, side, pkm);
    }
}

static void func_ov119_021d94a0(PokemonTradeWork *wk, int side) {
    int i;
    if (!func_ov119_021d8aa8(wk)) {
        func_ov119_021e36d8(wk, side);
        func_ov119_021e2438(wk, side);
    } else {
        for (i = 0; i < 3; i++) {
            func_ov119_021e62d4(wk, side, i);
        }
    }
}

BOOL func_ov119_021d94d4(PokemonTradeWork *wk) {
    u32 value;
    PartyPkm *pkm;
    if (!func_ov119_021d8aa8(wk)) {
        pkm = func_ov119_021d8fa4(wk, 0);
        value = func_ov119_021d8f20(wk, 0);
    } else {
        pkm = func_ov119_021d8fa4(wk, 1);
        value = func_ov119_021d8f20(wk, 1);
    }
    if (PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL) == 0) {
        return FALSE;
    }
    if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
        return FALSE;
    }
    if (countActivePkms(wk->party) == 1 && value != 0) {
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov119_021d9548(PokemonTradeWork *wk, int box, int slot) {
    BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, box, slot, wk);
    BOOL ret;
    if (wk->type != 0) {
        return FALSE;
    }
    if (box != wk->unk630) {
        return FALSE;
    }
    ret = FALSE;
    if (PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
        return ret;
    }
    if (!doesPkmHaveTmMove(pkm, 0)) {
        ret = TRUE;
    }
    return ret;
}


// Whether the Pokémon is a bad egg or not a valid species
// pokebw2's PokemonTrade_IsBadPkm
static BOOL func_ov119_021d95a0(PartyPkm *pkm) {
    int species;
    BOOL ret = FALSE;
    if (PokeParty_GetParam(pkm, PKM_PARAM_BAD_EGG, NULL) == TRUE) {
        return TRUE;
    }
    species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    if (species > SPECIES_BAD_EGG) {
        ret = TRUE;
    }
    return ret;
}

BOOL func_ov119_021d95d0(PokemonTradeWork *wk) {
    int side = 0;
    if (func_ov119_021d8aa8(wk)) {
        side = 1;
    }
    return func_ov119_021d95a0(func_ov119_021d8fa4(wk, side));
}

BOOL func_ov119_021d95f0(PokemonTradeWork *wk) {
    int side = 1;
    if (func_ov119_021d8aa8(wk)) {
        side = 0;
    }
    return func_ov119_021d95a0(func_ov119_021d8fa4(wk, side));
}

BOOL func_ov119_021d9610(PokemonTradeWork *wk) {
    PartyPkm *pkm;
    u32 value;
    if (!func_ov119_021d8aa8(wk)) {
        pkm = func_ov119_021d8fa4(wk, 0);
        value = func_ov119_021d8f20(wk, 1);
    } else {
        pkm = func_ov119_021d8fa4(wk, 1);
        value = func_ov119_021d8f20(wk, 0);
    }
    if (PML_ItemIsMail(PokeParty_GetParam(pkm, PKM_PARAM_ITEM, NULL)) && wk->unk11F4 != 0 && value == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021d9670(PokemonTradeWork *wk) {
    u8 command[5];
    BOOL sent;
    BOOL network = func_ov119_021d8a24(wk);
    if (!func_ov119_021e12ac(wk)) {
        return;
    }
    if (network) {
        if (func_ov119_021d94d4(wk)) {
            command[4] = 5;
            sent = func_0203d3a8(func_020399a8(), 0xc00, 1, &command[4]);
        } else if (func_ov119_021d95d0(wk)) {
            command[3] = 7;
            sent = func_0203d3a8(func_020399a8(), 0xc00, 1, &command[3]);
        } else if (func_ov119_021d95f0(wk)) {
            command[2] = 8;
            sent = func_0203d3a8(func_020399a8(), 0xc00, 1, &command[2]);
        } else if (func_ov119_021d9610(wk)) {
            command[1] = 6;
            sent = func_0203d3a8(func_020399a8(), 0xc00, 1, &command[1]);
        } else {
            command[0] = 2;
            sent = func_0203d3a8(func_020399a8(), 0xc00, 1, &command[0]);
        }
        if (sent) {
            func_02039c68(func_020399a8(), 0x1b, 8);
            func_ov119_021d8e84(wk, func_ov119_021da798);
        }
    } else {
        wk->command[0] = 2;
        wk->command[1] = 2;
        func_ov119_021d8e84(wk, func_ov119_021da798);
    }
}



void func_ov119_021d9750(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 6, wk->strbuf);
    func_ov119_021e1024(wk);
    func_ov119_021e1030(wk);
    func_ov119_021d8e84(wk, func_ov119_021d9670);
}

PartyPkm *func_ov119_021d8fa4(PokemonTradeWork *wk, int side);


void func_ov119_021d9780(PokemonTradeWork *wk, int side, BOOL reload) {
    if (side == 0) {
        ClActorPos pos = { 96, 16 };
        func_0204b404(wk->actors[4], &pos, 1);
    } else {
        ClActorPos pos = { 160, 16 };
        func_0204b404(wk->actors[4], &pos, 1);
    }
    if (func_020355c4()) {
        func_ov119_021e5b6c(wk, side, FALSE);
        func_0204b3dc(wk->actors[4], FALSE);
    } else {
        func_ov119_021e5b6c(wk, side, TRUE);
        func_0204b3dc(wk->actors[4], TRUE);
    }
    if (reload) {
        PartyPkm *pkm = func_ov119_021d8fa4(wk, wk->unk10A0);
        func_ov119_021e219c(wk, pkm, wk->unk10A0, TRUE);
    }
}


void func_ov119_021d9810(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        GFL_BGSysSetBGEnabled(3, FALSE);
        func_ov119_021e1ce8(wk, TRUE);
        GFL_BGSysSetBGEnabled(4, FALSE);
        func_ov119_021e3e80(wk);
        func_ov119_021e3208(wk, 0);
        GFL_BGSysQueueScrLoad(3);
        G2_BlendNone();
        func_ov119_021d9468(wk, 0, func_ov119_021d8fa4(wk, 0));
        func_ov119_021d9468(wk, 1, func_ov119_021d8fa4(wk, 1));
        func_ov119_021d8b38(wk);
        GFL_WipeSet(3, 1, 1, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021d9ad8);
    }
}

static inline u32 PokemonTrade_GetPressedKeys(void) {
    u32 keys = GCTX_HIDGetPressedKeys();

    if (keys != 0) {
        func_020355d8(FALSE);
    }
    return keys;
}

static inline BOOL PokemonTrade_GetTouch(void) {
    BOOL touched = func_02035c60();

    if (touched) {
        func_020355d8(TRUE);
    }
    return touched;
}
void func_ov119_021d98ac(PokemonTradeWork *wk) {
    BOOL done = FALSE;
    u32 keys;

    if (wk->unk11F9 != 0 || !GFL_WipeIsFinished()) {
        return;
    }
    if (func_020355c4()) {
        keys = PokemonTrade_GetPressedKeys();
        if (keys != 0) {
            func_020355d8(FALSE);
            func_ov119_021d9780(wk, wk->unk10A0, FALSE);
            return;
        }
    }
    GFL_BGSysSetBGEnabled(3, TRUE);
    switch (func_02035c08(data_ov119_021e75f0)) {
    case 0:
        if (wk->unk10A0 != 0 || !func_020355c4()) {
            wk->unk10A0 = 0;
            func_020355d8(TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov119_021d9780(wk, wk->unk10A0, TRUE);
        }
        break;
    case 1:
        if (wk->unk10A0 != 1 || !func_020355c4()) {
            wk->unk10A0 = 1;
            func_020355d8(TRUE);
            GFL_SndSEPlay(SEQ_SE_SELECT1);
            func_ov119_021d9780(wk, wk->unk10A0, TRUE);
        }
        break;
    case 2:
        done = TRUE;
        break;
    }
    if (PokemonTrade_GetPressedKeys() == PAD_KEY_LEFT || PokemonTrade_GetPressedKeys() == PAD_KEY_RIGHT) {
        wk->unk10A0 = 1 - wk->unk10A0;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_020355d8(FALSE);
        func_ov119_021d9780(wk, wk->unk10A0, TRUE);
    }
    func_ov067_021b63d0(wk->touchBar);
    if (func_ov067_021b6448(wk->touchBar) == 1) {
        done = TRUE;
    }
    if (done) {
        GFL_WipeSet(3, 0, 0, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021d9810);
    }
}


void func_ov119_021d99f4(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        wk->unk10A0 = 0;
        func_ov119_021e1d68(wk, func_ov119_021d8fa4(wk, 0));
        func_ov119_021d8b38(wk);
        GFL_WipeSet(3, 1, 1, 0, 6, 1, wk->heapId);
        func_ov119_021d8e84(wk, func_ov119_021d98ac);
    }
}

void func_ov119_021d9a50(PokemonTradeWork *wk) {
    u8 choice;
    if (func_020279e8(wk->menu)) {
        choice = func_02027a04(wk->menu);
        func_ov119_021e12f8(wk);
        func_ov119_021e1098(wk);
        wk->menu = NULL;
        switch (choice) {
        case 0:
            func_ov119_021d8e84(wk, func_ov119_021d9750);
            break;
        case 1:
            GFL_WipeSet(3, 0, 0, 0, 6, 1, wk->heapId);
            func_ov119_021d8e84(wk, func_ov119_021d99f4);
            break;
        case 2:
            func_ov119_021d8e84(wk, func_ov119_021dadac);
            break;
        }
    }
}



static void func_ov119_021d9ad8(PokemonTradeWork *wk) {
    int i;
    GFL_MsgDataLoadStrbuf(wk->msgData, 17, wk->strbufTemplate);
    for (i = 0; i < 2; i++) {
        loadPokemonNicknameToStrbuf(wk->wordSet, i, func_ov119_021d8fa4(wk, i));
    }
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->strbuf, wk->strbufTemplate);
    func_ov119_021e1024(wk);
    func_ov119_021d8e84(wk, func_ov119_021d9b34);
}

void func_ov119_021d9b34(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[3] = { 4, 3, 5 };
        func_ov119_021e1410(wk, items, 3);
        GFL_BGSysSetEnabledBGsA(0x1f);
        func_ov067_021b6558(wk->touchBar, 1, FALSE);
        func_ov119_021d8e84(wk, func_ov119_021d9a50);
    }
}


static void func_ov119_021d9b84(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        if (func_02039cb4(func_020399a8(), 0x14, 8)) {
            func_ov119_021e1098(wk);
            func_ov119_021d8e84(wk, func_ov119_021d9434);
        }
    } else {
        func_ov119_021e1098(wk);
        func_ov119_021d8e84(wk, func_ov119_021d9434);
    }
}

static void func_ov119_021d9bc4(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk) && wk->timer > 30) {
        if (func_ov119_021d8a24(wk)) {
            func_02039c68(func_020399a8(), 0x14, 8);
        }
        func_ov119_021d8e84(wk, func_ov119_021d9b84);
    }
}

void func_ov119_021d9c00(PokemonTradeWork *wk) {
    int msg;
    int myId = func_0203d1a4(func_020399a8());
    int otherId = 1 - myId;
    if (!func_ov119_021d8a24(wk)) {
        myId = 0;
        otherId = 1;
    }
    if (func_ov119_021d8a24(wk)) {
        if (!func_02039cb4(func_020399a8(), 0x1a, 8)) {
            func_ov119_021dd24c(wk);
            return;
        }
    }
    func_ov119_021dd294(wk, 1, 1);
    if (wk->command[myId] == 1 && wk->command[otherId] == 1) {
        func_ov119_021d8e84(wk, func_ov119_021d9ad8);
        return;
    }
    if (wk->command[myId] == 4 && wk->command[otherId] == 4) {
        wk->param->next = 3;
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x8d, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021e1030(wk);
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021d9bc4);
        return;
    }
    if (wk->command[otherId] == 4) {
        msg = 0x61;
    } else if (wk->command[myId] == 4) {
        msg = 0x66;
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->command[0] = 0;
    wk->command[1] = 0;
    func_ov119_021d8e84(wk, func_ov119_021daef0);
}

void func_ov119_021d9cfc(PokemonTradeWork *wk) {
    u8 command;
    BOOL network = func_ov119_021d8a24(wk);
    if (!func_ov119_021e12ac(wk)) {
        return;
    }
    if (!network) {
        wk->command[0] = 1;
        wk->command[1] = 1;
        func_ov119_021d8e84(wk, func_ov119_021d9c00);
        return;
    }
    command = 1;
    if (func_0203d3a8(func_020399a8(), 0xc00, 1, &command)) {
        func_02039c68(func_020399a8(), 0x1a, 8);
        func_ov119_021d8e84(wk, func_ov119_021d9c00);
    }
}

void func_ov119_021d9d60(PokemonTradeWork *wk) {
    u8 choice;
    if (func_020279e8(wk->menu)) {
        choice = func_02027a04(wk->menu);
        func_ov119_021e12f8(wk);
        wk->menu = NULL;
        if (choice == 0) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 6, wk->strbuf);
            func_ov119_021e1024(wk);
            func_ov119_021e1030(wk);
            func_ov119_021d8e84(wk, func_ov119_021d9cfc);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021dba80);
        }
    }
}




static void func_ov119_021d9dc4(PokemonTradeWork *wk) {
    if (func_ov119_021db0a0(wk)) {
        u32 items[] = { 0, 5 };
        func_ov119_021e1410(wk, items, NELEMS(items));
        G2S_SetBlendBrightness(0x1b, -8);
        func_ov119_021d8e84(wk, func_ov119_021d9d60);
    }
}

void func_ov119_021d9e0c(PokemonTradeWork *wk) {
    u32 inParty = FALSE;
    if (wk->selectBox == wk->unk630) {
        inParty = TRUE;
    }
    if (func_0203d3a8(func_020399a8(), 0xc16, 1, &inParty)) {
        func_ov119_021d8e84(wk, func_ov119_021d9dc4);
    }
}

void func_ov119_021d9e54(PokemonTradeWork *wk) {
    func_ov067_021b6558(wk->touchBar, 7, FALSE);
    func_ov067_021b6558(wk->touchBar, 8, FALSE);
    func_ov067_021b6558(wk->touchBar, 9, FALSE);
    func_ov067_021b6558(wk->touchBar, 1, FALSE);
    func_0204b3dc(wk->actors[2], FALSE);
    func_ov119_021d8c6c(wk);
    func_ov119_021d8e84(wk, func_ov119_021d9e0c);
}

void func_ov119_021d9ea8(PokemonTradeWork *wk, u32 a1, u32 a2) {
    wk->unkF74 = a1;
    wk->cursorRow = a2;
    func_ov119_021d8b38(wk);
}

BOOL func_ov119_021d9ebc(PokemonTradeWork *wk, int *box, int *slot, int *a3, int *a4) {
    u32 x, y;
    int unused;
    if (func_02035d20(&x, &y) == TRUE) {
        if (func_ov119_021e51a4(wk, x + 12, y + 12, box, slot, a3, a4, &unused)) {
            BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, *box, *slot, wk);
            if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

int func_ov119_021d9f24(PokemonTradeWork *wk, BOOL a1) {
    u32 x, y;
    int a, b, c;
    ClActor *icon;
    int ret = 0;
    if (func_02035d20(&x, &y) == TRUE) {
        icon = func_ov119_021e51a4(wk, x + 12, y + 12, &wk->heldBox, &wk->heldSlot, &a, &b, &c);
        if (icon != NULL) {
            BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, wk->heldBox, wk->heldSlot, wk);
            if (func_ov119_021dbd70(wk, wk->heldBox, wk->heldSlot)) {
                GFL_SndSEPlay(SEQ_SE_MSCL_05);
                if (!func_ov119_021d8aa8(wk)) {
                    func_ov119_021d8e84(wk, func_ov119_021db9b4);
                    return 3;
                }
                wk->unkF98 = wk->heldBox;
                wk->unkF94 = wk->heldSlot;
                func_ov119_021d8e84(wk, func_ov119_021df708);
                return 3;
            }
            func_ov119_021d8c84(wk);
            if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
                if (func_ov119_021dd0cc(wk, pkm)) {
                    GFL_MsgDataLoadStrbuf(wk->msgData, 0x8e, wk->strbuf);
                    func_ov119_021e0ff4(wk, TRUE, FALSE);
                    func_ov119_021d8e84(wk, func_ov119_021da064);
                    return 2;
                }
                func_ov119_021d8ccc(wk, icon, c, b, pkm);
                func_ov119_021d9ea8(wk, a, b);
                if (a1) {
                    wk->unkF98 = wk->heldBox;
                    wk->unkF94 = wk->heldSlot;
                }
                func_ov119_021d8b38(wk);
                ret = 1;
            }
        }
    }
    return ret;
}

static void func_ov119_021da064(PokemonTradeWork *wk) {
    u32 keys;
    if (!func_ov119_021e12ac(wk)) {
        return;
    }
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
    wk->heldBox = -1;
    wk->heldSlot = -1;
    func_ov119_021e1098(wk);
    func_ov119_021d8e84(wk, func_ov119_021dbe70);
    wk->firstColumn = -1;
    func_ov119_021e4e64(wk->boxes, wk, TRUE);
}

void func_ov119_021da0cc(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x65, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->firstColumn = -1;
    func_ov119_021e4e64(wk->boxes, wk, FALSE);
    func_ov119_021d8e84(wk, func_ov119_021da064);
}

void func_ov119_021da110(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x8e, wk->strbuf);
    func_ov119_021e0ff4(wk, TRUE, FALSE);
    wk->firstColumn = -1;
    func_ov119_021e4e64(wk->boxes, wk, FALSE);
    func_ov119_021d8e84(wk, func_ov119_021da064);
}

void func_ov119_021da158(PokemonTradeWork *wk) {
    PartyPkm *pkm = func_ov119_021d8d4c(wk->boxes, wk->unkF98, wk->unkF94, wk);
    func_ov119_021e2484(wk, FALSE);
    func_ov119_021e1e00(wk, wk->unk10A0, pkm);
    if (func_020355c4()) {
        func_ov119_021d8b38(wk);
    }
    GFL_HeapFree(pkm);
    func_ov119_021d8e84(wk, func_ov119_021da318);
}


void func_ov119_021da1b4(PokemonTradeWork *wk) {
    if (func_ov119_021dd5b4(wk, wk->selectSlot, wk->selectBox)) {
        func_ov119_021d8e84(wk, func_ov119_021dbbd0);
    }
}

void func_ov119_021da1dc(PokemonTradeWork *wk) {
    u32 inParty = FALSE;
    int index;
    if (wk->selectBox == wk->unk630) {
        inParty = TRUE;
    }
    index = func_ov119_021dd3d4(wk, 0, wk->selectSlot, wk->selectBox);
    if (index != -1) {
        if (func_0203d3a8(func_020399a8(), (u16)(TRADE_NET_CMD_UNK13 + index), 1, &inParty)) {
            func_ov119_021d8e84(wk, func_ov119_021da1b4);
        }
    } else {
        func_ov119_021d8e84(wk, func_ov119_021da1b4);
    }
}


void func_ov119_021da254(PokemonTradeWork *wk) {
    if (func_ov119_021dd5b4(wk, wk->selectSlot, wk->selectBox)) {
        func_ov119_021d8e84(wk, func_ov119_021df5d8);
    }
}

void func_ov119_021da27c(PokemonTradeWork *wk) {
    u32 inParty = FALSE;
    int index;
    if (wk->selectBox == wk->unk630) {
        inParty = TRUE;
    }
    index = func_ov119_021dd3d4(wk, 0, wk->selectSlot, wk->selectBox);
    if (index != -1) {
        if (func_0203d3a8(func_020399a8(), (u16)(TRADE_NET_CMD_UNK13 + index), 1, &inParty)) {
            func_ov119_021d8e84(wk, func_ov119_021da254);
        }
    } else {
        func_ov119_021d8e84(wk, func_ov119_021da254);
    }
}












static void func_ov119_021da2f4(PokemonTradeWork *wk) {
    u32 x, y;
    if (!func_02035cb8(&x, &y)) {
        func_ov119_021d8e84(wk, func_ov119_021dbe70);
    }
}

void func_ov119_021da318(PokemonTradeWork *wk) {
    u32 x, y;
    int b, a;
    BOOL selected = FALSE;
    BOOL showCursor = TRUE;
    int choice = -1;
    BOOL decided = FALSE;
    BOOL touched;
    if (!func_02027a20(wk->menu)) {
        touched = func_02035c60();
        if (touched) {
            func_020355d8(TRUE);
        }
        if (touched) {
            func_ov119_021d8c84(wk);
        }
        func_ov067_021b63d0(wk->touchBar);
        if (func_ov067_021b6470(wk->touchBar) == 9) {
            if (func_020355c4()) {
                func_02027a08(wk->menu, FALSE);
            }
        }
        if (func_ov067_021b6448(wk->touchBar) == 9) {
            selected = TRUE;
            wk->unk10A0 = 1 - wk->unk10A0;
        }
        if (func_02035d20(&x, &y) == TRUE) {
            func_020355d8(TRUE);
            func_02027a08(wk->menu, FALSE);
            if (func_ov119_021d9ebc(wk, &wk->unkF98, &wk->unkF94, &b, &a)) {
                wk->cursorRow = a;
                wk->unkF74 = b;
                wk->touchX = x;
                wk->unkF58 = y;
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                selected = TRUE;
            } else if (y >= 16 && y < 144) {
                decided = TRUE;
            }
        }
        if (selected) {
            func_ov119_021e2484(wk, TRUE);
            func_ov119_021d8e84(wk, func_ov119_021da158);
            return;
        }
        func_ov119_021d8c0c(wk);
        if (!decided && !func_02035c38() && wk->unkF34) {
            if (wk->unkF60 != NULL && wk->unkF5C) {
                GFL_SndSEPlay(SEQ_SE_SYS_56);
                choice = 0;
                decided = TRUE;
            } else {
                func_ov119_021d8c84(wk);
            }
        }
        func_ov119_021d8b54(wk);
        func_ov119_021d8c0c(wk);
    }
    if (func_020279e8(wk->menu)) {
        choice = func_02027a04(wk->menu);
        decided = TRUE;
    }
    if (decided) {
        if (choice == 0) {
            BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, wk->unkF98, wk->unkF94, wk);
            if (func_ov119_021dbd70(wk, wk->unkF98, wk->unkF94) && func_ov119_021d8aa8(wk)) {
                wk->selectSlot = wk->unkF94;
                wk->selectBox = wk->unkF98;
                func_ov119_021d8e84(wk, func_ov119_021da1dc);
            } else if (func_ov119_021d9548(wk, wk->unkF98, wk->unkF94)) {
                func_ov119_021df7ac(wk, TRUE);
                func_ov119_021d8e84(wk, func_ov119_021da0cc);
            } else if (func_ov119_021dd0cc(wk, pkm)) {
                func_ov119_021d8e84(wk, func_ov119_021da110);
                func_ov119_021df7ac(wk, TRUE);
            } else {
                if (!func_ov119_021d8aa8(wk)) {
                    wk->selectSlot = wk->unkF94;
                    wk->selectBox = wk->unkF98;
                    func_ov119_021d8e84(wk, func_ov119_021d9e54);
                    showCursor = FALSE;
                    func_ov067_021b6558(wk->touchBar, 7, FALSE);
                    func_ov067_021b6558(wk->touchBar, 8, FALSE);
                    func_ov067_021b6558(wk->touchBar, 9, FALSE);
                } else if (func_ov119_021dd2a8(wk) && !func_020355c4()) {
                    wk->selectSlot = wk->unkF94;
                    wk->selectBox = wk->unkF98;
                    func_ov119_021d8e84(wk, func_ov119_021da27c);
                    showCursor = FALSE;
                    func_ov067_021b6558(wk->touchBar, 7, FALSE);
                    func_ov067_021b6558(wk->touchBar, 8, FALSE);
                    func_ov067_021b6558(wk->touchBar, 9, FALSE);
                } else {
                    wk->selectSlot = wk->unkF94;
                    wk->selectBox = wk->unkF98;
                    func_ov119_021d8e84(wk, func_ov119_021da1dc);
                }
            }
        } else {
            func_ov119_021df7ac(wk, TRUE);
            func_ov119_021d8e84(wk, func_ov119_021da2f4);
        }
        wk->unkF98 = -1;
        wk->unkF94 = -1;
        if (showCursor) {
            func_ov119_021dbb6c(wk);
            func_0204b3dc(wk->actors[2], TRUE);
        }
        func_ov119_021e5ef8(&wk->infoIcons[0]);
        func_ov119_021e5ef8(&wk->infoIcons[1]);
        func_ov119_021e5ef8(&wk->infoIcons[2]);
        func_ov119_021e2484(wk, TRUE);
        GFL_BGSysQueueScrLoad(6);
        func_ov119_021e4174(wk);
        func_ov119_021e12f8(wk);
        wk->menu = NULL;
    }
    wk->unkF34 = func_02035c38();
}

// pokebw2's sSideButtonRects
static const TouchRect data_ov119_021e75f0[] = {
    { 4, 28, 88, 112 },
    { 4, 28, 152, 176 },
    { TOUCH_RECT_END },
};

void func_ov119_021da640(PokemonTradeWork *wk) {
    PartyPkm *pkm;

    func_ov119_021df7ac(wk, FALSE);
    func_ov067_021b6558(wk->touchBar, 1, FALSE);
    func_ov067_021b6558(wk->touchBar, 7, FALSE);
    func_ov067_021b6558(wk->touchBar, 9, TRUE);
    {
        u32 items[2] = { 2, 5 };
        func_ov119_021e1410(wk, items, 2);
    }
    pkm = func_ov119_021d8d4c(wk->boxes, wk->unkF98, wk->unkF94, wk);
    wk->unk10A0 = 0;
    func_ov119_021e1e00(wk, 0, pkm);
    GFL_HeapFree(pkm);
    func_0204b3dc(wk->actors[2], FALSE);
    func_ov119_021d8c84(wk);
    func_ov119_021d8b38(wk);
    if (func_020355c4()) {
        func_02027a08(wk->menu, FALSE);
    } else {
        func_02027a08(wk->menu, TRUE);
    }
    func_ov119_021d8e84(wk, func_ov119_021da318);
}


void func_ov119_021da700(PokemonTradeWork *wk) {
    u8 side = wk->unk10A0;

    if (func_ov119_021d8a24(wk)) {
        if (func_0203d3a8(func_020399a8(), 0xc03, 1, &side) == FALSE) {
            return;
        }
    }
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    if (func_ov119_021d8aa8(wk) == FALSE) {
        func_ov119_021d8e84(wk, func_ov117_021d4b88);
    } else {
        func_ov119_021d8e84(wk, func_ov118_021d6f2c);
    }
}



void func_ov119_021da76c(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk) || func_02039cb4(func_020399a8(), 13, 8)) {
        func_ov119_021d8e84(wk, func_ov119_021da700);
    }
}

void func_ov119_021da798(PokemonTradeWork *wk) {
    int myId = func_0203d1a4(func_020399a8());
    int otherId = 1 - myId;
    int msg;
    if (func_ov119_021d8a24(wk)) {
        if (!func_02039cb4(func_020399a8(), 0x1b, 8)) {
            func_ov119_021dd24c(wk);
            return;
        }
    }
    func_ov119_021dd294(wk, 1, 1);
    if (wk->command[myId] == 2 && wk->command[otherId] == 2) {
        if (func_ov119_021d8a24(wk)) {
            func_02039c68(func_020399a8(), 0xd, 8);
        }
        func_ov119_021d8e84(wk, func_ov119_021da76c);
        return;
    }
    if (wk->command[otherId] == 5) {
        msg = 97;
    } else if (wk->command[myId] == 5) {
        msg = 98;
    } else if (wk->command[otherId] == 7) {
        msg = 99;
    } else if (wk->command[myId] == 7) {
        msg = 100;
    } else if (wk->command[otherId] == 8) {
        msg = 100;
    } else if (wk->command[myId] == 8) {
        msg = 99;
    } else if (wk->command[otherId] == 9) {
        msg = 97;
    } else if (wk->command[myId] == 9) {
        msg = 101;
    } else if (wk->command[otherId] == 6) {
        msg = 97;
    } else if (wk->command[myId] == 6) {
        msg = 111;
    } else if (wk->command[otherId] == 3) {
        msg = 97;
    } else if (wk->command[myId] == 3) {
        msg = 102;
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, msg, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->command[0] = 0;
    wk->command[1] = 0;
    func_ov119_021d8e84(wk, func_ov119_021daef0);
}


static void func_ov119_021da898(PokemonTradeWork *wk) {
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021dba80(wk);
        func_ov119_021dbaf8(wk);
        func_ov119_021d8e84(wk, func_ov119_021dc370);
    } else {
        func_ov119_021dba80(wk);
        func_ov119_021dbaf8(wk);
    }
    GFL_WipeSet(0, 1, 1, 0, 6, 1, wk->heapId);
    GFL_BGSysSetEnabledBGsA(0x1f);
    GFL_BGSysSetEnabledBGsB(0x1e);
}

void func_ov119_021da8f8(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        if (!func_02039cb4(func_020399a8(), 0x12, 8)) {
            return;
        }
    }
    func_ov119_021e1098(wk);
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021dd1b8(wk);
        func_ov119_021df904(wk);
        if (wk->type == 2) {
            func_ov009_0215642c(FALSE);
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x86, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021dd528(wk, 0);
        func_ov119_021dd68c(wk);
        wk->unk11E8[0] = 0;
        wk->unk11E8[1] = 0;
        func_ov119_021df78c(wk);
        func_ov119_021df7ac(wk, TRUE);
        MI_CpuFill8(wk->iconSpecies, 0, 0x78);
        MI_CpuFill8(wk->iconForms, 0, 0x3c);
        MI_CpuFill8(wk->iconSexes, 0, 0x3c);
        func_ov119_021d8c84(wk);
        if (wk->menu != NULL) {
            func_02027858(wk->menu);
            wk->menu = NULL;
        }
        func_ov119_021e4420(wk);
        func_ov119_021e6700(wk);
        func_ov119_021dbb6c(wk);
    }
    func_ov119_021d8e84(wk, func_ov119_021da898);
    func_ov119_021db7ac(wk, 0, 1);
}


static void func_ov119_021da9f0(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk) && GFL_WipeIsFinished()) {
        func_ov119_021e1098(wk);
        func_ov119_021d8e84(wk, func_ov119_021da8f8);
    }
}

static void func_ov119_021daa18(PokemonTradeWork *wk) {
    GFL_WipeSet(4, 0, 0, 0, 6, 1, wk->heapId);
    func_ov119_021d8e84(wk, func_ov119_021da9f0);
}

static void func_ov119_021daa4c(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk) || func_02039cb4(func_020399a8(), 18, 8)) {
        func_ov119_021e31fc(wk);
        func_ov119_021d8e84(wk, func_ov119_021daa18);
    }
}

void func_ov119_021daa7c(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        if (func_0203d3f8(func_020399a8(), 0xff, 0xc0e, 0x2d6, wk->unk10, 0, FALSE, TRUE)) {
            func_02039c68(func_020399a8(), 0x12, 8);
            func_ov119_021d8e84(wk, func_ov119_021daa4c);
        }
    } else {
        func_ov119_021e31fc(wk);
        func_ov119_021d8e84(wk, func_ov119_021daa4c);
    }
}


static void func_ov119_021daae4(PokemonTradeWork *wk) {
    func_ov119_021dc62c(wk, wk->timer - 1);
    if (wk->timer >= 24) {
        func_ov119_021d8e84(wk, func_ov119_021daa7c);
    }
}

static void func_ov119_021dab0c(PokemonTradeWork *wk) {
    if (func_ov119_021e3e00(wk, wk->timer - 1)) {
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021daae4);
    }
}

void func_ov119_021dab34(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        u8 data[2];
        data[0] = wk->unk630;
        data[1] = func_ov119_021d89a8(wk);
        if (func_0203d3a8(func_020399a8(), 0xc10, 2, data)) {
            wk->timer = 0;
            func_ov119_021d8e84(wk, func_ov119_021dab0c);
        }
    } else {
        wk->unk11F2 = wk->unk630;
        wk->unk107E = wk->unk11F2 * 160 + 16;
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021dab0c);
    }
}

void func_ov119_021dabb0(PokemonTradeWork *wk) {
    func_ov119_021e5098(wk, wk->timer - 1);
    if (wk->timer > 25) {
        func_ov119_021e4e64(wk->boxes, wk, FALSE);
        func_ov119_021d8e84(wk, func_ov119_021dab34);
    }
}



static void func_ov119_021dabe8(PokemonTradeWork *wk) {
    if (func_02039cb4(func_020399a8(), 22, 8)) {
        func_0203d7b0(TRUE);
        func_0203d7bc(TRUE);
        wk->timer = 0;
        func_ov119_021d8e84(wk, func_ov119_021dabb0);
    }
}

static void func_ov119_021dac20(PokemonTradeWork *wk) {
    if (GFL_WipeIsFinished()) {
        if (func_ov119_021d8a24(wk)) {
            func_02039c68(func_020399a8(), 22, 8);
            func_ov119_021d8e84(wk, func_ov119_021dabe8);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021dab34);
        }
    }
}

static void func_ov119_021dac60(PokemonTradeWork *wk) {
    func_0203d34c(TRUE, wk->heapId);
    if (wk->type == 2) {
        func_ov009_0215642c(TRUE);
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, 6, wk->strbuf);
    func_ov119_021e0ff4(wk, TRUE, 0);
    func_ov119_021e1030(wk);
    GFL_BGSysSetEnabledBGsA(0x10);
    GFL_BGSysSetEnabledBGsB(4);
    killBrightnessEitherEngine(0);
    GFL_WipeSet(4, 1, 1, 0, 6, 1, wk->heapId);
    func_ov119_021d8e84(wk, func_ov119_021dac20);
}

void func_ov119_021dace4(PokemonTradeWork *wk) {
    int i;
    func_02039338();
    wk->selectBox = -1;
    wk->selectSlot = -1;
    wk->unk800 = 0;
    wk->unkF60 = NULL;
    wk->unk1060 = 0;
    func_ov119_021e36d8(wk, 0);
    func_ov119_021e36d8(wk, 1);
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021dd1b8(wk);
    }
    MI_CpuFill8(wk->unk5DC[0], 0, PokeParty_GetPkmRawSize());
    MI_CpuFill8(wk->unk5DC[1], 0, PokeParty_GetPkmRawSize());
    for (i = 0; i < 10; i++) {
        if (wk->actors[i] != NULL) {
            func_0204b3dc(wk->actors[i], TRUE);
        }
    }
    func_ov119_021e1098(wk);
    func_ov119_021d8e84(wk, func_ov119_021dac60);
}


static void func_ov119_021dad84(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        func_02039c68(func_020399a8(), 0x1b, 8);
        func_ov119_021d8e84(wk, func_ov119_021da798);
    }
}

static void func_ov119_021dadac(PokemonTradeWork *wk) {
    u8 command = 3;
    if (!func_ov119_021d8a24(wk)) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 97, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021d8e84(wk, func_ov119_021daef0);
        return;
    }
    if (func_0203d3a8(func_020399a8(), TRADE_NET_CMD_SELECT, 1, &command)) {
        func_02039c68(func_020399a8(), 0x1b, 8);
        GFL_MsgDataLoadStrbuf(wk->msgData, 6, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021e1030(wk);
        func_ov119_021d8e84(wk, func_ov119_021dad84);
    }
}

void func_ov119_021dae2c(PokemonTradeWork *wk) {
    int i;
    if (!func_02039cb4(func_020399a8(), 0x19, 8)) {
        func_ov119_021dd24c(wk);
        return;
    }
    func_ov119_021dd294(wk, 1, 1);
    func_ov119_021e1098(wk);
    wk->command[0] = 0;
    wk->command[1] = 0;
    wk->unk11E8[0] = 0;
    wk->unk11E8[1] = 0;
    for (i = 0; i < 6; i++) {
        wk->negoSlot[i / 3][i % 3] = -1;
        if (wk->negoPkm[i / 3][i % 3] != NULL) {
            MI_CpuFill8(wk->negoPkm[i / 3][i % 3], 0, PokeParty_GetPkmRawSize());
        }
    }
    func_ov119_021d8e84(wk, func_ov119_021dba80);
}


static void func_ov119_021daec8(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        func_02039c68(func_020399a8(), 0x19, 8);
        func_ov119_021d8e84(wk, func_ov119_021dae2c);
    }
}

void func_ov119_021daef0(PokemonTradeWork *wk) {
    u32 keys;
    if (!func_ov119_021e12ac(wk)) {
        return;
    }
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
    func_ov119_021e1098(wk);
    if (func_ov119_021d8a24(wk)) {
        GFL_MsgDataLoadStrbuf(wk->msgData, 0x32, wk->strbuf);
        func_ov119_021e1024(wk);
        func_ov119_021e1030(wk);
        func_ov119_021d8e84(wk, func_ov119_021daec8);
        return;
    }
    func_ov119_021d8e84(wk, func_ov119_021dba80);
}


static void func_ov119_021daf68(PokemonTradeWork *wk) {
    NetHandle *handle = func_020399a8();
    if (func_ov119_021e12ac(wk)) {
        if (func_ov119_021d8a24(wk)) {
            u8 command = 2;
            if (func_0203d3a8(handle, TRADE_NET_CMD_UNKF, 1, &command)) {
                func_ov119_021d8e84(wk, func_ov119_021d9c00);
            }
        } else {
            func_ov119_021d8e84(wk, func_ov119_021d9c00);
        }
    }
}

void func_ov119_021dafb8(PokemonTradeWork *wk) {
    if (func_ov119_021d8a24(wk)) {
        u8 command = 4;
        if (func_0203d3a8(func_020399a8(), 0xc00, 1, &command)) {
            GFL_MsgDataLoadStrbuf(wk->msgData, 0x88, wk->strbuf);
            func_ov119_021e1024(wk);
            func_ov119_021e1030(wk);
            func_02039c68(func_020399a8(), 0x1a, 8);
            func_ov119_021d8e84(wk, func_ov119_021daf68);
        }
    } else {
        wk->param->next = 3;
        func_ov119_021d8e84(wk, func_ov119_021d9434);
    }
}

void func_ov119_021db030(PokemonTradeWork *wk) {
    u8 choice;
    if (func_020279e8(wk->menu)) {
        choice = func_02027a04(wk->menu);
        func_ov119_021e12f8(wk);
        wk->menu = NULL;
        switch (choice) {
        case 0:
            func_ov119_021d8e84(wk, func_ov119_021dafb8);
            break;
        case 1:
            func_ov119_021e1098(wk);
            func_ov119_021d8e84(wk, func_ov119_021dbe70);
            wk->firstColumn = -1;
            func_ov119_021e4e64(wk->boxes, wk, FALSE);
            func_ov119_021e6700(wk);
            break;
        }
    }
}

BOOL func_ov119_021db0a0(PokemonTradeWork *wk) {
    PartyPkm *pkm = func_ov119_021d8d4c(wk->boxes, wk->selectBox, wk->selectSlot, wk);
    if (func_ov119_021d8a24(wk)) {
        if (!func_0203d3a8(func_020399a8(), 0xc01, (u16)PokeParty_GetPkmRawSize(), pkm)) {
            GFL_HeapFree(pkm);
            return FALSE;
        }
    }
    func_ov119_021d9468(wk, 0, pkm);
    GFL_HeapFree(pkm);
    return TRUE;
}

// Where each box starts in the strip
// pokebw2's sBoxStartX
static int data_ov119_021e7fe0[] = {
#include "asset/pokemontrade_proc_data_ov119_021e7fe0.inc"
};

// The boxes of the box list
// pokebw2's sBoxListRects
static TouchRect data_ov119_021e8044[] = {
#include "asset/pokemontrade_proc_data_ov119_021e8044.inc"
};

void func_ov119_021db10c(PokemonTradeWork *wk) {
    if (func_ov119_021e12ac(wk)) {
        u32 items[2] = { 0x18, 0x19 };

        func_ov119_021e131c(wk, items, 2, 32, 12);
        wk->firstColumn = -1;
        func_ov119_021e4e64(wk->boxes, wk, FALSE);
        func_ov119_021d8e84(wk, func_ov119_021db030);
    }
}


void func_ov119_021db164(PokemonTradeWork *wk) {
    GFL_MsgDataLoadStrbuf(wk->msgData, 0x8c, wk->strbuf);
    func_ov119_021e1024(wk);
    wk->unk1074 = 1;
    func_ov119_021d8e84(wk, func_ov119_021db10c);
}





static void func_ov119_021db198(PokemonTradeWork *wk) {
    G2S_SetBlendBrightness(0x1b, -8);
    func_ov119_021d8e84(wk, func_ov119_021db164);
}

int func_ov119_021db4b4(int column);

BOOL func_ov119_021db1bc(PokemonTradeWork *wk, u32 initial) {
    int column;
    int i;
    int slot;
    int box;
    BoxPkm *pkm;

    column = func_ov119_021e4dbc(wk);
    for (i = 0; i < wk->unk634; i++) {
        column++;
        if (column >= wk->unk634) {
            column = 0;
        }
        for (slot = 0; slot < 5; slot++) {
            box = func_ov119_021d8af4(column, wk);
            pkm = func_ov119_021d8dd0(wk->boxes, box, func_ov119_021d8b0c(column, slot), wk);
            if (pkm != NULL && func_ov119_021e4aa4(PML_PkmGetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL), initial) &&
                PML_PkmGetParam(pkm, PKM_PARAM_IS_EGG, NULL) == 0) {
                wk->firstColumn++;
                wk->unk107C = func_ov119_021db4b4(column) - 32;
                wk->unk1084 = 1;
                func_ov119_021db7ac(wk, 0, TRUE);
                return TRUE;
            }
        }
    }
    return FALSE;
}


void func_ov119_021db278(PokemonTradeWork *wk) {
    GFL_BGSysSetEnabledBGsB(0x1e);
    func_ov119_021d8e84(wk, func_ov119_021dbe70);
}

void func_ov119_021db290(PokemonTradeWork *wk) {
    GFL_BGSysSetEnabledBGsB(0x1a);
    func_ov119_021e5d84(wk);
    func_ov119_021e1098(wk);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov119_021df7ac(wk, 1);
    func_ov119_021e71e0(wk, 0);
    func_ov119_021dbb6c(wk);
    func_ov119_021e6700(wk);
    func_ov119_021e7058(wk);
    func_ov119_021d8e84(wk, func_ov119_021db278);
}

void func_ov119_021db2e8(PokemonTradeWork *wk) {
    if (!func_ov119_021e707c(wk)) {
        func_ov119_021d8e84(wk, func_ov119_021db290);
    }
}

void func_ov119_021db304(PokemonTradeWork *wk) {
    int box;

    GFL_BGSysSetEnabledBGsB(0x1f);
    if (func_ov119_021e12ac(wk)) {
        box = func_02035c08(data_ov119_021e8044);
        if (box != -1) {
            wk->unk800 = box + 1;
            func_ov119_021e6fd4(wk, data_ov119_021e8044[box].left, data_ov119_021e8044[box].top);
            if (func_ov119_021db1bc(wk, box)) {
                GFL_SndSEPlay(SEQ_SE_SELECT1);
                GXS_SetVisibleWnd(GX_WNDMASK_NONE);
                func_ov119_021d8e84(wk, func_ov119_021db2e8);
                return;
            }
            GFL_MsgDataLoadStrbuf(wk->msgData, 0x34, wk->strbuf);
            func_ov119_021e0ef4(wk, TRUE, 1, 1, 0x1d, 4);
            GFL_SndSEPlay(SEQ_SE_BEEP);
        }
        func_ov067_021b63d0(wk->touchBar);
        if (func_ov067_021b6448(wk->touchBar) == 1) {
            GFL_BGSysSetEnabledBGsB(0x1a);
            wk->unk800 = 0;
            func_ov119_021e5d84(wk);
            func_ov119_021e1098(wk);
            GXS_SetVisibleWnd(GX_WNDMASK_NONE);
            func_ov119_021df7ac(wk, TRUE);
            func_ov119_021e71e0(wk, FALSE);
            func_ov119_021dbb6c(wk);
            func_ov119_021e6700(wk);
            func_ov119_021e7058(wk);
            func_ov119_021d8e84(wk, func_ov119_021db290);
        }
    }
}


static void func_ov119_021db41c(PokemonTradeWork *wk) {
    func_ov119_021e5c64(wk);
    GFL_MsgDataLoadStrbuf(wk->msgData, 51, wk->strbuf);
    func_ov119_021e0ef4(wk, 1, 1, 1, 29, 4);
    func_ov119_021d8e84(wk, func_ov119_021db304);
}

void func_ov119_021db45c(PokemonTradeWork *wk) {
    func_ov067_021b6548(wk->touchBar, 7, FALSE);
    func_ov119_021df7ac(wk, FALSE);
    G2S_SetBlendBrightness(10, -8);
    func_ov119_021e71e0(wk, TRUE);
    if (func_ov119_021dd2f4(wk)) {
        func_ov067_021b6548(wk->touchBar, 8, FALSE);
    }
    func_ov119_021d8e84(wk, func_ov119_021db41c);
}


// The x of a column of the strip
// pokebw2's PokemonTrade_GetColumnX
static int func_ov119_021db4b4(int column) {
    int n, box;
    if (column == 0) {
        return 0;
    }
    if (column == 1) {
        return 48;
    }
    n = column - 2;
    box = n / 6;
    n %= 6;
    return box * 160 + 96 + n * 26;
}

BOOL func_ov119_021db4ec(PokemonTradeWork *wk, int column) {
    int x = func_ov119_021db4b4(column);
    if (wk->unk107C < x && wk->unk107C + 224 > x) {
        return TRUE;
    }
    x += wk->unk63C;
    if (wk->unk107C < x && wk->unk107C + 224 > x) {
        return TRUE;
    }
    return FALSE;
}

void func_ov119_021db52c(PokemonTradeWork *wk) {
    int i;
    int column;
    BOOL moved = FALSE;
    u32 keys = GCTX_HIDGetTypedKeys();

    if (wk->unkF60 != NULL) {
        return;
    }
    if (keys & PAD_BUTTON_R) {
        func_020355d8(FALSE);
        for (i = 0; i < wk->unk630 + 1; i++) {
            if (data_ov119_021e7fe0[i] > wk->unk107C) {
                wk->unk107C = data_ov119_021e7fe0[i];
                moved = TRUE;
                break;
            }
        }
        if (!moved) {
            moved = TRUE;
            wk->unk107C = data_ov119_021e7fe0[0];
        }
        func_ov119_021dd018(wk, moved);
    } else if (keys & PAD_BUTTON_L) {
        func_020355d8(FALSE);
        for (i = wk->unk630; i >= 0; i--) {
            if (data_ov119_021e7fe0[i] < wk->unk107C) {
                wk->unk107C = data_ov119_021e7fe0[i];
                moved = TRUE;
                break;
            }
        }
        if (!moved) {
            moved = TRUE;
            wk->unk107C = data_ov119_021e7fe0[wk->unk630];
        }
        func_ov119_021dd018(wk, moved);
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_UP) {
        func_020355d8(FALSE);
        if (func_ov119_021e4e0c(wk, &column) == 0) {
            wk->unkF74 = column;
        }
        wk->cursorRow--;
        if (wk->cursorRow < 0) {
            if (wk->unkF74 < 2) {
                wk->cursorRow = 2;
            } else {
                wk->cursorRow = 4;
            }
        }
        wk->firstColumn--;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_DOWN) {
        func_020355d8(FALSE);
        if (func_ov119_021e4e0c(wk, &column) == 0) {
            wk->unkF74 = column;
        }
        wk->cursorRow++;
        if (wk->unkF74 < 2) {
            if (wk->cursorRow >= 3) {
                wk->cursorRow = 0;
            }
        } else if (wk->cursorRow >= 5) {
            wk->cursorRow = 0;
        }
        wk->firstColumn--;
        moved = TRUE;
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_RIGHT) {
        func_020355d8(FALSE);
        moved = TRUE;
        wk->firstColumn--;
        if (func_ov119_021e4e0c(wk, &column) == 0) {
            wk->unkF74 = column;
        } else {
            wk->unkF74++;
            if (func_ov119_021db4ec(wk, wk->unkF74) == 0) {
                wk->unk107C = func_ov119_021db4b4(wk->unkF74 + 1) - 256;
                wk->firstColumn = -1;
            }
        }
        if (wk->unkF74 >= wk->unk634) {
            wk->unkF74 = 0;
        }
        if (wk->unkF74 < 2 && wk->cursorRow >= 3) {
            wk->cursorRow = 2;
        }
    } else if (GCTX_HIDGetTypedKeys() == PAD_KEY_LEFT) {
        func_020355d8(FALSE);
        wk->firstColumn++;
        moved = TRUE;
        if (func_ov119_021e4e0c(wk, &column) == 0) {
            wk->unkF74 = column;
        } else {
            wk->unkF74--;
            if (wk->unkF74 < 0) {
                wk->unkF74 = wk->unk634 - 1;
            }
            if (func_ov119_021db4ec(wk, wk->unkF74) == 0) {
                wk->unk107C = func_ov119_021db4b4(wk->unkF74);
                wk->firstColumn = -1;
            }
        }
        if (wk->unkF74 < 0) {
            wk->unkF74 = wk->unk634 - 1;
        }
        if (wk->unkF74 < 2 && wk->cursorRow >= 3) {
            wk->cursorRow = 2;
        }
    }
    if (moved) {
        wk->unk1084 = 1;
        GFL_SndSEPlay(SEQ_SE_SELECT1);
        func_ov119_021db7ac(wk, 0, TRUE);
    }
}


void func_ov119_021db7ac(PokemonTradeWork *wk, BOOL playSound, BOOL send) {
    if (wk->unk107C < 0) {
        wk->unk107C += wk->unk63C;
    }
    if (wk->unk63C <= wk->unk107C) {
        wk->unk107C -= wk->unk63C;
    }
    func_ov119_021e4e64(wk->boxes, wk, TRUE);
    func_ov119_021e42b4(wk);
    func_ov119_021e4598(wk);
    if (!GFL_SndPlayerIsActiveAny() && playSound) {
        GFL_SndSEPlay(SEQ_SE_SYS_57);
    }
    if (func_ov119_021d8a24(wk) && send) {
        if (func_0203d29c()) {
            if (wk->unk107C % 16 == 0) {
                func_0203d3f8(func_020399a8(), 0xff, 0xc0c, 2, &wk->unk107C, 0, TRUE, TRUE);
            }
        } else {
            func_0203d3f8(func_020399a8(), 0xff, 0xc0c, 2, &wk->unk107C, 0, TRUE, TRUE);
        }
    } else if (!func_ov119_021d8a24(wk)) {
        wk->unk107E = wk->unk107C;
    }
}

void func_ov119_021db89c(PokemonTradeWork *wk) {
    u32 x, y;
    int i;
    if (wk->unkF60 != NULL) {
        return;
    }
    if (func_02035cb8(&x, &y)) {
        if (wk->unk1088 != 0) {
            if (x >= 0x40 && x < 0xc0 && y >= 0xa8 && y < 0xbc) {
                func_0204b92c(wk->actors[2], TRUE);
                wk->unk107A = (x - wk->unk1078) * 2;
                wk->unk1078 = x;
                wk->unk107C -= wk->unk107A;
                if (wk->unk107A > 12) {
                    func_ov119_021db7ac(wk, TRUE, TRUE);
                } else {
                    func_ov119_021db7ac(wk, FALSE, TRUE);
                }
            } else {
                wk->unk107A = 0;
            }
        }
        wk->unk1078 = x;
    } else {
        wk->unk1078 = 0;
    }
    if (!func_02035c38() && wk->unk107A != 0) {
        for (i = 2; i != 0 && wk->unk107A != 0; i--) {
            if (wk->unk107A < 0) {
                wk->unk107A++;
            }
            if (wk->unk107A > 0) {
                wk->unk107A--;
            }
        }
        wk->unk107C -= wk->unk107A;
        func_ov119_021db7ac(wk, TRUE, TRUE);
    }
}




static void func_ov119_021db998(PokemonTradeWork *wk) {
    if (!func_02035c38()) {
        func_ov119_021d8e84(wk, func_ov119_021dbe70);
    }
}

void func_ov119_021db9b4(PokemonTradeWork *wk) {
    func_ov119_021dd34c(wk, wk->unkF98, wk->unkF94);
    if (func_ov119_021d8a24(wk)) {
        if (!func_0203d3a8(func_020399a8(), 0xc06, 0, NULL)) {
            return;
        }
    }
    func_ov067_021b6558(wk->touchBar, 8, FALSE);
    wk->selectSlot = -1;
    wk->selectBox = -1;
    wk->heldSlot = -1;
    wk->heldBox = -1;
    wk->unkF94 = -1;
    wk->unkF98 = -1;
    func_ov119_021d94a0(wk, 0);
    func_ov119_021d8b38(wk);
    func_ov119_021d8e84(wk, func_ov119_021db998);
}


static void func_ov119_021dba38(PokemonTradeWork *wk) {
    if (!func_ov119_021d8a24(wk)) {
        func_ov119_021d94a0(wk, 1);
        func_ov119_021d8e80(wk);
        func_ov119_021d8e84(wk, func_ov119_021dbaf8);
    } else if (func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNK6, 0, NULL)) {
        func_ov119_021d8e84(wk, func_ov119_021dbaf8);
    }
}

void func_ov119_021dba80(PokemonTradeWork *wk) {
    wk->unk1088 = 0;
    wk->unk1078 = 0;
    wk->unk107A = 0;
    MI_CpuFill8(wk->iconSpecies, 0, 0x78);
    MI_CpuFill8(wk->iconForms, 0, 0x3c);
    MI_CpuFill8(wk->iconSexes, 0, 0x3c);
    func_ov119_021d8c84(wk);
    func_ov119_021df7ac(wk, TRUE);
    if (wk->menu != NULL) {
        func_02027858(wk->menu);
        wk->menu = NULL;
    }
    func_ov119_021d94a0(wk, 0);
    func_ov119_021d8e84(wk, func_ov119_021dba38);
}

void func_ov119_021dbaf8(PokemonTradeWork *wk) {
    func_ov067_021b6558(wk->touchBar, 8, FALSE);
    func_ov067_021b6558(wk->touchBar, 7, TRUE);
    func_ov067_021b6548(wk->touchBar, 7, TRUE);
    func_ov067_021b6558(wk->touchBar, 9, FALSE);
    func_ov119_021d8980(wk);
    func_0204b3dc(wk->actors[2], TRUE);
    func_ov119_021e4420(wk);
    func_ov119_021e6700(wk);
    wk->selectSlot = -1;
    wk->selectBox = -1;
    func_ov119_021d8b38(wk);
    func_ov119_021d8e84(wk, func_ov119_021dbe70);
}

void func_ov119_021dbb6c(PokemonTradeWork *wk) {
    func_0204b3dc(wk->actors[2], TRUE);
    func_ov067_021b6558(wk->touchBar, 8, func_ov119_021dd2f4(wk));
    func_ov067_021b6548(wk->touchBar, 8, func_ov119_021dd2f4(wk));
    func_ov067_021b6558(wk->touchBar, 7, TRUE);
    func_ov067_021b6548(wk->touchBar, 7, TRUE);
    func_ov067_021b6558(wk->touchBar, 9, FALSE);
    func_ov119_021d8980(wk);
    func_ov119_021d8b38(wk);
}

void func_ov119_021dbbd0(PokemonTradeWork *wk) {
    func_ov119_021df7ac(wk, TRUE);
    MI_CpuFill8(wk->iconSpecies, 0, 0x78);
    MI_CpuFill8(wk->iconForms, 0, 0x3c);
    MI_CpuFill8(wk->iconSexes, 0, 0x3c);
    func_ov119_021d8c84(wk);
    if (wk->menu != NULL) {
        func_02027858(wk->menu);
        wk->menu = NULL;
    }
    func_ov119_021e4420(wk);
    func_ov119_021e6700(wk);
    func_ov119_021dbb6c(wk);
    func_ov119_021d8e84(wk, func_ov119_021dbe70);
}

void func_ov119_021dbc3c(PokemonTradeWork *wk) {
    if (func_ov119_021d8aa8(wk)) {
        if (!func_ov119_021dd5b4(wk, wk->selectSlot, wk->selectBox)) {
            return;
        }
    } else if (!func_ov119_021db0a0(wk)) {
        return;
    }
    wk->unkF98 = -1;
    wk->unkF94 = -1;
    wk->heldSlot = -1;
    wk->heldBox = -1;
    func_ov119_021d8b38(wk);
    func_ov119_021d8e84(wk, func_ov119_021dbe70);
}

void func_ov119_021dbc98(PokemonTradeWork *wk) {
    u32 inParty = FALSE;
    if (wk->selectBox == wk->unk630) {
        inParty = TRUE;
    }
    if (func_ov119_021d8aa8(wk)) {
        int index = func_ov119_021dd3d4(wk, 0, wk->selectSlot, wk->selectBox);
        if (index != -1) {
            if (func_0203d3a8(func_020399a8(), (u16)(TRADE_NET_CMD_UNK13 + index), 1, &inParty)) {
                func_ov119_021d8e84(wk, func_ov119_021dbc3c);
            }
        } else {
            func_ov119_021d8e84(wk, func_ov119_021dbc3c);
        }
    } else if (func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNK16, 1, &inParty)) {
        func_ov119_021d8e84(wk, func_ov119_021dbc3c);
    }
}







void func_ov119_021dbd3c(PokemonTradeWork *wk) {
    if (func_ov119_021e665c(wk)) {
        if (func_ov119_021d8a24(wk)) {
            func_ov119_021d8e84(wk, func_ov119_021dbc98);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021dbc3c);
        }
    }
}

BOOL func_ov119_021dbd70(PokemonTradeWork *wk, int box, int slot) {
    if (func_ov119_021d8aa8(wk) == FALSE) {
        if (wk->selectSlot == slot && wk->selectBox == box) {
            return TRUE;
        }
    } else {
        if (func_ov119_021dd34c(wk, box, slot) != -1) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov119_021dbdb4(PokemonTradeWork *wk) {
    BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, wk->unkF98, wk->unkF94, wk);
    if (func_ov119_021dbd70(wk, wk->unkF98, wk->unkF94)) {
        GFL_SndSEPlay(SEQ_SE_MSCL_05);
        if (!func_ov119_021d8aa8(wk)) {
            func_ov119_021d8e84(wk, func_ov119_021db9b4);
            return TRUE;
        }
        func_ov119_021d8e84(wk, func_ov119_021df708);
        return TRUE;
    }
    if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
        GFL_SndSEPlay(0x54b);
        func_ov067_021b6558(wk->touchBar, 8, FALSE);
        func_ov119_021e12f8(wk);
        func_ov119_021d8e84(wk, func_ov119_021da640);
        return TRUE;
    }
    func_ov119_021d8c84(wk);
    wk->unkF98 = -1;
    wk->unkF94 = -1;
    return FALSE;
}

void func_ov119_021dbe70(PokemonTradeWork *wk) {
    int column;
    int box;
    int slot;
    int target;
    int n;
    u32 keys;
    BoxPkm *pkm;

    wk->unk11EA = 1;
    if (func_02035c38()) {
        func_020355d8(TRUE);
        if (GCTX_HIDGetHeldKeys() != 0) {
            return;
        }
    }
    if (func_020355c4()) {
        keys = PokemonTrade_GetPressedKeys();
        if (keys != 0) {
            func_020355d8(FALSE);
            if (func_ov119_021e4e0c(wk, &column) == 0) {
                wk->unkF74 = column;
                if (wk->unkF74 < 2 && wk->cursorRow >= 3) {
                    wk->cursorRow = 2;
                }
            } else if (func_ov119_021db4ec(wk, wk->unkF74) == 0) {
                int top;
                int col;
                n = func_ov119_021e4dbc(wk);
                col = wk->unkF74;
                top = n + 5;
                target = col;
                if (n + 10 >= wk->unk634 && col < 20) {
                    target = col + (wk->unk634 + 1);
                }
                if (top < target) {
                    wk->unk107C = func_ov119_021db4b4(col + 1) - 256;
                } else {
                    wk->unk107C = func_ov119_021db4b4(col);
                }
                wk->firstColumn = -1;
                wk->unk1084 = 1;
                func_ov119_021db7ac(wk, 0, TRUE);
                wk->unk107A = 0;
                return;
            }
            func_ov119_021d8b38(wk);
            wk->unk107A = 0;
            return;
        }
    }
    if (PokemonTrade_GetPressedKeys() != 0 || PokemonTrade_GetTouch()) {
        wk->unk107A = 0;
    }
    if (PokemonTrade_GetTouch()) {
        func_ov119_021d8c84(wk);
        wk->unkF98 = -1;
        wk->unkF94 = -1;
    }
    func_ov119_021d8b54(wk);
    func_ov119_021d8c0c(wk);
    func_ov119_021db52c(wk);
    func_ov119_021df7f0(wk);
    keys = PokemonTrade_GetPressedKeys();
    if (keys == PAD_BUTTON_A) {
        func_020355d8(FALSE);
        if (func_ov119_021e4e0c(wk, &column) == 0) {
            wk->unkF74 = column;
            if (wk->unkF74 < 2) {
                if (wk->cursorRow >= 3) {
                    wk->cursorRow = 0;
                }
            } else if (wk->cursorRow >= 5) {
                wk->cursorRow = 0;
            }
        }
        box = func_ov119_021d8af4(wk->unkF74, wk);
        slot = func_ov119_021d8b0c(wk->unkF74, wk->cursorRow);
        pkm = func_ov119_021d8dd0(wk->boxes, box, slot, wk);
        if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
            if (func_ov119_021dd0cc(wk, pkm)) {
                GFL_MsgDataLoadStrbuf(wk->msgData, 0x8e, wk->strbuf);
                func_ov119_021e0ff4(wk, TRUE, FALSE);
                func_ov119_021d8e84(wk, func_ov119_021da064);
                return;
            }
            wk->unkF98 = func_ov119_021d8af4(wk->unkF74, wk);
            wk->unkF94 = func_ov119_021d8b0c(wk->unkF74, wk->cursorRow);
            func_020355d8(FALSE);
            if (func_ov119_021d8aa8(wk)) {
                if (func_ov119_021dd34c(wk, wk->unkF98, wk->unkF94) != -1) {
                    GFL_SndSEPlay(SEQ_SE_MSCL_05);
                    func_ov119_021d8e84(wk, func_ov119_021df708);
                    return;
                }
            }
            func_ov119_021d8dd0(wk->boxes, wk->unkF98, wk->unkF94, wk);
            func_ov119_021d8c84(wk);
            func_ov119_021e4a1c(wk);
            func_ov119_021dbdb4(wk);
            return;
        }
    }
    box = func_ov119_021d9f24(wk, 0);
    switch (box) {
    case 2:
        return;
    case 3:
        return;
    case 1:
        func_020355d8(TRUE);
        wk->unkF70 = 1;
        break;
    }
    if (func_02035c38() == 0) {
        if (wk->unk1088 && wk->unkF5C && wk->unkF60 != NULL) {
            wk->unk1088 = 0;
            wk->unkF5C = 0;
            if (func_ov119_021d9548(wk, wk->heldBox, wk->heldSlot)) {
                wk->heldBox = -1;
                wk->heldSlot = -1;
                func_ov119_021d8c6c(wk);
                func_ov119_021d8e84(wk, func_ov119_021da0cc);
                return;
            }
            wk->unkF98 = wk->heldBox;
            wk->unkF94 = wk->heldSlot;
            wk->selectSlot = wk->heldSlot;
            wk->selectBox = wk->heldBox;
            wk->unkF60 = NULL;
            func_ov067_021b6558(wk->touchBar, 8, TRUE);
            func_ov067_021b6548(wk->touchBar, 8, TRUE);
            func_ov119_021e6544(wk);
            GFL_SndSEPlay(SEQ_SE_SYS_56);
            func_ov119_021d8e84(wk, func_ov119_021dbd3c);
            return;
        } else if (wk->unk1088 && wk->unkF60 != NULL && wk->unkF70) {
            wk->unkF98 = wk->heldBox;
            wk->unkF94 = wk->heldSlot;
            func_ov119_021dbdb4(wk);
            return;
        }
        if (wk->heldSlot != -1 && wk->heldBox != -1) {
            func_ov119_021d8c84(wk);
            func_ov119_021d8b38(wk);
        } else {
            func_ov119_021d8c84(wk);
        }
        wk->unk1088 = 0;
        wk->unkF5C = 0;
        func_0204b92c(wk->actors[2], FALSE);
    }
    func_ov119_021db89c(wk);
    if (func_02035c38()) {
        wk->unk1088 = 1;
    }
    func_ov067_021b63d0(wk->touchBar);
    box = func_ov067_021b6470(wk->touchBar);
    switch (box) {
    case 1:
    case 7:
    case 8:
        func_ov119_021d8e84(wk, func_ov119_021dc2b0);
        break;
    }
}

void func_ov119_021dc2b0(PokemonTradeWork *wk) {
    int r;
    func_ov067_021b63d0(wk->touchBar);
    r = func_ov067_021b6448(wk->touchBar);
    switch (r) {
    case 1:
        func_ov119_021d8e84(wk, func_ov119_021db198);
        break;
    case 7:
        func_ov119_021d8e84(wk, func_ov119_021db45c);
        break;
    case 8:
        if (func_ov119_021d8aa8(wk)) {
            if (func_ov119_021dd2f4(wk)) {
                func_ov067_021b6558(wk->touchBar, 7, FALSE);
                func_ov067_021b6558(wk->touchBar, 8, FALSE);
                func_ov119_021d8c6c(wk);
                func_ov119_021d8e84(wk, func_ov119_021df5d8);
                return;
            }
            func_ov067_021b6558(wk->touchBar, 7, TRUE);
            func_ov067_021b6558(wk->touchBar, 8, FALSE);
        } else if (wk->selectSlot == -1) {
            func_ov067_021b6558(wk->touchBar, 7, TRUE);
            func_ov067_021b6558(wk->touchBar, 8, FALSE);
        } else {
            func_ov119_021d8e84(wk, func_ov119_021d9e54);
            return;
        }
        func_ov119_021d8e84(wk, func_ov119_021dbe70);
        break;
    }
}

void func_ov119_021dc370(PokemonTradeWork *wk) {
    u32 keys;
    BOOL touched;
    BOOL done = FALSE;
    if (!func_ov119_021e12ac(wk)) {
        return;
    }
    keys = GCTX_HIDGetPressedKeys();
    if (keys) {
        func_020355d8(FALSE);
    }
    if (keys) {
        func_020355d8(FALSE);
        done = TRUE;
    }
    touched = func_02035c60();
    if (touched) {
        func_020355d8(TRUE);
    }
    if (touched) {
        func_020355d8(TRUE);
        done = TRUE;
    }
    if (done) {
        func_ov119_021e1098(wk);
        func_ov119_021e6700(wk);
        func_ov119_021d8e84(wk, func_ov119_021dbe70);
        wk->firstColumn = -1;
        func_ov119_021e4e64(wk->boxes, wk, TRUE);
    }
}

void func_ov119_021dc3ec(PokemonTradeWork *wk) {
    func_ov119_021e55bc(wk);
    wk->mcssSys = func_02014b08(4, (u16)((wk->heapId & 0x7fff) | 0x8000));
    func_02016020(wk->mcssSys, 0x30000);
    func_02015bf0(wk->mcssSys);
}

void func_ov119_021dc428(PokemonTradeWork *wk) {
    func_ov119_021e36d8(wk, 0);
    func_ov119_021e36d8(wk, 1);
    func_02014ba8(wk->mcssSys);
    wk->mcssSys = NULL;
    GFL_G3DCameraFree(wk->unk810);
    GFL_G3DSysFree();
}

void func_ov119_021dc458(PokemonTradeWork *wk) {
    GFL_BGSysCreate(0x91);
    BmpWin_InitAllocator(wk->heapId);
    func_ov119_021e53f8(wk);
    func_ov119_021e5430(wk);
    func_ov119_021e5680(wk);
}








void func_ov119_021dc484(PokemonTradeWork *wk) {
    func_0204a5ac();
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
}

void func_ov119_021dc494(PokemonTradeWork *wk) {
    func_ov119_021e4068(wk);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x1c0, 0x20, wk->heapId);
    func_ov119_021df904(wk);
    if (wk->type < 4) {
        func_ov119_021e58a0(wk);
    }
    func_ov119_021e467c(wk);
    func_ov119_021e3f74(wk);
    func_ov119_021e3e80(wk);
    func_ov119_021e4420(wk);
    func_ov119_021e4e64(wk->boxes, wk, FALSE);
}

void func_ov119_021dc500(TCB *tcb, void *work) {
    PokemonTradeWork *wk = work;

    func_0204a648();
    if (wk->unk1084 != 0) {
        GFL_BGSysMoveBG(7, 0, wk->unk108C);
        GFL_BGSysMoveBG(5, 0, wk->unk108C);
        wk->unk1084 = 0;
    }
}

void func_ov119_021dc538(PokemonTradeWork *wk, GameData *gameData, u16 friendIndex) {
    u32 i;

    for (i = 0; i < 3; i++) {
        wk->unk1064[i] = GFL_HeapAllocate(HEAPID_POKEMON_TRADE, PokeParty_GetPkmRawSize(), TRUE, "pokemontrade_proc.c", 0xfde);
    }
    if (gameData != NULL) {
        wk->gameData = gameData;
        wk->boxes = GameData_GetBoxSaveAccessor(gameData);
        wk->myInfo = GetGameDataPlayerInfo(gameData);
        wk->party = GameData_GetParty(gameData);
        if (wk->partnerInfo == NULL) {
            if (func_ov119_021d8a24(wk)) {
                wk->partnerInfo = func_02012950(gameData, 1 - func_02039338());
            } else {
                wk->partnerInfo = func_02012950(gameData, 1);
            }
        }
    }
}


void func_ov119_021dc5d0(PokemonTradeWork *wk, BOOL flag) {
    u32 i;
    for (i = 0; i < 3; i++) {
        GFL_HeapFree(wk->unk1064[i]);
    }
}

void func_ov119_021dc5f0(PokemonTradeWork *wk) {
    int count = BoxSaveAccessor_GetAvailableBoxCount(wk->boxes);
    wk->unk634 = count * 6 + 2;
    wk->unk630 = count;
    wk->unk638 = count * 20 + 12;
    wk->unk63C = count * 160 + 96;
}

void func_ov119_021dc62c(PokemonTradeWork *wk, int box) {
    TradeBoxEntry *entries = wk->boxEntries;
    long i;

    if (box == 0) {
        MI_CpuFill8(entries, 0, sizeof(wk->boxEntries));
        for (i = 0; i < 6; i++) {
            BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, wk->unk630, i, wk);
            if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
                u32 species = PML_PkmGetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
                if (species != 0) {
                    entries[i].species = species;
                    entries[i].form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
                    entries[i].sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
                }
            }
        }
    }
    while (box < 24) {
        int base = box * 30 + 6;
        for (i = 0; i < 30; i++) {
            int idx = i + base;
            BoxPkm *pkm = func_ov119_021d8dd0(wk->boxes, box, i, wk);
            if (pkm != NULL && PML_PkmGetParam(pkm, PKM_PARAM_SPECIES_VALID, NULL)) {
                u32 species = PML_PkmGetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
                if (species != 0) {
                    entries[idx].species = species;
                    entries[idx].form = PML_PkmGetParam(pkm, PKM_PARAM_FORM, NULL);
                    entries[idx].sex = PML_PkmGetParam(pkm, PKM_PARAM_SEX, NULL);
                }
            }
        }
        return;
    }
}


void func_ov119_021dc720(PokemonTradeWork *wk) {
    int i;

    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    func_ov119_021e64b8(wk);
    func_ov119_021e3230(wk);
    func_ov119_021dd588(wk);
    func_ov119_021df8e0(wk);
    func_ov119_021dd9dc(wk);
    func_ov119_021e4570(wk);
    func_ov119_021e36d8(wk, 1);
    func_ov119_021e69a4(wk);
    func_ov119_021df7d0(wk);
    func_ov119_021e2680(wk);
    func_ov119_021e4888(wk);
    func_ov119_021e2438(wk, 0);
    func_ov119_021e2438(wk, 1);
    if (wk->touchBar != NULL) {
        func_ov067_021b639c(wk->touchBar);
        wk->touchBar = NULL;
    }
    func_ov119_021e48e0(wk);
    if (wk->clactUnit != NULL) {
        func_0204b1cc(wk->clactUnit);
        wk->clactUnit = NULL;
    }
    func_ov119_021e11a8(wk);
    func_ov119_021dd174(wk);
    func_ov119_021e293c(wk, 0);
    func_ov119_021e293c(wk, 1);
    func_ov119_021e3f08(wk);
    func_ov119_021e4000(wk);
    func_ov119_021e57a4(wk);
    if (wk->fade != NULL) {
        GFL_HeapFree(wk->fade);
        wk->fade = NULL;
    }
    for (i = 0; i < 4; i++) {
        if (wk->mcssMoves[i] != NULL) {
            GFL_HeapFree(wk->mcssMoves[i]);
            wk->mcssMoves[i] = NULL;
        }
    }
}


void func_ov119_021dc818(PokemonTradeWork *wk) {
    switch (wk->unk11F8) {
    case 0:
        break;
    case 2:
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
    case 1:
        GFL_SndBGMPop();
        GFL_SndBGMSetPaused(FALSE);
        GFL_SndBGMFadeIn(30);
        break;
    }
    wk->unk11F8 = 0;
}

PokemonTradeWork *func_ov119_021dc854(GameProc *proc, u32 heapSize) {
    PokemonTradeWork *wk;

    GFL_HeapCreateRoot(data_ov119_021e8560, sizeof(data_ov119_021e8560), HEAPID_POKEMON_TRADE);
    GFL_HeapCreateChild(HEAPID_USER, HEAPID_IRC_BATTLE_MENU, heapSize);
    wk = GFL_ProcInitSubsystem(proc, sizeof(PokemonTradeWork), HEAPID_POKEMON_TRADE);
    MI_CpuFill8(wk, 0, sizeof(PokemonTradeWork));
    return wk;
}

BOOL func_ov119_021dc894(GameProc *proc, u32 *state, PokemonTradeParam *trade, PokemonTradeWork *wk, int type) {
    if (type < 5) {
        GFL_OvlLoad(OVERLAY_ID(67));
        GFL_OvlLoad(OVERLAY_ID(114));
    }
    if (type == 0 || type == 4) {
        GFL_OvlLoad(OVERLAY_ID(117));
    } else {
        GFL_OvlLoad(OVERLAY_ID(118));
    }
    GFL_BGSysSetDisplayLayout(1);
    wk->heapId = HEAPID_IRC_BATTLE_MENU;
    wk->type = type;
    wk->param = trade;
    wk->unk5DC[0] = GFL_HeapAllocate(HEAPID_POKEMON_TRADE, PokeParty_GetPkmRawSize(), TRUE, "pokemontrade_proc.c", 4363);
    wk->unk5DC[1] = GFL_HeapAllocate(HEAPID_POKEMON_TRADE, PokeParty_GetPkmRawSize(), TRUE, "pokemontrade_proc.c", 4364);
    wk->savedObjPalette = GFL_HeapAllocate(HEAPID_POKEMON_TRADE, 0x200, FALSE, "pokemontrade_proc.c", 4365);
    wk->savedBGPalette = GFL_HeapAllocate(HEAPID_POKEMON_TRADE, 0x200, FALSE, "pokemontrade_proc.c", 4366);
    wk->nationalDex = PokeDex_IsNationalObtained(GameData_GetPokedex(trade->gameData));
    if (trade != NULL) {
        func_ov119_021dc538(wk, trade->gameData, trade->unk2E);
    }
    func_ov119_021dc5f0(wk);
    func_ov119_021dc3ec(wk);
    func_ov119_021dc458(wk);
    wk->clactUnit = func_0204b100(340, 0, wk->heapId);
    if (func_ov119_021d8a24(wk)) {
        func_0203a574(TRADE_NET_CMD_BASE, data_ov119_021e7614, NELEMS(data_ov119_021e7614), wk);
        func_0203d7b0(TRUE);
        func_0203d7bc(TRUE);
    }
    wk->unk9C8 = GFL_VBlankTCBAdd(func_ov119_021dc500, wk, 0);
    wk->unkF60 = NULL;
    wk->selectSlot = -1;
    wk->sceneId = -1;
    wk->heldSlot = -1;
    wk->heldBox = -1;
    wk->unkF98 = -1;
    wk->unkF94 = -1;
    func_ov119_021e30ec(wk);
    if (trade->next == 1) {
        GFL_BGSysSetEnabledBGsA(0x10);
        func_ov119_021d8e84(wk, func_ov119_021e0b34);
        trade->next = 0;
        return TRUE;
    }
    func_ov119_021e10d8(wk);
    func_ov119_021e3fe4(wk);
    if (wk->type < 4) {
        func_ov119_021dc494(wk);
        func_ov119_021e3c20(wk);
        func_0203d34c(TRUE, wk->heapId);
    }
    func_ov119_021e3208(wk, 0);
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    func_ov119_021d8e84(wk, func_ov119_021dac60);
    return TRUE;
}








// pokebw2's PokemonTrade_ProcInitWifiClub
static BOOL func_ov119_021dca84(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0xc6000);
    return func_ov119_021dc894(proc, state, param, wk, 2);
}

// pokebw2's PokemonTrade_ProcInitIrc
static BOOL func_ov119_021dcaac(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0xc6000);
    return func_ov119_021dc894(proc, state, param, wk, 0);
}

int func_ov119_021dcad4(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeParam *trade = param;
    PokemonTradeWork *wk;
    int i;

    wk = func_ov119_021dc854(proc, 0xc6000);
    for (i = 0; i < 2; i++) {
        wk->unk1048[i] = 0;
        wk->unk1050[i] = 1;
    }
    wk->partnerInfo = func_02012950(trade->gameData, 1 - func_02039338());
    return func_ov119_021dc894(proc, state, trade, wk, TRUE);
}

int func_ov119_021dcb30(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeParam *trade = param;
    GtsNegoParam *nego = trade->nego;
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0xc6000);
    int i;

    if (nego != NULL) {
        if (nego->result == 2) {
            for (i = 0; i < 2; i++) {
                wk->unk1048[i] = nego->unk4[i].unk4;
                wk->unk1050[i] = nego->unk4[i].unk0;
            }
        } else {
            for (i = 0; i < 2; i++) {
                wk->unk1048[i] = -1;
                wk->unk1050[i] = -1;
            }
        }
    }
    return func_ov119_021dc894(proc, state, trade, wk, 3);
}

void func_ov119_021dcba4(PokemonTradeDemoParam *demo, PokemonTradeWork *wk) {
    MI_CpuCopy8(demo->pkm[0], wk->unk5DC[0], PokeParty_GetPkmRawSize());
    MI_CpuCopy8(demo->pkm[1], wk->unk5DC[1], PokeParty_GetPkmRawSize());
    func_ov119_021e36c8(wk, 0, TRUE, demo->pkm[0], TRUE);
    func_ov119_021e36c8(wk, 1, TRUE, demo->pkm[1], TRUE);
    if (!func_ov119_021d8aa8(wk)) {
        func_02015ea0(wk->mcss[1]);
    } else {
        func_02015ea0(wk->mcss[0]);
    }
    wk->myInfo = demo->myInfo;
    wk->partnerInfo = demo->partnerInfo;
    func_0203d34c(FALSE, wk->heapId);
    GFL_FadeSet(3, 16, 0, 2);
}




static BOOL func_ov119_021dcc38(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeDemoParam *demo = param;
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0xc6000);
    BOOL ret;
    demo->trade.gameData = demo->gameData;
    ret = func_ov119_021dc894(proc, state, &demo->trade, wk, 4);
    func_ov119_021dcba4(demo, wk);
    func_ov119_021d8e84(wk, func_ov117_021d4b88);
    return ret;
}

static BOOL func_ov119_021dcc78(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeDemoParam *demo = param;
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0x96000);
    BOOL ret;
    demo->trade.gameData = demo->gameData;
    ret = func_ov119_021dc894(proc, state, &demo->trade, wk, 7);
    func_ov119_021dcba4(demo, wk);
    func_ov119_021d8e84(wk, func_ov118_021d59b0);
    return ret;
}

static BOOL func_ov119_021dccb8(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeDemoParam *demo = param;
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0x96000);
    BOOL ret;
    demo->trade.gameData = demo->gameData;
    ret = func_ov119_021dc894(proc, state, &demo->trade, wk, 8);
    func_ov119_021dcba4(demo, wk);
    func_ov119_021d8e84(wk, func_ov118_021d6f2c);
    return ret;
}

static BOOL func_ov119_021dccf8(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeDemoParam *demo = param;
    PokemonTradeWork *wk = func_ov119_021dc854(proc, 0x96000);
    BOOL ret;
    demo->trade.gameData = demo->gameData;
    ret = func_ov119_021dc894(proc, state, &demo->trade, wk, 6);
    func_ov119_021dcba4(demo, wk);
    func_ov119_021d8e84(wk, func_ov118_021d4ab4);
    return ret;
}

BOOL func_ov119_021dcd38(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeWork *wk = work;
    u8 letter, shadow, background;
    BOOL done = TRUE;
    PokemonTradeState step = wk->state;
    int i;

    if (func_020275dc()) {
        return FALSE;
    }
    if (step != NULL) {
        step(wk);
        wk->timer++;
        done = FALSE;
        for (i = 0; i < 2; i++) {
            func_ov119_021dd90c(wk, i, wk->unk11E8[i], FALSE);
        }
    }
    if (wk->unkF9C != 0 && wk->unk11F3 == 0) {
        func_ov119_021d9468(wk, 1, wk->unk5DC[wk->unkF9C - 1]);
        wk->unkF9C = 0;
    }
    func_ov119_021d8a50(wk);
    func_ov119_021df868(wk);
    func_0204a600();
    if (wk->menu != NULL) {
        func_02027974(wk->menu);
    }
    if (wk->menuWin != NULL) {
        func_02028178(wk->menuWin);
    }
    if (wk->unk11FB_4 != 0) {
        if (func_0203d3a8(func_020399a8(), TRADE_NET_CMD_UNK17, 0, NULL)) {
            wk->unk11FB_4 = 0;
        }
    }
    if (wk->tcbEx != NULL) {
        GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
        GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
        GFL_TCBExMgrUpdate(wk->tcbEx);
        GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
        func_0201c4e4(wk->printQueue);
    }
    func_ov119_021df978(wk);
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(wk->unk810);
    GFL_G3DSysMtxViewFlush();
    func_02014c08(wk->mcssSys);
    func_02014d2c(wk->mcssSys);
    func_ov119_021e31b4(wk);
    GFL_G3DSysReqSwapBuffers();
    if (func_ov119_021d8a24(wk) && wk->unkEF8 == NULL && func_0200fb24()) {
        if (wk->unk11F7) {
            func_0200f7c0(0x29);
        }
        if (wk->unk0 != NULL) {
            func_ov114_021b9774(wk->unk0);
            func_ov114_021b9838(wk->unk0);
            wk->unk0 = NULL;
            func_ov009_02156dc4(NULL, NULL);
            func_ov009_02156ed0();
        }
        if (func_0203d29c()) {
            func_ov009_02157170(1, 1);
        } else if (wk->type != 1) {
            func_0200fb38();
        }
        wk->param->next = 2;
        func_ov119_021dc818(wk);
        done = TRUE;
        func_0202223c(0, 0);
        func_0202223c(1, 0);
    }
    return done;
}


BOOL func_ov119_021dcf24(GameProc *proc, u32 *state, void *param, void *work) {
    PokemonTradeWork *wk = work;
    int type = wk->type;

    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    GFL_HeapFree(wk->savedObjPalette);
    GFL_HeapFree(wk->savedBGPalette);
    func_ov119_021e5ef8(&wk->infoIcons[0]);
    func_ov119_021e5ef8(&wk->infoIcons[1]);
    func_ov119_021e5ef8(&wk->infoIcons[2]);
    func_ov119_021e5d84(wk);
    func_ov119_021dc720(wk);
    func_020275e0();
    if (func_ov119_021d8a24(wk)) {
        func_0203a5e4(TRADE_NET_CMD_BASE);
    }
    func_ov119_021e31bc(wk);
    GFL_HeapFree(wk->unk5DC[0]);
    GFL_HeapFree(wk->unk5DC[1]);
    GFL_TCBRemove(wk->unk9C8);
    func_ov119_021dc5d0(wk, param != NULL);
    func_ov119_021dc428(wk);
    func_ov119_021dc484(wk);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(HEAPID_IRC_BATTLE_MENU);
    GFL_HeapDelete(HEAPID_POKEMON_TRADE);
    if (type < 5) {
        GFL_OvlUnload(OVERLAY_ID(114));
        GFL_OvlUnload(OVERLAY_ID(67));
    }
    if (type == 0 || type == 4) {
        GFL_OvlUnload(OVERLAY_ID(117));
    } else {
        GFL_OvlUnload(OVERLAY_ID(118));
    }
    return TRUE;
}


void func_ov119_021dd018(PokemonTradeWork *wk, BOOL moved) {
    if (moved) {
        int column = func_ov119_021e4dbc(wk) + 2;
        if (column >= wk->unk634) {
            column -= wk->unk634;
        }
        if (wk->unk107C == data_ov119_021e7fe0[0]) {
            column = 2;
        } else if (wk->unk107C == data_ov119_021e7fe0[wk->unk630]) {
            column = 0;
            if (wk->cursorRow >= 3) {
                wk->cursorRow = 2;
            }
        }
        wk->unkF74 = column;
        wk->firstColumn = -1;
    }
}


// The trade's procs, in pokebw2's order of its eight (its POKEMONTRADE_PROC_FUNCTIONS third, its WIFICLUB one fifth):
// MWCC sorts them with the file's other 12-byte objects, and this order gives Black's
const GameProcFunctions data_ov119_021e75cc = {
    func_ov119_021dccb8,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

const GameProcFunctions data_ov119_021e75fc = {
    func_ov119_021dccf8,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

// pokebw2's POKEMONTRADE_PROC_FUNCTIONS
const GameProcFunctions data_ov119_021e759c = {
    func_ov119_021dcb30,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

const GameProcFunctions data_ov119_021e75a8 = {
    func_ov119_021dcc38,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

// pokebw2's POKEMONTRADE_WIFICLUB_PROC_FUNCTIONS
const GameProcFunctions data_ov119_021e75c0 = {
    func_ov119_021dca84,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

const GameProcFunctions data_ov119_021e75d8 = {
    func_ov119_021dcaac,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

const GameProcFunctions data_ov119_021e75b4 = {
    func_ov119_021dcc78,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

const GameProcFunctions data_ov119_021e7608 = {
    func_ov119_021dcad4,
    func_ov119_021dcd38,
    func_ov119_021dcf24,
};

// The memory of the trade's root heap
u8 data_ov119_021e8560[WORK_AREA_SIZE(0xc000)];
