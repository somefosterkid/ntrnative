#include "global.h"

#include "filesystem.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_ov96_0220_data.h"

struct UnkStruct_ov96_021E9A78 *ov96_021E9A78(enum HeapID, u32, u32);

extern s32 ov96_0221CC1C;

extern s32 ov96_0221CC30;

extern s32 ov96_0221CC80;

void ov96_02209840(Ov96_0220Data *arg0);

void ov96_02209840(Ov96_0220Data *arg0) {
    SpriteResourceCountsListUnion sp34;
    OamManagerParam sp14;
    OamCharTransferParam sp0;
    SpriteSystem *temp_r0;

    sp34 = *(SpriteResourceCountsListUnion *)&ov96_0221CC30;
    sp14 = *(OamManagerParam *)&ov96_0221CC80;
    sp0 = *(OamCharTransferParam *)&ov96_0221CC1C;
    sp0.maxTasks = 0x80;
    temp_r0 = SpriteSystem_Alloc(arg0->heapId);
    arg0->spriteSystem = temp_r0;
    arg0->spriteManager = SpriteManager_New(temp_r0);
    SpriteSystem_Init(arg0->spriteSystem, &sp14, &sp0, 0x20);
    SpriteSystem_InitSprites(arg0->spriteSystem, arg0->spriteManager, 0x100);
    SpriteSystem_InitManagerWithCapacities(arg0->spriteSystem, arg0->spriteManager, &sp34);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(arg0->spriteSystem), 0, 0x100000);
    arg0->resSrc = ov96_021E9A78(arg0->heapId, 0x2E7, 1);
    arg0->narc = NARC_New(NARC_poketool_icongra_poke_icon, arg0->heapId);
}
