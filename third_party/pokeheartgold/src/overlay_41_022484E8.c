#include "global.h"

#include "bg_window.h"
#include "math_util.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_0225500c.h"

struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
enum Accessory {
    ACCESSORY_WHITE_FLUFF = 0,
    ACCESSORY_YELLOW_FLUFF,
    ACCESSORY_PINK_FLUFF,
    ACCESSORY_BROWN_FLUFF,
    ACCESSORY_BLACK_FLUFF,
    ACCESSORY_ORANGE_FLUFF,
    ACCESSORY_ROUND_PEBBLE,
    ACCESSORY_GLITTER_BOULDER,
    ACCESSORY_SNAGGY_PEBBLE,
    ACCESSORY_JAGGED_BOULDER,
    ACCESSORY_BLACK_PEBBLE,
    ACCESSORY_MINI_PEBBLE,
    ACCESSORY_PINK_SCALE,
    ACCESSORY_BLUE_SCALE,
    ACCESSORY_GREEN_SCALE,
    ACCESSORY_PURPLE_SCALE,
    ACCESSORY_BIG_SCALE,
    ACCESSORY_NARROW_SCALE,
    ACCESSORY_BLUE_FEATHER,
    ACCESSORY_RED_FEATHER,
    ACCESSORY_YELLOW_FEATHER,
    ACCESSORY_WHITE_FEATHER,
    ACCESSORY_BLACK_MOUSTACHE,
    ACCESSORY_WHITE_MOUSTACHE,
    ACCESSORY_BLACK_BEARD,
    ACCESSORY_WHITE_BEARD,
    ACCESSORY_SMALL_LEAF,
    ACCESSORY_BIG_LEAF,
    ACCESSORY_NARROW_LEAF,
    ACCESSORY_SHED_CLAW,
    ACCESSORY_SHED_HOWN,
    ACCESSORY_THIN_MUSHROOM,
    ACCESSORY_THICK_MUSHROOM,
    ACCESSORY_STUMP,
    ACCESSORY_PRETTY_DEWDROP,
    ACCESSORY_SNOW_CRYSTAL,
    ACCESSORY_SPARKS,
    ACCESSORY_SHIMMERING_FIRE,
    ACCESSORY_MYSTIC_FIRE,
    ACCESSORY_DETERMINATION,
    ACCESSORY_PECULIAR_SPOON,
    ACCESSORY_PUFFY_SMOKE,
    ACCESSORY_POISON_EXTRACT,
    ACCESSORY_WEALTHY_COIN,
    ACCESSORY_EERIE_THING,
    ACCESSORY_SPRING,
    ACCESSORY_SEASHELL,
    ACCESSORY_HUMMING_NOTE,
    ACCESSORY_SHINY_POWDER,
    ACCESSORY_GLITTER_POWDER,
    ACCESSORY_RED_FLOWER,
    ACCESSORY_PINK_FLOWER,
    ACCESSORY_WHITE_FLOWER,
    ACCESSORY_BLUE_FLOWER,
    ACCESSORY_ORANGE_FLOWER,
    ACCESSORY_YELLOW_FLOWER,
    ACCESSORY_GOOGLY_SPECS,
    ACCESSORY_BLACK_SPECS,
    ACCESSORY_GORGEOUS_SPECS,
    ACCESSORY_SWEET_CANDY,
    ACCESSORY_CONFETTI,
    NON_UNIQUE_ACCESSORY_COUNT,
    ACCESSORY_COLORED_PARASOL = 61,
    ACCESSORY_OLD_UMBRELLA,
    ACCESSORY_SPOTLIGHT,
    ACCESSORY_CAPE,
    ACCESSORY_STANDING_MIKE,
    ACCESSORY_SURFBOARD,
    ACCESSORY_CARPET,
    ACCESSORY_RETRO_PIPE,
    ACCESSORY_FLUFFY_BED,
    ACCESSORY_MIRROR_BALL,
    ACCESSORY_PHOTO_BOARD,
    ACCESSORY_PINK_BARRETTE,
    ACCESSORY_RED_BARRETTE,
    ACCESSORY_BLUE_BARRETTE,
    ACCESSORY_YELLOW_BARRETTE,
    ACCESSORY_GREEN_BARRETTE,
    ACCESSORY_PINK_BALLOON,
    ACCESSORY_RED_BALLOONS,
    ACCESSORY_BLUE_BALLOONS,
    ACCESSORY_YELLOW_BALLOONS,
    ACCESSORY_GREEN_BALLOONS,
    ACCESSORY_LACE_HEADDRESS,
    ACCESSORY_TOP_HAT,
    ACCESSORY_SILK_VEIL,
    ACCESSORY_HEROIC_HEADBAND,
    ACCESSORY_PROFESSOR_HAT,
    ACCESSORY_FLOWER_STAGE,
    ACCESSORY_GOLD_PEDESTAL,
    ACCESSORY_GLASS_STAGE,
    ACCESSORY_AWARD_PODIUM,
    ACCESSORY_CUBE_STAGE,
    ACCESSORY_TURTWIG_MASK,
    ACCESSORY_CHIMCHAR_MASK,
    ACCESSORY_PIPLUP_MASK,
    ACCESSORY_BIG_TREE,
    ACCESSORY_FLAG,
    ACCESSORY_CROWN,
    ACCESSORY_TIARA,
    ACCESSORY_COMET,
    ACCESSORY_COUNT,
};
enum Backdrop {
    BACKDROP_DRESS_UP = 0,
    BACKDROP_RANCH = 1,
    BACKDROP_CITY_AT_NIGHT = 2,
    BACKDROP_SNOWY_TOWN = 3,
    BACKDROP_FIERY = 4,
    BACKDROP_OUTER_SPACE = 5,
    BACKDROP_CUMULUS_CLOUD = 6,
    BACKDROP_DESERT = 7,
    BACKDROP_FLOWER_PATCH = 8,
    BACKDROP_FUTURE_ROOM = 9,
    BACKDROP_OPEN_SEA = 10,
    BACKDROP_TOTAL_DARKNESS = 11,
    BACKDROP_TATAMI_ROOM = 12,
    BACKDROP_GINGERBREAD_ROOM = 13,
    BACKDROP_SEAFLOOR = 14,
    BACKDROP_UNDERGROUND = 15,
    BACKDROP_SKY = 16,
    BACKDROP_THEATER = 17,
    BACKDROP_COUNT = 18,
};
typedef struct {
    u32 accessoryCounts[ACCESSORY_COUNT];
    u32 backdropCounts[BACKDROP_COUNT];
} UnkStruct_ov22_0225899C;
u32 ov41_02248EF4(const UnkStruct_ov22_0225899C *param0, u32 param1);
void ov41_02248B48(int param0, int *param1, int *param2);
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
int ov41_022484E8(int param0, int param1, UnkStruct_ov22_0225899C *param2);
static void ov41_02248584(int param0, int param1, int *param2, int *param3, int param4, int param5, UnkStruct_ov22_0225899C *param6);
UnkStruct_ov22_02255040 *ov41_02245EE0(UnkStruct_ov22_0225500C *param0);
UnkStruct_ov22_02259560 *ov41_022499F0(UnkStruct_ov22_02259484 *param0, void *param1, int param2);
void ov41_02249A50(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_02259560 *param1);
void ov41_02249AF4(UnkStruct_ov22_02259560 *param0, int param1, int param2);
void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_02248B20(UnkStruct_ov22_02257F50 *param0, UnkStruct_ov22_02255040 *param1, int param2, int param3);

