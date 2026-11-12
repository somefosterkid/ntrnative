// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_field.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "battle/btl_field.h"
#include "battle/btl_main.h"
#include "battle/btl_ov169.h"
#include "battle/btl_pokeparam.h"
#include "gfl/heap.h"

// Overlay 167's btl_field.c (named by its string): the weather and the effects of the whole field, such as Trick
// Room and Gravity. The battle server's flow uses the static copy here; the main module allocates another for the
// clients. Function names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0) where it has them.

#define FIELD_EFFECT_COUNT 8
// Effect 3 is Imprison, whose user is the first of its depending Pokemon
#define FIELD_EFFECT_IMPRISON 3
#define FIELD_DEPEND_POKE_MAX 6
#define NO_MON 31
#define WEATHER_TURNS_PERMANENT 0xff

struct BtlField {
    u32 weather;
    u32 weatherTurns;
    // The effect's event handlers, from overlay 169
    void *events[FIELD_EFFECT_COUNT];
    BattleCondition conditions[FIELD_EFFECT_COUNT];
    u32 turnCounts[FIELD_EFFECT_COUNT];
    // The Pokemon the effect lasts for
    u32 dependPokes[FIELD_EFFECT_COUNT][FIELD_DEPEND_POKE_MAX];
    u32 dependPokeCounts[FIELD_EFFECT_COUNT];
    u32 active[FIELD_EFFECT_COUNT];
};

static void func_ov093_021ef83c(BtlField *field, u32 effect);
static void func_ov093_021ef988(BtlField *field, u32 weather);
static u32 func_ov093_021ef9b0(BtlField *field);
static u8 func_ov093_021ef9d0(BtlField *field);


static BtlField data_ov093_021f63f4;

// pokebw2's clearEffect
static void func_ov093_021ef83c(BtlField *field, u32 effect) {
    u32 i;

    field->events[effect] = NULL;
    field->conditions[effect] = ZeroConditionTurns();
    field->turnCounts[effect] = 0;
    field->dependPokeCounts[effect] = 0;
    field->active[effect] = 0;
    for (i = 0; i < FIELD_DEPEND_POKE_MAX; i++) {
        field->dependPokes[effect][i] = NO_MON;
    }
}

void func_ov093_021ef874(u32 weather) {
    func_ov093_021ef988(&data_ov093_021f63f4, weather);
}

// Function name from swan.
// pokebw2's GetFieldWeather
u8 func_ov093_021ef884(void) {
    return func_ov093_021ef9a8(&data_ov093_021f63f4);
}

u32 func_ov093_021ef894(void) {
    return func_ov093_021ef9b0(&data_ov093_021f63f4);
}

// Function name from swan.
void FieldStatusSetWeather(u8 weather, u8 duration) {
    func_ov093_021ef9c0(&data_ov093_021f63f4, weather, duration);
}

u8 func_ov093_021ef8b8(void) {
    return func_ov093_021ef9d0(&data_ov093_021f63f4);
}

// Function name from swan.
BOOL FieldStatusAddEffect(u32 effect, BattleCondition value) {
    return FieldStatusaddEffectCore(&data_ov093_021f63f4, effect, value, TRUE);
}

// Function name from swan.
BOOL FieldStatusRemoveEffect(u32 effect) {
    return func_ov093_021efa94(&data_ov093_021f63f4, effect);
}

// Function name from swan.
BOOL FieldStatusAddDependPoke(u32 effect, u8 monId) {
    return func_ov093_021efad8(&data_ov093_021f63f4, effect, monId);
}

void func_ov093_021ef90c(u8 monId) {
    func_ov093_021efb34(&data_ov093_021f63f4, monId);
}

BOOL func_ov093_021ef91c(BtlPokeCon *pokeCon, BattleMon *mon, u16 move) {
    return CheckImprison(&data_ov093_021f63f4, pokeCon, mon, move);
}

void func_ov093_021ef934(void (*callback)(u32 effect, BtlServerFlow *flow), BtlServerFlow *flow) {
    func_ov093_021efc78(&data_ov093_021f63f4, callback, flow);
}

// Function name from swan.
// pokebw2's IsFieldEffectActive
u32 func_ov093_021ef948(u32 fieldEffect) {
    return CheckFieldEffect(&data_ov093_021f63f4, fieldEffect);
}

BtlField *func_ov093_021ef958(HeapID heapId) {
    BtlField *field = GFL_HeapAllocate(heapId, sizeof(BtlField), TRUE, "btl_field.c", 0x10c);

    func_ov093_021ef988(field, 0);
    return field;
}

void func_ov093_021ef980(BtlField *field) {
    GFL_HeapFree(field);
}

// pokebw2's initField
static void func_ov093_021ef988(BtlField *field, u32 weather) {
    u32 i;

    for (i = 0; i < FIELD_EFFECT_COUNT; i++) {
        func_ov093_021ef83c(field, i);
    }
    field->weather = weather;
    field->weatherTurns = WEATHER_TURNS_PERMANENT;
}

u8 func_ov093_021ef9a8(BtlField *field) {
    return field->weather;
}

// pokebw2's getWeatherTurns
static u32 func_ov093_021ef9b0(BtlField *field) {
    if (field->weather != 0) {
        return field->weatherTurns;
    }
    return 0;
}

