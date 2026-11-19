#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_111.h"

extern const u32 ov111_021E6B8C[5];

extern const u32 ov111_021E6BA0[6];

extern const u32 ov111_021E6BB8[8];

void ov111_021E60D4(BugContestSwapMon *data);

void ov111_021E60D4(BugContestSwapMon *data) {
    OamManagerParam oam;
    OamCharTransferParam transfer;
    SpriteResourceCountsListUnion capacities;

    data->spriteSystem = SpriteSystem_Alloc(data->heapID);
    data->spriteManager = SpriteManager_New(data->spriteSystem);
    oam = *(const OamManagerParam *)ov111_021E6BB8;
    transfer = *(const OamCharTransferParam *)ov111_021E6B8C;
    *(u32 *)&transfer = 0x20;
    SpriteSystem_Init(data->spriteSystem, &oam, &transfer, 0x20);
    SpriteSystem_InitSprites(data->spriteSystem, data->spriteManager, 0x20);
    capacities = *(const SpriteResourceCountsListUnion *)ov111_021E6BA0;
    SpriteSystem_InitManagerWithCapacities(data->spriteSystem, data->spriteManager, &capacities);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(data->spriteSystem), 0, 0x20C000);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
}
