#include "global.h"
#include "sprite_resources_header.h"
#include "overlay_08_battle_party.h"
#include "struct_defs/resdat_narc_structs.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "pm_string.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "battle/battle_party_context.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_55.h"

struct Background;

struct BallCapsule;

struct BallSeal;

struct BattleParty;

struct BattlePartyContext;

struct BattlePartyPokemon;

struct BattlePartyPokemonMove;

struct BattleSubMenuCursor;

struct BattleSystem;

struct BgConfig;

struct BoxPokemon;


struct MailMessage;

struct MessagePrinter;

struct GridMenuCursorPosition;

struct Mail;

struct ManagedSprite;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct PaletteBuffer;

struct PaletteData;

struct SelectedPaletteData;

struct Party;

struct PartyPokemon;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceCollection;

struct GF_2DGfxResObjList;

struct GF_2DGfxResHeader;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskQueue;

struct Window;

enum MoveAttribute {
    MOVEATTRIBUTE_EFFECT = 0,
    MOVEATTRIBUTE_CLASS = 1,
    MOVEATTRIBUTE_POWER = 2,
    MOVEATTRIBUTE_TYPE = 3,
    MOVEATTRIBUTE_ACCURACY = 4,
    MOVEATTRIBUTE_PP = 5,
    MOVEATTRIBUTE_EFFECT_CHANCE = 6,
    MOVEATTRIBUTE_RANGE = 7,
    MOVEATTRIBUTE_PRIORTY = 8,
    MOVEATTRIBUTE_FLAGS = 9,
    MOVEATTRIBUTE_CONTEST_EFFECT = 10,
    MOVEATTRIBUTE_CONTEST_TYPE = 11,
};


typedef struct BattleSystem BattleSystem;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

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

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);


u32 GetMonData(Pokemon *mon, int param, void *dest);

u32 GetMonExpBySpeciesAndLevel(int monSpecies, int monLevel);

u8 GetMonGender(Pokemon *mon);

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

u32 Pokemon_GetStatusIconId(Pokemon *mon);

u32 GetMoveAttr(int move, enum MoveAttribute param);

u8 GetMoveMaxPP(u16 move, u8 ppUps);

void DrawXPBar(BattleParty *battleParty, enum BattlePartyScreen screen);

void ChangeBattlePartyScreen(BattleParty *battleParty, enum BattlePartyScreen screen);

void DrawScreenBackground(BattleParty *battleParty, enum BattlePartyScreen screen);

void ov08_02223390(BattleSystem *battleSys, u16 item, u16 category, enum HeapID heapID);

void ov08_0221D184(BattleParty *battleParty);

void ov08_0221D184(BattleParty *battleParty) {
    for (u16 i = 0; i < Party_GetCount(battleParty->context->party); i++) {
        battleParty->partyPokemon[i].mon = Party_GetMonByIndex(battleParty->context->party, i);
        battleParty->partyPokemon[i].species = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_SPECIES, 0L);

        if (battleParty->partyPokemon[i].species == SPECIES_NONE) {
            continue;
        }

        battleParty->partyPokemon[i].attack = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_ATK, 0L);
        battleParty->partyPokemon[i].defence = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_DEF, 0L);
        battleParty->partyPokemon[i].speed = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_SPEED, 0L);
        battleParty->partyPokemon[i].spAtk = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_SP_ATK, 0L);
        battleParty->partyPokemon[i].spDef = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_SP_DEF, 0L);
        battleParty->partyPokemon[i].curHP = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_HP, 0L);
        battleParty->partyPokemon[i].maxHP = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_MAX_HP, 0L);
        battleParty->partyPokemon[i].type_1 = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_TYPE_1, 0L);
        battleParty->partyPokemon[i].type_2 = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_TYPE_2, 0L);
        battleParty->partyPokemon[i].level = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_LEVEL, 0L);

        if (GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_NO_PRINT_GENDER, 0L) == 1) {
            battleParty->partyPokemon[i].hideGenderMarker = 0;
        } else {
            battleParty->partyPokemon[i].hideGenderMarker = 1;
        }

        battleParty->partyPokemon[i].gender = GetMonGender(battleParty->partyPokemon[i].mon);
        battleParty->partyPokemon[i].summaryStatus = Pokemon_GetStatusIconId(battleParty->partyPokemon[i].mon);
        battleParty->partyPokemon[i].isEgg = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_IS_EGG, 0L);
        battleParty->partyPokemon[i].ability = (u16)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_ABILITY, 0L);
        battleParty->partyPokemon[i].heldItem = (u16)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_HELD_ITEM, 0L);
        battleParty->partyPokemon[i].exp = GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_EXPERIENCE, 0L);
        battleParty->partyPokemon[i].currentLevelBaseExp = GetMonExpBySpeciesAndLevel(battleParty->partyPokemon[i].species, battleParty->partyPokemon[i].level);

        if (battleParty->partyPokemon[i].level == 100) {
            battleParty->partyPokemon[i].nextLevelExp = battleParty->partyPokemon[i].currentLevelBaseExp;
        } else {
            battleParty->partyPokemon[i].nextLevelExp = GetMonExpBySpeciesAndLevel(battleParty->partyPokemon[i].species, battleParty->partyPokemon[i].level + 1);
        }

        battleParty->partyPokemon[i].cool = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_COOL, 0L);
        battleParty->partyPokemon[i].beauty = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_BEAUTY, 0L);
        battleParty->partyPokemon[i].cute = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_CUTE, 0L);
        battleParty->partyPokemon[i].smart = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_SMART, 0L);
        battleParty->partyPokemon[i].tough = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_TOUGH, 0L);
        battleParty->partyPokemon[i].mail = (u16)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_BALL_CAPSULE_ID, 0L);
        battleParty->partyPokemon[i].form = (u8)GetMonData(battleParty->partyPokemon[i].mon, MON_DATA_FORM, 0L);

        for (u16 l = 0; l < 4; l++) {
            BattlePartyPokemonMove *move = &battleParty->partyPokemon[i].moves[l];

            move->move = GetMonData(battleParty->partyPokemon[i].mon, (int)(MON_DATA_MOVE1 + l), 0L);

            if (move->move == MOVE_NONE) {
                continue;
            }

            move->currentPP = GetMonData(battleParty->partyPokemon[i].mon, (int)(MON_DATA_MOVE1_PP + l), 0L);
            move->maxPP = GetMonData(battleParty->partyPokemon[i].mon, (int)(MON_DATA_MOVE1_PP_UPS + l), 0L);
            move->maxPP = GetMoveMaxPP(move->move, move->maxPP);
            move->type = GetMoveAttr(move->move, MOVEATTRIBUTE_TYPE);
            move->class = GetMoveAttr(move->move, MOVEATTRIBUTE_CLASS);
            move->accuracy = GetMoveAttr(move->move, MOVEATTRIBUTE_ACCURACY);
            move->power = GetMoveAttr(move->move, MOVEATTRIBUTE_POWER);
        }
    }
}
