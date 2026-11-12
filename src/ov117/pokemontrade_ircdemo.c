// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov192/pokemontrade_ircdemo.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/pokemontrade_ircdemo.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "system/mcss.h"
#include "system/wipe.h"

// pokemontrade_ircdemo.c, as the ROM's string has it: the scene of the trade demo of a trade over the infrared port,
// whose sprites appear in front of the effects after the common file's steps

static void func_ov117_021d4ce8(PokemonTradeWork *wk);

// pokebw2's PokemonTradeIrcDemo_Init
void func_ov117_021d4bf4(PokemonTradeWork *wk) {
    func_ov119_021dc720(wk);
    func_0204a5ac();
    func_ov119_021e3e40(wk);
    wk->clactUnit = func_0204b100(3, 0, wk->heapId);
    wk->work3D = GFL_HeapAllocate(wk->heapId, sizeof(PokemonTrade3DWork), TRUE, "pokemontrade_ircdemo.c", 101);
    wk->work3D->heapId = wk->heapId;
    func_ov119_021e5284(wk, TRUE);
    reg_G2S_DB_BLDCNT = 0;
    reg_G2_BLDCNT = 0;
    func_ov119_021e3208(wk, 2);
    func_ov119_021e36c8(wk, 1, FALSE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_ov119_021e36c8(wk, 2, FALSE, func_ov119_021d8fa4(wk, 0), TRUE);
    func_ov119_021e36c8(wk, 3, TRUE, func_ov119_021d8fa4(wk, 1), TRUE);
    func_02015c94(wk->mcss[3], &wk->work3D->ircSpriteSizes[1]);
    func_02015ea0(wk->mcss[1]);
    func_02015ea0(wk->mcss[2]);
    func_02015ea0(wk->mcss[3]);
    func_ov119_021e3070(wk->work3D);
    func_ov119_021e2efc(wk->work3D);
    func_ov117_021d4484(wk, 0);
    func_ov119_021d8e84(wk, func_ov117_021d4ce8);
}

// The scene, a step for each frame of the timeline
// pokebw2's PokemonTradeIrcDemo_Run
static void func_ov117_021d4ce8(PokemonTradeWork *wk) {
    VecFx32 pos1;
    VecFx32 pos2;
    VecFx32 size;
    VecFx32 pos3;

    if (wk->timer == func_ov117_021d4480(1)) {
        func_02015f50(wk->mcss[0], 8, 8, 0, 0xffff);
    }
    if (wk->timer == func_ov117_021d4480(2)) {
        func_02015f50(wk->mcss[0], 16, 16, 0, 0xffff);
    }
    if (wk->timer == func_ov117_021d4480(3)) {
        func_ov117_021d43c8(wk);
        func_ov117_021d434c(wk);
    }
    if (wk->timer == func_ov117_021d4480(4)) {
        func_ov119_021e71f0(wk, 0x11);
        func_ov119_021e7208(wk, 0x18);
    }
    if (wk->timer == func_ov117_021d4480(4)) {
        func_02015ea0(wk->mcss[0]);
    }
    if (wk->timer == func_ov117_021d4480(-1)) {
        func_020515cc(wk->work3D->systems[0], 0, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(0x15e)) {
        func_020515cc(wk->work3D->systems[1], 0, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(0xc8)) {
        func_020515cc(wk->work3D->systems[2], 1, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(0x14)) {
        func_020515cc(wk->work3D->systems[3], 2, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(0x258)) {
        func_020515cc(wk->work3D->systems[4], 2, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(-1)) {
        func_020515cc(wk->work3D->systems[5], 3, NULL, wk);
    }
    if (wk->timer == func_ov117_021d4480(0x256)) {
        func_ov117_021d434c(wk);
    }
    if (wk->timer == func_ov117_021d4480(0x82)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov117_021d4480(20), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov117_021d4480(0xb4)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov117_021d4480(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov117_021d4480(0xfa)) {
        func_02015eb0(wk->mcss[2]);
        pos1.x = FX32_CONST(-50);
        pos1.y = FX32_CONST(-500);
        pos1.z = FX32_CONST(10);
        func_02015c48(wk->mcss[2], &pos1);
        pos1.y = FX32_CONST(300);
        pos1.x = FX32_CONST(-50);
        pos1.z = FX32_CONST(10);
        func_ov119_021dfd24(wk->mcssMoves[0]);
        wk->mcssMoves[0] = func_ov119_021dfb84(wk->mcss[2], func_ov117_021d4480(150), &pos1, wk->heapId);
        func_02015c94(wk->mcss[2], &pos1);
        pos1.x *= 2;
        pos1.y *= 2;
        func_02015cac(wk->mcss[2], &pos1);
    }
    if (wk->timer == func_ov117_021d4480(0xfb)) {
        func_02015eb0(wk->mcss[3]);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(300);
        pos2.z = FX32_CONST(-10);
        func_02015c48(wk->mcss[3], &pos2);
        pos2.x = FX32_CONST(60);
        pos2.y = FX32_CONST(-370);
        pos2.z = FX32_CONST(-10);
        func_ov119_021dfd24(wk->mcssMoves[1]);
        wk->mcssMoves[1] = func_ov119_021dfb84(wk->mcss[3], func_ov117_021d4480(150), &pos2, wk->heapId);
        func_02015cac(wk->mcss[3], &wk->work3D->ircSpriteSizes[1]);
        func_02015c94(wk->mcss[3], &pos2);
        wk->work3D->ircSpriteSizes[0].x = pos2.x;
        wk->work3D->ircSpriteSizes[0].y = pos2.y;
        wk->work3D->ircSpriteSizes[0].z = pos2.z;
        func_02015c94(wk->mcss[3], &size);
    }
    func_ov117_021d4480(0xfc);
    if (wk->timer == func_ov117_021d4480(0x267)) {
        MCSS *shown;

        if (wk->unk11EE == 0) {
            shown = wk->mcss[3];
        } else {
            func_ov117_021d49f4(wk->mcss[3], wk);
            func_02015ea0(wk->mcss[3]);
            shown = wk->mcss[1];
        }
        func_ov117_021d49f4(shown, wk);
        func_ov117_021d4a48(wk, 3);
    }
    if (wk->timer == func_ov117_021d4480(0x268)) {
        func_02015f50(wk->mcss[3], 8, 8, 0, 0x7fff);
        func_02015f50(wk->mcss[1], 8, 8, 0, 0x7fff);
    }
    if (wk->timer == func_ov117_021d4480(0x269)) {
        func_02015f50(wk->mcss[3], 0, 0, 0, 0x7fff);
        func_02015f50(wk->mcss[1], 0, 0, 0, 0x7fff);
    }
    if (func_ov117_021d4480(0x267) < wk->timer && wk->unk11EE != 0 && wk->mcssMoves[3] != NULL &&
        wk->mcssMoves[3]->frame == 9) {
        func_02015c30(wk->mcss[1], &pos3);
        func_02015c48(wk->mcss[3], &pos3);
        func_02015eb0(wk->mcss[3]);
        func_02015ea0(wk->mcss[1]);
        wk->mcssMoves[3]->mcss = wk->mcss[3];
    }
    if (wk->timer == func_ov117_021d4480(0x279)) {
        func_02015e4c(wk->mcss[3]);
    }
    func_ov119_021dfa98(wk->timer);
    if (wk->timer == func_ov117_021d4480(0x199)) {
        GFL_WipeSet(0, 0, 0, 0x7fff, func_ov117_021d4480(10), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov117_021d4480(0x1a4)) {
        GFL_WipeSet(0, 1, 1, 0x7fff, func_ov117_021d4480(25), TRUE, wk->heapId);
    }
    if (wk->timer == func_ov117_021d4480(0x293)) {
        GFL_HeapFree(wk->mcssMoves[0]);
        wk->mcssMoves[0] = NULL;
        GFL_HeapFree(wk->mcssMoves[1]);
        wk->mcssMoves[1] = NULL;
        func_ov119_021e3230(wk);
        func_ov119_021d8e84(wk, func_ov117_021d49c4);
    }
    func_ov119_021dfc68(wk->mcssMoves[0], wk);
    func_ov119_021dfc68(wk->mcssMoves[1], wk);
    func_ov119_021dfc68(wk->mcssMoves[2], wk);
    func_ov119_021dfc68(wk->mcssMoves[3], wk);
}
