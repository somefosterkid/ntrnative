#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2E78(void *arg0, u8 *arg1, s32 arg2, s32 arg3) {
    ov14_021F2C1C(arg0, arg2, arg1, 0x200);
    ManagedSprite_SetPaletteOverride(((UnkStruct_ov14_boxsys *)arg0)->sprites[arg2], arg3 + 3);
}

void *ov14_021F2DC4(struct UnkStruct_ov14_boxsys *, BoxPokemon *, NNSG2dCharacterData **);

void ov14_021F2EA0(UnkStruct_ov14_boxapp *arg0, BoxPokemon *boxMon, s32 arg2) {
    NNSG2dCharacterData *v0;
    void *ptr;

    ptr = ov14_021F2DC4(arg0->sys, boxMon, &v0);
    ov14_021F2E78(arg0->sys, (u8 *)v0->pRawData, arg2, Boxmon_GetIconPalette(boxMon));
    Heap_Free(ptr);
}
