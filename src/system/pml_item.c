// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pml_item.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "pml/item.h"
#include "pml/poke_party.h"
#include "system/shooter_item.h"

struct ItemParams {
    u8 sleepHeal : 1;
    u8 poisonHeal : 1;
    u8 burnHeal : 1;
    u8 freezeHeal : 1;
    u8 paralysisHeal : 1;
    u8 confusionHeal : 1;
    u8 infatuationHeal : 1;
    u8 guardSpec : 1;
    u8 revive : 1;
    u8 reviveAll : 1;
    u8 levelUp : 1;
    u8 evolve : 1;
    u8 attackStages : 4;
    u8 defenseStages : 4;
    u8 spAttackStages : 4;
    u8 spDefenseStages : 4;
    u8 speedStages : 4;
    u8 accuracyStages : 4;
    u8 critStages : 2;
    u8 ppUp : 1;
    u8 ppMax : 1;
    u8 ppRestore : 1;
    u8 ppRestoreAll : 1;
    u8 hpRestore : 1;
    u8 hpEVUp : 1;
    u8 attackEVUp : 1;
    u8 defenseEVUp : 1;
    u8 speedEVUp : 1;
    u8 spAttackEVUp : 1;
    u8 spDefenseEVUp : 1;
    u8 friendshipUp1 : 1;
    u8 friendshipUp2 : 1;
    u8 friendshipUp3 : 1;
    u8 unk6_4 : 1;
    s8 hpEV;
    s8 attackEV;
    s8 defenseEV;
    s8 speedEV;
    s8 spAttackEV;
    s8 spDefenseEV;
    u8 hpRestoreAmount;
    u8 ppRestoreAmount;
    s8 friendship1;
    s8 friendship2;
    s8 friendship3;
};

// The item's file: its fields, then its work, which is a value or the item's params
struct ItemData {
    u16 price;
    u8 holdEffect;
    u8 holdParam;
    u8 pluckEffect;
    u8 flingEffect;
    u8 flingPower;
    u8 naturalGiftPower;
    u16 naturalGiftType : 5;
    u16 important : 1;
    u16 registrable : 1;
    u16 fieldPocket : 4;
    u16 battlePocket : 5;
    u8 fieldFunc;
    u8 battleFunc;
    u8 workType;
    u8 kind;
    u8 unkE;
    u8 sortIndex;
    union {
        u8 value;
        struct ItemParams params;
    } work;
};

// Item data. The file's name is a guess: the ROM has no string for it. The tables are named by swan except
// data_0209e9e8, which is ours

// The icon files of no item and of the return arrow
#define ICON_NONE_CHAR 0x3fd
#define ICON_NONE_PLTT 0x3fe
#define ICON_RETURN_CHAR 0x3ff
#define ICON_RETURN_PLTT 0x400

// The item names' and descriptions' message files in ARCID_SYSTEM_MESSAGE
#define MSG_ITEM_DESCRIPTIONS TEXT_BANK_ITEM_DESCRIPTIONS
#define MSG_ITEM_NAMES TEXT_BANK_ITEM_NAMES

// Where the HMs and TM93 to TM95 are in TM_MOVE_LIST
#define TM_INDEX_HM01 92
#define TM_INDEX_TM93 98
#define HM_COUNT 6

s32 PML_ItemGetBattleStat(ItemParams *params, u32 param);
static u16 PML_ItemMonsBallConvID(u16 id, u32 dir);

static const u16 MAIL_ITEM_IDS[] = {
    ITEM_GREET_MAIL, ITEM_FAVORED_MAIL,  ITEM_RSVP_MAIL,     ITEM_THANKS_MAIL,   ITEM_INQUIRY_MAIL,  ITEM_LIKE_MAIL,
    ITEM_REPLY_MAIL, ITEM_BRIDGE_MAIL_S, ITEM_BRIDGE_MAIL_D, ITEM_BRIDGE_MAIL_T, ITEM_BRIDGE_MAIL_V, ITEM_BRIDGE_MAIL_M,
};

