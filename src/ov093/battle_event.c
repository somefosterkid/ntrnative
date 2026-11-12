// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/battle_event.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_event.h"
#include "battle/btl_field.h"
#include "battle/btl_pokeparam.h"
#include "battle/btl_server_flow.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"

// The registered items, in priority order; depth counts the nested calls of the handlers
typedef struct {
    u32 depth;
    BattleEventItem *first;
    // How many of the pool's items are in use
    u32 poolCount;
} BattleEventList;

#define BATTLE_EVENT_ITEM_MAX 0x84
#define BATTLE_EVENT_VAR_MAX 0x60

BattleEventList data_ov093_021f3cb0;
BattleEventItem *data_ov093_021f3cbc[BATTLE_EVENT_ITEM_MAX];
BattleEventVarStack data_ov093_021f3ecc;
BattleEventItem data_ov093_021f4470[BATTLE_EVENT_ITEM_MAX];

// Function names from swan.
void func_ov093_021d6eac(void) {
    u32 i;
    BattleEventItem *item;

    for (i = 0; i < BATTLE_EVENT_ITEM_MAX; i++) {
        item = &data_ov093_021f4470[i];
        func_ov093_021d74bc(item);
        data_ov093_021f3cbc[i] = item;
    }
    data_ov093_021f3cb0.first = NULL;
    data_ov093_021f3cb0.poolCount = 0;
    data_ov093_021f3cb0.depth = 0;
    func_ov093_021d74c8();
}

void func_ov093_021d6ee8(void) {
    BattleEventItem *item;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        item->unk25 = FALSE;
    }
}

void func_ov093_021d6f08(void) {
}

BattleEventItem *BattleEvent_AddItem(u32 factorType, u16 subId, u32 priority, u32 subPriority, u8 monId,
                                     const BattleEventHandlerEntry *handlers, u16 numHandlers) {
    BattleEventItem *item = func_ov093_021d7474();
    BOOL usesMon;
    BattleEventItem *prev;
    BattleEventItem *cur;

    if (item != NULL) {
        item->priority = ((7 - priority) << 24) | subPriority;
        item->factorType = factorType;
        item->prev = NULL;
        item->next = NULL;
        item->handlers = handlers;
        item->numHandlers = numHandlers;
        item->subId = subId;
        item->running = FALSE;
        item->unk25 = FALSE;
        item->sleeping = FALSE;
        item->tempItem = FALSE;
        item->skipCheck = NULL;
        item->monId = monId;
        item->removeReserved = FALSE;
        item->recallEnable = FALSE;
        item->callDepth = data_ov093_021f3cb0.depth;
        item->active = TRUE;
        usesMon = FALSE;
        if (factorType == 0 || factorType == 4 || factorType == 5) {
            usesMon = TRUE;
        }
        if (usesMon) {
            item->dependMonId = monId;
        } else {
            item->dependMonId = 0x1f;
        }
        MI_CpuFill8(item->work, 0, sizeof(item->work));
        if (data_ov093_021f3cb0.first == NULL) {
            data_ov093_021f3cb0.first = item;
        } else if (item->priority > data_ov093_021f3cb0.first->priority) {
            data_ov093_021f3cb0.first->prev = item;
            item->next = data_ov093_021f3cb0.first;
            item->prev = NULL;
            data_ov093_021f3cb0.first = item;
        } else {
            prev = data_ov093_021f3cb0.first;
            for (cur = prev->next; cur != NULL; cur = cur->next) {
                if (item->priority > cur->priority) {
                    item->next = cur;
                    item->prev = cur->prev;
                    cur->prev->next = item;
                    cur->prev = item;
                    break;
                }
                prev = cur;
            }
            if (cur == NULL) {
                prev->next = item;
                item->prev = prev;
            }
        }
        func_ov093_021d6f08();
        return item;
    }
    return NULL;
}


void BattleEvent_RemoveIsolatedItems(void) {
    BattleEventItem *item = data_ov093_021f3cb0.first;
    BattleEventItem *next;

    while (item != NULL) {
        next = item->next;
        if (item->factorType == 6) {
            BattleEventItem_Remove(item);
        }
        item = next;
    }
}

void BattleEventItem_Remove(BattleEventItem *item) {
    if (item != NULL && item->active) {
        if (item->running) {
            item->removeReserved = TRUE;
            return;
        }
        if (item == data_ov093_021f3cb0.first) {
            data_ov093_021f3cb0.first = item->next;
        }
        if (item->prev != NULL) {
            item->prev->next = item->next;
        }
        if (item->next != NULL) {
            item->next->prev = item->prev;
        }
        func_ov093_021d6f08();
        func_ov093_021d7494(item);
    }
}

