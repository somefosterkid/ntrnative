#include "global.h"
struct PokeathlonMedals;

#include "sprite_system.h"
#include "overlay_99_pokeathlon_medals.h"
#include "overlay_98_ov98text_sys.h"

void ov99_021E7508(struct PokeathlonMedals *arg0);

void ov99_021E7508(PokeathlonMedals *arg0) {
    s32 temp_r2;
    s32 var_r1;
    s32 var_r5;
    u32 temp_r0;

    temp_r2 = *((u32 *)&arg0->flags);
    var_r1 = 0;
    temp_r0 = (u32)(temp_r2 << 0x1B) >> 0x1B;
    if ((temp_r0 != 0) && (((u32)(temp_r2 << 0xD) >> 0x1B) != 0)) {
        var_r1 = 1;
    }
    if ((temp_r0 != 0) && ((((u32)(*((s32 *)&arg0->flags) << 0xD)) >> 0x1B) < temp_r0)) {
        var_r5 = 1;
    } else {
        var_r5 = 0;
    }
    ManagedSprite_SetDrawFlag(arg0->sprites[1], var_r1);
    ManagedSprite_SetDrawFlag(arg0->sprites[2], var_r5);
    ov98_0221EDA4(arg0->textContext, ((u32)(*((s32 *)&arg0->flags) << 0xD) >> 0x1B) + 1, 2, 0);
    ov98_0221EDA4(arg0->textContext, ((u32)(*((s32 *)&arg0->flags) << 0x1B) >> 0x1B) + 1, 2, 1);
    ov98_0221ED48(arg0->textContext, 3, 0x81, 0, 0);
}
