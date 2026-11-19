#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct BgConfig;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct UnkStruct_ov22_02259560_t;
typedef struct UnkStruct_ov22_02259560_t UnkStruct_ov22_02259560;
struct UnkStruct_ov22_02259560_t {
    void *unk_00;
    int unk_04;
    struct UnkStruct_ov22_02259560_t *unk_08;
    struct UnkStruct_ov22_02259560_t *unk_0C;
};
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov22_02257F50_sub1_sub1;
void *Heap_Alloc(u32 heapID, u32 size);
void ov41_022489E4(UnkStruct_ov22_02259560 *param0, int param1);
void Heap_Free(void *ptr);
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
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
typedef struct {
    int unk_00;
    UnkStruct_ov22_0225899C *unk_04;
    UnkStruct_ov22_02259484 *unk_08;
    UnkStruct_ov22_02257F50_sub1_sub1 unk_0C[4];
    int unk_3C;
    int unk_40;
} UnkStruct_ov22_02257F50_sub1;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
typedef struct {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;
typedef struct BgConfig BgConfig;
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    BgConfig *unk_10;
    UnkStruct_ov22_02254DE0 *unk_14;
    UnkStruct_ov22_022597BC unk_18;
    UnkStruct_ov22_02254DE0 *unk_44;
} UnkStruct_ov22_02257F50_sub2;
typedef struct {
    UnkStruct_ov22_02257F50_sub1 unk_00;
    UnkStruct_ov22_02257F50_sub2 unk_44;
} UnkStruct_ov22_02257F50;

void ov41_02248A28(UnkStruct_ov22_02257F50_sub1_sub1 *param0, int param1);
void ov41_02248A6C(UnkStruct_ov22_02257F50_sub1_sub1 *param0);
UnkStruct_ov22_02259560 *ov41_02248A94(UnkStruct_ov22_02257F50 *param0);
UnkStruct_ov22_02259560 *ov41_02248ABC(UnkStruct_ov22_02257F50 *param0, int param1, int param2);

void ov41_02248A28(UnkStruct_ov22_02257F50_sub1_sub1 *param0, int param1) {
    int v0;

    param0->unk_00 = Heap_Alloc(HEAP_ID_14, sizeof(UnkStruct_ov22_02259560) * param1);
    param0->unk_04 = param1;
    param0->unk_08 = 0;

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        param0->unk_00[v0].unk_08 = param0->unk_00 + v0;
        param0->unk_00[v0].unk_0C = param0->unk_00 + v0;

        ov41_022489E4(param0->unk_00 + v0, 0);
    }
}

void ov41_02248A6C(UnkStruct_ov22_02257F50_sub1_sub1 *param0) {
    Heap_Free(param0->unk_00);
    param0->unk_00 = 0L;
    memset(param0, 0, sizeof(UnkStruct_ov22_02257F50_sub1_sub1));
}

UnkStruct_ov22_02259560 *ov41_02248A94(UnkStruct_ov22_02257F50 *param0) {
    UnkStruct_ov22_02257F50_sub1_sub1 v0;

    v0 = param0->unk_00.unk_0C[param0->unk_00.unk_00];
    return &v0.unk_00[v0.unk_08];
}

UnkStruct_ov22_02259560 *ov41_02248ABC(UnkStruct_ov22_02257F50 *param0, int param1, int param2) {
    UnkStruct_ov22_02257F50_sub1_sub1 v0;

    v0 = param0->unk_00.unk_0C[param1];
    return &v0.unk_00[param2];
}
