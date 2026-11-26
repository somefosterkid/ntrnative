#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "constants/heap.h"

struct BgConfig;
struct CharTransferAllocation;
struct DressUpPhoto;
struct TextOBJ;
struct G2dRenderer;
struct NARC;
struct Options;
struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokemonSpriteShadow;
struct PokepicTemplate;
struct PokemonSpriteTransforms;
struct GF_2DGfxRawResMan;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct Sprite;
struct SpriteAnimFrame;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct MessageFormat;
struct SysTask;
struct SysTaskManager;
struct TouchHitboxController;
struct PlayerProfile;
struct UnkStruct_02012744_t;
struct UnkStruct_02029C88_t;
struct UnkStruct_ov22_02258A48_t;
struct UnkStruct_ov22_02259C58_tag;
struct Window;
struct YesNoPrompt;
typedef struct String String;
typedef struct UnkStruct_02029C88_t UnkStruct_02029C88;
typedef struct Options Options;
typedef struct Pokemon Pokemon;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct NARC NARC;
typedef struct Pokepic Pokepic;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);
typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;
struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};
typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;
struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};
typedef struct PokepicManager PokepicManager;
typedef struct SysTaskManager SysTaskManager;
struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};
typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;
typedef struct SpriteList SpriteList;
typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;
struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};
typedef struct GF_2DGfxRawResMan GF_2DGfxRawResMan;
typedef struct SpriteResource SpriteResource;
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct PlayerProfile PlayerProfile;
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
typedef struct {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
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
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;
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
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_020298D8 unk_4C;
} UnkStruct_ov22_02257964_sub2;
typedef struct {
    UnkStruct_ov22_02257964_sub1 unk_00;
    UnkStruct_ov22_02257964_sub2 unk_2C;
} UnkStruct_ov22_02257964;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_ov22_02257F50_sub1_sub1;
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
    int unk_00;
    UnkStruct_ov22_0225899C *unk_04;
    UnkStruct_ov22_02259484 *unk_08;
    UnkStruct_ov22_02257F50_sub1_sub1 unk_0C[4];
    int unk_3C;
    int unk_40;
} UnkStruct_ov22_02257F50_sub1;
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
BOOL ov41_02248750(UnkStruct_ov22_02257F50 *param0, int param1, int param2);
BOOL ov41_02248998(UnkStruct_ov22_02257F50 *param0);
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
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_0225AD68;
typedef struct {
    BOOL unk_00;
    Sprite *unk_04[2];
    VecFx32 unk_0C[2];
    UnkStruct_ov22_0225AD68 unk_24;
    UnkStruct_ov22_0225AD68 unk_38;
    UnkStruct_ov22_0225AD68 unk_4C;
} UnkStruct_ov22_0225AE9C;
typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[2];
    Window *unk_18;
    int unk_1C;
    int unk_20;
    SysTask *unk_24;
    SysTask *unk_28;
    UnkStruct_02095C60 *unk_2C;
    UnkStruct_ov22_0225AE9C unk_30;
    BOOL unk_90;
} UnkStruct_ov22_0225AB54;
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
typedef struct UnkStruct_02012744_t UnkStruct_02012744;
typedef struct TextOBJ TextOBJ;
typedef struct {
    UnkStruct_ov22_02259C58 unk_00;
    TextOBJ *unk_10;
    CharTransferAllocation unk_14;
} UnkStruct_ov22_0225A154;
typedef struct TouchHitboxController TouchHitboxController;
typedef struct {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    UnkStruct_02012744 *unk_64;
    TouchHitboxController *unk_68;
    TouchscreenHitbox unk_6C[5];
} UnkStruct_ov22_02259D2C;
void ov41_0224A5A4(UnkStruct_ov22_02259D2C *param0, int param1, int param2);
typedef struct {
    const UnkStruct_ov22_02257964 *unk_00;
    UnkStruct_ov22_02259820 unk_04;
} UnkStruct_ov22_0225AF68;
void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, int planeMask, u32 screenSelect);
BOOL IsBrightnessTransitionActive(u32 screenSelect);
typedef struct MessageFormat MessageFormat;
void DestroySysTaskAndEnvironment(SysTask *task);
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
typedef struct {
    UnkStruct_ov22_02255D44 *unk_00;
    BOOL *unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_ov22_02256C48;
void ov41_02247414(UnkStruct_ov22_02255D44 *param0);
void ov41_02247480(UnkStruct_ov22_02255D44 *param0, int param1);
void ov41_02247588(UnkStruct_ov22_02255D44 *param0);
BOOL ov41_02247A48(UnkStruct_ov22_02256C48 *param0, int param1, int param2, int param3);
void ov41_02247B5C(UnkStruct_ov22_02255D44 *param0);
void ov22_02256FD8(UnkStruct_02029C88 *param0, UnkStruct_ov22_02257964 *param1, enum PokemonContestRank contestRank, const PlayerProfile *trainerInfo);
void String_Delete(String *string);
typedef struct DressUpPhoto DressUpPhoto;
String *PlayerProfile_GetPlayerName_NewString(const PlayerProfile *info, enum HeapID heapID);
u32 PlayerProfile_GetTrainerGender(const PlayerProfile *info);
void sub_0202BC38(DressUpPhoto *photo);
void sub_0202BC60(DressUpPhoto *photo);
void sub_0202BC88(DressUpPhoto *photo, Pokemon *mon, UnkStruct_020298D8 *param2);
void sub_0202BCAC(DressUpPhoto *photo, const UnkStruct_ov22_02255040 *param1, int param2);
#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_0202BD60(DressUpPhoto *photo, s8 param1);
#else
void sub_0202BD60(DressUpPhoto *photo, u8 param1);
#endif
void sub_0202BDC8(DressUpPhoto *photo, const String *name, int gender);
void ov41_022480F8(UnkStruct_ov22_02257964 *param0, int param1, int param2);

void ov41_02247850(SysTask *param0, void *param1);
void ov41_022479A8(DressUpPhoto *photo, UnkStruct_ov22_02257964 *param1, const PlayerProfile *info);
BOOL ov41_02247A48(UnkStruct_ov22_02256C48 *param0, int param1, int param2, int param3);

void ov41_02247850(SysTask *param0, void *param1) {
    UnkStruct_ov22_02256C48 *v0 = param1;
    BOOL v1;

    switch (v0->unk_0C) {
    case 0:
        StartBrightnessTransition(8, -16, 0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3, (1 << SCREEN_MAIN));
        v0->unk_0C++;
        break;
    case 1:
        v1 = IsBrightnessTransitionActive(1 << SCREEN_MAIN);

        if (v1) {
            v0->unk_0C++;
        }
        break;
    case 2:
        ov41_02247B5C(v0->unk_00);
        ov41_02247414(v0->unk_00);
        ov41_02247588(v0->unk_00);
        ov41_02247480(v0->unk_00, 0);
        ScheduleSetBgPosText(v0->unk_00->unk_00.unk_40, 1, BG_POS_OP_SET_Y, -(5 * 8));
        v0->unk_0C++;
        break;
    case 3:
        if (ov41_02247A48(v0, 8, -5, 8)) {
            v0->unk_08 = 0;
            v0->unk_0C++;
        }
        break;
    case 4:
        ov41_0224A5A4(&v0->unk_00->unk_544, 0, -8);
        v0->unk_08++;

        if (v0->unk_08 >= 8) {
            v0->unk_08 = 0;
            v0->unk_0C++;
        }
        break;
    case 5:
        StartBrightnessTransition(8, 0, -16, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG3, (1 << SCREEN_MAIN));
        v0->unk_0C++;
        break;
    case 6:
        v1 = IsBrightnessTransitionActive(1 << SCREEN_MAIN);

        if (v1) {
            v0->unk_0C++;
        }
        break;
    case 7:
        v1 = ov41_02248750(&v0->unk_00->unk_3CC, 0, 0);
        ((v1) ? (void)0 : GF_AssertFail());
        v0->unk_0C++;
        break;
    case 8:
        v1 = ov41_02248998(&v0->unk_00->unk_3CC);

        if (v1) {
            v0->unk_0C++;
        }
        break;
    case 9:
        *v0->unk_04 = 1;
        DestroySysTaskAndEnvironment(param0);
        break;
    default:
        break;
    }
}

void ov41_022479A8(DressUpPhoto *photo, UnkStruct_ov22_02257964 *param1, const PlayerProfile *info) {
    UnkStruct_ov22_02259560 *v0;
    int v1;
    String *name;
    int gender;

    sub_0202BC60(photo);
    sub_0202BC88(photo, param1->unk_2C.unk_4C.unk_0C, &param1->unk_2C.unk_4C);

    if (info != 0L) {
        name = PlayerProfile_GetPlayerName_NewString(info, HEAP_ID_13);
        gender = PlayerProfile_GetTrainerGender(info);
        sub_0202BDC8(photo, name, gender);
        String_Delete(name);
    }

    v1 = 0;
    v0 = param1->unk_00.unk_14.unk_08;

    while (v0 != &param1->unk_00.unk_14) {
        if (v0->unk_04 == 0) {
            sub_0202BCAC(photo, v0->unk_00, v1);
            v1++;
        }

        v0 = v0->unk_08;
    }

    v0 = param1->unk_00.unk_04.unk_08;

    while (v0 != &param1->unk_00.unk_04) {
        if (v0->unk_04 == 0) {
            sub_0202BCAC(photo, v0->unk_00, v1);
            v1++;
        }

        v0 = v0->unk_08;
    }

    sub_0202BD60(photo, param1->unk_2C.unk_48);
    sub_0202BC38(photo);
}

BOOL ov41_02247A48(UnkStruct_ov22_02256C48 *param0, int param1, int param2, int param3) {
    if (param0->unk_08 < 8) {
        ov41_022480F8(&param0->unk_00->unk_458, param1, param2);
    }

    if (param0->unk_08 >= 1) {
        ScheduleSetBgPosText(param0->unk_00->unk_00.unk_40, 2, BG_POS_OP_SUB_X, param1);
        ScheduleSetBgPosText(param0->unk_00->unk_00.unk_40, 1, BG_POS_OP_SUB_X, param1);
        ScheduleSetBgPosText(param0->unk_00->unk_00.unk_40, 2, BG_POS_OP_SUB_Y, param2);
        ScheduleSetBgPosText(param0->unk_00->unk_00.unk_40, 1, BG_POS_OP_SUB_Y, param2);
    }

    param0->unk_08++;

    if (param0->unk_08 > param3) {
        return 1;
    }

    return 0;
}
