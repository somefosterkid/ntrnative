#include "global.h"
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "touchscreen.h"
#include "unk_0200A090.h"
#include "palPark_migration.h"
#include "save_arrays.h"
#include "system.h"

struct SpriteList;

struct SpriteResourceCollection;

struct StringTemplate;

struct TrainerInfo;

struct YesNoTouchMenu;

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

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;

typedef struct TrainerInfo TrainerInfo;

typedef struct StringTemplate StringTemplate;

enum AGBGameType {
    AGB_TYPE_RUBY = 0,
    AGB_TYPE_SAPPHIRE,
    AGB_TYPE_LEAFGREEN,
    AGB_TYPE_FIRERED,
    AGB_TYPE_EMERALD,
};

u8 PmAgbCartridge_GetVersionInternal(void);

typedef struct YesNoTouchMenu YesNoTouchMenu;

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
    StringTemplate *unk_40;
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
    TrainerInfo *trainerInfo;
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
    YesNoTouchMenu *yesNoTouchMenu;
    UnkStruct_ov97_02233B8C unk_E8F0;
    UnkStruct_ov97_0223F434 unk_E8FC[(5 * 6)];
    void *unk_12664;
    String *unk_12668;
    String *unk_1266C;
} GBAMigrator;

void ov74_02231790(GBAMigrator *migrator);

u32 ov74_02233F68(void);
enum SaveResult {
    SAVE_RESULT_PROCEED = 0,
    SAVE_RESULT_PROCEED_FINAL,
    SAVE_RESULT_OK,
    SAVE_RESULT_CORRUPT,
};
BOOL ov74_02233E8C(void);
int ov74_02233ED4(void);
int ov74_02233F14(void);
enum SleepType {
    SLEEP_TYPE_SAVE_DATA = 1,
};
void ov74_0223195C(GBAMigrator *migrator);
int ov74_022317D8(GBAMigrator *migrator);

void ov74_02231790(GBAMigrator *migrator) {
    switch (PmAgbCartridge_GetVersionInternal()) {
    case AGB_TYPE_RUBY:
        migrator->gbaVersion = VERSION_RUBY;
        break;
    case AGB_TYPE_SAPPHIRE:
        migrator->gbaVersion = VERSION_SAPPHIRE;
        break;
    case AGB_TYPE_LEAFGREEN:
        migrator->gbaVersion = VERSION_LEAFGREEN;
        break;
    case AGB_TYPE_FIRERED:
        migrator->gbaVersion = VERSION_FIRERED;
        break;
    case AGB_TYPE_EMERALD:
        migrator->gbaVersion = VERSION_EMERALD;
        break;
    default:
        migrator->gbaVersion = -1;
        break;
    }
}

int ov74_022317D8(GBAMigrator *migrator) {
    int v0;
    u8 v1[16];
    int saveResult;
    MigratedPokemon *transferData;
    UnkStruct_ov97_02233B8C *v4 = &migrator->unk_E8F0;

    switch (v4->unk_00) {
    case 0:
        ov74_0223195C(migrator);
        transferData = Save_MigratedPokemon_Get(migrator->saveData);
        MigratedPokemon_RecordMigration(transferData, ov74_02233F68());
        v4->unk_00++;
        break;
    case 1:
        v4->unk_04 = 1 + OS_GetTick() % 120;
        v4->unk_00++;
        break;
    case 2:
        if (--v4->unk_04 == 0) {
            v4->unk_00++;
        }
        break;
    case 3:
        Save_PrepareForAsyncWrite(migrator->saveData, 2);
        v4->unk_00++;
        break;
    case 4:
        saveResult = Save_WriteFileAsync(migrator->saveData);

        if (saveResult == SAVE_RESULT_CORRUPT) {
            return 12;
        }

        if (saveResult == SAVE_RESULT_PROCEED_FINAL) {
            v4->unk_00++;
        }
        break;
    case 5:
        CTRDG_ReadAgbFlash(0, 0, v1, sizeof(v1));
        v4->unk_00++;
        break;
    case 6:
        v0 = ov74_02233E8C();

        if (v0 == 0) {
            Sys_ClearSleepDisableFlag(SLEEP_TYPE_SAVE_DATA);
            return 12;
        }

        v4->unk_00++;
        break;
    case 7:
        if (ov74_02233F14() == 9) {
            break;
        }

        if (ov74_02233F14() == 11) {
            v4->unk_00++;
        } else {
            v0 = ov74_02233ED4();

            if (v0 == 8) {
                Sys_ClearSleepDisableFlag(SLEEP_TYPE_SAVE_DATA);
                return 12;
            }
        }
        break;
    case 8:
        do {
            saveResult = Save_WriteFileAsync(migrator->saveData);

            if (saveResult == SAVE_RESULT_CORRUPT) {
                return 12;
            }
        } while (saveResult != SAVE_RESULT_OK);

        ov74_02233ED4();
        Sys_SetSleepDisableFlag(SLEEP_TYPE_SAVE_DATA);
        v4->unk_00++;
        break;
    case 9:
        v0 = ov74_02233ED4();

        if (v0 == 8) {
            Sys_ClearSleepDisableFlag(SLEEP_TYPE_SAVE_DATA);
            return 12;
        }

        if (v0 == 0) {
            Sys_ClearSleepDisableFlag(SLEEP_TYPE_SAVE_DATA);
            return 11;
        }
        break;
    }

    return 10;
}
