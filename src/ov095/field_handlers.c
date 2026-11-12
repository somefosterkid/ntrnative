// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/field_handlers.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_event.h"
#include "battle/btl_ov169.h"
#include "constants/battle.h"
#include "constants/types.h"

// One entry of the field effect table: an effect and the function that returns its handlers
// Black 2's, not checked against Black
typedef struct FieldEffectEventAddEntry {
    u32 effect;
    const BattleEventHandlerEntry *(*eventAdd)(u32 *numHandlers);
} FieldEffectEventAddEntry;

static const BattleEventHandlerEntry *EventAddFieldWeather(u32 *numHandlers);
static void HandlerFieldWeather(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldTrickRoom(u32 *numHandlers);
static void HandlerFieldTrickRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldGravity(u32 *numHandlers);
static void HandlerFieldGravityAccuracy(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static void HandlerFieldGravityGrounded(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldWaterSport(u32 *numHandlers);
static void HandlerFieldWaterSport(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldMudSport(u32 *numHandlers);
static void HandlerFieldMudSport(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldWonderRoom(u32 *numHandlers);
static void HandlerFieldWonderRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);
static const BattleEventHandlerEntry *EventAddFieldImprison(u32 *numHandlers);
static const BattleEventHandlerEntry *EventAddFieldMagicRoom(u32 *numHandlers);
static void HandlerFieldMagicRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

// Each field effect and the function that returns its handlers
// pokebw2's sFieldEffectEventAddTable
static const FieldEffectEventAddEntry data_ov095_0689d650[] = {
    { FIELD_CONDITION_WEATHER, EventAddFieldWeather },
    { FIELD_CONDITION_TRICK_ROOM, EventAddFieldTrickRoom },
    { FIELD_CONDITION_GRAVITY, EventAddFieldGravity },
    { FIELD_CONDITION_IMPRISON, EventAddFieldImprison },
    { FIELD_CONDITION_WATER_SPORT, EventAddFieldWaterSport },
    { FIELD_CONDITION_MUD_SPORT, EventAddFieldMudSport },
    { FIELD_CONDITION_WONDER_ROOM, EventAddFieldWonderRoom },
    { FIELD_CONDITION_MAGIC_ROOM, EventAddFieldMagicRoom },
};

BattleEventItem *FieldEffectEventAdd(u32 effect, u8 monId) {
    u32 i;
    u32 numHandlers;
    const BattleEventHandlerEntry *handlers;

    for (i = 0; i < NELEMS(data_ov095_0689d650); i++) {
        if (effect == data_ov095_0689d650[i].effect) {
            handlers = data_ov095_0689d650[i].eventAdd(&numHandlers);
            return BattleEvent_AddItem(3, effect, 3, 0, monId, handlers, numHandlers);
        }
    }
    return NULL;
}

// pokebw2's FieldEffectEventRemove
void data_ov095_06898081(BattleEventItem *item) {
    BattleEventItem_Remove(item);
}

// pokebw2's sHandlersFieldWeather
static const BattleEventHandlerEntry data_ov095_0689d638[] = {
    { 0x47, HandlerFieldWeather },
};

static const BattleEventHandlerEntry *EventAddFieldWeather(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d638);
    return data_ov095_0689d638;
}

static void HandlerFieldWeather(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
}

// pokebw2's sHandlersFieldTrickRoom
static const BattleEventHandlerEntry data_ov095_0689d630[] = {
    { 0x13, HandlerFieldTrickRoom },
};

static const BattleEventHandlerEntry *EventAddFieldTrickRoom(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d630);
    return data_ov095_0689d630;
}

// Reverses the turn order
static void HandlerFieldTrickRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleEventVar_RewriteValue(0x4a, TRUE);
}

// pokebw2's sHandlersFieldGravity
static const BattleEventHandlerEntry data_ov095_0689d640[] = {
    { 0x34, HandlerFieldGravityAccuracy },
    { 0x3e, HandlerFieldGravityGrounded },
};

static const BattleEventHandlerEntry *EventAddFieldGravity(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d640);
    return data_ov095_0689d640;
}

// Raises accuracy by 5/3 (the ratio is in 4096ths)
static void HandlerFieldGravityAccuracy(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleEventVar_MulValue(0x35, 0x1ab8);
}

// Lets Ground moves hit Pokémon in the air
static void HandlerFieldGravityGrounded(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(0x16) == TYPE_GROUND) {
        BattleEventVar_RewriteValue(0x4b, TRUE);
    }
}

// pokebw2's sHandlersFieldWaterSport
static const BattleEventHandlerEntry data_ov095_0689d620[] = {
    { 0x38, HandlerFieldWaterSport },
};

static const BattleEventHandlerEntry *EventAddFieldWaterSport(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d620);
    return data_ov095_0689d620;
}

// Weakens Fire moves to 0x548/4096 (about a third)
static void HandlerFieldWaterSport(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(0x16) == TYPE_FIRE) {
        BattleEventVar_MulValue(0x31, 0x548);
    }
}

// pokebw2's sHandlersFieldMudSport
static const BattleEventHandlerEntry data_ov095_0689d610[] = {
    { 0x38, HandlerFieldMudSport },
};

static const BattleEventHandlerEntry *EventAddFieldMudSport(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d610);
    return data_ov095_0689d610;
}

// Weakens Electric moves to 0x548/4096 (about a third)
static void HandlerFieldMudSport(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    if (BattleEventVar_GetValue(0x16) == TYPE_ELECTRIC) {
        BattleEventVar_MulValue(0x31, 0x548);
    }
}

// pokebw2's sHandlersFieldWonderRoom
static const BattleEventHandlerEntry data_ov095_0689d628[] = {
    { 0x3a, HandlerFieldWonderRoom },
};

static const BattleEventHandlerEntry *EventAddFieldWonderRoom(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d628);
    return data_ov095_0689d628;
}

// Swaps Defense and Special Defense
static void HandlerFieldWonderRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
    BattleEventVar_RewriteValue(0x3d, BattleEventVar_GetValue(0x3d) + 1);
}

// Imprison does its work elsewhere and shares Magic Room's empty handler
// pokebw2's sHandlersFieldImprison
static const BattleEventHandlerEntry data_ov095_0689d618[] = {
    { 0, HandlerFieldMagicRoom },
};

static const BattleEventHandlerEntry *EventAddFieldImprison(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d618);
    return data_ov095_0689d618;
}

// pokebw2's sHandlersFieldMagicRoom
static const BattleEventHandlerEntry data_ov095_0689d608[] = {
    { 0, HandlerFieldMagicRoom },
};

static const BattleEventHandlerEntry *EventAddFieldMagicRoom(u32 *numHandlers) {
    *numHandlers = NELEMS(data_ov095_0689d608);
    return data_ov095_0689d608;
}

static void HandlerFieldMagicRoom(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work) {
}
