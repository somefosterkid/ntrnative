#include "global.h"

#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

#include "overlay_96_ov96_rd_work.h"

UnkStruct_ov96_021E9A78 *ov96_021E9A78(enum HeapID heapId, u32 flags, u32 param2);

extern s32 ov96_0221D6F4;

extern s32 ov96_0221D720;

extern s32 ov96_0221D754;

typedef struct Words5 {
    u32 w[5];
} Words5;

void ov96_022162F4(Ov96RdWork *work);

void ov96_022162F4(Ov96RdWork *work) {
    SpriteResourceCountsListUnion sp34;
    OamManagerParam sp14;
    s32 sp0[5];
    SpriteSystem *temp_r0;

    sp34 = *(SpriteResourceCountsListUnion *)&ov96_0221D720;
    sp14 = *(OamManagerParam *)&ov96_0221D754;
    *(Words5 *)sp0 = *(Words5 *)&ov96_0221D6F4;
    sp0[0] = 0x80;
    temp_r0 = SpriteSystem_Alloc(work->heapId);
    work->spriteSystem = temp_r0;
    work->spriteManager = SpriteManager_New(temp_r0);
    SpriteSystem_Init(work->spriteSystem, &sp14, (OamCharTransferParam *)sp0, 0x20);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &sp34);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, 0x1E0000);
    work->resSrc = ov96_021E9A78(work->heapId, 0x2E7, 1);
}