void BattleEventItem_ConvertToIsolated(BattleEventItem *item) {
    item->factorType = 6;
}

BOOL BattleEventItem_IsIsolated(BattleEventItem *item) {
    if (item->factorType == 6) {
        return TRUE;
    }
    return FALSE;
}

u16 BattleEventItem_GetSubID(BattleEventItem *item) {
    return item->subId;
}

u8 HandlerGetMainModule(BattleEventItem *handler) {
    return handler->dependMonId;
}

u32 BattleEventItem_GetWorkValue(BattleEventItem *item, u32 index) {
    return item->work[index];
}

void BattleEventItem_SetTempItemFlag(BattleEventItem *item) {
    item->tempItem = TRUE;
}

void BattleEventItem_SetRecallEnable(BattleEventItem *item) {
    if (item->running) {
        item->recallEnable = TRUE;
    }
}

void BattleEventItem_SetWorkValue(BattleEventItem *item, u8 index, u32 value) {
    item->work[index] = value;
}

void BattleEvent_ForceCallHandlers(BtlServerFlow *flow, u32 event) {
    func_ov093_021d7108(flow, event, 7, FALSE);
}

void BattleEvent_CallHandlers(BtlServerFlow *flow, u32 event) {
    func_ov093_021d7108(flow, event, 7, TRUE);
}

void func_ov093_021d70fc(BtlServerFlow *flow, u32 event, u32 factorType) {
    func_ov093_021d7108(flow, event, factorType, TRUE);
}

void func_ov093_021d7108(BtlServerFlow *flow, u32 event, u32 factorType, BOOL checkSkip) {
    BattleEventItem *item;

    data_ov093_021f3cb0.depth++;
    func_ov093_021d713c(flow, event, factorType, checkSkip);
    if (--data_ov093_021f3cb0.depth == 0) {
        item = data_ov093_021f3cb0.first;
        if (item != NULL) {
            do {
                item->callDepth = 0;
                item = item->next;
            } while (item != NULL);
        }
    }
}

// Calls the handlers of the event of every item of the factor type, or every item for 7
void func_ov093_021d713c(BtlServerFlow *flow, u32 event, u32 factorType, BOOL checkSkip) {
    BattleEventItem *item = data_ov093_021f3cb0.first;
    const BattleEventHandlerEntry *handlers;
    u32 i;
    BattleEventItem *next;

    while (item != NULL) {
        next = item->next;
        if ((!item->running || item->recallEnable) && !item->unk25 && !item->sleeping
            && (factorType == 7 || item->factorType == factorType)
            && (item->callDepth == 0 || item->callDepth < data_ov093_021f3cb0.depth) && item->active
            && (event != 0x73 || item->tempItem)) {
            handlers = item->handlers;
            for (i = 0; i < item->numHandlers; i++) {
                const BattleEventHandlerEntry *entry = &handlers[i];
                if (event == entry->event) {
                    if (!checkSkip || !func_ov093_021d7240(flow, item, event)) {
                        item->running = TRUE;
                        entry->handler(item, flow, item->monId, item->work);
                        if (item->recallEnable) {
                            item->recallEnable = FALSE;
                        } else {
                            item->running = FALSE;
                        }
                        if (item->removeReserved) {
                            if (next != NULL && !next->active) {
                                next = item->next;
                            }
                            BattleEventItem_Remove(item);
                        }
                    }
                    break;
                }
            }
        }
        if (next == NULL) {
            break;
        }
        if (next->active) {
            item = next;
        } else {
            item = item->next;
        }
    }
}

// Whether an item's handlers are skipped: its mon's ability is suppressed, its held item can't be used, or another
// item's skip check skips it
BOOL func_ov093_021d7240(BtlServerFlow *flow, BattleEventItem *item, u32 event) {
    BattleMon *mon = NULL;
    BattleEventItem *other;
    BattleMon *otherMon;

    if (item->dependMonId != 0x1f) {
        mon = GetBattleMon(flow, item->dependMonId);
    }
    if (item->factorType == 4 && mon != NULL && CheckCondition(mon, 0x10)) {
        return TRUE;
    }
    if (item->factorType == 5) {
        if (func_ov093_021ef948(7)) {
            return TRUE;
        }
        if (mon != NULL && CheckCondition(mon, 0x13)) {
            return TRUE;
        }
        if (mon != NULL && GetTurnFlag(mon, 9)) {
            return TRUE;
        }
    }
    for (other = data_ov093_021f3cb0.first; other != NULL; other = other->next) {
        if (other->skipCheck == NULL) {
            continue;
        }
        if (other->factorType == 4) {
            otherMon = GetBattleMon(flow, other->dependMonId);
            if (otherMon != NULL && CheckCondition(otherMon, 0x10)) {
                continue;
            }
        }
        if (other->sleeping) {
            continue;
        }
        if (other->skipCheck(other, flow, item->factorType, event, item->subId, item->monId)) {
            return TRUE;
        }
    }
    return FALSE;
}

