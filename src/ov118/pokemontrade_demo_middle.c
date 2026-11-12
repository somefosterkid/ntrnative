// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov193/pokemontrade_demo_middle.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemon_trade_local.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "system/mcss.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/ability_handlers_3.h"

// pokemontrade_demo_middle.c, a guessed name: one of the trade demo's three scenes, a copy of the template that
// pokemontrade_demo_common.c and pokemontrade_demo_up.c share

static const VecFx32 sPosFirst = { 0, FX32_CONST(-23), FX32_CONST(10) };
static const VecFx32 sPosSecond = { 0, FX32_CONST(-23), FX32_CONST(-10) };

void func_ov118_021d6e6c(SPLEmitter *emitter, u32 type);
void func_ov118_021d6ecc(SPLEmitter *emitter, u32 type);
void func_ov118_021d6744(PokemonTradeWork *wk);
void func_ov118_021d6738(PokemonTradeWork *wk, int timer);
void func_ov118_021d6750(PokemonTradeWork *wk);
void func_ov119_021dc720(PokemonTradeWork *wk);
void func_ov119_021e35a0(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL flag);
void func_ov119_021e36c8(PokemonTradeWork *wk, int side, BOOL front, PartyPkm *pkm, BOOL flag);
void func_ov119_021e2fb0(PokemonTrade3DWork *work);
BOOL func_ov119_021d8aa8(PokemonTradeWork *wk);
void func_ov118_021d7100(PokemonTradeWork *wk);
void func_ov118_021d7118(PokemonTradeWork *wk);
void func_ov118_021d6f90(PokemonTradeWork *wk);
int func_ov118_021d6734(int frame);
void func_ov118_021d6600(PokemonTradeWork *wk);
void func_ov118_021d667c(PokemonTradeWork *wk);
void func_ov118_021d6c78(PokemonTradeWork *wk);
void func_ov118_021d6ca8(PokemonTradeWork *wk, int index);
void func_ov118_021d6cc4(PokemonTradeWork *wk, int index);
void func_ov118_021d6e04(PokemonTradeWork *wk, int index);
void func_ov118_021d6eac(SPLEmitter *emitter);
void func_ov118_021d6f0c(SPLEmitter *emitter);
void func_ov118_021d7130(PokemonTradeWork *wk);

// The hops of a Pokémon's sprite, as offsets from its place: one hop of 18 frames, a second copy of it, or two of 40.
// The common file has the same
static VecFx32 sTradeDemoMiddleHopPath[18] = {
#include "asset/pokemontrade_demo_middle_sTradeDemoMiddleHopPath.inc"
};

static VecFx32 sTradeDemoMiddleMoveHopPath[18] = {
#include "asset/pokemontrade_demo_middle_sTradeDemoMiddleMoveHopPath.inc"
};

static VecFx32 sTradeDemoMiddleLongHopPath[40] = {
#include "asset/pokemontrade_demo_middle_sTradeDemoMiddleLongHopPath.inc"
};

// The path that the sprites follow when they don't hop, which nothing fills
static VecFx32 sTradeDemoMiddleScenePath[20];

// pokebw2's PokemonTradeDemoMiddle_Wait2

void func_ov118_021d6e04(PokemonTradeWork *wk, int index) {
    VecFx32 pos;

    func_02015eb0(wk->mcss[index]);
    pos.x = 0;
    pos.y = FX32_CONST(-23);
    pos.z = FX32_CONST(10);
    func_02015c48(wk->mcss[index], &pos);
    func_02015e04(wk->mcss[index]);
    func_02015c94(wk->mcss[index], &pos);
    if (pos.x < 0) {
        pos.x = -FX32_ONE * 16;
    } else {
        pos.x = FX32_ONE * 16;
    }
    pos.y = FX32_ONE * 16;
    pos.z = FX32_ONE;
    func_02015cac(wk->mcss[index], &pos);
}

void func_ov118_021d6e6c(SPLEmitter *emitter, u32 type) {
    PokemonTradeWork *wk;

    if (type != 0 && emitter != NULL) {
        wk = func_02051a34(emitter);
        if (wk->work3D->emitterPaths[0] != NULL) {
            VecFx32 pos = { 0, 0, 0 };

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[0], &pos);
            func_02051838(emitter, &pos);
        }
    }
}


void func_ov118_021d6eac(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d6e6c);
}

void func_ov118_021d6ecc(SPLEmitter *emitter, u32 type) {
    PokemonTradeWork *wk;

    if (type != 0 && emitter != NULL) {
        wk = func_02051a34(emitter);
        if (wk->work3D->emitterPaths[1] != NULL) {
            VecFx32 pos = { 0, 0, 0 };

            GFL_G3DCurveGetNowTranslation(wk->work3D->emitterPaths[1], &pos);
            func_02051838(emitter, &pos);
        }
    }
}


