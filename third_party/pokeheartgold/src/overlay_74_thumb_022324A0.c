#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "save.h"
#include "touchscreen.h"
#include "unk_0200A090.h"
#include "gf_gfx_planes.h"

struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct YesNoPrompt;
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
void Sprite_SetMatrix(Sprite *sprite, const VecFx32 *position);
void Sprite_SetDrawFlag(Sprite *sprite, BOOL draw);
const VecFx32 *Sprite_GetMatrixPtr(const Sprite *sprite);
BOOL Sprite_GetDrawFlag(const Sprite *sprite);
typedef struct SpriteResourceCollection SpriteResourceCollection;
typedef struct G2dRenderer {
    NNSG2dRendererInstance renderer;
    NNSG2dRenderSurface mainScreen;
    NNSG2dRenderSurface subScreen;
} G2dRenderer;
typedef struct PlayerProfile PlayerProfile;
typedef struct MessageFormat MessageFormat;
typedef struct YesNoPrompt YesNoPrompt;
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
BOOL ov74_02232398(GBAMigrator *migrator, int boxPosition);
BOOL ov74_022323D0(GBAMigrator *migrator, int boxPosition);
BOOL ov74_02232424(GBAMigrator *migrator, int boxPosition);
BOOL ov74_02232474(GBAMigrator *migrator, int boxPosition);
BOOL ConvertRSStringToDPStringInternational(const u8 *src, charcode_t *dst, u32 length, u32 language);
u16 PmAgbCartridge_GetLanguage(void);
void ov74_02231A1C(GBAMigrator *migrator, UnkStruct_ov97_02233DAC *param1, int param2);
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
u32 AGB_GetBoxMonData(GBABoxPokemon *gbaBoxMon, int field, u8 *data);

int ov74_022324A0(GBAMigrator *migrator, int boxPosition);
void ov74_0223262C(GBAMigrator *migrator);
void ov74_02232678(GBAMigrator *migrator, int boxNum);
void ov74_02232700(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon);

int ov74_022324A0(GBAMigrator *migrator, int boxPosition) {
    int i, v1;

    if (migrator->unk_20C[boxPosition].monIconSprite != 0L
        && (Sprite_GetDrawFlag(migrator->unk_20C[boxPosition].monIconSprite) == 0)) {
        return GBA_MON_STATE_EMPTY_SLOT;
    }

    for (v1 = -1, i = 0; v1 == -1 && i < 6; i++) {
        if ((migrator->selectedMonData[i].boxId == migrator->currentBox) && (migrator->selectedMonData[i].boxPosition == boxPosition)) {
            v1 = i;
        }
    }

    if (ov74_02232398(migrator, boxPosition) == 1) {
        return GBA_MON_STATE_IS_EGG;
    }

    if (ov74_022323D0(migrator, boxPosition) == 1) {
        return GBA_MON_STATE_HAS_HM;
    }

    if (ov74_02232424(migrator, boxPosition) == 1) {
        return GBA_MON_STATE_HAS_INVALID_ITEM;
    }

    if (ov74_02232474(migrator, boxPosition) == 1) {
        return GBA_MON_STATE_IS_INVALID_SPECIES;
    }

    if (v1 == -1) {
        const VecFx32 *v2;
        VecFx32 v3;

        if (migrator->selectedCount == 6) {
            return GBA_MON_STATE_NONE;
        }

        for (i = 0; i < 6; i++) {
            if (migrator->selectedMonData[i].boxPosition == -1) {
                v2 = Sprite_GetMatrixPtr(migrator->unk_20C[boxPosition].monIconSprite);
                Sprite_SetMatrix(migrator->selectedMonData[i].selectionSprite, v2);
                v2 = Sprite_GetMatrixPtr(migrator->selectedMonData[i].selectionSprite);

                v3 = *v2;
                v3.x -= ((fx32)0x0000000000001000L) * 8;
                v3.y -= ((fx32)0x0000000000001000L) * 4;

                Sprite_SetMatrix(migrator->selectedMonData[i].selectionSprite, &v3);
                Sprite_SetDrawFlag(migrator->selectedMonData[i].selectionSprite, 1);

                migrator->selectedMonData[i].boxPosition = boxPosition;
                migrator->selectedMonData[i].boxId = migrator->currentBox;
                migrator->selectedCount++;

                return GBA_MON_STATE_SELECTED;
            }
        }
    } else {
        Sprite_SetDrawFlag(migrator->selectedMonData[v1].selectionSprite, 0);

        migrator->selectedMonData[v1].boxPosition = -1;
        migrator->selectedCount--;

        return GBA_MON_STATE_DESELECTED;
    }

    return GBA_MON_STATE_NONE;
}

void ov74_0223262C(GBAMigrator *migrator) {
    int i;

    for (i = 0; i < 6; i++) {
        if ((migrator->selectedMonData[i].boxPosition != -1) && (migrator->selectedMonData[i].boxId == migrator->currentBox)) {
            Sprite_SetDrawFlag(migrator->selectedMonData[i].selectionSprite, 1);
        } else {
            Sprite_SetDrawFlag(migrator->selectedMonData[i].selectionSprite, 0);
        }
    }
}

void ov74_02232678(GBAMigrator *migrator, int boxNum) {
    UnkStruct_ov97_02233DAC v0;
    u16 boxName[8 + 1];

    memset(&v0, 0, sizeof(UnkStruct_ov97_02233DAC));

    v0.unk_00 = &migrator->unk_4DC;
    v0.unk_08 = 6;
    v0.unk_0C = 21;
    v0.unk_10 = 13;
    v0.unk_14 = 2;
    v0.messageEntryID = -1;
    v0.unk_18 = 0;
    v0.unk_1C = 0;
    v0.bgLayer = GF_BG_LYR_MAIN_1;
    v0.unk_28 = 1;
    v0.unk_2C = ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0)));
    v0.unk_20 = 0xA0;

    ConvertRSStringToDPStringInternational(migrator->pokemonStorage->boxNames[boxNum], boxName, 8 + 1, PmAgbCartridge_GetLanguage());
    v0.unk_38 = boxName;
    ov74_02231A1C(migrator, &v0, 0x1);
}

void ov74_02232700(GBAMigrator *migrator, GBABoxPokemon *gbaBoxMon) {
    u16 *v0 = GetBgTilemapBuffer(migrator->bgConfig, GF_BG_LYR_MAIN_2);
    u8 markings;
    int i;

    if (gbaBoxMon != 0L) {
        markings = AGB_GetBoxMonData(gbaBoxMon, GBA_MON_DATA_MARKINGS, 0L);
    } else {
        markings = 0;
    }

    for (i = 0; i < 4; i++) {
        if (markings & 1) {
            v0[32 * 3 + 11 + i] = 0x80 + i;
        } else {
            v0[32 * 3 + 11 + i] = 0x60 + i;
        }

        markings >>= 1;
    }

    BgCommitTilemapBufferToVram(migrator->bgConfig, GF_BG_LYR_MAIN_2);
}
