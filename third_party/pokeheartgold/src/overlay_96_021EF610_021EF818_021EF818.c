#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "gf_gfx_planes.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

s32 ov96_021EF8C0(void *);

s32 ov96_021EF99C(void *);

typedef struct Words5 {
    u32 w[5];
} Words5;

extern const SpriteResourceCountsListUnion ov96_0221BA68;

extern const OamManagerParam ov96_0221BAA0;

extern const Words5 ov96_0221BA54;

void ov96_021EF818(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EF818(UnkStruct_ov96_021EF610 *arg0) {
    SpriteResourceCountsListUnion sp34;
    OamManagerParam sp14;
    Words5 sp0;
    SpriteSystem *temp_r0;

    sp34 = ov96_0221BA68;
    sp14 = ov96_0221BAA0;
    sp0 = ov96_0221BA54;
    sp0.w[0] = 4;
    temp_r0 = SpriteSystem_Alloc(*((enum HeapID *)&arg0->unk_C));
    *((SpriteSystem * *)&arg0->unk_10) = temp_r0;
    *((SpriteManager * *)&arg0->unk_14) = SpriteManager_New(temp_r0);
    SpriteSystem_Init(*((SpriteSystem * *)&arg0->unk_10), &sp14, (OamCharTransferParam *)&sp0, 0x20);
    SpriteSystem_InitSprites(*((SpriteSystem * *)&arg0->unk_10), *((SpriteManager * *)&arg0->unk_14), 4);
    SpriteSystem_InitManagerWithCapacities(*((SpriteSystem * *)&arg0->unk_10), *((SpriteManager * *)&arg0->unk_14), &sp34);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(*((SpriteSystem * *)&arg0->unk_10)), 0, 0x200000);
    ov96_021EF8C0(arg0);
    ov96_021EF99C(arg0);
}
