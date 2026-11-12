// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btl_monset.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btl_monset.h"
#include "types.h"
#include "battle/btl_calc.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "gfl/std.h"

// pokebw2's BtlMonSet_Clear
void data_ov095_0689cb3d(BtlMonSet *set) {
    MI_CpuFill8(set, 0, sizeof(BtlMonSet));
}

// pokebw2's BtlMonSet_Add
void data_ov095_0689cb49(BtlMonSet *set, BattleMon *mon) {
    u32 i;
    u8 count = set->count;

    if (count < BTL_MONSET_MAX) {
        for (i = 0; i < count; i++) {
            if (set->mons[i] == mon) {
                return;
            }
        }
        set->mons[set->count] = mon;
        set->damageKind[set->count] = BTL_MONSET_NO_DAMAGE;
        set->damage[set->count] = 0;
        set->substituteDamage[set->count] = 0;
        set->count++;
        if (set->count > set->countMax) {
            set->countMax = set->count;
        }
    }
}

// pokebw2's BtlMonSet_AddDamage
void data_ov095_0689cbb9(BtlMonSet *set, BattleMon *mon, u16 damage, BOOL substitute) {
    u32 i;

    data_ov095_0689cb49(set, mon);
    for (i = 0; i < set->count; i++) {
        if (set->mons[i] == mon) {
            if (substitute) {
                set->substituteDamage[i] += damage;
                set->damageKind[i] = BTL_MONSET_SUBSTITUTE_DAMAGE;
            } else {
                set->damage[i] += damage;
                set->damageKind[i] = BTL_MONSET_HP_DAMAGE;
            }
        }
    }
}

// pokebw2's BtlMonSet_Remove
void data_ov095_0689cc15(BtlMonSet *set, BattleMon *mon) {
    u32 count = set->count;
    u32 i;
    u32 j;

    for (i = 0; i < count; i++) {
        if (set->mons[i] == mon) {
            for (j = i + 1; j < set->count; j++) {
                set->mons[j - 1] = set->mons[j];
            }
            set->count--;
            set->getIndex--;
            return;
        }
    }
}

// pokebw2's BtlMonSet_Get
BattleMon *data_ov095_0689cc71(BtlMonSet *set, u32 index) {
    if (index < set->count) {
        return set->mons[index];
    }
    return NULL;
}

// pokebw2's BtlMonSet_ResetIndex
void data_ov095_0689cc85(BtlMonSet *set) {
    set->getIndex = 0;
}

// pokebw2's BtlMonSet_GetNext
BattleMon *data_ov095_0689cc8d(BtlMonSet *set) {
    u8 index = set->getIndex;

    if (index < set->count) {
        set->getIndex++;
        return set->mons[index];
    }
    return NULL;
}

// pokebw2's BtlMonSet_GetTotalDamage
BOOL data_ov095_0689ccb5(const BtlMonSet *set, BattleMon *mon, u32 *damage) {
    u32 i;

    for (i = 0; i < set->count; i++) {
        if (set->mons[i] == mon && set->damageKind[i] != BTL_MONSET_NO_DAMAGE) {
            *damage = set->damage[i] + set->substituteDamage[i];
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BtlMonSet_GetHpDamage
BOOL data_ov095_0689ccf9(const BtlMonSet *set, BattleMon *mon, u32 *damage) {
    u32 i;

    for (i = 0; i < set->count; i++) {
        if (set->mons[i] == mon && set->damageKind[i] == BTL_MONSET_HP_DAMAGE) {
            *damage = set->damage[i];
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BtlMonSet_GetCount
u32 data_ov095_0689cd39(BtlMonSet *set) {
    return set->count;
}

// pokebw2's BtlMonSet_GetCountMax
u32 data_ov095_0689cd41(BtlMonSet *set) {
    return set->countMax;
}

// pokebw2's BtlMonSet_SetNumTargets
void data_ov095_0689cd49(BtlMonSet *set, u8 numTargets) {
    set->numTargets = numTargets;
}

// pokebw2's BtlMonSet_IsEmptyAfterTargeting
BOOL data_ov095_0689cd51(BtlMonSet *set) {
    if (set->countMax != 0 || set->numTargets != 0) {
        if (set->count == 0) {
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// pokebw2's BtlMonSet_CopyNotFainted
u32 data_ov095_0689cd79(BtlMonSet *set, BtlMonSet *dest) {
    u32 count;
    u32 i;
    BattleMon *mon;

    data_ov095_0689cb3d(dest);
    count = data_ov095_0689cd39(set);
    for (i = 0; i < count; i++) {
        mon = data_ov095_0689cc71(set, i);
        if (!IsFainted(mon)) {
            data_ov095_0689cb49(dest, mon);
        }
    }
    return data_ov095_0689cd39(dest);
}

// pokebw2's BtlMonSet_CopyAllies
u32 data_ov095_0689cdcd(BtlMonSet *set, BattleMon *mon, BtlMonSet *out) {
    u32 count;
    u32 i;
    BattleMon *other;
    u8 monId;

    data_ov095_0689cb3d(out);
    monId = GetMonID(mon);
    count = data_ov095_0689cd39(set);
    for (i = 0; i < count; i++) {
        other = data_ov095_0689cc71(set, i);
        if (IsAllyMonID(monId, GetMonID(other))) {
            data_ov095_0689cb49(out, other);
        }
    }
    return data_ov095_0689cd39(out);
}

// pokebw2's BtlMonSet_CopyOpponents
u32 data_ov095_0689ce59(BtlMonSet *set, BattleMon *mon, BtlMonSet *out) {
    u32 count;
    u32 i;
    BattleMon *other;
    u8 monId;

    data_ov095_0689cb3d(out);
    monId = GetMonID(mon);
    count = data_ov095_0689cd39(set);
    for (i = 0; i < count; i++) {
        other = data_ov095_0689cc71(set, i);
        if (!IsAllyMonID(monId, GetMonID(other))) {
            data_ov095_0689cb49(out, other);
        }
    }
    return data_ov095_0689cd39(out);
}

// pokebw2's BtlMonSet_RemoveFainted
void data_ov095_0689cee5(BtlMonSet *set) {
    BattleMon *mon;

    data_ov095_0689cc85(set);
    while ((mon = data_ov095_0689cc8d(set)) != NULL) {
        if (IsFainted(mon)) {
            data_ov095_0689cc15(set, mon);
        }
    }
}

void SortBySpeed(BtlMonSet *set, BtlServerFlow *flow) {
    u32 i;
    unsigned long j;

    if (set->count > 1) {
        for (i = 0; i < set->count; i++) {
            set->speed[i] = func_ov093_021c81c8(flow, set->mons[i], TRUE);
        }
        for (i = 0; i < set->count; i++) {
            for (j = i + 1; j < set->count; j++) {
                if (set->speed[j] > set->speed[i] || (set->speed[j] == set->speed[i] && BattleRandom(2))) {
                    BattleMon *mon = set->mons[i];
                    u16 damage = set->damage[i];
                    u16 speed = set->speed[i];

                    set->mons[i] = set->mons[j];
                    set->damage[i] = set->damage[j];
                    set->speed[i] = set->speed[j];
                    set->mons[j] = mon;
                    set->damage[j] = damage;
                    set->speed[j] = speed;
                }
            }
        }
    }
}
