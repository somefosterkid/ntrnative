#include "global.h"

#include "error_handling.h"

extern s32 sTypeIconGfxCharFileIds;

s32 TypeIconGfx_GetCharFileId(s32 moveType);

s32 GetMoveTypeAndCategoryIconPlttFileId(void);

enum PokemonType {
    TYPE_NORMAL = 0,
    TYPE_FIGHTING = 1,
    TYPE_FLYING = 2,
    TYPE_POISON = 3,
    TYPE_GROUND = 4,
    TYPE_ROCK = 5,
    TYPE_BUG = 6,
    TYPE_GHOST = 7,
    TYPE_STEEL = 8,
    TYPE_MYSTERY = 9,
    TYPE_FIRE = 10,
    TYPE_WATER = 11,
    TYPE_GRASS = 12,
    TYPE_ELECTRIC = 13,
    TYPE_PSYCHIC = 14,
    TYPE_ICE = 15,
    TYPE_DRAGON = 16,
    TYPE_DARK = 17,
    NUM_POKEMON_TYPES = 18,
};
extern __attribute__((aligned(4))) const u8 sTypeIconGfxPlttOverrides[23];
typedef struct WiFiPlazaQuestion {
    u32 questionNumber;
    u32 selectedAnswer;
} WiFiPlazaQuestion;
u32 GetMoveTypeAndCategoryIconCellFileId(void);
u32 GetMoveTypeAndCategoryIconAnimFileId(void);
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 TypeIconGfx_GetPlttOverride(u16 moveType);
#else
u8 TypeIconGfx_GetPlttOverride(enum PokemonType moveType);
#endif
int TypeIconGfx_GetNarcId(void);

s32 TypeIconGfx_GetCharFileId(s32 moveType) {
    if ((u32)moveType >= 0x17U) {
        GF_AssertFail();
    }
    return *(s32 *)(&sTypeIconGfxCharFileIds + moveType);
}

s32 GetMoveTypeAndCategoryIconPlttFileId(void) {
    return 0x4A;
}

u32 GetMoveTypeAndCategoryIconCellFileId(void) {
    return 242;
}

u32 GetMoveTypeAndCategoryIconAnimFileId(void) {
    return 243;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 TypeIconGfx_GetPlttOverride(u16 moveType) {
#else
u8 TypeIconGfx_GetPlttOverride(enum PokemonType moveType) {
#endif
    ((moveType < (sizeof(sTypeIconGfxPlttOverrides) / sizeof(*(sTypeIconGfxPlttOverrides)))) ? (void)0 : GF_AssertFail());
    return sTypeIconGfxPlttOverrides[moveType];
}

int TypeIconGfx_GetNarcId(void) {
    return sizeof(WiFiPlazaQuestion);
}
