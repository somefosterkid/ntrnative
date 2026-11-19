#include "global.h"
#include "filesystem.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_unk_struct_ov22_0225a154.h"
#include "pokepic.h"
#include "filesystem_files_def.h"

s32 ov41_0224A27C(u8 *, void *, u8 *);

s32 ov41_0224A5D4(u8 *, s32, s32 *, u8 *, s32);

extern s32 ov41_02247598;

extern s32 ov41_022475B4;

extern s32 ov41_022475D4;

extern s32 ov41_022475F4;

extern s32 ov41_02247628;

void ov41_022474D4(u8 *arg0);

struct BgConfig;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct GF_2DGfxRawResMan;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct TouchHitboxController;
struct Window;
struct YesNoPrompt;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;
typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;
enum PokemonContestRank {
    CONTEST_RANK_NORMAL = 0,
    CONTEST_RANK_GREAT = 1,
    CONTEST_RANK_ULTRA = 2,
    CONTEST_RANK_MASTER = 3,
    CONTEST_NUM_RANKS = 3,
    CONTEST_MAX_RANKS = 4,
    CONTEST_RANK_LINK = 4,
};
enum PokemonContestType {
    CONTEST_TYPE_COOL = 0,
    CONTEST_TYPE_BEAUTY = 1,
    CONTEST_TYPE_CUTE = 2,
    CONTEST_TYPE_SMART = 3,
    CONTEST_TYPE_TOUGH = 4,
    CONTEST_TYPE_MAX = 5,
};
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
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    int unk_08;
    int unk_0C;
    SoftwareSpritePalette **unk_10;
    int unk_14;
    int unk_18;
    BOOL unk_1C;
    PokepicManager *unk_20;
    NNSGfdTexKey unk_24;
    NNSGfdPlttKey unk_28;
    BOOL unk_2C;
    u8 *unk_30;
    GF_2DGfxRawResMan *unk_34;
    NNSG2dCharacterData **unk_38;
    int unk_3C;
    BgConfig *unk_40;
    SpriteList *unk_44;
    SpriteResourceCollection *unk_48[4];
    G2dRenderer unk_58;
    NARC *unk_5C;
} UnkStruct_ov22_0225A0E4;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
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
typedef struct {
    const UnkStruct_ov22_022597BC *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov22_02259820;
typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;
typedef struct TouchHitboxController TouchHitboxController;
typedef struct {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    UnkStruct_02012744 *unk_64;
    TouchHitboxController *unk_68;
    TouchscreenHitbox unk_6C[5];
} UnkStruct_ov22_02259D2C;
void ov41_0224A3E4(UnkStruct_ov22_02259D2C *param0, UnkStruct_ov22_0225A0E4 *param1);
typedef struct {
    const UnkStruct_ov22_02257964 *unk_00;
    UnkStruct_ov22_02259820 unk_04;
} UnkStruct_ov22_0225AF68;
typedef struct YesNoPrompt YesNoPrompt;
typedef struct {
    UnkStruct_ov22_0225A0E4 unk_00;
    UnkStruct_ov22_0225899C unk_1E8;
    UnkStruct_ov22_02259484 unk_3C0;
    UnkStruct_ov22_02254DE0 *unk_3C8;
    UnkStruct_ov22_02257F50 unk_3CC;
    UnkStruct_ov22_02257964 unk_458;
    UnkStruct_ov22_0225AF68 unk_4E0;
    UnkStruct_ov22_02258A48 unk_4FC;
    UnkStruct_ov22_022597BC unk_518;
    UnkStruct_ov22_02259D2C unk_544;
    UnkStruct_ov22_0225A428 unk_5C4;
    int unk_70C;
    int unk_710;
    YesNoPrompt *unk_714;
    Window *unk_718;
    BOOL unk_71C;
    int unk_720;
    enum PokemonContestRank contestRank;
    int unk_728;
    enum PokemonContestType contestType;
    int unk_730;
    UnkStruct_02095C60 *unk_734;
    const Options *options;
    u32 unk_73C;
    String *unk_740;
    MessageFormat *unk_744;
} UnkStruct_ov22_02255D44;
void ov41_0224A54C(UnkStruct_ov22_02259D2C *param0);
void ov41_0224A580(UnkStruct_ov22_02259D2C *param0);
void ov41_02247568(UnkStruct_ov22_02255D44 *param0);
void ov41_02247578(UnkStruct_ov22_02255D44 *param0);
void ov41_02247588(UnkStruct_ov22_02255D44 *param0);

void ov41_022474D4(u8 *arg0) {
    ov41_0224A27C(arg0 + 0x4E0, arg0, arg0 + 0x6EC);
    ov41_0224A5D4(arg0 + 0x4E0, 0, &ov41_022475B4, arg0, 0);
    ov41_0224A5D4(arg0 + 0x4E0, 1, &ov41_022475D4, arg0, 0);
    ov41_0224A5D4(arg0 + 0x4E0, 2, &ov41_022475F4, arg0, 0);
    ov41_0224A5D4(arg0 + 0x4E0, 3, &ov41_02247628, arg0, 0);
    ov41_0224A5D4(arg0 + 0x4E0, 4, &ov41_02247598, arg0, 0);
}

void ov41_02247568(UnkStruct_ov22_02255D44 *param0) {
    ov41_0224A3E4(&param0->unk_544, &param0->unk_00);
}

void ov41_02247578(UnkStruct_ov22_02255D44 *param0) {
    ov41_0224A54C(&param0->unk_544);
}

void ov41_02247588(UnkStruct_ov22_02255D44 *param0) {
    ov41_0224A580(&param0->unk_544);
}
