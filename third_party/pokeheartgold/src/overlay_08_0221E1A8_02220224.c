#include "global.h"
#include "sprite_resources_header.h"

#include "gf_gfx_planes.h"
#include "seal_case.h"
#include "overlay_08_battle_party.h"
#include "overlay_08_battle_sub_menu_cursor.h"
#include "struct_defs/resdat_narc_structs.h"
#include "string_util.h"
#include "battle/battle.h"
#include "overlay_41_sprite_resource_collection.h"
#include "battle/battle_party_context.h"

struct BattleSystem;

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

void String_Delete(String *string);

u32 GetMonData(Pokemon *mon, int param, void *dest);

BoxPokemon *Mon_GetBoxMon(Pokemon *mon);

void BufferBoxMonNickname(MessageFormat *template, u32 idx, BoxPokemon *boxMon);

enum ItemFileType {
    ITEM_FILE_TYPE_DATA,
    ITEM_FILE_TYPE_ICON,
    ITEM_FILE_TYPE_PALETTE,
    ITEM_FILE_TYPE_GBA,
};

enum ItemDataParam {
    ITEM_PARAM_PRICE,
    ITEM_PARAM_HOLD_EFFECT,
    ITEM_PARAM_EFFECT_PARAM,
    ITEM_PARAM_PREVENT_TOSS,
    ITEM_PARAM_CAN_REGISTER,
    ITEM_PARAM_FIELD_POCKET,
    ITEM_PARAM_FIELD_USE_FUNC,
    ITEM_PARAM_BATTLE_USE_CATEGORY,
    ITEM_PARAM_PLUCK_EFFECT,
    ITEM_PARAM_FLING_EFFECT,
    ITEM_PARAM_FLING_POWER,
    ITEM_PARAM_NATURAL_GIFT_POWER,
    ITEM_PARAM_NATURAL_GIFT_TYPE,
    ITEM_PARAM_BATTLE_POCKET,
    ITEM_PARAM_PARTY_USE,
    ITEM_PARAM_HEAL_SLEEP,
    ITEM_PARAM_HEAL_POISON,
    ITEM_PARAM_HEAL_BURN,
    ITEM_PARAM_HEAL_FREEZE,
    ITEM_PARAM_HEAL_PARALYSIS,
    ITEM_PARAM_HEAL_CONFUSION,
    ITEM_PARAM_HEAL_ATTRACT,
    ITEM_PARAM_GUARD_SPEC,
    ITEM_PARAM_REVIVE,
    ITEM_PARAM_REVIVE_ALL,
    ITEM_PARAM_LEVEL_UP,
    ITEM_PARAM_EVOLVE,
    ITEM_PARAM_ATK_STAGES,
    ITEM_PARAM_DEF_STAGES,
    ITEM_PARAM_SPATK_STAGES,
    ITEM_PARAM_SPDEF_STAGES,
    ITEM_PARAM_SPEED_STAGES,
    ITEM_PARAM_ACC_STAGES,
    ITEM_PARAM_CRIT_STAGES,
    ITEM_PARAM_PP_UP,
    ITEM_PARAM_PP_MAX,
    ITEM_PARAM_PP_RESTORE,
    ITEM_PARAM_PP_RESTORE_ALL,
    ITEM_PARAM_HP_RESTORE,
    ITEM_PARAM_GIVE_HP_EVS,
    ITEM_PARAM_GIVE_ATK_EVS,
    ITEM_PARAM_GIVE_DEF_EVS,
    ITEM_PARAM_GIVE_SPEED_EVS,
    ITEM_PARAM_GIVE_SPATK_EVS,
    ITEM_PARAM_GIVE_SPDEF_EVS,
    ITEM_PARAM_GIVE_FRIENDSHIP_LOW,
    ITEM_PARAM_GIVE_FRIENDSHIP_MED,
    ITEM_PARAM_GIVE_FRIENDSHIP_HIGH,
    ITEM_PARAM_HP_EVS,
    ITEM_PARAM_ATK_EVS,
    ITEM_PARAM_DEF_EVS,
    ITEM_PARAM_SPEED_EVS,
    ITEM_PARAM_SPATK_EVS,
    ITEM_PARAM_SPDEF_EVS,
    ITEM_PARAM_HP_RESTORED,
    ITEM_PARAM_PP_RESTORED,
    ITEM_PARAM_FRIENDSHIP_LOW,
    ITEM_PARAM_FRIENDSHIP_MED,
    ITEM_PARAM_FRIENDSHIP_HIGH,
};