void func_ov118_021d6f0c(SPLEmitter *emitter) {
    func_02051a2c(emitter, func_02051774());
    func_02051a24(emitter, func_ov118_021d6ecc);
}

void func_ov118_021d6f2c(PokemonTradeWork *wk) {
    MCSS *tmp;

    func_ov118_021d6744(wk);
    wk->unk854 = TRUE;
    func_02016028(wk->mcss[0], 0);
    wk->unk858 = TRUE;
    func_02016028(wk->mcss[1], 0);
    if (func_ov119_021d8aa8(wk)) {
        tmp = wk->mcss[0];
        wk->mcss[0] = wk->mcss[1];
        wk->mcss[1] = tmp;
    }
    func_ov118_021d6738(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d6750);
}

void func_ov118_021d6f90(PokemonTradeWork *wk) {
    func_ov119_021dc720(wk);
    func_0204a5ac();
    func_ov119_021e3e40(wk);
    wk->clactUnit = func_0204b100(3, 0, wk->heapId);
    wk->work3D = GFL_HeapAllocate(wk->heapId, sizeof(PokemonTrade3DWork), TRUE, "pokemontrade_demo_middle.c", 157);
    wk->work3D->heapId = wk->heapId;
    func_ov119_021e5284(wk, FALSE);
    reg_G2S_DB_BLDCNT = 0;
    reg_G2_BLDCNT = 0;
    if (func_ov119_021d8aa8(wk)) {
        func_ov119_021e35a0(wk, 1, FALSE, func_ov119_021d8fa4(wk, 1), FALSE);
        func_ov119_021e35a0(wk, 2, FALSE, func_ov119_021d8fa4(wk, 0), FALSE);
        func_ov119_021e35a0(wk, 3, TRUE, func_ov119_021d8fa4(wk, 1), FALSE);
    } else {
        func_ov119_021e36c8(wk, 1, FALSE, func_ov119_021d8fa4(wk, 1), FALSE);
        func_ov119_021e36c8(wk, 2, FALSE, func_ov119_021d8fa4(wk, 0), FALSE);
        func_ov119_021e36c8(wk, 3, TRUE, func_ov119_021d8fa4(wk, 1), FALSE);
    }
    func_02015c94(wk->mcss[0], &wk->work3D->spriteSizes[0]);
    func_02015c94(wk->mcss[1], &wk->work3D->spriteSizes[1]);
    func_02015c94(wk->mcss[2], &wk->work3D->spriteSizes[2]);
    func_02015c94(wk->mcss[3], &wk->work3D->spriteSizes[3]);
    func_02015ea0(wk->mcss[1]);
    func_02015ea0(wk->mcss[2]);
    func_02015ea0(wk->mcss[3]);
    func_ov119_021e2f34(wk->work3D);
    func_ov119_021e2fb0(wk->work3D);
    func_ov119_021e2d70(wk->work3D, 0);
    func_ov118_021d6738(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d7100);
}


void func_ov118_021d7100(PokemonTradeWork *wk) {
    func_ov119_021e3004(wk->work3D);
    func_ov119_021d8e84(wk, func_ov118_021d7118);
}


void func_ov118_021d7118(PokemonTradeWork *wk) {
    func_ov119_021e3208(wk, 3);
    func_ov119_021d8e84(wk, func_ov118_021d7130);
}