// Each ball item and its ball ID
static const u16 MONS_BALL_ITEMS[][2] = {
    { ITEM_MASTER_BALL, 1 }, { ITEM_ULTRA_BALL, 2 },   { ITEM_GREAT_BALL, 3 },   { ITEM_POKE_BALL, 4 },
    { ITEM_SAFARI_BALL, 5 }, { ITEM_NET_BALL, 6 },     { ITEM_DIVE_BALL, 7 },    { ITEM_NEST_BALL, 8 },
    { ITEM_REPEAT_BALL, 9 }, { ITEM_TIMER_BALL, 10 },  { ITEM_LUXURY_BALL, 11 }, { ITEM_PREMIER_BALL, 12 },
    { ITEM_DUSK_BALL, 13 },  { ITEM_HEAL_BALL, 14 },   { ITEM_QUICK_BALL, 15 },  { ITEM_CHERISH_BALL, 16 },
    { ITEM_FAST_BALL, 17 },  { ITEM_LEVEL_BALL, 18 },  { ITEM_LURE_BALL, 19 },   { ITEM_HEAVY_BALL, 20 },
    { ITEM_LOVE_BALL, 21 },  { ITEM_FRIEND_BALL, 22 }, { ITEM_MOON_BALL, 23 },   { ITEM_SPORT_BALL, 24 },
    { ITEM_DREAM_BALL, 25 },
};

static const u16 BERRY_ITEM_IDS[] = {
    ITEM_CHERI_BERRY,  ITEM_CHESTO_BERRY, ITEM_PECHA_BERRY,  ITEM_RAWST_BERRY,  ITEM_ASPEAR_BERRY, ITEM_LEPPA_BERRY,
    ITEM_ORAN_BERRY,   ITEM_PERSIM_BERRY, ITEM_LUM_BERRY,    ITEM_SITRUS_BERRY, ITEM_FIGY_BERRY,   ITEM_WIKI_BERRY,
    ITEM_MAGO_BERRY,   ITEM_AGUAV_BERRY,  ITEM_IAPAPA_BERRY, ITEM_RAZZ_BERRY,   ITEM_BLUK_BERRY,   ITEM_NANAB_BERRY,
    ITEM_WEPEAR_BERRY, ITEM_PINAP_BERRY,  ITEM_POMEG_BERRY,  ITEM_KELPSY_BERRY, ITEM_QUALOT_BERRY, ITEM_HONDEW_BERRY,
    ITEM_GREPA_BERRY,  ITEM_TAMATO_BERRY, ITEM_CORNN_BERRY,  ITEM_MAGOST_BERRY, ITEM_RABUTA_BERRY, ITEM_NOMEL_BERRY,
    ITEM_SPELON_BERRY, ITEM_PAMTRE_BERRY, ITEM_WATMEL_BERRY, ITEM_DURIN_BERRY,  ITEM_BELUE_BERRY,  ITEM_OCCA_BERRY,
    ITEM_PASSHO_BERRY, ITEM_WACAN_BERRY,  ITEM_RINDO_BERRY,  ITEM_YACHE_BERRY,  ITEM_CHOPLE_BERRY, ITEM_KEBIA_BERRY,
    ITEM_SHUCA_BERRY,  ITEM_COBA_BERRY,   ITEM_PAYAPA_BERRY, ITEM_TANGA_BERRY,  ITEM_CHARTI_BERRY, ITEM_KASIB_BERRY,
    ITEM_HABAN_BERRY,  ITEM_COLBUR_BERRY, ITEM_BABIRI_BERRY, ITEM_CHILAN_BERRY, ITEM_LIECHI_BERRY, ITEM_GANLON_BERRY,
    ITEM_SALAC_BERRY,  ITEM_PETAYA_BERRY, ITEM_APICOT_BERRY, ITEM_LANSAT_BERRY, ITEM_STARF_BERRY,  ITEM_ENIGMA_BERRY,
    ITEM_MICLE_BERRY,  ITEM_CUSTAP_BERRY, ITEM_JABOCA_BERRY, ITEM_ROWAP_BERRY,
};

// The icons of the Wonder Launcher's items, in ShooterItem_GetIndex's order: characters and palette
// pokebw2's LIST_ITEM_GRAPHICS
static const u16 data_0209e9e8[][2] = {
#include "asset/pml_item_data_0209e9e8.inc"
};

