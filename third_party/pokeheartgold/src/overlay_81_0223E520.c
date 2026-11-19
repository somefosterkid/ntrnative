#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_81.h"
#include "screen_fade.h"
#include "overlay_81_battle_factory_app.h"

s32 ov80_02237254(u8);

s32 ov81_0224093C(void *, void *, s32, s32, u8);

s32 ov81_02240F08(void *, s32);

s32 ov81_0223E520(void *arg0) {
    u8 v0;

    v0 = ((BattleFactoryApp *)arg0)->subState;
    switch (v0) {
    case 0:
        if (ov81_02240F08(arg0, 0) == 1) {
            ov81_0223E5B4(arg0);
        } else {
            ov81_0223E8BC(arg0);
        }
        if (ov80_02237254(((BattleFactoryApp *)arg0)->challengeType) == 1) {
            ov81_0224093C(arg0, &((BattleFactoryApp *)arg0)->windows[1], 0, 0, 0);
            GfGfx_EngineATogglePlanes(4U, 1U);
        }
        if (((u32)(*((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)) << 0x1B) >> 0x1F) == 1) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 3, HEAP_ID_100);
        }
        *((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)) = (u8)(0x10 | *((u8 *)((u8 *)&((BattleFactoryApp *)arg0)->numPokeballs + 0x1)));
        ((BattleFactoryApp *)arg0)->subState = (u8)(((BattleFactoryApp *)arg0)->subState + 1);
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}