void BattleEventItem_AttachSkipCheckHandler(BattleEventItem *item, BattleEventSkipCheckFn handler) {
    item->skipCheck = handler;
}

void BattleEventItem_DetachSkipCheckHandler(BattleEventItem *item) {
    item->skipCheck = NULL;
}

BattleEventItem *BattleEvent_SeekItem(u32 factorType, u32 monId) {
    BattleEventItem *item;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        if (item->factorType == factorType && item->monId == monId && !item->removeReserved) {
            return item;
        }
    }
    return NULL;
}

BattleEventItem *BattleEvent_GetNextItem(BattleEventItem *item) {
    u32 factorType;
    u8 monId;

    if (item != NULL) {
        factorType = item->factorType;
        monId = item->monId;
        for (item = item->next; item != NULL; item = item->next) {
            if (item->factorType == factorType && item->monId == monId && !item->removeReserved) {
                return item;
            }
        }
    }
    return NULL;
}

void func_ov093_021d7394(u8 monId) {
    BattleEventItem *item;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        if (item->factorType == 0 && item->dependMonId == monId && func_ov093_021e03e8(item, item->work)) {
            item->unk25 = TRUE;
        }
    }
}

void func_ov093_021d73d4(u8 monId) {
    BattleEventItem *item;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        if (item->factorType == 0 && item->dependMonId == monId) {
            item->unk25 = FALSE;
        }
    }
}

void BattleEvent_ItemRotationSleep(u8 monId, u32 factorType) {
    BattleEventItem *item;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        if (item->dependMonId == monId && item->factorType == factorType) {
            item->sleeping = TRUE;
        }
    }
}

BOOL BattleEvent_ItemRotationWake(u8 monId, u32 factorType) {
    BattleEventItem *item;
    BOOL woken = FALSE;

    for (item = data_ov093_021f3cb0.first; item != NULL; item = item->next) {
        if (item->dependMonId == monId && item->factorType == factorType) {
            item->sleeping = FALSE;
            woken = TRUE;
        }
    }
    return woken;
}

BattleEventItem *func_ov093_021d7474(void) {
    if (data_ov093_021f3cb0.poolCount == BATTLE_EVENT_ITEM_MAX) {
        return NULL;
    }
    return data_ov093_021f3cbc[data_ov093_021f3cb0.poolCount++];
}

void func_ov093_021d7494(BattleEventItem *item) {
    if (data_ov093_021f3cb0.poolCount != 0) {
        func_ov093_021d74bc(item);
        data_ov093_021f3cbc[--data_ov093_021f3cb0.poolCount] = item;
    }
}

void func_ov093_021d74bc(BattleEventItem *item) {
    MI_CpuFill8(item, 0, sizeof(BattleEventItem));
}

void func_ov093_021d74c8(void) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    u32 i;

    vars->sp = 0;
    for (i = 0; i < BATTLE_EVENT_VAR_MAX; i++) {
        vars->keys[i] = 0;
    }
}


void func_ov093_021d74e4(void) {
    if (data_ov093_021f3ecc.sp != 0) {
        func_ov093_021d74c8();
    }
}


void BattleEventVar_Push(u32 line) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    for (; vars->sp < BATTLE_EVENT_VAR_MAX; vars->sp++) {
        if (vars->keys[vars->sp] == 0) {
            break;
        }
    }
    if (vars->sp < BATTLE_EVENT_VAR_MAX - 1) {
        vars->keys[vars->sp++] = 1;
    } else {
        GFL_ASSERT(0);
    }
}

void BattleEventVar_Pop(u32 line) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    u16 i;

    if (vars->sp != 0) {
        for (i = vars->sp; i < BATTLE_EVENT_VAR_MAX && vars->keys[i] != 0;) {
            vars->keys[i++] = 0;
        }
        vars->sp--;
        vars->keys[vars->sp] = 0;
        if (vars->sp != 0) {
            while (vars->sp != 0) {
                vars->sp--;
                if (vars->keys[vars->sp] == 1) {
                    vars->sp++;
                    break;
                }
            }
        }
    }
}


