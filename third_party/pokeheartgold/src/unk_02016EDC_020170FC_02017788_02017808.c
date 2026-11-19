#include "global.h"
#include "pokemon_types_def.h"
#include "sys_task.h"
#include "transform_data.h"
#include "pokemon_anim.h"

#include "constants/pokemon_anim.h"
#include "pokepic.h"

void sub_02017808(PokemonAnim *monAnim);

void sub_02017808(PokemonAnim *monAnim) {
    u8 index;
    int radians, amplitude;

    sub_020173C8(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_SinIdx(radians) * amplitude);
}

void sub_0201783C(PokemonAnim *monAnim);

void sub_0201783C(PokemonAnim *monAnim) {
    u8 index;
    int radians, amplitude;

    sub_020173C8(monAnim, &index, &radians, &amplitude);
    monAnim->vars[index] = FX_Whole(FX_CosIdx(radians) * amplitude);
}

void sub_02017874(PokemonAnim *monAnim);

void sub_02017874(PokemonAnim *monAnim) {
    u8 index, translationType;

    sub_02017294(monAnim, &index);
    sub_02017280(monAnim, &translationType);

    if (translationType == ANIM_TRANSLATE_X) {
        monAnim->translateX = monAnim->vars[index];
    } else if (translationType == ANIM_TRANSLATE_Y) {
        monAnim->translateY = monAnim->vars[index];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void sub_020178BC(PokemonAnim *monAnim);

void sub_020178BC(PokemonAnim *monAnim) {
    u8 index, translationType;

    sub_02017294(monAnim, &index);
    sub_02017280(monAnim, &translationType);

    if (translationType == ANIM_TRANSLATE_X) {
        monAnim->translateX += monAnim->vars[index];
    } else if (translationType == ANIM_TRANSLATE_Y) {
        monAnim->translateY += monAnim->vars[index];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void sub_0201790C(PokemonAnim *monAnim);

void sub_0201790C(PokemonAnim *monAnim) {
    int *attributePtr;
    u8 attribute;

    sub_02017280(monAnim, &attribute);

    if (attribute == ANIM_TRANSLATE_X) {
        attributePtr = &monAnim->translateX;
    } else if (attribute == ANIM_TRANSLATE_Y) {
        attributePtr = &monAnim->translateY;
    } else if (attribute == ANIM_OFFSET_X) {
        attributePtr = &monAnim->offsetX;
    } else if (attribute == ANIM_OFFSET_Y) {
        attributePtr = &monAnim->offsetY;
    } else if (attribute == ANIM_SCALE_X) {
        attributePtr = &monAnim->scaleX;
    } else if (attribute == ANIM_SCALE_Y) {
        attributePtr = &monAnim->scaleY;
    } else if (attribute == ANIM_ROTATION_Z) {
        attributePtr = &monAnim->rotationZ;
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }

    u8 index, readType;
    int value;

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
        *attributePtr = value;
    } else if (updateType == ANIM_ATTRIBUTE_ADD) {
        *attributePtr += value;
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

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

void sub_020179D4(PokemonAnim *monAnim);

void sub_020179D4(PokemonAnim *monAnim) {
    if (monAnim->flipSprite) {
        Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX - (monAnim->translateX + monAnim->offsetX));
    } else {
        Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_X_CENTER, monAnim->originalX + monAnim->translateX + monAnim->offsetX);
    }

    Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_Y_CENTER, monAnim->originalY + monAnim->translateY + monAnim->offsetY);
}

void sub_02017488(PokemonAnim *monAnim);

void sub_02017A1C(PokemonAnim *monAnim);

void sub_02017A1C(PokemonAnim *monAnim) {
    Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_SCALE_X, (1 << 8) + monAnim->scaleX);
    Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_SCALE_Y, (1 << 8) + monAnim->scaleY);
    Pokepic_SetAttr(monAnim->sprite, MON_SPRITE_ROTATION_Z, (u16)monAnim->rotationZ);

    if (monAnim->yNormalization == Y_NORMALIZATION_NEGATIVE_SCALE) {
        if (monAnim->scaleY < 0) {
            sub_02017488(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_ON) {
        if (monAnim->scaleY != 0) {
            sub_02017488(monAnim);
        }
    } else if (monAnim->yNormalization == Y_NORMALIZATION_OFF) {
        return;
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}

void sub_02017A84(PokemonAnim *monAnim);

void sub_02017A84(PokemonAnim *monAnim) {
    u8 index, attribute;

    sub_02017294(monAnim, &index);

    monAnim->scriptPtr++;
    attribute = sub_02017214(monAnim->scriptPtr);

    if (attribute == ANIM_TRANSLATE_X || attribute == ANIM_OFFSET_X) {
        monAnim->offsetX = monAnim->vars[index];
    } else if (attribute == ANIM_TRANSLATE_Y || attribute == ANIM_OFFSET_Y) {
        monAnim->offsetY = monAnim->vars[index];
    } else {
        ((0) ? (void)0 : GF_AssertFail());
    }
}
