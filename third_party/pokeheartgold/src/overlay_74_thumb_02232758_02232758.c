#include "global.h"
#include "sprite_resources_header.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "string_util.h"
#include "msgdata.h"

struct BgConfig;

struct G2dRenderer;

struct GBABoxPokemon;

struct GBAPokemonSubstruct0;

struct GBAPokemonSubstruct1;

struct GBAPokemonSubstruct2;

struct GBAPokemonSubstruct3;

struct MsgData;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct PlayerProfile;

struct Window;

struct YesNoPrompt;

enum GbaPokemonAttribute {
    GBA_MON_DATA_PERSONALITY,
    GBA_MON_DATA_OT_ID,
    GBA_MON_DATA_NICKNAME,
    GBA_MON_DATA_LANGUAGE,
    GBA_MON_DATA_SANITY_IS_BAD_EGG,
    GBA_MON_DATA_SANITY_HAS_SPECIES,
    GBA_MON_DATA_SANITY_IS_EGG,
    GBA_MON_DATA_OT_NAME,
    GBA_MON_DATA_MARKINGS,
    GBA_MON_DATA_CHECKSUM,
    GBA_MON_DATA_ENCRYPT_SEPARATOR,
    GBA_MON_DATA_SPECIES,
    GBA_MON_DATA_HELD_ITEM,
    GBA_MON_DATA_MOVE1,
    GBA_MON_DATA_MOVE2,
    GBA_MON_DATA_MOVE3,
    GBA_MON_DATA_MOVE4,
    GBA_MON_DATA_PP1,
    GBA_MON_DATA_PP2,
    GBA_MON_DATA_PP3,
    GBA_MON_DATA_PP4,
    GBA_MON_DATA_PP_BONUSES,
    GBA_MON_DATA_COOL,
    GBA_MON_DATA_BEAUTY,
    GBA_MON_DATA_CUTE,
    GBA_MON_DATA_EXP,
    GBA_MON_DATA_HP_EV,
    GBA_MON_DATA_ATK_EV,
    GBA_MON_DATA_DEF_EV,
    GBA_MON_DATA_SPEED_EV,
    GBA_MON_DATA_SPATK_EV,
    GBA_MON_DATA_SPDEF_EV,
    GBA_MON_DATA_FRIENDSHIP,
    GBA_MON_DATA_SMART,
    GBA_MON_DATA_POKERUS,
    GBA_MON_DATA_MET_LOCATION,
    GBA_MON_DATA_MET_LEVEL,
    GBA_MON_DATA_MET_GAME,
    GBA_MON_DATA_POKEBALL,
    GBA_MON_DATA_HP_IV,
    GBA_MON_DATA_ATK_IV,
    GBA_MON_DATA_DEF_IV,
    GBA_MON_DATA_SPEED_IV,
    GBA_MON_DATA_SPATK_IV,
    GBA_MON_DATA_SPDEF_IV,
    GBA_MON_DATA_IS_EGG,
    GBA_MON_DATA_ABILITY_NUM,
    GBA_MON_DATA_TOUGH,
    GBA_MON_DATA_SHEEN,
    GBA_MON_DATA_OT_GENDER,
    GBA_MON_DATA_COOL_RIBBON,
    GBA_MON_DATA_BEAUTY_RIBBON,
    GBA_MON_DATA_CUTE_RIBBON,
    GBA_MON_DATA_SMART_RIBBON,
    GBA_MON_DATA_TOUGH_RIBBON,
    GBA_MON_DATA_STATUS,
    GBA_MON_DATA_LEVEL,
    GBA_MON_DATA_HP,
    GBA_MON_DATA_MAX_HP,
    GBA_MON_DATA_ATK,
    GBA_MON_DATA_DEF,
    GBA_MON_DATA_SPEED,
    GBA_MON_DATA_SPATK,
    GBA_MON_DATA_SPDEF,
    GBA_MON_DATA_MAIL,
    GBA_MON_DATA_SPECIES_OR_EGG,
    GBA_MON_DATA_IVS,
    GBA_MON_DATA_CHAMPION_RIBBON,
    GBA_MON_DATA_WINNING_RIBBON,
    GBA_MON_DATA_VICTORY_RIBBON,
    GBA_MON_DATA_ARTIST_RIBBON,
    GBA_MON_DATA_EFFORT_RIBBON,
    GBA_MON_DATA_MARINE_RIBBON,
    GBA_MON_DATA_LAND_RIBBON,
    GBA_MON_DATA_SKY_RIBBON,
    GBA_MON_DATA_COUNTRY_RIBBON,
    GBA_MON_DATA_NATIONAL_RIBBON,
    GBA_MON_DATA_EARTH_RIBBON,
    GBA_MON_DATA_WORLD_RIBBON,
    GBA_MON_DATA_UNUSED_RIBBONS,
    GBA_MON_DATA_MODERN_FATEFUL_ENCOUNTER,
    GBA_MON_DATA_KNOWN_MOVES,
    GBA_MON_DATA_RIBBON_COUNT,
    GBA_MON_DATA_RIBBONS,
    GBA_MON_DATA_ATK2,
    GBA_MON_DATA_DEF2,
    GBA_MON_DATA_SPEED2,
    GBA_MON_DATA_SPATK2,
    GBA_MON_DATA_SPDEF2,
};

