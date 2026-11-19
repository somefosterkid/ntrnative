#include "global.h"

#include "pokemon_summary_app.h"

struct CharTransferTemplateWithModes;

struct RenderOamTemplate;

struct SpriteManager;

struct SpriteSystem;



typedef struct CharTransferTemplateWithModes {
    int maxTasks;
    int sizeMain;
    int sizeSub;
    GXOBJVRamModeChar modeMain;
    GXOBJVRamModeChar modeSub;
} CharTransferTemplateWithModes;

typedef struct RenderOamTemplate {
    int mainOamStart;
    int mainOamCount;
    int mainAffineOamStart;
    int mainAffineOamCount;
    int subOamStart;
    int subOamCount;
    int subAffineOamStart;
    int subAffineOamCount;
} RenderOamTemplate;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const RenderOamTemplate *oamTemplate, const CharTransferTemplateWithModes *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_LoadResourceDataFromFilepaths(SpriteSystem *spriteSys, SpriteManager *spriteMan);

enum SummarySprite {
    SUMMARY_SPRITE_TAB_INFO = 0,
    SUMMARY_SPRITE_TAB_MEMO,
    SUMMARY_SPRITE_TAB_SKILLS,
    SUMMARY_SPRITE_TAB_BATTLE_MOVES,
    SUMMARY_SPRITE_TAB_CONDITION,
    SUMMARY_SPRITE_TAB_CONTEST_MOVES,
    SUMMARY_SPRITE_TAB_RIBBONS,
    SUMMARY_SPRITE_TAB_EXIT,
    SUMMARY_SPRITE_CAUGHT_BALL,
    SUMMARY_SPRITE_MOVE_SELECTOR_1,
    SUMMARY_SPRITE_MOVE_SELECTOR_2,
    SUMMARY_SPRITE_MON_TYPE_ICON_1,
    SUMMARY_SPRITE_MON_TYPE_ICON_2,

    SUMMARY_MOVE_TYPE_ICON_SPRITE_START,
    SUMMARY_SPRITE_MOVE_TYPE_ICON_1 = SUMMARY_MOVE_TYPE_ICON_SPRITE_START,
    SUMMARY_SPRITE_MOVE_TYPE_ICON_2,
    SUMMARY_SPRITE_MOVE_TYPE_ICON_3,
    SUMMARY_SPRITE_MOVE_TYPE_ICON_4,
    SUMMARY_SPRITE_MOVE_TYPE_ICON_5,
    SUMMARY_MOVE_TYPE_ICON_SPRITE_END = SUMMARY_SPRITE_MOVE_TYPE_ICON_5,

    SUMMARY_SPRITE_MOVE_CATEGORY_ICON,
    SUMMARY_SPRITE_MON_ICON,
    SUMMARY_SPRITE_STATUS_ICON,
    SUMMARY_SPRITE_PAGE_ARROW_LEFT,
    SUMMARY_SPRITE_PAGE_ARROW_RIGHT,

    SUMMARY_MARKING_SPRITES_START,
    SUMMARY_SPRITE_MARKING_CIRCLE = SUMMARY_MARKING_SPRITES_START,
    SUMMARY_SPRITE_MARKING_TRIANGLE,
    SUMMARY_SPRITE_MARKING_SQUARE,
    SUMMARY_SPRITE_MARKING_HEART,
    SUMMARY_SPRITE_MARKING_STAR,
    SUMMARY_SPRITE_MARKING_DIAMOND,
    SUMMARY_MARKING_SPRITES_END = SUMMARY_SPRITE_MARKING_DIAMOND,

    SUMMARY_SHEEN_SPRITES_START,
    SUMMARY_SPRITE_SHEEN_1 = SUMMARY_SHEEN_SPRITES_START,
    SUMMARY_SPRITE_SHEEN_2,
    SUMMARY_SPRITE_SHEEN_3,
    SUMMARY_SPRITE_SHEEN_4,
    SUMMARY_SPRITE_SHEEN_5,
    SUMMARY_SPRITE_SHEEN_6,
    SUMMARY_SPRITE_SHEEN_7,
    SUMMARY_SPRITE_SHEEN_8,
    SUMMARY_SPRITE_SHEEN_9,
    SUMMARY_SPRITE_SHEEN_10,
    SUMMARY_SPRITE_SHEEN_11,
    SUMMARY_SPRITE_SHEEN_12,
    SUMMARY_SHEEN_SPRITES_END = SUMMARY_SPRITE_SHEEN_12,

    SUMMARY_SPRITE_BUTTON_TAP_CIRCLE,

