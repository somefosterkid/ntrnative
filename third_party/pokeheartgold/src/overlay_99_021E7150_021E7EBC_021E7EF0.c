#include "global.h"

#include "overlay_99_pokeathlon_medals.h"

u32 ov98_0221E5E0(u32, u32 *, u32);

void ov98_0221E6F0(void *, void *, u8 *, u32, u32, s16, s16, u32);

u32 ov99_021E724C(void *);

void ov99_021E7F90(void *param_1);

void ov99_021E80D0(void *param_1);

void ov99_021E810C(void *param_1);

void ov99_021E8198(void *param_1);

extern u32 ov99_021E9F60;

typedef struct {
    u32 a[6];
} ov99_021E9FC4_Array;

extern const ov99_021E9FC4_Array ov99_021E9FC4;

void ov99_021E7EF0(void *param_1);

void ov99_021E7EF0(void *param_1) {
    u32 uVar1;
    ov99_021E9FC4_Array local = ov99_021E9FC4;
    u32 *end = local.a + 6;
    uVar1 = ov98_0221E5E0(*(u32 *)&((PokeathlonMedals *)param_1)->heapId, local.a, 0x32);
    *(u32 *)&((PokeathlonMedals *)param_1)->spriteContext = uVar1;
    ov99_021E7F90(param_1);
    ov99_021E80D0(param_1);
    ov99_021E810C(param_1);
    ov99_021E8198(param_1);
    uVar1 = ov99_021E724C(param_1);
    ov98_0221E6F0(*(void **)&((PokeathlonMedals *)param_1)->spriteContext, &((PokeathlonMedals *)param_1)->sprites[0], (unsigned char *)(&ov99_021E9F60), uVar1, 5, 8, 0x98, 1);
    return;
}
