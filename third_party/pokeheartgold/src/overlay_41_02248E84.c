#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "gf_3d_loader.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

struct BgConfig;
struct FashionCase;
struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct SysTask;
struct SysTaskQueue;
struct Window;
typedef struct FashionCase FashionCase;
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
u32 sub_0202BA70(const FashionCase *fashionCase, u32 accessoryID);
u32 sub_0202BAB0(const FashionCase *fashionCase, u32 backdropID);
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
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
typedef struct UnkStruct_ov22_02258A48_t {
    void *unk_00;
    void (*unk_04)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_08)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_0C)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_10)(struct UnkStruct_ov22_02258A48_t *param0);
    s16 unk_14;
    s16 unk_16;
    u8 unk_18;
    u8 padding[3];
} UnkStruct_ov22_02258A48;
typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[20];
    int unk_60;
    int unk_64;
} UnkStruct_ov22_0225A914;
typedef struct {
    UnkStruct_ov22_022597BC unk_00;
    Window *unk_2C;
    SpriteList *unk_30;
    SpriteResourceCollection **unk_34;
    UnkStruct_ov22_0225A914 unk_38;
    UnkStruct_ov22_0225AB54 unk_A0;
    Window *unk_134;
    int unk_138;
    int unk_13C;
    u32 unk_140;
    String *unk_144;
} UnkStruct_ov22_0225A428;
void ov41_02248F6C(UnkStruct_ov22_02258A48 *param0);
void ov41_02248E28(UnkStruct_ov22_02258A48 *param0);
void ov41_02248F80(UnkStruct_ov22_02258A48 *param0);
void ov41_022490F0(UnkStruct_ov22_02258A48 *param0);
void ov41_02249280(UnkStruct_ov22_02258A48 *param0);

void ov41_02248E84(const FashionCase *fashionCase, UnkStruct_ov22_0225899C *param1);
u32 ov41_02248ED4(const UnkStruct_ov22_0225899C *param0, u32 param1);
u32 ov41_02248EE8(const UnkStruct_ov22_0225899C *param0, u32 param1);
u32 ov41_02248EF4(const UnkStruct_ov22_0225899C *param0, u32 param1);
void ov41_02248F18(UnkStruct_ov22_02258A48 *param0, UnkStruct_ov22_02257964 *param1, UnkStruct_ov22_02257F50 *param2, UnkStruct_ov22_0225A0E4 *param3, UnkStruct_ov22_0225A428 *param4, BOOL param5);

void ov41_02248E84(const FashionCase *fashionCase, UnkStruct_ov22_0225899C *param1) {
    int i;
    u32 v1;

    for (i = 0; i < ACCESSORY_COUNT; i++) {
        param1->accessoryCounts[i] = sub_0202BA70(fashionCase, i);
    }

    for (i = 0; i < BACKDROP_COUNT; i++) {
        param1->backdropCounts[i] = BACKDROP_COUNT;
    }

    for (i = 0; i < BACKDROP_COUNT; i++) {
        v1 = sub_0202BAB0(fashionCase, i);

        if (v1 != BACKDROP_COUNT) {
            param1->backdropCounts[v1] = i;
        }
    }
}

u32 ov41_02248ED4(const UnkStruct_ov22_0225899C *param0, u32 param1) {
    ((param1 < 100) ? (void)0 : GF_AssertFail());
    return param0->accessoryCounts[param1];
}

u32 ov41_02248EE8(const UnkStruct_ov22_0225899C *param0, u32 param1) {
    return param0->backdropCounts[param1];
}

u32 ov41_02248EF4(const UnkStruct_ov22_0225899C *param0, u32 param1) {
    int v0;

    for (v0 = 0; v0 < 18; v0++) {
        if (param0->backdropCounts[v0] == param1) {
            return v0;
        }
    }

    return v0;
}

void ov41_02248F18(UnkStruct_ov22_02258A48 *param0, UnkStruct_ov22_02257964 *param1, UnkStruct_ov22_02257F50 *param2, UnkStruct_ov22_0225A0E4 *param3, UnkStruct_ov22_0225A428 *param4, BOOL param5) {
    UnkStruct_ov22_02258EE4 *v0;

    ov41_02248E28(param0);

    param0->unk_00 = Heap_Alloc(HEAP_ID_13, sizeof(UnkStruct_ov22_02258EE4));
    memset(param0->unk_00, 0, sizeof(UnkStruct_ov22_02258EE4));
    v0 = param0->unk_00;

    v0->unk_04 = param1;
    v0->unk_08 = param2;
    v0->unk_00 = param3;
    v0->unk_0C = param4;
    v0->unk_2C = param5;

    param0->unk_04 = ov41_02248F80;
    param0->unk_08 = ov41_022490F0;
    param0->unk_0C = ov41_02249280;
    param0->unk_10 = ov41_02248F6C;
}