int ov41_022484E8(int param0, int param1, UnkStruct_ov22_0225899C *param2);
static void ov41_02248584(int param0, int param1, int *param2, int *param3, int param4, int param5, UnkStruct_ov22_0225899C *param6);
void ov41_022485DC(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

int ov41_022484E8(int param0, int param1, UnkStruct_ov22_0225899C *param2) {
    int v0;

    switch (param0) {
    case 0:
        if (param1 <= 5) {
            v0 = 0;
        } else if (param1 <= 11) {
            v0 = 1;
        } else if (param1 <= 17) {
            v0 = 2;
        } else if (param1 <= 21) {
            v0 = 3;
        } else if (param1 <= 28) {
            v0 = 4;
        } else if (param1 <= 33) {
            v0 = 5;
        } else if (param1 <= 38) {
            v0 = 6;
        } else if (param1 <= 42) {
            v0 = 7;
        } else if (param1 <= 49) {
            v0 = 8;
        } else if (param1 <= 55) {
            v0 = 9;
        } else if (param1 <= 60) {
            v0 = 10;
        } else if (param1 <= 71) {
            v0 = 11;
        } else if (param1 <= 91) {
            v0 = 12;
        } else if (param1 <= 99) {
            v0 = 13;
        }
        break;
    case 1:
        param1 = ov41_02248EF4(param2, param1);
        v0 = param1 / 9;
        break;
    case 2:
        param1 = ov41_02248EF4(param2, param1);
        v0 = param1 / 9;
        break;
    }

    return v0;
}

static void ov41_02248584(int param0, int param1, int *param2, int *param3, int param4, int param5, UnkStruct_ov22_0225899C *param6) {
    switch (param0) {
    case 0:
        *param2 = (8 + 2);
        *param3 = (16 + 2);
        *param2 += (MTRandom() % ((112 - (2 * 2)) - param4));
        *param3 += (MTRandom() % ((129 - (2 * 2)) - param5));
        break;
    case 1:
    case 2:
        param1 = ov41_02248EF4(param6, param1);
        ov41_02248B48(param1, param2, param3);
        break;
    }
}

void ov41_022485DC(UnkStruct_ov22_02257F50 *param0, int param1, int param2) {
    UnkStruct_ov22_0225500C v0;
    UnkStruct_ov22_02255040 *v1;
    UnkStruct_ov22_02259560 *v2;
    int v3;
    int v4, v5;
    int v6, v7;
    int v8, v9;

    v0.unk_00 = param0->unk_44.unk_14;
    v0.unk_18 = param2;
    v0.unk_04 = param0->unk_44.unk_00;
    v0.unk_10 = 0;
    v0.unk_14 = 0;

    switch (param1) {
    case 0:
        v4 = param2;
        v5 = 0;
        v0.unk_1C = param0->unk_44.unk_0C[param2];
        break;
    case 1:
        v4 = param2 + 100;
        v5 = param2 + 1;
        v0.unk_1C = 0;
        break;
    case 2:
        v4 = param2 + 100;
        v5 = param2 + 1;
        v0.unk_1C = 0;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    v0.unk_08 = param0->unk_44.unk_04[v4];
    v0.unk_0C = param0->unk_44.unk_08[v5];

    ((v0.unk_08) ? (void)0 : GF_AssertFail());
    ((v0.unk_0C) ? (void)0 : GF_AssertFail());

    v3 = ov41_022484E8(param1, param2, param0->unk_00.unk_04);
    v1 = ov41_02245EE0(&v0);
    v2 = ov41_022499F0(param0->unk_00.unk_08, v1, param1);

    ov41_02249A50(v2, param0->unk_00.unk_0C[param1].unk_00[v3].unk_0C);
    ov41_02249B94(v2, &v8, &v9);
    ov41_02248584(param1, param2, &v6, &v7, v8, v9, param0->unk_00.unk_04);
    ov41_02249AF4(v2, v6, v7);
    ov41_02248B20(param0, v1, param1, v3);
}
