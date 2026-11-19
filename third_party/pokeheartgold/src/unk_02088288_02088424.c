#include "global.h"

#include "constants/balls.h"
#include "constants/heap.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/ribbon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "bg_window.h"
#include "camera.h"
#include "dex_mon_measures.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "message_printer.h"
#include "move.h"
#include "msgdata.h"
#include "options.h"
#include "overlay_manager.h"
#include "party.h"
#include "party_menu.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokepic.h"
#include "render_window.h"
#include "ribbon.h"
#include "save_vars_flags.h"
#include "screen_fade.h"
#include "sound_chatot.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_flags.h"
#include "system.h"
#include "touchscreen.h"
#include "unk_02016EDC.h"
#include "unk_0208805C.h"
#include "unk_02088288.h"
#include "unk_0208B1AC.h"
#include "unk_0208DE40.h"
#include "vram_transfer_manager.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"

struct BgConfig;

struct Camera;

struct ConditionChartQuad;

struct ConditionChartVertex;

struct MessageFormat;

struct MessagePrinter;

struct MsgData;

struct NARC;

struct OverlayManager;

struct OverlayManagerTemplate;

struct PokedexData;

struct PokemonSummary;

struct PokemonSummaryArgs;

struct PokemonSummaryMon;

struct Pokepic;

struct PokepicAnimManager;

struct PokepicAnimScript;

struct PokepicManager;

struct Sprite;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct Window;

enum PokemonSummarySprite {
    POKEMON_SUMMARY_SPRITE_CURSOR = 9,
    POKEMON_SUMMARY_SPRITE_SWAP_CURSOR = 10,
    POKEMON_SUMMARY_SPRITE_TYPE_1 = 11,
    POKEMON_SUMMARY_SPRITE_TYPE_2 = 12,
    POKEMON_SUMMARY_SPRITE_MOVE_TYPES = 13,
    POKEMON_SUMMARY_SPRITE_MOVE_TO_LEARN_TYPE = 17,
    POKEMON_SUMMARY_SPRITE_MOVE_CATEGORY = 18,
    POKEMON_SUMMARY_SPRITE_MON_ICON = 19,
    POKEMON_SUMMARY_SPRITE_STATUS = 20,
    POKEMON_SUMMARY_SPRITE_MARKINGS = 23,
    POKEMON_SUMMARY_SPRITE_SHEEN_SPARKLES = 29,
    POKEMON_SUMMARY_SPRITE_CONDITION_CHANGES = 41,
    POKEMON_SUMMARY_SPRITE_CONDITION_COOL = 47,
    POKEMON_SUMMARY_SPRITE_CONDITION_BEAUTY = 48,
    POKEMON_SUMMARY_SPRITE_CONDITION_CUTE = 49,
    POKEMON_SUMMARY_SPRITE_CONDITION_SMART = 50,
    POKEMON_SUMMARY_SPRITE_CONDITION_TOUGH = 51,
    POKEMON_SUMMARY_SPRITE_SHINY = 52,
    POKEMON_SUMMARY_SPRITE_POKERUS_CURED = 53,
    POKEMON_SUMMARY_SPRITE_RIBBONS = 54,
    POKEMON_SUMMARY_SPRITE_RIBBON_CURSOR = 63,
    POKEMON_SUMMARY_SPRITE_RIBBON_PAGE_UP = 64,
    POKEMON_SUMMARY_SPRITE_RIBBON_PAGE_DOWN = 65,
    POKEMON_SUMMARY_SPRITE_POKERUS = 72,
    POKEMON_SUMMARY_SPRITE_PARTY_ICONS = 73,
    POKEMON_SUMMARY_SPRITE_POKEATHLON_STARS = 79,
    POKEMON_SUMMARY_SPRITE_POKEATHLON_MODIFIERS = 104,
    POKEMON_SUMMARY_SPRITE_SHINY_LEAVES = 109,
    POKEMON_SUMMARY_SPRITE_SHINY_LEAF_CROWN = 114,
    POKEMON_SUMMARY_SPRITE_COUNT,
};

