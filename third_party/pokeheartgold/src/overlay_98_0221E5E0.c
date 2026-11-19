#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "sprite_system.h"
#include "overlay_98_sprite_ctx.h"

extern s32 ov98_0221F1AC;

extern s32 ov98_0221F1C0;

typedef struct Words5 {
    u32 w[5];
} Words5;

typedef struct Words8 {
    u32 w[8];
} Words8;

PokeathlonSpriteContext *ov98_0221E5E0(enum HeapID arg0, SpriteResourceCountsListUnion *arg1, int arg2) {
    s32 sp18[8];
    s32 sp4[5];
    SpriteResourceCountsListUnion *sp0;
    SpriteSystem *temp_r0;
    PokeathlonSpriteContext *var_r4;

    sp0 = arg1;
    var_r4 = Heap_Alloc(arg0, sizeof(PokeathlonSpriteContext));
    var_r4->heapId = arg0;
    temp_r0 = SpriteSystem_Alloc(arg0);
    var_r4->spriteSystem = temp_r0;
    var_r4->spriteManager = SpriteManager_New(temp_r0);
    var_r4->narc = NARC_New(NARC_poketool_icongra_poke_icon, var_r4->heapId);
    var_r4->paletteOffset = -1;
    *(Words8 *)sp18 = *(Words8 *)&ov98_0221F1C0;
    *(Words5 *)sp4 = *(Words5 *)&ov98_0221F1AC;
    sp4[0] = arg2;
    SpriteSystem_Init(var_r4->spriteSystem, (OamManagerParam *)sp18, (OamCharTransferParam *)sp4, 0x20);
    SpriteSystem_InitSprites(var_r4->spriteSystem, var_r4->spriteManager, arg2);
    SpriteSystem_InitManagerWithCapacities(var_r4->spriteSystem, var_r4->spriteManager, sp0);
    var_r4->paletteData = PaletteData_Init(var_r4->heapId);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    return var_r4;
}
