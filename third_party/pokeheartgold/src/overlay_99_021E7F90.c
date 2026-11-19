#include "global.h"
struct PokeathlonMedals;

#include "sprite_system.h"
#include "overlay_99_pokeathlon_medals.h"

SpriteSystem *ov98_0221E5C0(void *);

SpriteManager *ov98_0221E5D0(void *);

s32 ov98_0221E8A8(void *, s32 *, s32, s32, s32);

s32 ov99_021E7F64(void *);

extern s32 ov99_021E9F90;

void ov99_021E7F90(struct PokeathlonMedals *arg0);

ManagedSprite *ov98_0221E6E0(void *, s32 *);
extern s32 ov99_021EA180;
void ov99_021E80D0(struct PokeathlonMedals *arg0);

void ov99_021E7F90(PokeathlonMedals *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;

    spriteSystem = ov98_0221E5C0(*((void **)&arg0->spriteContext));
    spriteManager = ov98_0221E5D0(*((void **)&arg0->spriteContext));
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 1, 1, 1, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0, 0, 1, 1, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 2, 1, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 3, 1, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 4, 0, 5, 2, 3);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 5, 1, 2, 0x20);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 6, 1, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 7, 1, 3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x18, 0, 1, 1, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x19, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x1A, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0x1B, 1, 0);
    ov98_0221E8A8(*((void **)&arg0->spriteContext), &ov99_021E9F90, 0x1E, 1, 0);
    ov99_021E7F64(arg0);
}

void ov99_021E80D0(PokeathlonMedals *arg0) {
    s32 *v0;
    s32 v2;

    v0 = &ov99_021EA180;
    v2 = 0;
    do {
        arg0->sprites[v2] = ov98_0221E6E0(*((void **)&arg0->spriteContext), v0);
        ManagedSprite_SetAnimateFlag(arg0->sprites[v2], 1);
        v0 += 13;
        v2 += 1;
    } while (v2 < 4);
}