void BattleEventVar_SetValue(u32 key, s32 value) {
    u32 index = func_ov093_021d77c4(&data_ov093_021f3ecc, key);

    data_ov093_021f3ecc.keys[index] = key;
    data_ov093_021f3ecc.values[index] = value;
    data_ov093_021f3ecc.minValues[index] = 0;
    data_ov093_021f3ecc.maxValues[index] = 0;
    data_ov093_021f3ecc.kinds[index] = 0;
}

void BattleEventVar_SetConstValue(u32 key, intptr_t value) {
    u32 index = func_ov093_021d77c4(&data_ov093_021f3ecc, key);

    data_ov093_021f3ecc.keys[index] = key;
    data_ov093_021f3ecc.values[index] = value;
    data_ov093_021f3ecc.minValues[index] = 0;
    data_ov093_021f3ecc.maxValues[index] = 0;
    data_ov093_021f3ecc.kinds[index] = 4;
}

void BattleEventVar_SetRewriteOnceValue(u32 key, s32 value) {
    u32 index = func_ov093_021d77c4(&data_ov093_021f3ecc, key);

    data_ov093_021f3ecc.keys[index] = key;
    data_ov093_021f3ecc.values[index] = value;
    data_ov093_021f3ecc.minValues[index] = 0;
    data_ov093_021f3ecc.maxValues[index] = 0;
    data_ov093_021f3ecc.kinds[index] = 1;
}

void BattleEventVar_SetMulValue(u32 key, s32 value, s32 minValue, s32 maxValue) {
    u32 index = func_ov093_021d77c4(&data_ov093_021f3ecc, key);

    data_ov093_021f3ecc.keys[index] = key;
    data_ov093_021f3ecc.values[index] = value;
    data_ov093_021f3ecc.minValues[index] = minValue;
    data_ov093_021f3ecc.maxValues[index] = maxValue;
    data_ov093_021f3ecc.kinds[index] = 3;
}

BOOL BattleEventVar_RewriteValue(u16 key, s32 value) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    s32 index = func_ov093_021d77e8(vars, key);

    if (index >= 0 && vars->kinds[index] <= 1) {
        vars->keys[index] = key;
        vars->values[index] = value;
        if (vars->kinds[index] == 1) {
            vars->kinds[index] = 2;
        }
        return TRUE;
    }
    return FALSE;
}

void BattleEventVar_MulValue(u16 key, s32 value) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    s32 index = func_ov093_021d77e8(vars, key);

    if (index >= 0 && vars->kinds[index] == 3) {
        vars->values[index] =
            func_ov093_021d780c(vars, index, FX_Mul(vars->values[index], value));
    }
}

uintptr_t BattleEventVar_GetValue(u32 key) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    u32 i;

    for (i = vars->sp; i < BATTLE_EVENT_VAR_MAX; i++) {
        if (key == vars->keys[i]) {
            return vars->values[i];
        }
        if (vars->keys[i] == 0) {
            break;
        }
    }
    return 0;
}

BOOL func_ov093_021d778c(u16 key, s32 *value) {
    BattleEventVarStack *vars = &data_ov093_021f3ecc;
    u32 i;

    for (i = vars->sp; i < BATTLE_EVENT_VAR_MAX; i++) {
        if (key == vars->keys[i]) {
            *value = vars->values[i];
            return TRUE;
        }
        if (vars->keys[i] == 0) {
            break;
        }
    }
    return FALSE;
}

// The slot of the frame to set the variable in: its own, or the first free one
u32 func_ov093_021d77c4(BattleEventVarStack *vars, u32 key) {
    u32 i;

    for (i = vars->sp; i < BATTLE_EVENT_VAR_MAX; i++) {
        if (vars->keys[i] == 0 || key == vars->keys[i]) {
            break;
        }
    }
    if (i >= BATTLE_EVENT_VAR_MAX) {
        i = 0;
    }
    return i;
}

s32 func_ov093_021d77e8(BattleEventVarStack *vars, u16 key) {
    u32 i;

    for (i = vars->sp; i < BATTLE_EVENT_VAR_MAX && vars->keys[i] != 0; i++) {
        if (key == vars->keys[i]) {
            return i;
        }
    }
    return -1;
}

// Clamps the value to the variable's range, if it has one
s32 func_ov093_021d780c(BattleEventVarStack *vars, u32 index, s32 value) {
    if (vars->maxValues[index] != 0 || vars->minValues[index] != 0) {
        if (vars->minValues[index] > value) {
            value = vars->minValues[index];
        }
        if (vars->maxValues[index] < value) {
            value = vars->maxValues[index];
        }
    }
    return value;
}