typedef u16 charcode_t;

BOOL ConvertRSStringToDPStringInternational(const u8 *src, charcode_t *dst, u32 length, u32 language);

typedef struct GBAPokemonSubstruct0 {
    u16 species;
    u16 heldItem;
    u32 experience;
    u8 ppBonuses;
    u8 friendship;
    u16 padding;
} GBAPokemonSubstruct0;

typedef struct GBAPokemonSubstruct1 {
    u16 moves[4];
    u8 pp[4];
} GBAPokemonSubstruct1;

typedef struct GBAPokemonSubstruct2 {
    u8 hpEV;
    u8 attackEV;
    u8 defenseEV;
    u8 speedEV;
    u8 spAttackEV;
    u8 spDefenseEV;
    u8 cool;
    u8 beauty;
    u8 cute;
    u8 smart;
    u8 tough;
    u8 sheen;
} GBAPokemonSubstruct2;

typedef struct GBAPokemonSubstruct3 {
    u32 pokerus : 8;
    u32 metLocation : 8;
    u32 metLevel : 7;
    u32 metGame : 4;
    u32 pokeball : 4;
    u32 otGender : 1;
    u32 hpIV : 5;
    u32 attackIV : 5;
    u32 defenseIV : 5;
    u32 speedIV : 5;
    u32 spAttackIV : 5;
    u32 spDefenseIV : 5;
    u32 isEgg : 1;
    u32 abilityNum : 1;
    u32 coolRibbon : 3;
    u32 beautyRibbon : 3;
    u32 cuteRibbon : 3;
    u32 smartRibbon : 3;
    u32 toughRibbon : 3;
    u32 championRibbon : 1;
    u32 winningRibbon : 1;
    u32 victoryRibbon : 1;
    u32 artistRibbon : 1;
    u32 effortRibbon : 1;
    u32 marineRibbon : 1;
    u32 landRibbon : 1;
    u32 skyRibbon : 1;
    u32 countryRibbon : 1;
    u32 nationalRibbon : 1;
    u32 earthRibbon : 1;
    u32 worldRibbon : 1;
    u32 unusedRibbons : 4;

    u32 modernFatefulEncounter : 1;
} GBAPokemonSubstruct3;

union GBAPokemonSubstruct {
    GBAPokemonSubstruct0 type0;
    GBAPokemonSubstruct1 type1;
    GBAPokemonSubstruct2 type2;
    GBAPokemonSubstruct3 type3;
    u16 raw[(((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) > ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3))) ? ((sizeof(GBAPokemonSubstruct0)) > (sizeof(GBAPokemonSubstruct1)) ? (sizeof(GBAPokemonSubstruct0)) : (sizeof(GBAPokemonSubstruct1))) : ((sizeof(GBAPokemonSubstruct2)) > (sizeof(GBAPokemonSubstruct3)) ? (sizeof(GBAPokemonSubstruct2)) : (sizeof(GBAPokemonSubstruct3)))) / sizeof(u16)];
};