void func_ov118_021d7130(PokemonTradeWork *wk) {
    VecFx32 pos1;
    VecFx32 pos2;
    VecFx32 pos3;
    VecFx32 pos4;
    VecFx32 pos5;
    VecFx32 pos6;
    VecFx32 pos7;
    int index1;
    int index2;
    int index3;
    int index4;

    func_ov119_021dfa60(wk->timer);
    if (wk->timer == func_ov118_021d6734(0x12d)) {
        func_ov119_021e3230(wk);
        func_ov119_021e3208(wk, 4);
    }
    if (wk->timer == func_ov118_021d6734(0x2b6)) {
        func_ov119_021e3230(wk);
        func_ov119_021e3208(wk, 5);
    }
    if (wk->timer == func_ov118_021d6734(1)) {
        func_02015f50(wk->mcss[0], 8, 8, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d6734(2)) {
        func_02015f50(wk->mcss[0], 16, 16, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d6734(3)) {
        func_ov118_021d667c(wk);
        func_ov118_021d6600(wk);
    }
    if (wk->timer == func_ov118_021d6734(4)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0x18);
    }
    if (wk->timer == func_ov118_021d6734(4)) {
        func_02015ea0(wk->mcss[0]);
    }
    if (wk->timer == func_ov118_021d6734(854)) {
        func_020515cc(wk->work3D->systems[0], 10, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(101)) {
        func_020515cc(wk->work3D->systems[1], 1, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(154)) {
        func_020515cc(wk->work3D->systems[2], 2, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(854)) {
        func_020515cc(wk->work3D->systems[8], 2, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(14)) {
        func_020515cc(wk->work3D->systems[3], 3, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(1155)) {
        func_020515cc(wk->work3D->systems[7], 3, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(154)) {
        func_020515cc(wk->work3D->systems[4], 4, func_ov118_021d6eac, wk);
    }
    if (wk->timer == func_ov118_021d6734(338)) {
        func_020515cc(wk->work3D->systems[5], 5, NULL, wk);
    }
    if (wk->timer == func_ov118_021d6734(700)) {
        func_020515cc(wk->work3D->systems[6], 6, func_ov118_021d6f0c, wk);
    }
    if (wk->timer == func_ov118_021d6734(1151)) {
        func_ov118_021d6600(wk);
    }
    if (wk->timer == func_ov118_021d6734(266)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d6734(20), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(304)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d6734(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(329)) {
        func_ov119_021e36d8(wk, 0);
        if (func_ov119_021d8aa8(wk)) {
            func_ov119_021e35a0(wk, 0, TRUE, func_ov119_021d8fa4(wk, 0), FALSE);
        } else {
            func_ov119_021e36c8(wk, 0, TRUE, func_ov119_021d8fa4(wk, 0), FALSE);
        }
        func_02015c94(wk->mcss[0], &wk->work3D->spriteSizes[0]);
        func_02015ea0(wk->mcss[0]);
    }
    if (wk->timer == func_ov118_021d6734(330)) {
        index1 = 2;
        if (func_ov119_021d8aa8(wk)) {
            index1 = 1;
        }
        func_02015eb0(wk->mcss[index1]);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-150);
        pos1.z = FX32_CONST(10);
        func_02015c48(wk->mcss[index1], &pos1);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-55);
        pos1.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[index1], func_ov118_021d6734(15), &pos1, wk->heapId);
        func_02015e04(wk->mcss[index1]);
        pos1.x = wk->work3D->spriteSizes[index1].x * 1.2;
        pos1.y = wk->work3D->spriteSizes[index1].y * 1.2;
        pos1.z = wk->work3D->spriteSizes[index1].z;
        func_02015cac(wk->mcss[index1], &pos1);
    }
    if (wk->timer == func_ov118_021d6734(331)) {
        index2 = 3;
        if (func_ov119_021d8aa8(wk)) {
            index2 = 0;
        }
        func_02015eb0(wk->mcss[index2]);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(80);
        pos2.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[index2], &pos2);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(5);
        pos2.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[index2], func_ov118_021d6734(15), &pos2, wk->heapId);
        func_02015e04(wk->mcss[index2]);
        pos2.x = wk->work3D->spriteSizes[index2].x * 1.2;
        pos2.y = wk->work3D->spriteSizes[index2].y * 1.2;
        pos2.z = wk->work3D->spriteSizes[index2].z;
        func_02015cac(wk->mcss[index2], &pos2);
    }
    if (wk->timer == func_ov118_021d6734(340)) {
        pos3.x = FX32_CONST(-50);
        pos3.y = FX32_CONST(-55);
        pos3.z = FX32_CONST(10);
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d6734(28), &pos3);
        pos3.x = FX32_CONST(60);
        pos3.y = FX32_CONST(5);
        pos3.z = FX32_CONST(-10);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d6734(28), &pos3);
    }
    if (wk->timer == func_ov118_021d6734(368)) {
        int left;
        int right;

        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        left = 2;
        right = 3;
        if (func_ov119_021d8aa8(wk)) {
            right = 0;
        }
        if (func_ov119_021d8aa8(wk)) {
            left = 1;
        }
        pos4.x = FX32_CONST(-150);
        pos4.y = FX32_CONST(-55);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[left], func_ov118_021d6734(46), &pos4, wk->heapId);
        pos4.x = FX32_CONST(150);
        pos4.y = FX32_CONST(5);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[right], func_ov118_021d6734(46), &pos4, wk->heapId);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d6734(434)) {
        index3 = 0;
        if (func_ov119_021d8aa8(wk)) {
            index3 = 3;
        }
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        func_02015eb0(wk->mcss[index3]);
        func_02015f50(wk->mcss[index3], 0, 0, 0, 0x7fff);
        pos5.x = FX32_CONST(-150);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[index3], &pos5);
        pos5.x = FX32_CONST(60);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[index3], func_ov118_021d6734(103), &pos5, wk->heapId);
        pos5.x = wk->work3D->spriteSizes[index3].x * 1.2;
        pos5.y = wk->work3D->spriteSizes[index3].y * 1.2;
        pos5.z = wk->work3D->spriteSizes[index3].z;
        func_02015cac(wk->mcss[index3], &pos5);
    }
    if (wk->timer == func_ov118_021d6734(434)) {
        index4 = 1;
        if (func_ov119_021d8aa8(wk)) {
            index4 = 2;
        }
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        func_02015eb0(wk->mcss[index4]);
        pos6.x = FX32_CONST(150);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_02015c48(wk->mcss[index4], &pos6);
        pos6.x = FX32_CONST(-45);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[index4], func_ov118_021d6734(103), &pos6, wk->heapId);
        pos6.x = wk->work3D->spriteSizes[index4].x * 1.2;
        pos6.y = wk->work3D->spriteSizes[index4].y * 1.2;
        pos6.z = wk->work3D->spriteSizes[index4].z;
        func_02015cac(wk->mcss[index4], &pos6);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d6734(504)) {
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d6734(128), &wk->mcssMoves[0]->end);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d6734(128), &wk->mcssMoves[1]->end);
    }
    if (wk->timer == func_ov118_021d6734(637)) {
        int left = 1;
        int right = 0;

        if (func_ov119_021d8aa8(wk)) {
            right = 3;
        }
        if (func_ov119_021d8aa8(wk)) {
            left = 2;
        }
        func_ov118_021d6ca8(wk, 0);
        func_ov118_021d6ca8(wk, 1);
        pos7.x = FX32_CONST(60);
        pos7.y = FX32_CONST(100);
        pos7.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[right], func_ov118_021d6734(7), &pos7, wk->heapId);
        pos7.x = FX32_CONST(-45);
        pos7.y = FX32_CONST(-160);
        pos7.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[left], func_ov118_021d6734(7), &pos7, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(693)) {
        func_02015ea0(wk->mcss[0]);
        func_02015ea0(wk->mcss[1]);
        func_02015ea0(wk->mcss[2]);
        func_02015ea0(wk->mcss[3]);
    }
    u32 pair[4] = { 2, 3, 0, 1 };
    int index = 3;
    int other;

    if (func_ov119_021d8aa8(wk)) {
        index = 0;
    }
    other = pair[index];
    if (wk->timer == func_ov118_021d6734(1166)) {
        if (wk->unk11EE == 0) {
            func_ov118_021d6e04(wk, index);
            func_ov118_021d6cc4(wk, index);
            func_02015f50(wk->mcss[index], 16, 16, 0, 0x7fff);
        } else {
            func_ov118_021d6e04(wk, index);
            func_02015ea0(wk->mcss[index]);
            func_ov118_021d6e04(wk, other);
            func_ov118_021d6cc4(wk, index);
            func_02015f50(wk->mcss[other], 16, 16, 0, 0x7fff);
        }
    }
    if (wk->timer == func_ov118_021d6734(1167)) {
        func_02015f50(wk->mcss[index], 8, 8, 0, 0x7fff);
    }
    if (wk->timer == func_ov118_021d6734(1168)) {
        func_02015f50(wk->mcss[index], 0, 0, 0, 0x7fff);
    }
    if (func_ov118_021d6734(1166) < wk->timer && wk->unk11EE != 0 && wk->mcssMoves[3] != NULL
        && wk->mcssMoves[3]->frame == 9) {
        VecFx32 pos8;

        func_02015c30(wk->mcss[other], &pos8);
        func_02015c48(wk->mcss[index], &pos8);
        func_02015eb0(wk->mcss[index]);
        func_02015ea0(wk->mcss[other]);
        wk->mcssMoves[3]->mcss = wk->mcss[index];
    }
    if (wk->timer == func_ov118_021d6734(1184)) {
        func_02015e4c(wk->mcss[index]);
    }
    if (wk->timer == func_ov118_021d6734(651)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d6734(10), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(696)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d6734(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(896)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d6734(30), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(970)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d6734(30), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov118_021d6734(1217)) {
        func_ov118_021d6ca8(wk, 0);
        func_ov118_021d6ca8(wk, 1);
        func_ov119_021e3230(wk);
        func_ov119_021d8e84(wk, func_ov118_021d6c78);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    func_ov119_021dfc68(wk->mcssMoves[2], wk);
    func_ov119_021dfc68(wk->mcssMoves[3], wk);
}

