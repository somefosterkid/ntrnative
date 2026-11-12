// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/pass_power.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "save/high_link.h"
#include "constants/pokemon.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/vm.h"

// The pass powers in effect: one for each effect, with the time it has left, and what each does to the game's
// numbers. The file's name is a guess, from what it does: the ROM has no string for it. The functions' names are
// swan's

// The pass powers' table
#define ARCID_PASS_POWER 38

// A second of a pass power's time, and the HP that stands for all the party's HP and PP
#define PASS_POWER_TICKS_PER_SECOND 60
#define PASS_POWER_RESTORE_ALL 999

typedef struct {
    // The time left; at 1 the pass power is spent, and runs on until it is ended
    u32 timers[PASS_POWER_EFFECT_COUNT];
    u8 passPowers[PASS_POWER_EFFECT_COUNT];
    u16 values[PASS_POWER_EFFECT_COUNT];
    // The effect of the pass power bought last, or PASS_POWER_EFFECT_COUNT
    u8 paidEffect;
} PassPowerState;

static void PassPower_DeactivateAllEffects(void);
static void PassPower_DeactivateEffect(int effect);
void func_02010df4(void);
void func_02010fac(void);

// pokebw2's g_PassPowerState
static PassPowerState data_021461e8;


void func_02010db4(void)
{
    MI_CpuFill8(&data_021461e8, 0, 0x60);
    func_02010df4();
}

void PassPower_Update(void)
{
    PassPowerEffect i;

    for (i = 0; i < PASS_POWER_EFFECT_COUNT; i++) {
        if (data_021461e8.timers[i] > 1) {
            data_021461e8.timers[i]--;
        }
    }
    func_02010fac();
}

void func_02010df4(void)
{
    PassPowerEffect i;

    for (i = 0; i < PASS_POWER_EFFECT_COUNT; i++) {
        PassPower_DeactivateEffect(i);
    }
    data_021461e8.paidEffect = PASS_POWER_EFFECT_COUNT;
}

void func_02010e14(int passPower, void *data, u32 paid)
{
    const PassPowerData *entry;
    int effect;

    if (passPower < 0x30) {
        entry = &((const PassPowerData *)data)[passPower];
        effect = entry->effect;
        data_021461e8.passPowers[effect] = passPower;
        data_021461e8.timers[effect] = entry->minutes * 3600;
        data_021461e8.values[effect] = entry->value;
        if (data_021461e8.paidEffect == effect) {
            data_021461e8.paidEffect = PASS_POWER_EFFECT_COUNT;
        }
        if (paid == 1) {
            data_021461e8.paidEffect = effect;
        }
        switch (effect) {
        case PASS_POWER_EFFECT_ENCOUNTER_UP:
            PassPower_DeactivateEffect(PASS_POWER_EFFECT_ENCOUNTER_DOWN);
            break;
        case PASS_POWER_EFFECT_ENCOUNTER_DOWN:
            PassPower_DeactivateEffect(PASS_POWER_EFFECT_ENCOUNTER_UP);
            break;
        case PASS_POWER_EFFECT_EXP_UP:
            PassPower_DeactivateEffect(PASS_POWER_EFFECT_EXP_DOWN);
            break;
        case PASS_POWER_EFFECT_EXP_DOWN:
            PassPower_DeactivateEffect(PASS_POWER_EFFECT_EXP_UP);
            break;
        }
    }
}




u32 PassPower_GetUsedIDByPassPower(u32 passPower, void *data) {
    return data_021461e8.passPowers[((const PassPowerData *)data)[passPower].effect];
}

u32 PassPower_GetUsedIDByEffect(int effect) {
    return data_021461e8.passPowers[effect];
}

u32 PassPower_GetRemainingSeconds(int effect) {
    u32 seconds;

    if (data_021461e8.passPowers[effect] == PASS_POWER_NONE) {
        return 0;
    }
    seconds = data_021461e8.timers[effect] / PASS_POWER_TICKS_PER_SECOND;
    if (seconds == 0) {
        seconds = 1;
    }
    return seconds;
}

s32 func_02010eec(void)
{
    u32 seconds;

    if (data_021461e8.paidEffect == PASS_POWER_EFFECT_COUNT) {
        return 0;
    }
    seconds = data_021461e8.timers[data_021461e8.paidEffect] / PASS_POWER_TICKS_PER_SECOND;
    if (seconds == 0) {
        return 1;
    }
    return seconds;
}

u32 PassPower_GetDepletedID(void)
{
    PassPowerEffect i;

    for (i = 0; i < PASS_POWER_EFFECT_COUNT; i++) {
        if (data_021461e8.timers[i] == 1) {
            return data_021461e8.passPowers[i];
        }
    }
    return PASS_POWER_NONE;
}



void PassPower_Deactivate(u32 passPower, void *data) {
    PassPower_DeactivateEffect(((const PassPowerData *)data)[passPower].effect);
}

static void PassPower_DeactivateEffect(int effect)
{
    if (effect < 0xd) {
        data_021461e8.passPowers[effect] = 0x30;
        data_021461e8.timers[effect] = 0;
        if (data_021461e8.paidEffect == effect) {
            data_021461e8.paidEffect = 0xd;
        }
    }
}

void *PassPowerData_Create(HeapID heapId) {
    return GFL_ArcSysReadHeapNew(ARCID_PASS_POWER, 0, heapId);
}

void PassPowerData_Free(void *data) {
    GFL_HeapFree(data);
}

u8 PassPowerData_GetEffect(void *data, u32 passPower) {
    return ((const PassPowerData *)data)[passPower].effect;
}