enum PokemonSummaryState {
    POKEMON_SUMMARY_STATE_START_FADE_IN,
    POKEMON_SUMMARY_STATE_WAIT_FADE_IN,
    POKEMON_SUMMARY_STATE_INPUT,
    POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO,
    POKEMON_SUMMARY_STATE_SLIDE_OUT_MOVE_INFO,
    POKEMON_SUMMARY_STATE_MOVE_SELECT_INPUT,
    POKEMON_SUMMARY_STATE_MOVE_SWAP_INPUT,
    POKEMON_SUMMARY_STATE_START_MOVE_SWAP,
    POKEMON_SUMMARY_STATE_FORGET_MOVE_INPUT,
    POKEMON_SUMMARY_STATE_FORGET_MOVE_CONFIRM_INPUT,
    POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST,
    POKEMON_SUMMARY_STATE_CLOSE_RIBBON_LIST,
    POKEMON_SUMMARY_STATE_RIBBON_INPUT,
    POKEMON_SUMMARY_STATE_RIBBON_PAGE_UP,
    POKEMON_SUMMARY_STATE_RIBBON_PAGE_DOWN,
    POKEMON_SUMMARY_STATE_FLASH_BUTTON,
    POKEMON_SUMMARY_STATE_UPDATE_CONDITION,
    POKEMON_SUMMARY_STATE_SHOW_CONDITION_CHANGES,
    POKEMON_SUMMARY_STATE_WAIT_CONDITION_EXIT,
    POKEMON_SUMMARY_STATE_SHOW_POKEPIC,
    POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON,
    POKEMON_SUMMARY_STATE_START_FADE_OUT,
    POKEMON_SUMMARY_STATE_WAIT_FADE_OUT,
};

typedef struct PokemonSummaryMon {
    String *speciesName;
    String *nickname;
    String *otName;
    u16 species;
    u16 heldItem;
    u8 type1;
    u8 type2;
    u8 level : 7;
    u8 noPrintGender : 1;
    u8 gender : 2;
    u8 pokeball : 6;
    u32 otId;
    u32 exp;
    u32 curLevelExp;
    u32 nextLevelExp;
    u16 hp;
    u16 maxHp;
    u16 attack;
    u16 defense;
    u16 spAttack;
    u16 spDefense;
    u16 speed;
    u8 ability;
    u8 nature;
    u16 moves[4];
    u8 pp[4];
    u8 maxPP[4];
    u8 otGender;
    u8 contestStats[6];
    u8 favoriteFlavor;
    u16 markings;
    u16 form;
    u32 status : 28;
    u32 isEgg : 1;
    u32 shiny : 1;
    u32 pokerus : 2;
    u32 ribbons[4];
    u8 shinyLeaves[6];
} PokemonSummaryMon;

typedef struct ConditionChartVertex {
    s16 x, y, z;
} ConditionChartVertex;

typedef struct ConditionChartQuad {
    ConditionChartVertex vertices[4];
} ConditionChartQuad;

u8 sub_02089698(PokemonSummary *summary);

enum PokemonSummaryState sub_02088B08(PokemonSummary *summary);

enum PokemonSummaryState sub_02088B40(PokemonSummary *summary);

enum PokemonSummaryState sub_02088D18(PokemonSummary *summary);

enum PokemonSummaryState sub_02088D34(PokemonSummary *summary);

enum PokemonSummaryState sub_02088D48(PokemonSummary *summary);

enum PokemonSummaryState sub_02088E68(PokemonSummary *summary);

enum PokemonSummaryState sub_02088E98(PokemonSummary *summary);

enum PokemonSummaryState sub_02089028(PokemonSummary *summary);

enum PokemonSummaryState sub_02089208(PokemonSummary *arg);

enum PokemonSummaryState sub_020892F4(PokemonSummary *summary);

enum PokemonSummaryState sub_02089308(PokemonSummary *summary);

enum PokemonSummaryState sub_0208931C(PokemonSummary *summary);

enum PokemonSummaryState sub_0208942C(PokemonSummary *summary);

enum PokemonSummaryState sub_02089454(PokemonSummary *summary);

enum PokemonSummaryState sub_02089478(PokemonSummary *summary);

enum PokemonSummaryState sub_02089608(PokemonSummary *summary);

enum PokemonSummaryState sub_02089658(PokemonSummary *summary);

enum PokemonSummaryState sub_02089670(PokemonSummary *summary);

u8 sub_02089680(PokemonSummary *summary);

enum PokemonSummaryState sub_02089794(PokemonSummary *summary);

enum PokemonSummaryState PokemonSummary_HandleTouchInput(PokemonSummary *arg);

enum PokemonSummaryState PokemonSummary_StartButtonFlash(PokemonSummary *summary, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u8 restorePalette, u8 nextState);

