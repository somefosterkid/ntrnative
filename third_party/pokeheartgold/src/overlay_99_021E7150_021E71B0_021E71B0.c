#include "global.h"
#include "overlay_99_pokeathlon_medals.h"

u16 ov99_021E71B0(void *arg0);

u16 ov99_021E71B0(void *arg0) {
    s32 temp_r1;
    u32 new_var;
    s32 temp_r2;
    temp_r1 = *((s32 *)&((PokeathlonMedals *)arg0)->flags);
    temp_r2 = 0x1B;
    new_var = (u32)(temp_r1 << 5);
    temp_r2 = (((u32)(temp_r1 << 5)) >> 0x18) + (0x1E * (((u32)(temp_r1 << 0xD)) >> temp_r2));
    if (temp_r2 >= 0x1ED) {
        return 0U;
    }
    return *((u16 *)&((PokeathlonMedals *)arg0)->species[((new_var >> 0x18) + (0x1E * (((u32)(temp_r1 << 0xD)) >> 0x1B)))]);
}
