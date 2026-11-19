#include "global.h"

#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

void *ov96_021E9A78(enum HeapID, s32, s32);

extern s32 ov96_0221CF2C;

extern s32 ov96_0221CF40;

extern s32 ov96_0221CF74;

typedef struct Words5 {
    u32 w[5];
} Words5;

void ov96_0220EA08(UnkStruct_ov96_0220E3F0 *arg0);

void ov96_0220EA08(UnkStruct_ov96_0220E3F0 *arg0) {
    SpriteResourceCountsListUnion sp34;
    OamManagerParam sp14;
    s32 sp0[5];
    SpriteSystem *temp_r0;

    sp34 = *(SpriteResourceCountsListUnion *)&ov96_0221CF40;
    sp14 = *(OamManagerParam *)&ov96_0221CF74;
    *(Words5 *)sp0 = *(Words5 *)&ov96_0221CF2C;
    sp0[0] = 0x80;
    temp_r0 = SpriteSystem_Alloc(arg0->heapId);
    arg0->spriteSystem = temp_r0;
    arg0->spriteManager = SpriteManager_New(temp_r0);
    SpriteSystem_Init(arg0->spriteSystem, &sp14, (OamCharTransferParam *)sp0, 0x20);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(arg0->spriteSystem, arg0->spriteManager, &sp34);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSystem), 0, 0x1E0000);
    arg0->unk_1C = ov96_021E9A78(arg0->heapId, 0xAA7, 1);
}
