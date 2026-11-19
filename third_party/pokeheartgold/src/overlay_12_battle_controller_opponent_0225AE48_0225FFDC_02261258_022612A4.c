#include "global.h"

#include "battle/battler_sprite_context.h"
#include "battle/battle.h"
#include "filesystem_files_def.h"

struct BattleSystem;

struct Pokepic;

struct PokepicManager;

struct PokepicTemplate;

struct PokepicDrawParam;

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

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

Pokepic *PokepicManager_CreatePokepicAt(PokepicManager *monSpriteMan, PokepicTemplate *spriteTemplate, int x, int y, int z, int polygonID, int index, PokepicAnimScript *animFrames, PokemonSpriteCallback *callback);

PokemonSpriteData *ov12_0223A99C(BattleSystem *battleSys);

u8 ov12_0223AB0C(BattleSystem *battleSys, int battler);

u8 *ov12_0223BB94(PokemonSpriteData *pokemonSpriteData, int idx);

void ov12_0223BBA8(PokemonSpriteData *pokemonSpriteData, int idx, int value);

void ov12_0223BBC0(PokemonSpriteData *pokemonSpriteData, int idx, int value);

void ov12_0223BBD8(PokemonSpriteData *pokemonSpriteData, int idx, int value);

void sub_02014540(enum NarcId narcID, int narcIdx, enum HeapID heapID, void *destPtr, u32 personality, BOOL isAnimated, int face, int species);

Pokepic *ov12_022612A4(BattleSystem *battleSys, PokepicManager *monSpriteMan, PokepicTemplate *spriteTemplate, int x, int y, int z, int yOffset, int height, int xOffset, int shadowSize, int battler, PokepicAnimScript *animFrames, PokemonSpriteCallback *callback);

Pokepic *ov12_022612A4(BattleSystem *battleSys, PokepicManager *monSpriteMan, PokepicTemplate *spriteTemplate, int x, int y, int z, int yOffset, int height, int xOffset, int shadowSize, int battler, PokepicAnimScript *animFrames, PokemonSpriteCallback *callback) {
    Pokepic *monSprite;
    u8 *tiles = ov12_0223BB94(ov12_0223A99C(battleSys), battler);
    int battlerType = ov12_0223AB0C(battleSys, battler);
    int face;

    if (battlerType & 1) {
        face = 2;
    } else {
        face = 0;
    }

    sub_02014540((enum NarcId)(spriteTemplate->narcID), spriteTemplate->charDataID, HEAP_ID_BATTLE, tiles, spriteTemplate->personality, 0, face, (int)(spriteTemplate->species));
    ov12_0223BBA8(ov12_0223A99C(battleSys), battler, spriteTemplate->narcID);
    ov12_0223BBC0(ov12_0223A99C(battleSys), battler, spriteTemplate->palDataID);
    ov12_0223BBD8(ov12_0223A99C(battleSys), battler, yOffset);

    monSprite = PokepicManager_CreatePokepicAt(monSpriteMan, spriteTemplate, x, y + yOffset, z, battler, battler, animFrames, callback);

    if (battlerType & 1) {
        if (battlerType > 1) {
            battlerType = battlerType >> 1;
        }

        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_PLTT_SLOT, battlerType);
        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_SIZE, shadowSize);
        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_Y, y + 36);
        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_X_OFFSET, xOffset);
        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_Y_OFFSET, 36 - yOffset);
        Pokepic_SetAttr(monSprite, MON_SPRITE_SHADOW_HEIGHT, height);
    }

    return monSprite;
}