void func_ov093_021ef9c0(BtlField *field, u8 weather, u16 turns) {
    field->weather = weather;
    field->weatherTurns = turns;
}

void func_ov093_021ef9c8(BtlField *field) {
    field->weather = 0;
    field->weatherTurns = 0;
}

// Counts down a weather that lasts some turns, and returns the weather that ended, or 0
// pokebw2's decWeatherTurns
static u8 func_ov093_021ef9d0(BtlField *field) {
    if (field->weather != 0 && field->weatherTurns != WEATHER_TURNS_PERMANENT) {
        if (--field->weatherTurns == 0) {
            u8 weather = field->weather;

            field->weather = 0;
            return weather;
        }
    }
    return 0;
}

// Function name from swan.
BOOL FieldStatusaddEffectCore(BtlField *field, u32 effect, BattleCondition value, BOOL addEvent) {
    u32 i;
    u8 monId;

    if (field->active[effect] == 0) {
        if (addEvent) {
            field->events[effect] = FieldEffectEventAdd(effect, 0);
            if (field->events[effect] == NULL) {
                return FALSE;
            }
        }
        field->active[effect] = 1;
        field->conditions[effect] = value;
        field->turnCounts[effect] = 0;
        field->dependPokeCounts[effect] = 0;
        for (i = 0; i < FIELD_DEPEND_POKE_MAX; i++) {
            field->dependPokes[effect][i] = NO_MON;
        }
        monId = Condition_GetMonID(value);
        if (monId != NO_MON) {
            func_ov093_021efad8(field, effect, monId);
        }
        return TRUE;
    }
    return FALSE;
}




BOOL func_ov093_021efa94(BtlField *field, u32 effect) {
    if (CheckFieldEffect(field, effect)) {
        if (field->events[effect] != NULL) {
            data_ov095_06898081(field->events[effect]);
            field->events[effect] = NULL;
        }
        func_ov093_021ef83c(field, effect);
        return TRUE;
    }
    return FALSE;
}




BOOL func_ov093_021efad8(BtlField *field, u32 effect, u8 monId) {
    u32 count;
    u32 i;

    if (CheckFieldEffect(field, effect)) {
        count = field->dependPokeCounts[effect];
        if (count < FIELD_DEPEND_POKE_MAX) {
            for (i = 0; i < count; i++) {
                if (field->dependPokes[effect][i] == monId) {
                    return FALSE;
                }
            }
            field->dependPokes[effect][count] = monId;
            field->dependPokeCounts[effect] = count + 1;
            return TRUE;
        }
    }
    return FALSE;
}

// Removes a Pokemon from every effect that depends on it, ending the effects that no other Pokemon keeps up
void func_ov093_021efb34(BtlField *field, u8 monId) {
    unsigned long effect;
    u32 i;
    BOOL found;

    for (effect = 0; effect < FIELD_EFFECT_COUNT; effect++) {
        if (CheckFieldEffect(field, effect) && field->dependPokeCounts[effect] != 0) {
            found = FALSE;
            for (i = 0; i < field->dependPokeCounts[effect]; i++) {
                if (field->dependPokes[effect][i] == monId) {
                    found = TRUE;
                    break;
                }
            }
            if (found) {
                for (; i < FIELD_DEPEND_POKE_MAX - 1; i++) {
                    field->dependPokes[effect][i] = field->dependPokes[effect][i + 1];
                }
                field->dependPokes[effect][i] = NO_MON;
                if (--field->dependPokeCounts[effect] == 0) {
                    if (field->events[effect] != NULL) {
                        data_ov095_06898081(field->events[effect]);
                        field->events[effect] = NULL;
                    }
                    func_ov093_021ef83c(field, effect);
                } else if (monId == Condition_GetMonID(field->conditions[effect])) {
                    func_ov093_021e8ac8(&field->conditions[effect], (u8)field->dependPokes[effect][0]);
                }
            }
        }
    }
}




// Function name from swan.
BOOL CheckImprison(BtlField *field, BtlPokeCon *pokeCon, BattleMon *mon, u16 move) {
    u8 attackerId = GetMonID(mon);
    u8 monId;
    u32 count = field->dependPokeCounts[FIELD_EFFECT_IMPRISON];
    u32 i;

    for (i = 0; i < count; i++) {
        monId = field->dependPokes[FIELD_EFFECT_IMPRISON][i];
        if (!IsAllyMonID(attackerId, monId) && MoveIsUsable(GetPokeParamConst(pokeCon, monId), move)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Counts the turns of the timed effects, ending those whose turns ran out
void func_ov093_021efc78(BtlField *field, void (*callback)(u32 effect, BtlServerFlow *flow), BtlServerFlow *flow) {
    u32 effect;
    u8 turns;

    for (effect = 1; effect < FIELD_EFFECT_COUNT; effect++) {
        if (CheckFieldEffect(field, effect)) {
            turns = func_ov093_021e8afc(field->conditions[effect]);
            if (turns != 0 && ++field->turnCounts[effect] >= turns) {
                if (field->events[effect] != NULL) {
                    data_ov095_06898081(field->events[effect]);
                    field->events[effect] = NULL;
                }
                func_ov093_021ef83c(field, effect);
                if (callback != NULL) {
                    callback(effect, flow);
                }
            }
        }
    }
}




// Function name from swan.
BOOL CheckFieldEffect(BtlField *field, u32 effect) {
    return field->active[effect];
}
