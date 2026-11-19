#include "global.h"

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

extern s32 ov96_0221BAE8;

void ov96_021EFA04(void *arg0);

void ov96_021EFA04(void *arg0) {
    void *new_var;
    ManagedSprite **new_var2;
    new_var2 = (ManagedSprite **)(((u8 *)(new_var = arg0)) + 0x40);
    ManagedSprite_SetAnim(*new_var2, *((s32 *)((((UnkStruct_ov96_021EF610 *)new_var)->filler_21) + ((&ov96_0221BAE8) + (((UnkStruct_ov96_021EF610 *)new_var)->unk_20) * 2))));
}
