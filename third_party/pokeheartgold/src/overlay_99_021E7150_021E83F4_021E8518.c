#include "global.h"

#include "overlay_99_pokeathlon_event_record.h"

u32 ov98_0221EFE8(u32, u32, u32);

u32 ov99_021E94CC(void *, u32, u32, u32);

u32 ov99_021E8518(u32 *param_1, u32 param_2, u32 param_3);

u32 ov99_021E8518(u32 *param_1, u32 param_2, u32 param_3) {
    u32 uVar1;

    uVar1 = ov99_021E94CC(((PokeathlonEventRecord *)param_1)->data, param_2, 0, 0);
    if (*(u32 *)&((PokeathlonEventRecord *)param_1)->linkMode != 0) {
        return 0;
    }
    uVar1 = ov98_0221EFE8(param_2 & 0xff, param_3, uVar1);
    return uVar1;
}
