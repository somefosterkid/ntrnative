#ifndef POKEHEARTGOLD_OVERLAY_74_GBAMIGRATOR_H
#define POKEHEARTGOLD_OVERLAY_74_GBAMIGRATOR_H
#include "sprite_resources_header.h"

#include "g2d_renderer.h"
#include "bg_window.h"

// GBAMigrator: merged by tools/hg/promote.py from 18 m2c local copies (1 variants, base
// src/overlay_74_thumb_02231790.c) and 21 accesses; fields at every accessed offset, pads elsewhere.

struct BgConfig;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct StringTemplate;
struct TrainerInfo;
struct Window;
struct YesNoTouchMenu;

typedef struct GBAMigrator {
    int gbaVersion;
    int canMigrateStatus;
    int messageEntryID;
    int unk_0C;
    struct SaveData *saveData;
    struct TrainerInfo *trainerInfo;
    struct Options *options;
    int messageBoxFrame;
    struct BgConfig *bgConfig;
    int unk_24;
    struct SpriteList *unk_28;
    G2dRenderer unk_2C;
    struct SpriteResourceCollection *unk_1B8[6];
    struct SpriteResource *unk_1D0[6];
    SpriteResourcesHeader unk_1E8;
    struct { struct Sprite *monIconSprite; struct Sprite *heldItemSprite; int unk_08; } unk_20C[30];
    u8 pad_310[0x88];
    struct Sprite *unk_3FC[4];
    struct Sprite *unk_40C[4];
    struct Sprite *unk_41C[4];
    struct { struct Sprite *selectionSprite; int boxPosition; int boxId; } selectedMonData[6];
    int selectedCount;
    struct Sprite *selectedMonIcons[6];
    struct { Window *unk_00; int unk_04; int unk_08; int unk_0C; int unk_10; int unk_14; int unk_18; int unk_1C; int unk_20; int bgLayer; int unk_28; u32 unk_2C; u32 unk_30; int messageEntryID; u16 *unk_38; struct String *unk_3C; struct StringTemplate *unk_40; int unk_44; int unk_48; } unk_42C;
    Window unk_478;
    Window unk_488;
    Window unk_498;
    int unk_4A8;
    int unk_4AC;
    struct { u32 unk_00[1024]; u8 saveBlock2[4096]; u8 saveBlock1[16384]; struct { u8 currentBox; u8 unk_4_pad[0x3]; struct { u32 personality; u32 otId; u8 nickname[10]; u8 language; u8 isBadEgg : 1; u8 hasSpecies : 1; u8 isEgg : 1; u8 unused : 5; u8 otName[7]; u8 markings; u16 checksum; u16 unknown; u8 secure[0x30]; } boxes[30][14]; u8 boxNames[9][14]; u8 boxWallpapers[14]; } pokemonStorage; } saveSlot;
    void *pokemonStorage;
    int currentBox;
    int fullSelectionTimer;
    struct YesNoTouchMenu *yesNoTouchMenu;
    struct { int unk_00; int unk_04; void *unk_08; } unk_E8F0;
    struct { int unk_00; int unk_04; struct Sprite *iconSprite; u8 unk_0C[512]; } unk_E8FC[30];
    void *unk_12664;
    struct String *unk_12668;
    struct String *unk_1266C;
} GBAMigrator;

// sizeof(GBAMigrator) == 0x12610

#endif // POKEHEARTGOLD_OVERLAY_74_GBAMIGRATOR_H