enum PokemonSummaryState PokemonSummary_FlashCancelButton(PokemonSummary *summary, u8 nextState);

enum PokemonSummaryState PokemonSummary_FlashConfirmButton(PokemonSummary *summary, u8 nextState);

enum PokemonSummaryState PokemonSummary_FlashSwapButton(PokemonSummary *summary, u8 nextState);

enum PokemonSummaryState PokemonSummary_FlashMonButton(PokemonSummary *summary, int which, u8 nextState);

enum PokemonSummaryState PokemonSummary_FlashRibbonCancelButton(PokemonSummary *summary, u8 nextState);

BOOL PokemonSummary_Main(OverlayManager *manager, int *state);

BOOL PokemonSummary_Main(OverlayManager *manager, int *state) {
    PokemonSummary *summary = OverlayManager_GetData(manager);

    switch (*state) {
    case POKEMON_SUMMARY_STATE_START_FADE_IN:
        App_StartScreenFade(0, HEAP_ID_19);
        Pokepic_SetAttr(summary->pokepic, POKEPIC_VANISHED, 0);
        *state = POKEMON_SUMMARY_STATE_WAIT_FADE_IN;
        break;
    case POKEMON_SUMMARY_STATE_WAIT_FADE_IN:
        *state = sub_02088B08(summary);
        break;
    case POKEMON_SUMMARY_STATE_INPUT:
        *state = sub_02088B40(summary);
        break;
    case POKEMON_SUMMARY_STATE_SLIDE_IN_MOVE_INFO:
        *state = sub_02088D18(summary);
        break;
    case POKEMON_SUMMARY_STATE_SLIDE_OUT_MOVE_INFO:
        *state = sub_02088D34(summary);
        break;
    case POKEMON_SUMMARY_STATE_MOVE_SELECT_INPUT:
        *state = sub_02088D48(summary);
        break;
    case POKEMON_SUMMARY_STATE_START_MOVE_SWAP:
        *state = sub_02088E68(summary);
        break;
    case POKEMON_SUMMARY_STATE_MOVE_SWAP_INPUT:
        *state = sub_02088E98(summary);
        break;
    case POKEMON_SUMMARY_STATE_FORGET_MOVE_INPUT:
        *state = sub_02089028(summary);
        break;
    case POKEMON_SUMMARY_STATE_FORGET_MOVE_CONFIRM_INPUT:
        *state = sub_02089208(summary);
        break;
    case POKEMON_SUMMARY_STATE_OPEN_RIBBON_LIST:
        *state = sub_020892F4(summary);
        break;
    case POKEMON_SUMMARY_STATE_CLOSE_RIBBON_LIST:
        *state = sub_02089308(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_INPUT:
        *state = sub_0208931C(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_PAGE_UP:
        *state = sub_0208942C(summary);
        break;
    case POKEMON_SUMMARY_STATE_RIBBON_PAGE_DOWN:
        *state = sub_02089454(summary);
        break;
    case POKEMON_SUMMARY_STATE_FLASH_BUTTON:
        *state = sub_02089698(summary);
        break;
    case POKEMON_SUMMARY_STATE_UPDATE_CONDITION:
        *state = sub_02089478(summary);
        break;
    case POKEMON_SUMMARY_STATE_SHOW_CONDITION_CHANGES:
        *state = sub_02089608(summary);
        break;
    case POKEMON_SUMMARY_STATE_WAIT_CONDITION_EXIT:
        *state = sub_02089658(summary);
        break;
    case POKEMON_SUMMARY_STATE_START_FADE_OUT:
        *state = sub_02089670(summary);
        break;
    case POKEMON_SUMMARY_STATE_WAIT_FADE_OUT:
        if (sub_02089680(summary) == 1) {
            return TRUE;
        }
        break;
    case POKEMON_SUMMARY_STATE_SHOW_POKEPIC:
        Pokepic_SetAttr(summary->pokepic, POKEPIC_VANISHED, 0);
        *state = POKEMON_SUMMARY_STATE_INPUT;
        break;
    case POKEMON_SUMMARY_STATE_CHANGE_MON_BY_BUTTON:
        *state = sub_02089794(summary);
        break;
    }
    sub_0208B278(summary);
    sub_0208C3C0(summary);
    SpriteSystem_DrawSprites(summary->spriteManager);
    sub_0208DEDC(summary);
    return FALSE;
}
