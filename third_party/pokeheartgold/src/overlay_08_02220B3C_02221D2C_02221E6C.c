#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "gf_gfx_planes.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuButtonDimensions;

struct BattleSystem;

struct BgConfig;


struct MessagePrinter;

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

typedef struct BattleSubMenuButtonDimensions {
    u8 xCoord;
    u8 yCoord;
    u8 width;
    u8 height;
} BattleSubMenuButtonDimensions;

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

void LoadRectToBgTilemapRect(BgConfig *bgConfig, u8 bgLayer, const void *src, u8 destX, u8 destY, u8 destWidth, u8 destHeight);

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

void Heap_Free(void *ptr);

extern const BattleSubMenuButtonDimensions ov08_02225A54[34];

void ov08_02221D2C(BattleParty *battleParty, u16 *buttonData, u8 button, enum BattleSubMenuButtonState buttonState, u8 isAltButton);

void ov08_02221E6C(BattleParty *battleParty, u8 button, u8 buttonState, u8 isAltButton);

void ov08_02221E6C(BattleParty *battleParty, u8 button, u8 buttonState, u8 isAltButton) {
    u16 *buttonData = Heap_Alloc(battleParty->context->heapID, ov08_02225A54[button].width * ov08_02225A54[button].height * sizeof(u16));

    ov08_02221D2C((struct BattleParty *)(battleParty), (unsigned short *)(buttonData), button, (enum BattleSubMenuButtonState)(buttonState), isAltButton);

    LoadRectToBgTilemapRect(battleParty->background, GF_BG_LYR_SUB_2, buttonData, ov08_02225A54[button].xCoord, ov08_02225A54[button].yCoord, ov08_02225A54[button].width, ov08_02225A54[button].height);
    ScheduleBgTilemapBufferTransfer(battleParty->background, GF_BG_LYR_SUB_2);
    Heap_Free(buttonData);
}