// The moves of TM01 to TM92, HM01 to HM06 and TM93 to TM95, then 0
static const u16 TM_MOVE_LIST[] = {
    MOVE_HONE_CLAWS,   MOVE_DRAGON_CLAW, MOVE_PSYSHOCK,     MOVE_CALM_MIND,    MOVE_ROAR,         MOVE_TOXIC,
    MOVE_HAIL,         MOVE_BULK_UP,     MOVE_VENOSHOCK,    MOVE_HIDDEN_POWER, MOVE_SUNNY_DAY,    MOVE_TAUNT,
    MOVE_ICE_BEAM,     MOVE_BLIZZARD,    MOVE_HYPER_BEAM,   MOVE_LIGHT_SCREEN, MOVE_PROTECT,      MOVE_RAIN_DANCE,
    MOVE_TELEKINESIS,  MOVE_SAFEGUARD,   MOVE_FRUSTRATION,  MOVE_SOLAR_BEAM,   MOVE_SMACK_DOWN,   MOVE_THUNDERBOLT,
    MOVE_THUNDER,      MOVE_EARTHQUAKE,  MOVE_RETURN,       MOVE_DIG,          MOVE_PSYCHIC,      MOVE_SHADOW_BALL,
    MOVE_BRICK_BREAK,  MOVE_DOUBLE_TEAM, MOVE_REFLECT,      MOVE_SLUDGE_WAVE,  MOVE_FLAMETHROWER, MOVE_SLUDGE_BOMB,
    MOVE_SANDSTORM,    MOVE_FIRE_BLAST,  MOVE_ROCK_TOMB,    MOVE_AERIAL_ACE,   MOVE_TORMENT,      MOVE_FACADE,
    MOVE_FLAME_CHARGE, MOVE_REST,        MOVE_ATTRACT,      MOVE_THIEF,        MOVE_LOW_SWEEP,    MOVE_ROUND,
    MOVE_ECHOED_VOICE, MOVE_OVERHEAT,    MOVE_ALLY_SWITCH,  MOVE_FOCUS_BLAST,  MOVE_ENERGY_BALL,  MOVE_FALSE_SWIPE,
    MOVE_SCALD,        MOVE_FLING,       MOVE_CHARGE_BEAM,  MOVE_SKY_DROP,     MOVE_INCINERATE,   MOVE_QUASH,
    MOVE_WILL_O_WISP,  MOVE_ACROBATICS,  MOVE_EMBARGO,      MOVE_EXPLOSION,    MOVE_SHADOW_CLAW,  MOVE_PAYBACK,
    MOVE_RETALIATE,    MOVE_GIGA_IMPACT, MOVE_ROCK_POLISH,  MOVE_FLASH,        MOVE_STONE_EDGE,   MOVE_VOLT_SWITCH,
    MOVE_THUNDER_WAVE, MOVE_GYRO_BALL,   MOVE_SWORDS_DANCE, MOVE_STRUGGLE_BUG, MOVE_PSYCH_UP,     MOVE_BULLDOZE,
    MOVE_FROST_BREATH, MOVE_ROCK_SLIDE,  MOVE_X_SCISSOR,    MOVE_DRAGON_TAIL,  MOVE_WORK_UP,      MOVE_POISON_JAB,
    MOVE_DREAM_EATER,  MOVE_GRASS_KNOT,  MOVE_SWAGGER,      MOVE_PLUCK,        MOVE_U_TURN,       MOVE_SUBSTITUTE,
    MOVE_FLASH_CANNON, MOVE_TRICK_ROOM,  MOVE_CUT,          MOVE_FLY,          MOVE_SURF,         MOVE_STRENGTH,
    MOVE_WATERFALL,    MOVE_DIVE,        MOVE_WILD_CHARGE,  MOVE_ROCK_SMASH,   MOVE_SNARL,        MOVE_NONE,
};

// Each item's icon: characters and palette
// pokebw2's ITEM_GRAPHICS_CHARS_AND_PALETTES
static const u16 data_0209eb6c[][2] = {
#include "asset/pml_item_data_0209eb6c.inc"
};

// pokebw2's PML_ItemGetIconArcID

