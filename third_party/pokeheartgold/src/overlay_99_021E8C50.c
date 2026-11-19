#include "global.h"
struct PokeathlonEventRecord;

#include "sprite_system.h"
#include "overlay_99_pokeathlon_event_record.h"

s32 ov99_021E8F50(void *);

void ov99_021E8C50(void *arg0);

ManagedSprite *ov98_0221E6E0(void *, s32 *);
extern s32 ov99_021EA3F4;
void ov99_021E8D5C(struct PokeathlonEventRecord *arg0);

void ov99_021E8C50(void *arg0) {
    s32 flag;

    if (*((s32 *)&((PokeathlonEventRecord *)arg0)->linkMode) != 0) {
        flag = 0;
    } else if (*((s32 *)&((PokeathlonEventRecord *)arg0)->noRecord[(((PokeathlonEventRecord *)arg0)->cursor)]) == 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    ov99_021E8F50(arg0);
    ManagedSprite_SetDrawFlag(((PokeathlonEventRecord *)arg0)->sprites[1], flag);
}

SpriteSystem *ov98_0221E5C0(void *);

SpriteManager *ov98_0221E5D0(void *);

s32 ov98_0221E8A8(void *, s32 *, s32, s32, s32);

extern s32 ov99_021EA364;

void ov99_021E8C88(struct PokeathlonEventRecord *arg0);

void ov99_021E8C88(PokeathlonEventRecord *arg0) {
    SpriteManager *spriteManager;
    SpriteSystem *spriteSystem;

    spriteSystem = ov98_0221E5C0(*((void **)&arg0->spriteContext));
    spriteManager = ov98_0221E5D0(*((void **)&arg0->spriteContext));
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 1, 1, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0, 0, 1, 1, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 2, 1, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 3, 1, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xC, 0, 1, 2, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xD, 1, 2, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xE, 1, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, arg0->narc, 0xF, 1, 1);
    ov98_0221E8A8(*((void **)&arg0->spriteContext), &ov99_021EA364, 0xF, 0, 1);
}

void ov99_021E8D5C(PokeathlonEventRecord *arg0) {
    s32 *v0;
    ManagedSprite *managedSprite;
    s32 v2;

    v0 = &ov99_021EA3F4;
    v2 = 0;
    do {
        managedSprite = ov98_0221E6E0(*((void **)&arg0->spriteContext), v0);
        arg0->sprites[v2] = managedSprite;
        ManagedSprite_SetAnimateFlag(managedSprite, 1);
        v0 += 13;
        v2 += 1;
    } while (v2 < 2);
}
