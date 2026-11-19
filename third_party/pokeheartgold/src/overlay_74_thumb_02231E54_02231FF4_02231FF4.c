#include "global.h"
#include "sprite_resources_header.h"
#include "filesystem_files_def.h"
#include "constants/heap.h"

struct BgConfig;

struct G2dRenderer;

struct GBABoxPokemon;

struct GBAPokemonSubstruct0;

struct GBAPokemonSubstruct1;

struct GBAPokemonSubstruct2;

struct GBAPokemonSubstruct3;

struct NARC;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct System;

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

typedef struct SaveData SaveData;

typedef struct Options Options;

typedef struct String String;

typedef struct NARC NARC;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID, s32);

void NARC_Delete(NARC *narc);

typedef u32 TextColor;

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

void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

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

typedef struct PlayerProfile PlayerProfile;

typedef struct MessageFormat MessageFormat;

void *Heap_AllocAtEnd(u32 heapID, u32 size);

void Heap_Free(void *ptr);

typedef void (*Callback)(void *);

typedef void (*UnkCallback)(void);

typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskManager *mainTaskQueue;
    SysTaskManager *vblankTaskQueue;
    SysTaskManager *vwaitTaskQueue;
    SysTaskManager *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;

extern System gSystem;

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

void ov74_02232678(GBAMigrator *migrator, int boxNum);

void ov74_0223262C(GBAMigrator *migrator);

int ov74_02231D70(GBAMigrator *migrator, int boxId, int boxPosition);

int ov74_02231D94(GBAMigrator *migrator, int boxId, int boxPosition);

int ov74_02231DB8(GBAMigrator *migrator, int boxId, int boxPosition);

void *ov74_02231E00(u32 narcID, u32 memberIndex, NNSG2dCharacterData **param2, enum HeapID heapID);

u8 ov74_02231E54(int speciesNDS, u32 personality, int gbaVersion);

void ov74_02231F30(int speciesGBA, int isEgg, int form, int param3, Sprite *monIconSprite, void *param5, NARC *param6);

void ov74_02231FB0(void);

void ov74_02232910(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov74_02232940(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov74_02231FF4(GBAMigrator *migrator, s32 arg1, s32 arg2);

void ov74_02231FF4(GBAMigrator *migrator, s32 arg1, s32 arg2) {
    int i, speciesGBA, isEgg, gbaVersion, form;
    u32 personality;
    void *v6;
    NARC *v7 = NARC_New((enum NarcId)20, HEAP_ID_76, arg2);
    v6 = Heap_AllocAtEnd(HEAP_ID_76, 4096);

    for (i = 0; i < (5 * 6); i++) {
        if (AGB_GetBoxMonData(&(migrator->pokemonStorage->boxes[migrator->currentBox][i]), GBA_MON_DATA_SANITY_HAS_SPECIES, 0L)) {
            speciesGBA = ov74_02231D70(migrator, migrator->currentBox, i);
            isEgg = ov74_02231D94(migrator, migrator->currentBox, i);
            personality = ov74_02231DB8(migrator, migrator->currentBox, i);
            gbaVersion = gSystem.unk6A;
            form = ov74_02231E54(TranslateAgbSpecies(speciesGBA), personality, gbaVersion);

            ov74_02231F30(speciesGBA, isEgg, form, i, migrator->unk_20C[i].monIconSprite, v6, v7);
            Sprite_SetDrawFlag(migrator->unk_20C[i].monIconSprite, 1);

            if (AGB_GetBoxMonData(&(migrator->pokemonStorage->boxes[migrator->currentBox][i]), GBA_MON_DATA_HELD_ITEM, 0L)) {
                Sprite_SetDrawFlag(migrator->unk_20C[i].heldItemSprite, 1);
            } else {
                Sprite_SetDrawFlag(migrator->unk_20C[i].heldItemSprite, 0);
            }
        } else {
            ov74_02231F30(speciesGBA, isEgg, form, i, 0L, v6, v7);
            Sprite_SetDrawFlag(migrator->unk_20C[i].monIconSprite, 0);
            Sprite_SetDrawFlag(migrator->unk_20C[i].heldItemSprite, 0);
        }
    }

    Heap_Free(v6);
    NARC_Delete(v7);

    migrator->unk_12664 = ov74_02231FB0;

    ov74_02232678(migrator, migrator->currentBox);
    ov74_0223262C(migrator);
}
