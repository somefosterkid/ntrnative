#include "global.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"

struct ManagedSprite;

struct Sprite;

struct SpriteManager;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct ManagedSpriteTemplate;

typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void Sprite_TickFrame(Sprite *sprite);

extern const ManagedSpriteTemplate ov80_0223DA54[4];

ManagedSprite *ov80_02239BB8(SpriteSystem *param0, SpriteManager *param1, u16 param2);

ManagedSprite *ov80_02239BB8(SpriteSystem *param0, SpriteManager *param1, u16 param2) {
    ManagedSprite *v0;

    GF_ASSERT(param2 < NELEMS(ov80_0223DA54));

    v0 = SpriteSystem_NewSprite(param0, param1, &ov80_0223DA54[param2]);
    Sprite_TickFrame(v0->sprite);
    return v0;
}
