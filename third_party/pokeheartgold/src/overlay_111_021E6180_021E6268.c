#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_111.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"

void ov111_021E62E0(ManagedSprite *, void *, u32);

void ov111_021E6268(ManagedSprite *param_1, Pokemon *param_2, s32 param_3, enum HeapID param_4) {
    int new_var2;
    void *ptr;
    NARC *narc;
    u32 memberNo;
    ManagedSprite *new_var;
    NNSG2dCharacterData *pNStack_1c;
    u8 bVar1;
    narc = NARC_New(NARC_poketool_icongra_poke_icon, param_4);
    memberNo = Pokemon_GetIconNaix(param_2);
    ptr = GfGfxLoader_GetCharDataFromOpenNarc(narc, memberNo, 0, &pNStack_1c, param_4);
    bVar1 = 0xff;
    ov111_021E62E0(param_1, pNStack_1c->pRawData, 0x400);
    Heap_Free(ptr);
    ;
    new_var2 = (((u32)Pokemon_GetIconPalette(param_2)) + param_3) & bVar1;
    ManagedSprite_SetPaletteOverride(param_1, new_var2);
    ManagedSprite_SetAnim(param_1, 1);
    ManagedSprite_SetAnimSpeed(new_var = param_1, 0x1000);
    ManagedSprite_SetAnimateFlag(param_1, 1);
    NARC_Delete(narc);
    return;
}