u16 GetItemGraphicsDatID(u16 item, u32 type) {
    if (item > ITEM_LAST) {
        item = ITEM_NONE;
    }
    switch (type) {
    case ITEM_FILE_DATA:
        if (item != ITEM_NONE && item != 0xffff) {
            return item;
        }
        break;
    case ITEM_FILE_ICON_CHAR:
        if (item == ITEM_NONE) {
            return 0x3e9;
        }
        if (item == 0xffff) {
            return 0x3eb;
        }
        return data_0209eb6c[item][0];
    case ITEM_FILE_ICON_PLTT:
        if (item == ITEM_NONE) {
            return 0x3ea;
        }
        if (item == 0xffff) {
            return 0x3ec;
        }
        return data_0209eb6c[item][1];
    case ITEM_FILE_LIST_ICON_CHAR:
        return data_0209e9e8[func_0202f170(item)][0];
    case ITEM_FILE_LIST_ICON_PLTT:
        return data_0209e9e8[func_0202f170(item)][1];
    }
    return 0;
}

u32 func_02020f08(void) {
    return ARCID_ITEMGRA;
}

// pokebw2's PML_ItemGetIconCellDatID
u32 func_02020f0c(void) {
    return 1;
}

// pokebw2's PML_ItemGetIconAnimDatID
u32 func_02020f10(void) {
    return 0;
}

ArcTool *PML_ItemArcHandleCreate(HeapID heapId) {
    return GFL_ArcSysCreateFileHandle(ARCID_ITEMINFO, heapId);
}

ItemData *PML_ItemArcHandleReadFile(ArcTool *handle, u16 item, HeapID heapId) {
    if (item > ITEM_LAST) {
        item = ITEM_NONE;
    }
    return GFL_ArcToolReadHeapNew(handle, item, heapId);
}

void *PML_ItemReadDataFile(u16 item, u32 type, HeapID heapId) {
    if (item > ITEM_LAST) {
        item = ITEM_NONE;
    }
    switch (type) {
    case ITEM_FILE_DATA:
        return GFL_ArcSysReadHeapNew(ARCID_ITEMINFO, item, heapId);
    case ITEM_FILE_ICON_CHAR:
        return GFL_ArcSysReadHeapNew(ARCID_ITEMGRA, data_0209eb6c[item][0], heapId);
    case ITEM_FILE_ICON_PLTT:
        return GFL_ArcSysReadHeapNew(ARCID_ITEMGRA, data_0209eb6c[item][1], heapId);
    }
    return NULL;
}

void setItemNameToStrbuf(StrBuf *strbuf, u16 item, HeapID heapId) {
    MsgData *msgData;

    if (item > ITEM_LAST) {
        item = ITEM_NONE;
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, MSG_ITEM_NAMES, heapId);
    GFL_MsgDataLoadStrbuf(msgData, item, strbuf);
    GFL_MsgDataFree(msgData);
}

void setItemDescriptionTextToStrbuf(StrBuf *strbuf, u16 item, HeapID heapId) {
    MsgData *msgData;

    if (item > ITEM_LAST) {
        item = ITEM_NONE;
    }
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, MSG_ITEM_DESCRIPTIONS, heapId);
    GFL_MsgDataLoadStrbuf(msgData, item, strbuf);
    GFL_MsgDataFree(msgData);
}

s32 GetItemParam(u16 item, u16 param, HeapID heapId) {
    ItemData *data = PML_ItemReadDataFile(item, ITEM_FILE_DATA, HEAPID_TAIL(heapId));
    s32 value = PML_ItemGetParam(data, param);

    GFL_HeapFree(data);
    return value;
}