u16 PassPowerData_GetPrice(void *data, u32 passPower) {
    return ((const PassPowerData *)data)[passPower].unk02;
}

// pokebw2's PassPower_Dummy
void func_02010fac(void) {
}

u32 PassPower_ApplyEncounter(u32 rate) {
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_ENCOUNTER_UP] != PASS_POWER_NONE) {
        rate = data_021461e8.values[PASS_POWER_EFFECT_ENCOUNTER_UP] * rate / 100;
        if (rate > 100) {
            rate = 100;
        }
    } else if (data_021461e8.passPowers[PASS_POWER_EFFECT_ENCOUNTER_DOWN] != PASS_POWER_NONE) {
        rate = data_021461e8.values[PASS_POWER_EFFECT_ENCOUNTER_DOWN] * rate / 100;
        if (rate > 100) {
            rate = 100;
        }
    }
    return rate;
}

u32 PassPower_ApplyHatch(u32 steps)
{
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_HATCH] != PASS_POWER_NONE) {
        steps = data_021461e8.values[PASS_POWER_EFFECT_HATCH] * steps / 100;
    }
    return steps;
}

int PassPower_ApplyBefriend(int friendship)
{
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_BEFRIEND] != PASS_POWER_NONE) {
        friendship += data_021461e8.values[PASS_POWER_EFFECT_BEFRIEND];
    }
    return friendship;
}

u32 PassPower_ApplyBargain(u32 price)
{
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_BARGAIN] != PASS_POWER_NONE) {
        price = data_021461e8.values[PASS_POWER_EFFECT_BARGAIN] * price / 100;
    }
    return price;
}

u32 PassPower_ApplyHPRestore(PokeParty *party)
{
    PartyPkm *pkm;
    u32 index;
    BOOL wasEncrypted;
    int maxHP;
    int hp;

    if (data_021461e8.passPowers[PASS_POWER_EFFECT_HP_RESTORE] != PASS_POWER_NONE) {
        if (data_021461e8.values[PASS_POWER_EFFECT_HP_RESTORE] == PASS_POWER_RESTORE_ALL) {
            PokeParty_RecoverAll(party);
            index = 0;
        } else {
            index = PokeParty_GetFirstBattleReady(party);
            pkm = PokeParty_GetPkm(party, index);
            wasEncrypted = PokeParty_DecryptPkm(pkm);
            maxHP = PokeParty_GetParam(pkm, PKM_PARAM_MAX_HP, NULL);
            hp = PokeParty_GetParam(pkm, PKM_PARAM_HP, NULL);
            hp += data_021461e8.values[PASS_POWER_EFFECT_HP_RESTORE];
            if (hp > maxHP) {
                hp = maxHP;
            }
            PokeParty_SetParam(pkm, PKM_PARAM_HP, hp);
            PokeParty_EncryptPkm(pkm, wasEncrypted);
        }
        PassPower_DeactivateEffect(PASS_POWER_EFFECT_HP_RESTORE);
        return index;
    }
    return 0xff;
}

u32 PassPower_ApplyPPRestore(PokeParty *party)
{
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_PP_RESTORE] != PASS_POWER_NONE) {
        u32 index = PokeParty_GetFirstBattleReady(party);
        PartyPkm *pkm = PokeParty_GetPkm(party, index);
        BOOL wasEncrypted = PokeParty_DecryptPkm(pkm);
        u32 i;

        for (i = 0; i < 4; i++) {
            u32 move = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1 + i, NULL);

            if (move != 0) {
                u32 maxPP = PML_MoveGetMaxPP(move, PokeParty_GetParam(pkm, PKM_PARAM_MOVE1_PP_UP + i, NULL));
                u32 pp = PokeParty_GetParam(pkm, PKM_PARAM_MOVE1_PP + i, NULL);

                pp += data_021461e8.values[PASS_POWER_EFFECT_PP_RESTORE];
                if (pp > maxPP) {
                    pp = maxPP;
                }
                PokeParty_SetParam(pkm, PKM_PARAM_MOVE1_PP + i, pp);
            }
        }
        PokeParty_EncryptPkm(pkm, wasEncrypted);
        PassPower_DeactivateEffect(PASS_POWER_EFFECT_PP_RESTORE);
        return index;
    }
    return 0xff;
}



u32 PassPower_ApplyEXP(u32 exp) {
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_EXP_UP] != PASS_POWER_NONE) {
        return data_021461e8.values[PASS_POWER_EFFECT_EXP_UP] * exp / 100;
    }
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_EXP_DOWN] != PASS_POWER_NONE) {
        return data_021461e8.values[PASS_POWER_EFFECT_EXP_DOWN] * exp / 100;
    }
    return exp;
}

u32 PassPower_ApplyPrizeMoney(u32 money)
{
    if (data_021461e8.passPowers[PASS_POWER_EFFECT_PRIZE_MONEY] != PASS_POWER_NONE) {
        money = data_021461e8.values[PASS_POWER_EFFECT_PRIZE_MONEY] * money / 100;
    }
    return money;
}

static inline s32 PassPower_ScaleRate(s32 rate, u16 value) {
    return (s64)value * rate / 100;
}

s32 PassPower_ApplyCapture(s32 rate)
{
    s64 result;

    if (data_021461e8.passPowers[PASS_POWER_EFFECT_CAPTURE] == PASS_POWER_NONE) {
        return rate;
    }
    result = data_021461e8.values[PASS_POWER_EFFECT_CAPTURE];
    result *= rate;
    result /= 100;
    return result;
}
