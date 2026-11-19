#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02077678.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3D0C(UnkStruct_ov14_boxsys *param_1, u8 param_2, s32 param_3);

void ov14_021F3D0C(UnkStruct_ov14_boxsys *param_1, u8 param_2, s32 param_3) {
    u8 bVar1;
    NarcId narcId;
    s32 iVar2;
    void *ptr;
    NNSG2dCharacterData *apNStack_1c;

    narcId = TypeIconGfx_GetNarcId();
    iVar2 = TypeIconGfx_GetCharFileId(param_2);
    ptr = GfGfxLoader_GetCharData(narcId, iVar2, 1, &apNStack_1c, HEAP_ID_10);
    ov14_021F2C50(param_1, param_3, apNStack_1c->pRawData, 0x100);
    Heap_Free(ptr);
    bVar1 = ManagedSprite_GetPaletteOverrideOffset(param_1->sprites[param_3]);
    iVar2 = TypeIconGfx_GetPlttOverride((u16)param_2);
    ManagedSprite_SetPaletteOverride(param_1->sprites[param_3], (u32)bVar1 + iVar2);
    return;
}