s32 PML_ItemGetParam(ItemData *data, u32 param) {
    switch (param) {
    case ITEM_PARAM_PRICE:
        return data->price * 10;
    case ITEM_PARAM_HOLD_EFFECT:
        return data->holdEffect;
    case ITEM_PARAM_HOLD_PARAM:
        return data->holdParam;
    case ITEM_PARAM_IMPORTANT:
        return data->important;
    case ITEM_PARAM_REGISTRABLE:
        return data->registrable;
    case ITEM_PARAM_FIELD_POCKET:
        return data->fieldPocket;
    case ITEM_PARAM_FIELD_FUNC:
        return data->fieldFunc;
    case ITEM_PARAM_BATTLE_FUNC:
        return data->battleFunc;
    case ITEM_PARAM_PLUCK_EFFECT:
        return data->pluckEffect;
    case ITEM_PARAM_FLING_EFFECT:
        return data->flingEffect;
    case ITEM_PARAM_FLING_POWER:
        return data->flingPower;
    case ITEM_PARAM_NATURAL_GIFT_POWER:
        return data->naturalGiftPower;
    case ITEM_PARAM_NATURAL_GIFT_TYPE:
        return data->naturalGiftType;
    case ITEM_PARAM_BATTLE_POCKET:
        return data->battlePocket;
    case ITEM_PARAM_WORK_TYPE:
        return data->workType;
    case ITEM_PARAM_KIND:
        return data->kind;
    case ITEM_PARAM_UNK_E:
        return data->unkE;
    case ITEM_PARAM_SORT_INDEX:
        return data->sortIndex;
    }
    switch (data->workType) {
    case ITEM_WORK_VALUE:
        return data->work.value;
    case ITEM_WORK_PARAMS:
        return PML_ItemGetBattleStat(&data->work.params, param);
    }
    return 0;
}

s32 PML_ItemGetBattleStat(ItemParams *params, u32 param) {
    switch (param) {
    case ITEM_PARAM_SLEEP_HEAL:
        return params->sleepHeal;
    case ITEM_PARAM_POISON_HEAL:
        return params->poisonHeal;
    case ITEM_PARAM_BURN_HEAL:
        return params->burnHeal;
    case ITEM_PARAM_FREEZE_HEAL:
        return params->freezeHeal;
    case ITEM_PARAM_PARALYSIS_HEAL:
        return params->paralysisHeal;
    case ITEM_PARAM_CONFUSION_HEAL:
        return params->confusionHeal;
    case ITEM_PARAM_INFATUATION_HEAL:
        return params->infatuationHeal;
    case ITEM_PARAM_GUARD_SPEC:
        return params->guardSpec;
    case ITEM_PARAM_REVIVE:
        return params->revive;
    case ITEM_PARAM_REVIVE_ALL:
        return params->reviveAll;
    case ITEM_PARAM_LEVEL_UP:
        return params->levelUp;
    case ITEM_PARAM_EVOLVE:
        return params->evolve;
    case ITEM_PARAM_ATTACK_STAGES:
        return params->attackStages;
    case ITEM_PARAM_DEFENSE_STAGES:
        return params->defenseStages;
    case ITEM_PARAM_SP_ATTACK_STAGES:
        return params->spAttackStages;
    case ITEM_PARAM_SP_DEFENSE_STAGES:
        return params->spDefenseStages;
    case ITEM_PARAM_SPEED_STAGES:
        return params->speedStages;
    case ITEM_PARAM_ACCURACY_STAGES:
        return params->accuracyStages;
    case ITEM_PARAM_CRIT_STAGES:
        return params->critStages;
    case ITEM_PARAM_PP_UP:
        return params->ppUp;
    case ITEM_PARAM_PP_MAX:
        return params->ppMax;
    case ITEM_PARAM_PP_RESTORE:
        return params->ppRestore;
    case ITEM_PARAM_PP_RESTORE_ALL:
        return params->ppRestoreAll;
    case ITEM_PARAM_HP_RESTORE:
        return params->hpRestore;
    case ITEM_PARAM_HP_EV_UP:
        return params->hpEVUp;
    case ITEM_PARAM_ATTACK_EV_UP:
        return params->attackEVUp;
    case ITEM_PARAM_DEFENSE_EV_UP:
        return params->defenseEVUp;
    case ITEM_PARAM_SPEED_EV_UP:
        return params->speedEVUp;
    case ITEM_PARAM_SP_ATTACK_EV_UP:
        return params->spAttackEVUp;
    case ITEM_PARAM_SP_DEFENSE_EV_UP:
        return params->spDefenseEVUp;
    case ITEM_PARAM_FRIENDSHIP_UP_1:
        return params->friendshipUp1;
    case ITEM_PARAM_FRIENDSHIP_UP_2:
        return params->friendshipUp2;
    case ITEM_PARAM_FRIENDSHIP_UP_3:
        return params->friendshipUp3;
    case ITEM_PARAM_UNK_51:
        return params->unk6_4;
    case ITEM_PARAM_HP_EV:
        return params->hpEV;
    case ITEM_PARAM_ATTACK_EV:
        return params->attackEV;
    case ITEM_PARAM_DEFENSE_EV:
        return params->defenseEV;
    case ITEM_PARAM_SPEED_EV:
        return params->speedEV;
    case ITEM_PARAM_SP_ATTACK_EV:
        return params->spAttackEV;
    case ITEM_PARAM_SP_DEFENSE_EV:
        return params->spDefenseEV;
    case ITEM_PARAM_HP_RESTORE_AMOUNT:
        return params->hpRestoreAmount;
    case ITEM_PARAM_PP_RESTORE_AMOUNT:
        return params->ppRestoreAmount;
    case ITEM_PARAM_FRIENDSHIP_1:
        return params->friendship1;
    case ITEM_PARAM_FRIENDSHIP_2:
        return params->friendship2;
    case ITEM_PARAM_FRIENDSHIP_3:
        return params->friendship3;
    }
    return 0;
}




