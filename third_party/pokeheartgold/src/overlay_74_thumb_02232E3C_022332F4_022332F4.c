#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_41_sprite_resource_collection.h"
#include "g2d_renderer.h"
#include "sys_task.h"
#include "constants/heap.h"
#include "screen_fade.h"
#include "save.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "screen_fade_internal.h"
#include "touch_hitbox_controller.h"
#include "gf_gfx_planes.h"
#include "struct_defs/resdat_narc_structs.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct Background;

struct BgConfig;

struct G2dRenderer;

struct GBABoxPokemon;

struct GBAPokemonSubstruct0;

struct GBAPokemonSubstruct1;

struct GBAPokemonSubstruct2;

struct GBAPokemonSubstruct3;

struct Options;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskManager;

struct TouchHitboxController;

struct TouchScreenHitTableActionStateInternal;

struct PlayerProfile;

struct Window;

struct YesNoButtonAnimData;

struct YesNoButtonSprite;

struct YesNoPrompt;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_GetData(OverlayManager *appMan, int *);

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

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    fx32 xScale;
    fx32 yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

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

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

void Save_Cancel(SaveData *saveData);

typedef u32 TextColor;

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

typedef struct PlayerProfile {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
} PlayerProfile;

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef struct MessageFormat {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
} MessageFormat;

int PmAgbCartridgeGetOffsets(GBASaveSlot *saveSlot);

void ResetLoadedGBACartInfo(void);

int ov97_022360D8(void);

GBAPokemonStorage *ov74_02233F4C(void);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

void *WaitingIcon_New(Window *window, u32 baseTile);

void sub_0200F450(void *taskData);

void Main_SetVBlankIntrCB(Callback cb, void *data);

BOOL IsPaletteFadeFinished(void);

BOOL PlaySE(u16 seqID);

extern int TouchScreen_CheckRectanglePressed(const TouchscreenHitbox rects[]);

void sub_0209A8E0(enum HeapID heapID);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

enum YesNoTouchButton {
    BUTTON_YES = 0,
    BUTTON_NO,
    BUTTON_COUNT,
    BUTTON_NONE,
};

typedef struct YesNoButtonAnimData {
    const fx32 *frameThresholds;
    u32 frameCount;
    fx32 frameProgress;
} YesNoButtonAnimData;

typedef struct YesNoButtonSprite {
    BgConfig *bgConfig;
    enum GFBgLayer bgLayer;
    void *nscrBuffers[4];
    NNSG2dScreenData *screenData[4];
    YesNoButtonAnimData animData;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 animFrame;
} YesNoButtonSprite;

typedef struct YesNoPrompt {
    TouchHitboxController *buttonAction;
    TouchscreenHitbox hitTable[BUTTON_COUNT];
    YesNoButtonSprite buttonSprites[BUTTON_COUNT];
    BgConfig *bgConfig;
    u32 bgLayer;
    enum HeapID heapID;
    fx32 frameThresholds[4];
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 pressedButton;
    u8 paramsApplied : 4;
    u8 buttonState : 4;
} YesNoPrompt;

u32 YesNoPrompt_HandleInput(YesNoPrompt *menu);

void YesNoPrompt_Reset(YesNoPrompt *menu);

enum GBAMigratorState {
    GBA_MIGRATOR_STATE_0 = 0,
    GBA_MIGRATOR_STATE_1,
    GBA_MIGRATOR_STATE_2,
    GBA_MIGRATOR_STATE_3,
    GBA_MIGRATOR_STATE_4,
    GBA_MIGRATOR_STATE_5,
    GBA_MIGRATOR_STATE_6,
    GBA_MIGRATOR_STATE_7,
    GBA_MIGRATOR_STATE_8,
    GBA_MIGRATOR_STATE_9,
    GBA_MIGRATOR_STATE_10,
    GBA_MIGRATOR_STATE_11,
    GBA_MIGRATOR_STATE_12,
    GBA_MIGRATOR_STATE_13,
    GBA_MIGRATOR_STATE_14,
    GBA_MIGRATOR_STATE_WAIT_FOR_FULL_SELECTION_TIMER,
    GBA_MIGRATOR_STATE_16,
    GBA_MIGRATOR_STATE_17,
    GBA_MIGRATOR_STATE_18,
    GBA_MIGRATOR_STATE_19,
    GBA_MIGRATOR_STATE_20,
    GBA_MIGRATOR_STATE_21,
    GBA_MIGRATOR_STATE_22,
    GBA_MIGRATOR_STATE_FADE_OUT_TO_SELECTED_MON,
    GBA_MIGRATOR_STATE_24,
};

