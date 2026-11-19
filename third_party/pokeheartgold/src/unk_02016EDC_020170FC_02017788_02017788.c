#include "global.h"
#include "overlay_74_main_menu_util.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"
#include "pokepic.h"

struct SysTask;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

void sub_02017788(PokemonAnim *monAnim);

void sub_02017788(PokemonAnim *monAnim) {
    int attribute, value;

    sub_0201726C(monAnim, &attribute);

    u8 index, readType;

    sub_02017280(monAnim, &readType);

    if (readType == ANIM_READ_TYPE_VALUE2) {
        sub_0201726C(monAnim, &value);
    } else if (readType == ANIM_READ_TYPE_VAR2) {
        sub_02017294(monAnim, &index);
        value = monAnim->vars[index];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    u8 updateType;

    sub_02017280(monAnim, &updateType);

    if (updateType == ANIM_ATTRIBUTE_SET) {
        Pokepic_SetAttr(monAnim->sprite, (enum PokemonSpriteAttribute)(attribute), value);
    } else if (updateType == ANIM_ATTRIBUTE_ADD) {
        Pokepic_AddAttr(monAnim->sprite, (enum PokemonSpriteAttribute)(attribute), value);
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}
