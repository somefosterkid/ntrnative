#include "global.h"

#include "constants/heap.h"

#include "overlay_112_pokewalker_app.h"
#include "sprite_system.h"

extern const SpriteResourceCountsListUnion ov112_021FECAC;

extern const OamManagerParam ov112_021FECC4;

extern const OamCharTransferParam ov112_021FEC98;

void ov112_021EA230(Ov112PokewalkerApp *app);

void ov112_021EA230(Ov112PokewalkerApp *app) {
    SpriteResourceCountsListUnion capacities = ov112_021FECAC;
    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_154);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    OamManagerParam renderOamTemplate = ov112_021FECC4;
    OamCharTransferParam charTransferTemplate = ov112_021FEC98;
    SpriteSystem_Init(app->spriteSystem, &renderOamTemplate, &charTransferTemplate, 32);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, 217);
    SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &capacities);
}
