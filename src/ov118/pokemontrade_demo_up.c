// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov193/pokemontrade_demo_up.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemontrade_demo_up.h"
#include "app/pokemontrade_demo_common.h"
#include "app/pokemon_trade_local.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/ioreg.h"
#include "system/mcss.h"
#include "system/wipe.h"

// A guessed name: the scene of the trade demo whose steps the other two files copy

void func_ov118_021d4c08(PokemonTradeWork *wk);
void func_ov118_021d4c20(PokemonTradeWork *wk);
void func_ov118_021d4c40(PokemonTradeWork *wk);

// pokebw2's PokemonTradeDemoUp_SetUpScene


void func_ov118_021d4b14(PokemonTradeWork *wk) {
    func_ov119_021dc720(wk);
    func_0204a5ac();
    func_ov119_021e3e40(wk);
    wk->clactUnit = func_0204b100(3, 0, wk->heapId);
    wk->work3D = GFL_HeapAllocate(wk->heapId, sizeof(PokemonTrade3DWork), TRUE, "pokemontrade_demo_up.c", 152);
    wk->work3D->heapId = wk->heapId;
    func_ov119_021e5284(wk, FALSE);
    *(vu16 *)REG_DB_BLDCNT_ADDR = 0;
    *(vu16 *)REG_BLDCNT_ADDR = 0;
    func_ov119_021e3208(wk, 3);
    func_ov119_021e36c8(wk, 1, FALSE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_ov119_021e36c8(wk, 2, FALSE, func_ov119_021d8fa4(wk, 0), TRUE);
    func_ov119_021e36c8(wk, 3, TRUE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_02015ea0(wk->mcss[1]);
    func_02015ea0(wk->mcss[2]);
    func_02015ea0(wk->mcss[3]);
    func_ov119_021e2f34(wk->work3D);
    func_ov119_021e2fb0(wk->work3D);
    func_ov119_021e2d70(wk->work3D, TRUE);
    func_ov118_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov118_021d4c08);
}

void func_ov118_021d4c08(PokemonTradeWork *wk) {
    func_ov119_021e3004(wk->work3D);
    func_ov119_021d8e84(wk, func_ov118_021d4c20);
}

void func_ov118_021d4c20(PokemonTradeWork *wk) {
    func_ov119_021e3230(wk);
    func_ov119_021e3208(wk, 3);
    func_ov119_021d8e84(wk, func_ov118_021d4c40);
}

void func_ov118_021d4c40(PokemonTradeWork *wk) {
    VecFx32 pos1;
    VecFx32 pos2;
    VecFx32 pos3;
    VecFx32 pos4;
    VecFx32 pos5;
    VecFx32 pos6;
    VecFx32 pos7;
    VecFx32 pos8;

    func_ov119_021dfa60(wk->timer);
    if (wk->timer == func_ov118_021d4480(1)) {
        func_02015f50(wk->mcss[0], 8, 8, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d4480(2)) {
        func_02015f50(wk->mcss[0], 16, 16, 0, 0xffff);
    }
    if (wk->timer == func_ov118_021d4480(3)) {
        func_ov118_021d43c8(wk);
        func_ov118_021d434c(wk);
    }
    if (wk->timer == func_ov118_021d4480(4)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0x18);
    }
    if (wk->timer == func_ov118_021d4480(4)) {
        func_02015ea0(wk->mcss[0]);
    }
    if (wk->timer == func_ov118_021d4480(854)) {
        func_020515cc(wk->work3D->systems[0], 10, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(101)) {
        func_020515cc(wk->work3D->systems[1], 1, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(154)) {
        func_020515cc(wk->work3D->systems[2], 2, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(854)) {
        func_020515cc(wk->work3D->systems[8], 2, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(14)) {
        func_020515cc(wk->work3D->systems[3], 3, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(1155)) {
        func_020515cc(wk->work3D->systems[7], 3, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(154)) {
        func_020515cc(wk->work3D->systems[4], 4, func_ov118_021d4a34, wk);
    }
    if (wk->timer == func_ov118_021d4480(338)) {
        func_020515cc(wk->work3D->systems[5], 5, 0, wk);
    }
    if (wk->timer == func_ov118_021d4480(700)) {
        func_020515cc(wk->work3D->systems[6], 6, func_ov118_021d4a94, wk);
    }
    if (wk->timer == func_ov118_021d4480(1151)) {
        func_ov118_021d434c(wk);
    }
    if (wk->timer == func_ov118_021d4480(266)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d4480(20), 1, wk->heapId);
    }
    if (wk->timer == func_ov118_021d4480(301) - 2) {
        if (wk->mcssMoves[0] != NULL) {
            GFL_HeapFree(wk->mcssMoves[0]);
            wk->mcssMoves[0] = NULL;
        }
        if (wk->mcssMoves[1] != NULL) {
            GFL_HeapFree(wk->mcssMoves[1]);
            wk->mcssMoves[1] = NULL;
        }
        func_ov119_021e3230(wk);
        func_ov119_021d8e84(wk, func_ov118_021d49c4);
    }
    if (wk->timer == func_ov118_021d4480(330)) {
        func_02015eb0(wk->mcss[2]);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-150);
        pos1.z = FX32_CONST(10);
        func_02015c48(wk->mcss[2], &pos1);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-55);
        pos1.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[2], func_ov118_021d4480(15), &pos1, wk->heapId);
        func_02015e04(wk->mcss[2]);
        func_02015c94(wk->mcss[2], &pos1);
        pos1.x *= 1.2;
        pos1.y *= 1.2;
        func_02015cac(wk->mcss[2], &pos1);
    }
    if (wk->timer == func_ov118_021d4480(331)) {
        func_02015eb0(wk->mcss[3]);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(80);
        pos2.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[3], &pos2);
        pos2.y = FX32_CONST(5);
        pos2.x = FX32_CONST(60);
        pos2.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[3], func_ov118_021d4480(15), &pos2, wk->heapId);
        func_02015e04(wk->mcss[3]);
        func_02015c94(wk->mcss[3], &pos2);
        pos2.x *= 1.2;
        pos2.y *= 1.2;
        func_02015cac(wk->mcss[3], &pos2);
    }
    if (wk->timer == func_ov118_021d4480(340)) {
        pos3.x = FX32_CONST(-50);
        pos3.y = FX32_CONST(-55);
        pos3.z = FX32_CONST(10);
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d4480(28), &pos3);
        pos3.x = FX32_CONST(60);
        pos3.y = FX32_CONST(5);
        pos3.z = FX32_CONST(-10);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d4480(28), &pos3);
    }
    if (wk->timer == func_ov118_021d4480(368)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        pos4.x = FX32_CONST(-150);
        pos4.y = FX32_CONST(-55);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[2], func_ov118_021d4480(46), &pos4, wk->heapId);
        pos4.x = FX32_CONST(150);
        pos4.y = FX32_CONST(5);
        pos4.z = 0;
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[3], func_ov118_021d4480(46), &pos4, wk->heapId);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d4480(434)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        func_02015eb0(wk->mcss[0]);
        func_02015f50(wk->mcss[0], 0, 0, 0, 0x7fff);
        pos5.x = FX32_CONST(-150);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[0], &pos5);
        pos5.x = FX32_CONST(60);
        pos5.y = FX32_CONST(5);
        pos5.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d4480(103), &pos5, wk->heapId);
        func_02015c94(wk->mcss[0], &pos5);
        pos5.x *= 1.2;
        pos5.y *= 1.2;
        func_02015cac(wk->mcss[0], &pos5);
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        func_02015eb0(wk->mcss[1]);
        pos6.x = FX32_CONST(150);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_02015c48(wk->mcss[1], &pos6);
        pos6.x = FX32_CONST(-45);
        pos6.y = FX32_CONST(-55);
        pos6.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d4480(103), &pos6, wk->heapId);
        func_02015c94(wk->mcss[1], &pos6);
        pos6.x *= 1.2;
        pos6.y *= 1.2;
        func_02015cac(wk->mcss[1], &pos6);
        wk->mcssMoves[0]->bounce = 1;
        wk->mcssMoves[1]->bounce = 1;
    }
    if (wk->timer == func_ov118_021d4480(504)) {
        func_ov119_021dfb4c(wk->mcssMoves[0], func_ov118_021d4480(128), &wk->mcssMoves[0]->end);
        func_ov119_021dfb4c(wk->mcssMoves[1], func_ov118_021d4480(128), &wk->mcssMoves[1]->end);
    }
    if (wk->timer == func_ov118_021d4480(637)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        pos7.x = FX32_CONST(60);
        pos7.z = FX32_CONST(-10);
        pos7.y = FX32_CONST(100);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[0], func_ov118_021d4480(7), &pos7, wk->heapId);
        pos7.x = FX32_CONST(-45);
        pos7.y = FX32_CONST(-160);
        pos7.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[1], func_ov118_021d4480(7), &pos7, wk->heapId);
    }
    func_ov118_021d4480(1163);
    if (wk->timer == func_ov118_021d4480(1166)) {
        func_02015eb0(wk->mcss[3]);
        pos8.x = 0;
        pos8.y = FX32_CONST(-23);
        pos8.z = FX32_CONST(10);
        func_02015c48(wk->mcss[3], &pos8);
        func_02015e04(wk->mcss[3]);
        func_02015f50(wk->mcss[3], 16, 16, 0, 0x7fff);
    }
    if (wk->timer == func_ov118_021d4480(1167)) {
        func_02015f50(wk->mcss[3], 8, 8, 0, 0x7fff);
    }
    if (wk->timer == func_ov118_021d4480(1168)) {
        func_02015f50(wk->mcss[3], 0, 0, 0, 0x7fff);
    }
    if (wk->timer == func_ov118_021d4480(651)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d4480(10), 1, wk->heapId);
    }
    if (wk->timer == func_ov118_021d4480(693)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d4480(25), 1, wk->heapId);
    }
    if (wk->timer == func_ov118_021d4480(896)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov118_021d4480(30), 1, wk->heapId);
    }
    if (wk->timer == func_ov118_021d4480(970)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov118_021d4480(30), 1, wk->heapId);
    }
    if (wk->timer == func_ov118_021d4480(1217)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        func_ov119_021e3230(wk);
        func_ov119_021d8e84(wk, func_ov118_021d49c4);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
}


