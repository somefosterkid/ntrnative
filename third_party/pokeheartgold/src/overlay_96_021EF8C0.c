#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

void ov96_021EF8C0(struct UnkStruct_ov96_021EF610 *arg0);

s32 ov96_021EFA04(void *);
void ov96_021EF924(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EF8C0(UnkStruct_ov96_021EF610 *arg0) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;

    spriteManager = *((SpriteManager * *)&arg0->unk_14);
    spriteSystem = *((SpriteSystem * *)&arg0->unk_10);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, *((NARC * *)&arg0->unk_8), 0, 0, 0x10, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, *((NARC * *)&arg0->unk_8), 1, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, *((NARC * *)&arg0->unk_8), 2, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, *((NARC * *)&arg0->unk_8), 3, 1, 0);
}

void ov96_021EF924(UnkStruct_ov96_021EF610 *arg0) {
    if (ManagedSprite_IsAnimated(*((ManagedSprite * *)&arg0->unk_40)) == 0) {
        arg0->filler_21 = (u8)(arg0->filler_21 + 1);
        if ((u32) arg0->filler_21 >= 2U) {
            arg0->filler_21 = 0U;
        }
        ov96_021EFA04(arg0);
    }
}
