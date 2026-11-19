#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

void sub_02065998(void);

void sub_02065998(void) {
    return;
}

u8 sub_0206599C(LocalMapObject *param_1);

u8 sub_0206599C(LocalMapObject *param_1) {
    u8 *puVar1;

    puVar1 = sub_0205F394(param_1);
    return puVar1[3];
}

u32 sub_020659A8(LocalMapObject *arg0);

u32 sub_020659A8(LocalMapObject *object) {
    return (u32)(*(u16 *)((u8 *)(sub_0205F394(object)) + 0xA) << 0x1D) >> 0x1E;
}

void sub_020659B8(LocalMapObject *arg0);

void sub_020659B8(LocalMapObject *object) {
    u8 *v0;

    v0 = sub_0205F394(object);
    *(u16 *)((u8 *)v0 + 0xA) = (u16)(1 | (*(u16 *)((u8 *)v0 + 0xA) & ~1));
}
