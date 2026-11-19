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

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

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

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCountsListUnion *capacities);

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

void ov08_02220B3C(BattleParty *battleParty, ManagedSprite *sprite, u16 moveClass);

void ov08_0222171C(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_0222061C(BattleParty *battleParty);

void ov08_0222061C(BattleParty *battleParty) {
    SpriteResourceCountsListUnion capacities = { .asArray[GF_GFX_RES_TYPE_CHAR] = 18, .asArray[GF_GFX_RES_TYPE_PLTT] = 6, .asArray[GF_GFX_RES_TYPE_CELL] = 6, .asArray[GF_GFX_RES_TYPE_ANIM] = 6, .asArray[GF_GFX_RES_TYPE_MCEL] = 0, .asArray[GF_GFX_RES_TYPE_MANM] = 0 };
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);

    battleParty->spriteManager = SpriteManager_New(spriteSystem);

    SpriteSystem_InitSprites(spriteSystem, battleParty->spriteManager, 43);
    SpriteSystem_InitManagerWithCapacities(spriteSystem, battleParty->spriteManager, &capacities);
}