Pokemon *BattleSystem_GetPartyMon(BattleSystem *battleSys, int battler, int slot);

enum HealedStatusEffectFlag {
    HEALED_STATUS_EFFECT_FLAG_SLEEP = 1,
    HEALED_STATUS_EFFECT_FLAG_POISON = 2,
    HEALED_STATUS_EFFECT_FLAG_BURN = 4,
    HEALED_STATUS_EFFECT_FLAG_FREEZE = 8,
    HEALED_STATUS_EFFECT_FLAG_PARALYSIS = 16,
    HEALED_STATUS_EFFECT_FLAG_CONFUSION = 32,
    HEALED_STATUS_EFFECT_FLAG_INFATUATION = 64,
};

void ov08_02220224(BattleParty *battleParty);

void ov08_02220224(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;
    Pokemon *mon;
    void *itemData = LoadItemDataOrGfx(context->selectedBattleBagItem, ITEM_FILE_TYPE_DATA, context->heapID);
    u16 newHP;
    u8 healedStatusEffectFlags;

    mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex]);
    newHP = GetMonData(mon, MON_DATA_HP, 0L);
    healedStatusEffectFlags = 0;

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_SLEEP) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_SLEEP;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_POISON) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_POISON;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_BURN) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_BURN;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_FREEZE) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_FREEZE;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_PARALYSIS) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_PARALYSIS;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_CONFUSION) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_CONFUSION;
    }

    if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_HEAL_ATTRACT) != 0) {
        healedStatusEffectFlags |= HEALED_STATUS_EFFECT_FLAG_INFATUATION;
    }

    if (battleParty->partyPokemon[context->selectedPartyIndex].curHP == 0 && newHP != 0) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 88);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (battleParty->partyPokemon[context->selectedPartyIndex].curHP != newHP) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 82);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        BufferIntegerAsString(battleParty->stringTemplate, 1, newHP - battleParty->partyPokemon[context->selectedPartyIndex].curHP, 3, PRINTING_MODE_LEFT_ALIGN, CHARSET_MODE_EN);
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_PP_RESTORE) != 0 || GetItemAttr_PreloadedItemData(itemData, ITEM_PARAM_PP_RESTORE_ALL) != 0) {
        ReadMsgDataIntoString(battleParty->messageLoader, 87, battleParty->string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_SLEEP) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 92);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_POISON) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 83);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_BURN) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 85);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_FREEZE) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 86);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_PARALYSIS) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 84);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_CONFUSION) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 90);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else if (healedStatusEffectFlags == HEALED_STATUS_EFFECT_FLAG_INFATUATION) {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 91);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    } else {
        String *string = NewString_ReadMsgData(battleParty->messageLoader, 89);
        BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
        StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
        String_Delete(string);
    }

    Heap_Free(itemData);
}

void ov08_0222057C(BattleParty *battleParty);

void ov08_0222057C(BattleParty *battleParty) {
    BattlePartyContext *context = battleParty->context;
    Pokemon *mon = BattleSystem_GetPartyMon(context->battleSys, context->battler, context->pokemonPartySlots[context->selectedPartyIndex]);
    String *string = NewString_ReadMsgData(battleParty->messageLoader, 95);

    BufferBoxMonNickname(battleParty->stringTemplate, 0, Mon_GetBoxMon(mon));
    BufferMoveName(battleParty->stringTemplate, 1, MOVE_EMBARGO);
    StringExpandPlaceholders(battleParty->stringTemplate, battleParty->string, string);
    String_Delete(string);
}

s32 ov08_0222061C(BattleParty *battleParty);

s32 ov08_02220668(BattleParty *);

s32 ov08_02220750(void *);

s32 ov08_02220800(void *);

s32 ov08_02220878(void *);

s32 ov08_02220928(void *);

s32 ov08_02220A28(void *);

s32 ov08_0222162C(void *);

void ov08_022205E0(BattleParty *battleParty);

void ov08_022205E0(BattleParty *battleParty) {
    ov08_0222061C(battleParty);
    ov08_02220668(battleParty);
    ov08_02220750(battleParty);
    ov08_02220800(battleParty);
    ov08_02220878(battleParty);
    ov08_02220928(battleParty);
    ov08_02220A28(battleParty);
    ov08_0222162C(battleParty);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
