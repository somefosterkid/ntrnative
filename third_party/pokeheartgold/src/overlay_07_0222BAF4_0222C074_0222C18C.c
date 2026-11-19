#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

typedef struct KinesisSpoon {
    ManagedSprite *sprite;
    XYTransformContext revs;
    ValueLerpContext cosWave;
    s16 revStepSizeX;
    s16 posX;
    s16 posY;
    int state;
    BOOL active;
} KinesisSpoon;

void ov07_0222C18C(KinesisSpoon *spoon, SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template, const KinesisSpoon *source);

void ov07_0222C18C(KinesisSpoon *spoon, SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template, const KinesisSpoon *source) {
    spoon->sprite = SpriteSystem_NewSprite(spriteSys, spriteMan, template);
    spoon->revs = source->revs;
    spoon->cosWave = source->cosWave;
    spoon->revStepSizeX = source->revStepSizeX;
    spoon->posX = source->posX;
    spoon->posY = source->posY;
    spoon->state = source->state;
    spoon->active = 0;

    ManagedSprite_SetDrawFlag(spoon->sprite, 0);
    ManagedSprite_SetDrawPriority(spoon->sprite, 100);
    ManagedSprite_SetPriority(spoon->sprite, 1);
}
