#include "global.h"

#include "field_types_def.h"
#include "map_object.h"

void ov01_02205808(s32, LocalMapObject *, u32);

void ov01_021F902C(s32 a0, LocalMapObject *a1);

void ov01_021F9048(LocalMapObject *arg0);

void ov01_021F902C(s32 a0, LocalMapObject *a1) {
    u8 *puVar1;

    puVar1 = sub_0205F40C(a1);
    ov01_02205808(a0, a1, *(u32 *)puVar1);
    return;
}

void ov01_021F9048(LocalMapObject *object) {
    u8 *v0;

    v0 = sub_0205F40C(object);
    *(u8 *)((u8 *)v0 + 0x17) = (u8)(*(u8 *)((u8 *)v0 + 0x17) & ~1);
}
