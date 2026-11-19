#include "global.h"
#include "overlay_59_apricorn_app.h"

#include "overlay_manager.h"
#include "sprite_system.h"

s32 ov59_02237FA4(Ov59ApricornApp *arg0);

s32 ov59_0223801C(Ov59ApricornApp *arg0);

s32 ov59_0223834C(Ov59ApricornApp *arg0);

s32 ov59_02238578(Ov59ApricornApp *arg0);

s32 ov59_0223858C(Ov59ApricornApp *arg0);

s32 ov59_02238068(Ov59ApricornApp *arg0);

s32 ov59_022380E4(Ov59ApricornApp *arg0);

s32 ov59_022380EC(Ov59ApricornApp *arg0);

s32 ov59_0223811C(Ov59ApricornApp *arg0);

s32 ov59_02238124(Ov59ApricornApp *arg0);

s32 ov59_02238218(Ov59ApricornApp *arg0);

s32 ov59_022382F4(Ov59ApricornApp *arg0);

s32 ov59_022383B4(Ov59ApricornApp *arg0);

s32 ov59_022384E4(Ov59ApricornApp *arg0);

u32 ov59_02237DA8(OverlayManager *param_1, s32 *param_2);

u32 ov59_02237DA8(OverlayManager *param_1, s32 *param_2) {
    Ov59ApricornApp *arg0;
    s32 sVar1;

    arg0 = OverlayManager_GetData(param_1);
    switch (*param_2) {
    case 0:
        sVar1 = ov59_02237FA4(arg0);
        *param_2 = sVar1;
        break;
    case 1:
        sVar1 = ov59_0223801C(arg0);
        *param_2 = sVar1;
        break;
    case 2:
        sVar1 = ov59_02238068(arg0);
        *param_2 = sVar1;
        break;
    case 3:
        sVar1 = ov59_022380E4(arg0);
        *param_2 = sVar1;
        break;
    case 4:
        sVar1 = ov59_022380EC(arg0);
        *param_2 = sVar1;
        break;
    case 5:
        sVar1 = ov59_0223811C(arg0);
        *param_2 = sVar1;
        break;
    case 6:
        sVar1 = ov59_02238124(arg0);
        *param_2 = sVar1;
        break;
    case 7:
        sVar1 = ov59_02238218(arg0);
        *param_2 = sVar1;
        break;
    case 8:
        sVar1 = ov59_022382F4(arg0);
        *param_2 = sVar1;
        break;
    case 9:
        sVar1 = ov59_0223834C(arg0);
        *param_2 = sVar1;
        break;
    case 10:
        sVar1 = ov59_022383B4(arg0);
        *param_2 = sVar1;
        break;
    case 0xb:
        sVar1 = ov59_022384E4(arg0);
        *param_2 = sVar1;
        break;
    case 0xe:
        sVar1 = ov59_02238578(arg0);
        *param_2 = sVar1;
        break;
    case 0xf:
        sVar1 = ov59_0223858C(arg0);
        *param_2 = sVar1;
        break;
    case 0x10:
        return 1;
    }
    SpriteSystem_DrawSprites(arg0->spriteManager);
    return 0;
}
