#include "global.h"
#include "overlay_99_pokeathlon_event_record.h"

void *ov98_0221E5E0(u32, u32 *, u32);

void ov99_021E8C50(void *);

void ov99_021E8C88(void *);

void ov99_021E8D5C(void *);

void ov99_021E8D88(void *);

void ov99_021E8EBC(void *);

typedef struct {
    u32 a[6];
} ov99_021EA3AC_Array;

extern const ov99_021EA3AC_Array ov99_021EA3AC;

void ov99_021E8C04(u8 *param_1);

void ov99_021E8C04(u8 *param_1) {
    void *uVar1;
    ov99_021EA3AC_Array local = ov99_021EA3AC;
    uVar1 = ov98_0221E5E0(((PokeathlonEventRecord *)param_1)->heapId, local.a, 0x25);
    *((void **)&((PokeathlonEventRecord *)param_1)->spriteContext) = uVar1;
    ov99_021E8C88(param_1);
    ov99_021E8D5C(param_1);
    ov99_021E8D88(param_1);
    ov99_021E8EBC(param_1);
    ov99_021E8C50(param_1);
    return;
}
