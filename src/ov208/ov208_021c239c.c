// Black's code at ov208 0x021c239c-0x021c2560. No name is known: the module and address.

#include "types.h"
#include "app/ov208_021c239c.h"
#include "app/zukan_detail.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"

void func_ov208_021c239c(ZukanDetailWork *wk) {
    reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff;
    func_ov208_021bc518(wk->blendMain);
}

void func_ov208_021c23bc(ZukanDetailWork *wk) {
    s16 value;

    G2_SetBlendAlpha(0, ALL_PLANES, wk->glowEv, 16 - wk->glowEv);
    if (wk->glowPhase + 0x400 >= 0x10000u) {
        wk->glowPhase = wk->glowPhase + 0x400 - 0x10000;
    } else {
        wk->glowPhase += 0x400;
    }
    value = FX_CosIdx(wk->glowPhase);
    value = (value + FX32_ONE) / 2;
    wk->glowEv = ((value * 14) >> FX32_SHIFT) + 1;
}

void func_ov208_021c2430(ZukanDetailWork *wk) {
    wk->glowEv = 1;
    wk->glowPhase = 0x8000;
    G2_SetBlendAlpha(0, ALL_PLANES, wk->glowEv, 16 - wk->glowEv);
}

void func_ov208_021c2458(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        if (i != ACTOR_UNKNOWN) {
            func_0204b688(wk->actors[i], 0);
        }
    }
    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (wk->places[i].state == PLACE_SHOWN) {
            func_0204b688(wk->places[i].actor, 0);
        }
    }
}

void func_ov208_021c24a4(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < ACTOR_COUNT; i++) {
        if (i != ACTOR_UNKNOWN) {
            func_0204b688(wk->actors[i], 1);
        }
    }
    for (i = 0; i < TOWNMAP_PLACE_COUNT; i++) {
        if (wk->places[i].state == PLACE_SHOWN) {
            func_0204b688(wk->places[i].actor, 1);
        }
    }
}

void func_ov208_021c24f0(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (wk->icons[i] != NULL) {
            func_0204b688(wk->icons[i], 0);
        }
    }
    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG3, ALL_PLANES, 12, 4);
}

void func_ov208_021c252c(ZukanDetailMapParam *param, ZukanDetailWork *wk, ZukanDetailCommon *common) {
    u8 i;

    for (i = 0; i < 2; i++) {
        if (wk->icons[i] != NULL) {
            func_0204b688(wk->icons[i], 1);
        }
    }
    func_ov208_021bc518(wk->blendSub);
}

void func_ov208_021c255c(ZukanDetailVoiceParam *param, HeapID heapId) {
    param->heapId = heapId;
}