typedef struct GBABoxPokemon {
    u32 personality;
    u32 otId;
    u8 nickname[10];
    u8 language;
    u8 isBadEgg : 1;
    u8 hasSpecies : 1;
    u8 isEgg : 1;
    u8 unused : 5;
    u8 otName[7];
    u8 markings;
    u16 checksum;
    u16 unknown;
    union {
        u32 raw[12];
        union GBAPokemonSubstruct substructs[4];
    } secure;
} GBABoxPokemon;

typedef struct {
    u8 currentBox;
    GBABoxPokemon boxes[14][(5 * 6)];
    u8 boxNames[14][8 + 1];
    u8 boxWallpapers[14];
} GBAPokemonStorage;

typedef struct {
    u32 unk_00[1024];
    u8 saveBlock2[(3968 + 128) * 1];
    u8 saveBlock1[(3968 + 128) * 4];
    GBAPokemonStorage pokemonStorage;
} GBASaveSlot;

u32 AGB_GetBoxMonData(GBABoxPokemon *gbaBoxMon, int field, u8 *data);

int TranslateAgbSpecies(int species);

u32 ov74_02234A0C(GBABoxPokemon *gbaBoxMon);

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

void CopyWindowToVram(Window *window);

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

void String_Delete(String *string);

typedef u32 TextColor;

void DestroyMsgData(MsgData *loader);

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

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

u16 UpConvertItemId_Gen3to4(u16 gbaID);

void GetItemNameIntoString(String *dst, u16 item, enum HeapID heapID);

BOOL PlayCry(u16 species, u8 form);

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

typedef struct YesNoPrompt YesNoPrompt;

typedef struct {
    Window *unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int bgLayer;
    int unk_28;
    TextColor unk_2C;
    TextColor unk_30;
    int messageEntryID;
    charcode_t *unk_38;
    String *unk_3C;
    MessageFormat *unk_40;
    int unk_44;
    int unk_48;
} UnkStruct_ov97_02233DAC;

typedef struct {
    Sprite *monIconSprite;
    Sprite *heldItemSprite;
    int unk_08;
} UnkStruct_ov97_02234A2C_sub1;

typedef struct {
    Sprite *selectionSprite;
    int boxPosition;
    int boxId;
} SelectedMonData;

typedef struct {
    int unk_00;
    int unk_04;
    void *unk_08;
} UnkStruct_ov97_02233B8C;

typedef struct {
    int unk_00;
    int unk_04;
    Sprite *iconSprite;
    u8 unk_0C[512];
} UnkStruct_ov97_0223F434;

typedef struct {
    int gbaVersion;
    int canMigrateStatus;
    int messageEntryID;
    int unk_0C;
    SaveData *saveData;
    PlayerProfile *trainerInfo;
    Options *options;
    int messageBoxFrame;
    BgConfig *bgConfig;
    int unk_24;
    SpriteList *unk_28;
    G2dRenderer unk_2C;
    SpriteResourceCollection *unk_1B8[6];
    SpriteResource *unk_1D0[6];
    SpriteResourcesHeader unk_1E8;
    UnkStruct_ov97_02234A2C_sub1 unk_20C[(5 * 6)];
    TouchScreenRect pad_310[(5 * 6) + 4];
    Sprite *unk_3FC[4];
    Sprite *unk_40C[4];
    Sprite *unk_41C[4];
    SelectedMonData selectedMonData[6];
    int selectedCount;
    Sprite *selectedMonIcons[6];
    UnkStruct_ov97_02233DAC unk_490;
    Window unk_4DC;
    Window unk_4EC;
    Window unk_4FC;
    int unk_50C;
    int unk_510;
    GBASaveSlot saveSlot;
    GBAPokemonStorage *pokemonStorage;
    int currentBox;
    int fullSelectionTimer;
    YesNoPrompt *yesNoTouchMenu;
    UnkStruct_ov97_02233B8C unk_E8F0;
    UnkStruct_ov97_0223F434 unk_E8FC[(5 * 6)];
    void *unk_12664;
    String *unk_12668;
    String *unk_1266C;
} GBAMigrator;

