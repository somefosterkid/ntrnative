#include "global.h"
#include "overlay_99_pokeathlon_medals.h"
struct PokeathlonMedals;

void ov98_0221E7E8(void *, u32 *, u32, u32, u32);

u32 ov99_021E71E4(void *, u32);

u32 ov99_021E7208(void *, u32);

u32 ov99_021E723C(void *);

void ov99_021E76D8(void *param_1, u32 param_2);

void ov99_021E76D8(void *param_1, u32 param_2) {
    u32 uVar1;
    u32 uStack_1c[3] = { 0 };
    uStack_1c[0] = (uStack_1c[0] & 0xFFFFFE00) | (0x1FF & param_2);
    uVar1 = ov99_021E71E4(param_1, param_2);
    uStack_1c[0] = (uStack_1c[0] & 0xffffc1ff) | ((uVar1 & 0x1f) << 9);
    uVar1 = ov99_021E7208(param_1, param_2);
    uStack_1c[0] = (uStack_1c[0] & 0xffff3fff) | ((uVar1 & 3) << 0xe);
    if (param_2 == 0x147) {
        uStack_1c[1] = ov99_021E723C(param_1);
    }
    ov98_0221E7E8(*((void **)&((PokeathlonMedals *)param_1)->sprites[3]), uStack_1c, 0, 0, *((u32 *)&((PokeathlonMedals *)param_1)->heapId));
    return;
}