enum GBAMonState {
    GBA_MON_STATE_NONE,
    GBA_MON_STATE_SELECTED,
    GBA_MON_STATE_DESELECTED,
    GBA_MON_STATE_EMPTY_SLOT,
    GBA_MON_STATE_IS_EGG,
    GBA_MON_STATE_HAS_HM,
    GBA_MON_STATE_HAS_INVALID_ITEM,
    GBA_MON_STATE_IS_INVALID_SPECIES,
};

enum CannotMigrateMsg {
    CAN_MIGRATE = 0,
    CANNOT_MIGRATE_GBA_PAK_READ_ERROR,
    CANNOT_MIGRATE_WAIT_FOR_FULL_DAY,
    CANNOT_MIGRATE_DIFFERENT_CONSOLE,
    CANNOT_MIGRATE_INTERNAL_CLOCK_ALTERED,
    CANNOT_MIGRATE_LESS_THAN_6_IN_GBA_BOXES,
    CANNOT_MIGRATE_ALREADY_STOCKED,
    CANNOT_MIGRATE_CLOCK_ADJUSTED,
};

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
    TouchscreenHitbox pad_310[(5 * 6) + 4];
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

void ov97_02235310(GBAMigrator *migrator);

extern int ov74_0223C968[6];

void ov97_02233B44(GBAMigrator *migrator);

int ov97_02233B8C(GBAMigrator *migrator);

void ov97_02233CE4(GBAMigrator *migrator);

void ov97_02233DD0(GBAMigrator *migrator, UnkStruct_ov97_02233DAC *param1, int param2);

void ov97_02233F74(void);

void ov97_02233FA4(GBAMigrator *migrator);

void ov74_02231CFC(GBAMigrator *migrator);

void *ov74_02231E00(u32 narcID, u32 memberIndex, NNSG2dCharacterData **param2, enum HeapID heapID);

void ov97_022343A8(GBAMigrator *migrator);

void InitBoxPokemonSprites(GBAMigrator *migrator);

void ov97_0223468C(GBAMigrator *migrator);

int GBAMigrator_TrySelectGBABoxMon(GBAMigrator *migrator, int boxPosition);

void PrintGBABoxMonInfo(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon);