BOOL PML_ItemIsTMHM(u16 item) {
    if ((item >= ITEM_TM01 && item <= ITEM_HM06) || (item >= ITEM_TM93 && item <= ITEM_TM95)) {
        return TRUE;
    }
    return FALSE;
}

u16 PML_ItemGetTMWazaID(u16 item) {
    if (PML_ItemIsTMHM(item) == FALSE) {
        return MOVE_NONE;
    }
    if (item >= ITEM_TM93) {
        item -= ITEM_TM93 - TM_INDEX_TM93;
    } else {
        item -= ITEM_TM01;
    }
    return TM_MOVE_LIST[item];
}

BOOL PML_MoveIsHM(u16 move) {
    u8 i;

    for (i = 0; i < HM_COUNT; i++) {
        if (move == TM_MOVE_LIST[TM_INDEX_HM01 + i]) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 PML_ItemGetTMBitMask(u16 item) {
    if (PML_ItemIsTMHM(item) == FALSE) {
        return 0xff;
    }
    // The bits run TM01 to TM92, then TM93 to TM95 and the HMs: the opposite of TM_MOVE_LIST's order
    if (item >= ITEM_TM93) {
        return item - ITEM_TM93 + 92;
    }
    if (item >= ITEM_HM01) {
        return item - ITEM_HM01 + 95;
    }
    return item - ITEM_TM01;
}

u8 PML_ItemGetHMID(u16 item) {
    if (item >= ITEM_HM01 && item <= ITEM_HM06) {
        return item - ITEM_HM01;
    }
    return 0xff;
}

BOOL PML_ItemIsMail(u16 item) {
    u32 i;

    for (i = 0; i < NELEMS(MAIL_ITEM_IDS); i++) {
        if (item == MAIL_ITEM_IDS[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 PML_ItemGetMailID(u16 item) {
    u32 i;

    for (i = 0; i < NELEMS(MAIL_ITEM_IDS); i++) {
        if (item == MAIL_ITEM_IDS[i]) {
            return i;
        }
    }
    return 0;
}

u16 PML_ItemGetMailItemID(u8 mail) {
    if (mail >= NELEMS(MAIL_ITEM_IDS)) {
        return ITEM_NONE;
    }
    return MAIL_ITEM_IDS[mail];
}

BOOL PML_ItemIsBerry(u16 item) {
    u32 i;

    for (i = 0; i < NELEMS(BERRY_ITEM_IDS); i++) {
        if (item == BERRY_ITEM_IDS[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PML_ItemIsNotSpecialMonsball(u16 item) {
    if ((u16)(item + 0xfe14) > 8)
        return TRUE;
    return FALSE;
}

// Converts a ball item to its ball ID (dir 0) or back (dir 1), or gives 0
static u16 PML_ItemMonsBallConvID(u16 id, u32 dir) {
    u32 i;

    for (i = 0; i < NELEMS(MONS_BALL_ITEMS); i++) {
        if (id == MONS_BALL_ITEMS[i][dir]) {
            return MONS_BALL_ITEMS[i][dir ^ 1];
        }
    }
    return 0;
}

u16 PML_ItemGetMonsBallID(u16 item) {
    return PML_ItemMonsBallConvID(item, 0);
}
