#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pm_string.h"
#include "msgdata.h"
#include "battle/battle_party_context.h"
#include "battle/battle.h"

struct BallSeal;

struct BattleParty;

struct BattleSystem;

struct BattleCursor;

struct MessagePrinter;

struct G2dRenderer;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct SelectedPaletteData;

struct SpriteList;

struct SpriteManager;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesAndPriority;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

enum MoveClass {
    CLASS_PHYSICAL = 0,
    CLASS_SPECIAL = 1,
    CLASS_STATUS = 2,
};

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

typedef struct GridMenuCursorPosition {
    u8 xCoord1;
    u8 yCoord1;
    u8 xCoord2;
    u8 yCoord2;
    u8 upIndex;
    u8 downIndex;
    u8 leftIndex;
    u8 rightIndex;
} GridMenuCursorPosition;

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

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

enum BattlePartyScreen {
    BATTLE_PARTY_SCREEN_POKEMON_PARTY = 0,
    BATTLE_PARTY_SCREEN_SELECT_POKEMON,
    BATTLE_PARTY_SCREEN_POKEMON_SUMMARY,
    BATTLE_PARTY_SCREEN_POKEMON_MOVES,
    BATTLE_PARTY_SCREEN_MOVE_SUMMARY,
    BATTLE_PARTY_SCREEN_RESTORE_MOVE_PP,
    BATTLE_PARTY_SCREEN_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE,
    BATTLE_PARTY_SCREEN_LEARN_MOVE_CONTEST_STATS,
    BATTLE_PARTY_SCREEN_CONFIRM_LEARN_MOVE_CONTEST_STATS,
};

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);

typedef struct SpriteResourcesAndPriority {
    int charResID;
    int plttResID;
    int cellResID;
    int animResID;
    int priority;
} SpriteResourcesAndPriority;

extern const SpriteResourcesAndPriority ov08_02225644[39];

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *sprite, u16 moveClass);

void ov08_0222171C(BattleParty *battleParty, enum BattlePartyScreen screen);

ManagedSprite *ov08_022209B8(BattleParty *battleParty, u32 spriteIndex);

ManagedSprite *ov08_022209B8(BattleParty *battleParty, u32 spriteIndex) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    ManagedSpriteTemplate template;

    template.x = 0;
    template.y = 0;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = ov08_02225644[spriteIndex].priority;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DSUB;
    template.resIdList[GF_GFX_RES_TYPE_CHAR] = ov08_02225644[spriteIndex].charResID;
    template.resIdList[GF_GFX_RES_TYPE_PLTT] = ov08_02225644[spriteIndex].plttResID;
    template.resIdList[GF_GFX_RES_TYPE_CELL] = ov08_02225644[spriteIndex].cellResID;
    template.resIdList[GF_GFX_RES_TYPE_ANIM] = ov08_02225644[spriteIndex].animResID;
    template.bgPriority = 1;
    template.vramTransfer = FALSE;

    return SpriteSystem_NewSprite(spriteSystem, battleParty->spriteManager, &template);
}