void ov74_02231A1C(GBAMigrator *migrator, UnkStruct_ov97_02233DAC *param1, int param2);

void *ov74_02231E00(u32 narcID, u32 memberIndex, NNSG2dCharacterData **param2, enum HeapID heapID);

void ov74_02232700(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon);

void ov97_02234CC4(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov97_02234CF4(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov74_02232758(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon);

void ov74_02232758(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon) {
    int species, level;
    int gbaItemID, itemID;
    UnkStruct_ov97_02233DAC v4;
    MsgData *msgLoader;
    u8 gbaNickname[10 + 1];
    u16 dsNickname[10 + 1];
    String *string;

    memset(&v4, 0, sizeof(UnkStruct_ov97_02233DAC));

    v4.unk_00 = &migrator->unk_4EC;
    v4.unk_08 = 0;
    v4.unk_0C = 0;
    v4.unk_10 = 32;
    v4.unk_14 = 4;
    v4.bgLayer = GF_BG_LYR_MAIN_1;
    v4.unk_28 = 1;
    v4.unk_20 = (14 * 2 + 0xA0);
    v4.unk_2C = ((TextColor)(((15 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0)));
    v4.unk_38 = 0L;

    v4.messageEntryID = 44;
    v4.unk_18 = 18 * 8;
    v4.unk_1C = 0;

    ov74_02231A1C(migrator, &v4, 0x4);

    v4.messageEntryID = 43;
    v4.unk_18 = 10 * 8;
    v4.unk_1C = 8;

    ov74_02231A1C(migrator, &v4, 0x4 | 0x2);
    ov74_02232700(migrator, gbaBoxMon);

    if (gbaBoxMon == 0L) {
        CopyWindowToVram(v4.unk_00);
        return;
    }

    AGB_GetBoxMonData(gbaBoxMon, GBA_MON_DATA_NICKNAME, gbaNickname);
    ConvertRSStringToDPStringInternational(gbaNickname, dsNickname, 10 + 1, AGB_GetBoxMonData(gbaBoxMon, GBA_MON_DATA_LANGUAGE, 0L));

    v4.unk_38 = dsNickname;
    v4.messageEntryID = -1;
    v4.unk_18 = 1 * 8;
    v4.unk_1C = 0;

    ov74_02231A1C(migrator, &v4, 0x4 | 0x2);

    string = String_New(64, HEAP_ID_76);
    msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, 237, HEAP_ID_76);
    species = TranslateAgbSpecies(AGB_GetBoxMonData(gbaBoxMon, GBA_MON_DATA_SPECIES, 0L));

    ReadMsgDataIntoString(msgLoader, species, string);

    v4.unk_3C = string;
    v4.messageEntryID = -1;
    v4.unk_18 = 2 * 8;
    v4.unk_1C = 2 * 8;

    ov74_02231A1C(migrator, &v4, 0x4 | 0x2);

    DestroyMsgData(msgLoader);
    String_Delete(string);

    gbaItemID = AGB_GetBoxMonData(gbaBoxMon, GBA_MON_DATA_HELD_ITEM, 0L);

    if (gbaItemID) {
        itemID = UpConvertItemId_Gen3to4(gbaItemID);
        string = String_New(64, HEAP_ID_76);

        GetItemNameIntoString(string, itemID, HEAP_ID_76);

        v4.unk_3C = string;
        v4.messageEntryID = -1;
        v4.unk_18 = 19 * 8;
        v4.unk_1C = 2 * 8;

        ov74_02231A1C(migrator, &v4, 0x4 | 0x2);
        String_Delete(string);
    }

    level = ov74_02234A0C(gbaBoxMon);
    string = String_New(10, HEAP_ID_76);

    String16_FormatInteger(string, level, 3, PRINTING_MODE_RIGHT_ALIGN, CHARSET_MODE_EN);

    v4.unk_3C = string;
    v4.messageEntryID = -1;
    v4.unk_18 = 12 * 8 + 4;
    v4.unk_1C = 1 * 8;

    ov74_02231A1C(migrator, &v4, 0x2);

    String_Delete(string);
    PlayCry(species, 0);
}