void ov97_02234CC4(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov97_02234CF4(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov97_02234DFC(GBAMigrator *migrator);

void ov97_02234E7C(GBAMigrator *migrator);

void ov97_02234F88(GBAMigrator *migrator);

void ov97_02235158(Window *window);

void ResetChosenPokemonData(GBAMigrator *migrator);

void ov97_022351F0(GBAMigrator *migrator);

void ov97_02235344(GBAMigrator *migrator);

void ov74_02233024(void *param0);

int GetCanMigrateStatus(GBAMigrator *migrator);

BOOL ov97_022354C4(GBAMigrator *migrator, int canMigrateStatus);

BOOL ov97_02235528(GBAMigrator *migrator, int cannotMigrateID);

BOOL ov97_02235590(GBAMigrator *migrator, int param1);

extern int _02111864;

int ov74_022332F4(OverlayManager *appMan, int *state);

void ShowGBACartRemovedError(enum HeapID heapID);

void PmAgbCartridgeUnlinkSpec(void);

int ov74_02233CE4(void);

void ov74_02231790(GBAMigrator *migrator);

void ov74_02232A48(GBAMigrator *migrator);

void ov74_02231BC0(void);

void ov74_02231BF0(GBAMigrator *migrator);

void ov74_022322D8(GBAMigrator *migrator);

void ov74_02232910(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

void ov74_02232F9C(GBAMigrator *migrator);

void ov74_02232940(GBAMigrator *migrator, enum FadeType fadeType, int param2, int *state);

BOOL ov74_0223319C(GBAMigrator *migrator, int param1);

void ov74_02232F5C(GBAMigrator *migrator);

int ov74_02233060(GBAMigrator *migrator);

BOOL ov74_02233134(GBAMigrator *migrator, int cannotMigrateID);

void ov74_02231A1C(GBAMigrator *migrator, UnkStruct_ov97_02233DAC *param1, int param2);

void ov74_02231930(GBAMigrator *migrator);

BOOL ov74_022330D0(GBAMigrator *migrator, int canMigrateStatus);

void ov74_02232DA4(Window *window);

void ov74_02232AC8(GBAMigrator *migrator);

void ov74_02232154(GBAMigrator *migrator);

void ov74_02231FF4(GBAMigrator *migrator);

void ov74_02232758(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon);

int ov74_022324A0(GBAMigrator *migrator, int boxPosition);

void ov74_02232BD4(GBAMigrator *migrator);

void ov74_02232DC4(GBAMigrator *migrator);

int ov74_022317D8(GBAMigrator *migrator);

void ov74_02232E3C(GBAMigrator *migrator);

int ov74_022332F4(OverlayManager *appMan, int *state) {
    int boxPos, gbaMonValidity, v2;
    GBAMigrator *migrator = OverlayManager_GetData(appMan, state);

    CTRDG_IsExisting();

    if ((CTRDG_IsPulledOut() == 1) || _02111864) {
        UnkStruct_ov97_02233B8C *v4 = &migrator->unk_E8F0;

        if ((v4->unk_00 == 3) || (v4->unk_00 == 4)) {
            Save_Cancel(migrator->saveData);
        }

        ShowGBACartRemovedError(HEAP_ID_76);
    }

    migrator->unk_0C++;

    switch (*state) {
    case GBA_MIGRATOR_STATE_0:
        PmAgbCartridgeUnlinkSpec();
        migrator->unk_50C = PmAgbCartridgeGetOffsets(&migrator->saveSlot);

        if (migrator->unk_50C == 0) {
            migrator->unk_510 = ov74_02233CE4();

            if (migrator->unk_510 == 0) {
                migrator->pokemonStorage = ov74_02233F4C();
                migrator->currentBox = migrator->pokemonStorage->currentBox;
            } else {
                migrator->canMigrateStatus = CANNOT_MIGRATE_GBA_PAK_READ_ERROR;
            }
        } else {
            (void)0;
        }

        ov74_02231790(migrator);
        *state = GBA_MIGRATOR_STATE_1;
        break;

    case GBA_MIGRATOR_STATE_1:
        ov74_02232A48(migrator);
        ov74_02231BC0();
        ov74_02231BF0(migrator);
        ov74_02231CFC(migrator);
        ov74_022322D8(migrator);

        Main_SetVBlankIntrCB(ov74_02233024, migrator);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 0);

        if (migrator->canMigrateStatus == CANNOT_MIGRATE_GBA_PAK_READ_ERROR) {
            ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_IN, 11, state);
        } else {
            *state = GBA_MIGRATOR_STATE_2;
        }
        break;
    case GBA_MIGRATOR_STATE_2:
        ov74_02232F9C(migrator);
        ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_IN, 3, state);
        break;
    case GBA_MIGRATOR_STATE_3:
        boxPos = YesNoPrompt_HandleInput(migrator->yesNoTouchMenu);

        switch (boxPos) {
        case 1:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
#ifdef HEARTGOLD
            migrator->messageEntryID = 34;
#else
            migrator->messageEntryID = 35;
#endif
            *state = GBA_MIGRATOR_STATE_4;
            break;
        case 2:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            ov74_02232940(migrator, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case GBA_MIGRATOR_STATE_4:
        if (ov74_0223319C(migrator, 1)) {
            ov74_02232F5C(migrator);
            *state = GBA_MIGRATOR_STATE_5;
        }
        break;
    case GBA_MIGRATOR_STATE_5:
        boxPos = YesNoPrompt_HandleInput(migrator->yesNoTouchMenu);

        switch (boxPos) {
        case 1:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);

            migrator->canMigrateStatus = ov74_02233060(migrator);

            if (migrator->canMigrateStatus != CAN_MIGRATE) {
                if ((migrator->canMigrateStatus == CANNOT_MIGRATE_DIFFERENT_CONSOLE)
                    || (migrator->canMigrateStatus == CANNOT_MIGRATE_INTERNAL_CLOCK_ALTERED)) {
                    *state = GBA_MIGRATOR_STATE_8;
                } else {
                    *state = GBA_MIGRATOR_STATE_11;
                }
            } else {
                migrator->messageEntryID = 6;
                *state = GBA_MIGRATOR_STATE_6;
            }
            break;
        case 2:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            ov74_02232940(migrator, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case GBA_MIGRATOR_STATE_6:
        if (ov74_0223319C(migrator, 1)) {
            *state = GBA_MIGRATOR_STATE_7;
        }
        break;
    case GBA_MIGRATOR_STATE_7:
        if (gSystem.touchNew || gSystem.newKeys) {
            ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_OUT, 12, state);
        }
        break;
    case GBA_MIGRATOR_STATE_8:
        if (ov74_02233134(migrator, migrator->canMigrateStatus)) {
            *state = GBA_MIGRATOR_STATE_9;
        }
        break;
    case GBA_MIGRATOR_STATE_9:
        boxPos = YesNoPrompt_HandleInput(migrator->yesNoTouchMenu);

        switch (boxPos) {
        case 1:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            migrator->unk_490.unk_44 = 0;
            migrator->unk_490.messageEntryID = 40;
            ov74_02231A1C(migrator, &migrator->unk_490, 0);
            migrator->unk_E8F0.unk_08 = WaitingIcon_New(&migrator->unk_4FC, 0x3F0 - (18 + 12));
            *state = GBA_MIGRATOR_STATE_10;
            break;
        case 2:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            ov74_02232940(migrator, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            break;
        }
        break;
    case GBA_MIGRATOR_STATE_10:
        ov74_02231930(migrator);
        sub_0200F450(migrator->unk_E8F0.unk_08);
        migrator->canMigrateStatus = CANNOT_MIGRATE_CLOCK_ADJUSTED;
        *state = GBA_MIGRATOR_STATE_11;
        break;
    case GBA_MIGRATOR_STATE_11:
        if (ov74_022330D0(migrator, migrator->canMigrateStatus)) {
            *state = GBA_MIGRATOR_STATE_22;
        }
        break;
    case GBA_MIGRATOR_STATE_12:
        ov74_02232DA4(&migrator->unk_4FC);
        ov74_02232AC8(migrator);
        ov74_02232154(migrator);
        ov74_02231FF4(migrator);

        Main_SetVBlankIntrCB(ov74_02233024, migrator);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, 1);

        ov74_02232758(migrator, 0L);
        ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_IN, 13, state);

        break;
    case GBA_MIGRATOR_STATE_13:
        boxPos = TouchscreenHitbox_FindRectAtTouchNew((const TouchscreenHitbox *)migrator->pad_310);

        if (boxPos != -1) {
            if (boxPos < (5 * 6)) {
                gbaMonValidity = ov74_022324A0(migrator, boxPos);

                if (gbaMonValidity == GBA_MON_STATE_SELECTED) {
                    ov74_02232758(migrator, &(migrator->pokemonStorage->boxes[migrator->currentBox][boxPos]));

                    if (migrator->selectedCount == 6) {
                        migrator->fullSelectionTimer = 45;
                        *state = GBA_MIGRATOR_STATE_WAIT_FOR_FULL_SELECTION_TIMER;
                    }
                } else if (gbaMonValidity == GBA_MON_STATE_DESELECTED) {
                    ov74_02232758(migrator, 0L);
                } else if (gbaMonValidity == GBA_MON_STATE_IS_EGG) {
                    migrator->messageEntryID = 8;
                    *state = GBA_MIGRATOR_STATE_14;
                } else if (gbaMonValidity == GBA_MON_STATE_HAS_HM) {
                    migrator->messageEntryID = 9;
                    *state = GBA_MIGRATOR_STATE_14;
                } else if (gbaMonValidity == GBA_MON_STATE_HAS_INVALID_ITEM) {
                    migrator->messageEntryID = 38;
                    *state = GBA_MIGRATOR_STATE_14;
                } else if (gbaMonValidity == GBA_MON_STATE_IS_INVALID_SPECIES) {
                    migrator->messageEntryID = 38;
                    *state = GBA_MIGRATOR_STATE_14;
                }
            } else {
                switch (boxPos) {
                case ((5 * 6) + 1):
                    if (migrator->currentBox == 0) {
                        migrator->currentBox = 14 - 1;
                    } else {
                        migrator->currentBox--;
                    }

                    ov74_02231FF4(migrator);
                    PlaySE(1500);
                    break;
                case ((5 * 6) + 2):
                    if (++migrator->currentBox == 14) {
                        migrator->currentBox = 0;
                    }

                    ov74_02231FF4(migrator);
                    PlaySE(1500);
                    break;
                case ((5 * 6) + 0):
                    ov74_02232940(migrator, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
                    PlaySE(1500);
                    break;
                }
            }
        }
        break;
    case GBA_MIGRATOR_STATE_14:
        if (ov74_0223319C(migrator, 0)) {
            ov74_02232DA4(&migrator->unk_4FC);
            *state = GBA_MIGRATOR_STATE_13;
        }
        break;
    case GBA_MIGRATOR_STATE_WAIT_FOR_FULL_SELECTION_TIMER:
        if (--migrator->fullSelectionTimer == 0) {
            ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_OUT, 16, state);
        }
        break;
    case GBA_MIGRATOR_STATE_16:
        ov74_02232BD4(migrator);
        ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_IN, 17, state);
        ov74_02232F5C(migrator);

        break;
    case GBA_MIGRATOR_STATE_17:
        boxPos = YesNoPrompt_HandleInput(migrator->yesNoTouchMenu);

        switch (boxPos) {
        case 1:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            *state = GBA_MIGRATOR_STATE_18;
            break;
        case 2:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_OUT, 20, state);
            break;
        }
        break;
    case GBA_MIGRATOR_STATE_18: {

        migrator->unk_490.messageEntryID = 11;

        ov74_02231A1C(migrator, &migrator->unk_490, 0);
        ov74_02232F5C(migrator);

        PlaySE(1500);
        *state = GBA_MIGRATOR_STATE_19;
    } break;
    case GBA_MIGRATOR_STATE_19:
        boxPos = YesNoPrompt_HandleInput(migrator->yesNoTouchMenu);

        switch (boxPos) {
        case 1:
            migrator->unk_490.messageEntryID = ov74_0223C968[migrator->gbaVersion];
            ov74_02231A1C(migrator, &migrator->unk_490, 0);
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            migrator->unk_E8F0.unk_00 = 0;
            migrator->unk_E8F0.unk_08 = WaitingIcon_New(&migrator->unk_4FC, 0x3F0 - (18 + 12));
            *state = GBA_MIGRATOR_STATE_21;
            sub_0201A728(1 << 2);
            break;
        case 2:
            YesNoPrompt_Reset(migrator->yesNoTouchMenu);
            ov74_02232910(migrator, FADE_TYPE_BRIGHTNESS_OUT, 20, state);
            break;
        }
        break;
    case GBA_MIGRATOR_STATE_20:
        ov74_02232DC4(migrator);
        *state = GBA_MIGRATOR_STATE_12;
        break;
    case GBA_MIGRATOR_STATE_21:
        v2 = ov74_022317D8(migrator);

        if (v2 != 10) {
            sub_0200F450(migrator->unk_E8F0.unk_08);
            PlaySE(1562);

            if (v2 == 11) {
#ifdef HEARTGOLD
                migrator->unk_490.messageEntryID = 29;
#else
                migrator->unk_490.messageEntryID = 30;
#endif
            } else {
                migrator->unk_490.messageEntryID = 36;
            }

            ov74_02231A1C(migrator, &migrator->unk_490, 0);
            *state = GBA_MIGRATOR_STATE_22;
            sub_0201A738(1 << 2);
        }
        break;
    case GBA_MIGRATOR_STATE_22:
        if (gSystem.touchNew || gSystem.newKeys) {
            ov74_02232940(migrator, FADE_TYPE_BRIGHTNESS_OUT, 24, state);
            PlaySE(1500);
        }
        break;
    case GBA_MIGRATOR_STATE_FADE_OUT_TO_SELECTED_MON:
        if (IsPaletteFadeFinished()) {
            *state = migrator->unk_24;
        }
        break;
    case GBA_MIGRATOR_STATE_24:
        ov74_02232E3C(migrator);
        return 1;
        break;
    }

    if (migrator->unk_28 != 0L) {
        SpriteList_RenderAndAnimateSprites(migrator->unk_28);
    }

    return 0;
}
