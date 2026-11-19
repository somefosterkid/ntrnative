#include "global.h"

#include "overlay_99_pokeathlon_event_record.h"

void ov98_0221EFE8(u32, u32, u32);

u32 ov99_021E945C(void *);

u32 ov99_021E94CC(void *, u32, u32, u32);

void ov99_021E84EC(u32 *param_1, u32 param_2);

void ov99_021E84EC(u32 *param_1, u32 param_2) {
    u32 uVar1;
    u32 uVar2;

    uVar1 = ov99_021E94CC(((PokeathlonEventRecord *)param_1)->data, param_2, 0, *(u32 *)&((PokeathlonEventRecord *)param_1)->linkMode);
    uVar2 = ov99_021E945C((u8 *)((PokeathlonEventRecord *)param_1)->data);
    ov98_0221EFE8(param_2 & 0xff, uVar2, uVar1);
    return;
}
