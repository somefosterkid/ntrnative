#include "global.h"
#include "gf_gfx_planes.h"
#include "overlay_108_021E8850.h"
#include "screen_fade.h"
#include "overlay_108_safari_decoration_app.h"

u32 ov108_021E8B24(SafariDecorationApp *param_1) {
    u32 uVar1;
    s32 sVar2;

    switch ((u32)param_1->state) {
    case 0:
        uVar1 = ov108_021E9388(param_1);
        param_1->state = uVar1;
        break;
    case 1:
        param_1->state = 0;
        return 2;
    case 2:
        sVar2 = ov108_021E93A8(param_1);
        param_1->state = sVar2;
        break;
    case 3:
        param_1->state = 0;
        return (u32)param_1->result;
    }
    return 1;
}

s32 ov108_021E8B68(SafariDecorationApp *arg0) {
    s32 v0;
    s32 v1;

    v1 = arg0->state;
    switch (v1) {
    case 0:
        ov108_021E940C(arg0, 1);
        ov108_021E9488(arg0);
        arg0->state = (s32)(arg0->state + 1);
        break;
    case 1:
        v0 = ov108_021E94E8(arg0);
        if (v0 >= 0) {
            if (v0 == 1) {
                ov108_021E8C64(arg0);
                arg0->state = 0;
                return 3;
            }
            arg0->state = (s32)(arg0->state + 1);
            break;
        }
        break;
    default:
        ov108_021E940C(arg0, 0);
        arg0->state = 0;
        return 1;
    }
    return 2;
}

s32 ov108_021E8BC0(SafariDecorationApp *arg0) {
    s32 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, arg0->heapID);
        GfGfx_EngineATogglePlanes(0x10U, 1U);
        GfGfx_EngineBTogglePlanes(0x10U, 1U);
        arg0->state = (s32)(arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0;
            return 1;
        }
        break;
    }
    return 0;
}

s32 ov108_021E8C18(SafariDecorationApp *arg0) {
    s32 v0;

    v0 = arg0->state;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, arg0->heapID);
        arg0->state = (s32)(arg0->state + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            arg0->state = 0;
            return 4;
        }
        break;
    }
    return 3;
}
