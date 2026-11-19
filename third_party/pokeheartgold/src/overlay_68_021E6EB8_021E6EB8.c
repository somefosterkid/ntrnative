#include "global.h"
struct MoveRelearner;

#include "filesystem.h"
#include "sprite_system.h"
#include "unk_02077678.h"
#include "overlay_68_move_relearner.h"

void ov68_021E6EB8(struct MoveRelearner *param_1, NARC *param_2);

void ov68_021E6EB8(MoveRelearner *param_1, NARC *param_2) {
    u32 resId;
    u32 limit;

    SpriteSystem_LoadCharResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 6, 0, 2, 0xb8a8);
    SpriteSystem_LoadCharResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 0xb, 0, 2, 0xb8a9);
    resId = 0xb8ab;
    limit = resId + 4;
    do {
        if (resId < limit) {
            SpriteSystem_LoadMoveTypeIconCharResObj(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), NNS_G2D_VRAM_TYPE_2DSUB, 0, resId);
        } else {
            SpriteSystem_LoadMoveTypeIconCharResObj(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), NNS_G2D_VRAM_TYPE_2DMAIN, 0, resId);
        }
        resId = resId + 1;
    } while (resId <= 0xB8B2);
    SpriteSystem_LoadMoveCategoryIconCharResObj(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), NNS_G2D_VRAM_TYPE_2DSUB, 0, 0xB8AA);
    SpriteSystem_LoadPlttResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 5, 0, 4, 2, 0xb8a8);
    SpriteSystem_LoadMoveTypeAndCategoryIconsPltt(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), 2, 0xb8a9);
    SpriteSystem_LoadMoveTypeAndCategoryIconsPltt(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), 1, 0xB8AA);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 7, 0, 0xb8a8);
    SpriteSystem_LoadCellResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 10, 0, 0xb8a9);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 8, 0, 0xb8a8);
    SpriteSystem_LoadAnimResObjFromOpenNarc(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), param_2, 9, 0, 0xb8a9);
    SpriteSystem_LoadMoveTypeAndCategoryIconsCellAndAnim(*((SpriteSystem * *)&param_1->spriteSystem), *((SpriteManager * *)&param_1->spriteManager), 0xB8AA, 0xB8AA);
    return;
}