    SUMMARY_CONDITION_ARROW_SPRITES_START,
    SUMMARY_SPRITE_CONDITION_ARROW_COOL = SUMMARY_CONDITION_ARROW_SPRITES_START,
    SUMMARY_SPRITE_CONDITION_ARROW_BEAUTY,
    SUMMARY_SPRITE_CONDITION_ARROW_CUTE,
    SUMMARY_SPRITE_CONDITION_ARROW_SMART,
    SUMMARY_SPRITE_CONDITION_ARROW_TOUGH,
    SUMMARY_CONDITION_ARROW_SPRITES_END = SUMMARY_SPRITE_CONDITION_ARROW_TOUGH,

    SUMMARY_SPRITE_A_BUTTON,
    SUMMARY_SPRITE_CONTEST_STAT_DOT_COOL,
    SUMMARY_SPRITE_CONTEST_STAT_DOT_BEAUTY,
    SUMMARY_SPRITE_CONTEST_STAT_DOT_CUTE,
    SUMMARY_SPRITE_CONTEST_STAT_DOT_SMART,
    SUMMARY_SPRITE_CONTEST_STAT_DOT_TOUGH,
    SUMMARY_SPRITE_SHINY_ICON,
    SUMMARY_SPRITE_POKERUS_CURED_ICON,

    SUMMARY_RIBBON_SPRITES_START,
    SUMMARY_SPRITE_RIBBON_1 = SUMMARY_RIBBON_SPRITES_START,
    SUMMARY_SPRITE_RIBBON_2,
    SUMMARY_SPRITE_RIBBON_3,
    SUMMARY_SPRITE_RIBBON_4,
    SUMMARY_SPRITE_RIBBON_5,
    SUMMARY_SPRITE_RIBBON_6,
    SUMMARY_SPRITE_RIBBON_7,
    SUMMARY_SPRITE_RIBBON_8,
    SUMMARY_SPRITE_RIBBON_9,
    SUMMARY_SPRITE_RIBBON_10,
    SUMMARY_SPRITE_RIBBON_11,
    SUMMARY_SPRITE_RIBBON_12,
    SUMMARY_RIBBON_SPRITES_END = SUMMARY_SPRITE_RIBBON_12,

    SUMMARY_SPRITE_RIBBON_CURSOR,
    SUMMARY_SPRITE_RIBBON_ARROW_UP,
    SUMMARY_SPRITE_RIBBON_ARROW_DOWN,
    SUMMARY_SPRITE_RIBBON_FLASH,

    SUMMARY_CONDITION_FLASH_SPRITES_START,
    SUMMARY_SPRITE_CONDITION_FLASH_COOL = SUMMARY_CONDITION_FLASH_SPRITES_START,
    SUMMARY_SPRITE_CONDITION_FLASH_BEAUTY,
    SUMMARY_SPRITE_CONDITION_FLASH_CUTE,
    SUMMARY_SPRITE_CONDITION_FLASH_SMART,
    SUMMARY_SPRITE_CONDITION_FLASH_TOUGH,
    SUMMARY_CONDITION_FLASH_SPRITES_END = SUMMARY_SPRITE_CONDITION_FLASH_TOUGH,

    SUMMARY_SPRITE_POKERUS_ICON,

    SUMMARY_SPRITE_MAX = 77,
};



void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

void sub_0208B1AC(PokemonSummary *summaryScreen);

BOOL sub_0200D294(SpriteSystem *spriteSystem, SpriteManager *spriteManager, void *);

void sub_0208B1AC(PokemonSummary *summaryScreen) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GF_CreateVramTransferManager(32, HEAP_ID_19);

    summaryScreen->spriteSystem = SpriteSystem_Alloc(HEAP_ID_19);
    summaryScreen->spriteManager = SpriteManager_New(summaryScreen->spriteSystem);

    RenderOamTemplate renderOamTemplate = {
        .mainOamStart = 0,
        .mainOamCount = 128,
        .mainAffineOamStart = 0,
        .mainAffineOamCount = 32,
        .subOamStart = 0,
        .subOamCount = 128,
        .subAffineOamStart = 0,
        .subAffineOamCount = 32,
    };

    CharTransferTemplateWithModes charTransferTemplate = {
        .maxTasks = 115,
        .sizeMain = 1024,
        .sizeSub = 1024,
        .modeMain = GX_OBJVRAMMODE_CHAR_1D_32K,
        .modeSub = GX_OBJVRAMMODE_CHAR_1D_32K
    };

    SpriteSystem_Init(summaryScreen->spriteSystem, &renderOamTemplate, &charTransferTemplate, 32);
    SpriteSystem_InitSprites(summaryScreen->spriteSystem, summaryScreen->spriteManager, NELEMS(summaryScreen->sprites));

    u16 resourceIDs[7] = { 0x36, 0x37, 0x35, 0x34, 0xFFFF, 0xFFFF, 0x55 };

    sub_0200D294(summaryScreen->spriteSystem, summaryScreen->spriteManager, resourceIDs);
}
