#include "global.h"

#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_app_021ed1d0.h"

extern s32 ov96_0221AF74;

extern s32 ov96_0221AF88;

extern s32 ov96_0221B010;

void ov96_021EC490(Ov96App_021ED1D0 *arg0);

void ov96_021EC490(Ov96App_021ED1D0 *arg0) {
    SpriteResourceCountsListUnion sp34;
    OamManagerParam sp14;
    OamCharTransferParam sp0;
    SpriteSystem *temp_r0;

    sp34 = *(SpriteResourceCountsListUnion *)&ov96_0221AF88;
    sp14 = *(OamManagerParam *)&ov96_0221B010;
    sp0 = *(OamCharTransferParam *)&ov96_0221AF74;
    sp0.maxTasks = 0x80;
    temp_r0 = SpriteSystem_Alloc(arg0->heapId);
    arg0->spriteSystem = temp_r0;
    arg0->spriteManager = SpriteManager_New(temp_r0);
    SpriteSystem_Init(arg0->spriteSystem, &sp14, &sp0, 0x20);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0x80);
    SpriteSystem_InitManagerWithCapacities(arg0->spriteSystem, arg0->spriteManager, &sp34);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSystem), 0, 0x20C000);
}
