#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"
#include "battle/battle_cursor.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSystem;

struct BgConfig;

struct BgTemplate;


struct MessagePrinter;

struct GraphicsModes;

struct GridMenuCursorPosition;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct SelectedPaletteData;

struct Party;

struct Pokemon;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

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

enum BgType {
    BG_TYPE_STATIC = 0,
    BG_TYPE_AFFINE,
    BG_TYPE_STATIC_WITH_AFFINE,
};

void InitBgFromTemplate(BgConfig *bgConfig, u8 bgLayer, const BgTemplate *bgTemplate, u8 bgType);

void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);

void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);

void ScheduleBgTilemapBufferTransfer(BgConfig *bgConfig, u8 bgLayer);

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

void DrawXPBar(BattleParty *battleParty, enum BattlePartyScreen screen);

void ChangeBattlePartyScreen(BattleParty *battleParty, enum BattlePartyScreen screen);

void DrawScreenBackground(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

void ov08_0221CDF8(BattleParty *battleParty);

extern const GraphicsModes ov08_02224E34;

extern const BgTemplate ov08_02224EAC;

extern const BgTemplate ov08_02224EE4;

extern const BgTemplate ov08_02224F00;

extern const BgTemplate ov08_02224EC8;

void ov08_0221CDF8(BattleParty *battleParty) {
    {
        GraphicsModes graphicsMode = ov08_02224E34;

        SetScreenModeAndDisable(&graphicsMode, SCREEN_SUB);
    }

    {
        BgTemplate background = ov08_02224EAC;

        InitBgFromTemplate(battleParty->background, GF_BG_LYR_SUB_3, &background, BG_TYPE_STATIC);
    }

    {
        BgTemplate background = ov08_02224EE4;

        InitBgFromTemplate(battleParty->background, GF_BG_LYR_SUB_2, &background, BG_TYPE_STATIC);
    }

    {
        BgTemplate background = ov08_02224F00;

        InitBgFromTemplate(battleParty->background, GF_BG_LYR_SUB_1, &background, BG_TYPE_STATIC);
        BgClearTilemapBufferAndCommit(battleParty->background, GF_BG_LYR_SUB_1);
    }

    {
        BgTemplate background = ov08_02224EC8;

        InitBgFromTemplate(battleParty->background, GF_BG_LYR_SUB_0, &background, BG_TYPE_STATIC);
        BgClearTilemapBufferAndCommit(battleParty->background, GF_BG_LYR_SUB_0);
    }

    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, ((16) * sizeof(u16)), 0, battleParty->context->heapID);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, ((16) * sizeof(u16)), 0, battleParty->context->heapID);
    ScheduleBgTilemapBufferTransfer(battleParty->background, GF_BG_LYR_SUB_1);
    ScheduleBgTilemapBufferTransfer(battleParty->background, GF_BG_LYR_SUB_0);
}
