#include "global.h"
struct PokeathlonCourseRecord;

#include "sprite_system.h"
#include "overlay_99_pokeathlon_course_record.h"

SpriteSystem *ov98_0221E5C0(void *);

SpriteManager *ov98_0221E5D0(void *);

s32 ov98_0221E8A8(void *, s32 *, s32, s32, s32);

s32 ov99_021E6400(void *);

extern s32 ov99_021E9554;

void ov99_021E6274(struct PokeathlonCourseRecord *arg0);

void ov99_021E6274(PokeathlonCourseRecord *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;

    spriteSystem = ov98_0221E5C0(*((void **)&arg0->spriteContext));
    spriteManager = ov98_0221E5D0(*((void **)&arg0->spriteContext));
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x15, 1, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x14, 0, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x16, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x17, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 1, 1, 1, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0, 0, 1, 1, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 2, 1, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 3, 1, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 5, 1, 1, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 4, 0, 5, 1, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 6, 1, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 7, 1, 2);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 9, 1, 2, 6);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 8, 0, 1, 2, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xA, 1, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xB, 1, 3);
    ov99_021E6400(arg0);
    ov98_0221E8A8(*((void **)&arg0->spriteContext), &ov99_021E9554, 3, 0, 1);
}
