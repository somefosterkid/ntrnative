#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_icon_idx.h"
#include "battle/battle_cursor.h"
#include "battle/battle_party_context.h"

struct BattleSystem;

struct SpriteList;

struct SysTask;

struct SysTaskQueue;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

BOOL SpriteSystem_LoadCellResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

BOOL SpriteSystem_LoadAnimResObjFromOpenNarc(SpriteSystem *spriteSys, SpriteManager *spriteMan, NARC *narc, int memberIdx, BOOL compressed, int resourceID);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

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

SpriteSystem *BattleSystem_GetSpriteSystem(BattleSystem *battleSys);


u32 sub_02074490(void);

u32 sub_02074498(void);

u32 sub_020744A4(void);

void ov08_02220668(BattleParty *battleParty);

u32 sub_0208AD54(void);
u32 sub_0208AD58(void);
u32 sub_0208AD5C(void);
u32 sub_0208AD60(void);
void ov08_02220750(BattleParty *battleParty);

void ov08_02220668(BattleParty *battleParty) {
    NARC *narc = NARC_New((enum NarcId)20, battleParty->context->heapID);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(battleParty->palette, PLTTBUF_SUB_OBJ, spriteSystem, battleParty->spriteManager, narc, sub_02074490(), 0, 3, NNS_G2D_VRAM_TYPE_2DSUB, 45063);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, sub_02074498(), 0, 45063);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, sub_020744A4(), 0, 45063);

    for (u32 i = 0; i < 6; i++) {
        if (battleParty->partyPokemon[i].species != SPECIES_NONE) {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, Pokemon_GetIconNaix(battleParty->partyPokemon[i].mon), 0, NNS_G2D_VRAM_TYPE_2DSUB, 45063 + i);
        } else {
            SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, GetMonIconNaixEx(SPECIES_NONE, 0, 0), 0, NNS_G2D_VRAM_TYPE_2DSUB, 45063 + i);
        }
    }

    NARC_Delete(narc);
}

void ov08_02220750(BattleParty *battleParty) {
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(battleParty->context->battleSys);
    NARC *narc = NARC_New(NARC_a_0_3_9, battleParty->context->heapID);

    SpriteSystem_LoadPaletteBufferFromOpenNarc(battleParty->palette, PLTTBUF_SUB_OBJ, spriteSystem, battleParty->spriteManager, narc, sub_0208AD58(), 0, 1, NNS_G2D_VRAM_TYPE_2DSUB, 45064);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, sub_0208AD5C(), 0, 45064);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, sub_0208AD60(), 0, 45064);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, battleParty->spriteManager, narc, sub_0208AD54(), 0, NNS_G2D_VRAM_TYPE_2DSUB, 45069);
    NARC_Delete(narc);
}
