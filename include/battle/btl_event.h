// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_event.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_BATTLE_BTL_EVENT_H
#define BWPORT_BATTLE_BTL_EVENT_H

#include "types.h"
#include "stddef.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// Checks whether an item's handlers are skipped for the item being run: the checking item, the server flow, the
// checked item's factor type, the event, and the checked item's sub ID and mon
typedef BOOL (*BattleEventSkipCheckFn)(BattleEventItem *item, BtlServerFlow *flow, u32 factorType, u32 event, u16 subId,
                                       u8 monId);

BattleEventItem *BattleEvent_AddItem(u32 factorType, u16 subId, u32 priority, u32 subPriority, u8 monId,
                                     const BattleEventHandlerEntry *handlers, u16 numHandlers);
BattleEventItem *BattleEvent_GetNextItem(BattleEventItem *item);
void BattleEventItem_ConvertToIsolated(BattleEventItem *item);
u16 BattleEventItem_GetSubID(BattleEventItem *item);
u8 HandlerGetMainModule(BattleEventItem *handler);
u32 BattleEventItem_GetWorkValue(BattleEventItem *item, u32 index);
void BattleEventItem_SetTempItemFlag(BattleEventItem *item);
void BattleEventItem_SetRecallEnable(BattleEventItem *item);
void BattleEventItem_SetWorkValue(BattleEventItem *item, u8 index, u32 value);
void BattleEventItem_DetachSkipCheckHandler(BattleEventItem *item);
BattleEventItem *BattleEvent_SeekItem(u32 type, u32 monId);
void BattleEventItem_Remove(BattleEventItem *item);
void BattleEvent_ItemRotationSleep(u8 monId, u32 factorType);
BOOL BattleEvent_ItemRotationWake(u8 monId, u32 factorType);
uintptr_t BattleEventVar_GetValue(u32 key);
BOOL BattleEventVar_RewriteValue(u16 key, s32 value);
void BattleEventVar_MulValue(u16 key, s32 value);

// Black 2's, not checked against Black
// Called with the item, the server flow, the item's mon and its work
typedef void (*BattleEventHandlerFn)(BattleEventItem *item, BtlServerFlow *flow, u8 monId, s32 *work);

struct BattleEventHandlerEntry {
    u32 event;
    BattleEventHandlerFn handler;
};

// Whether the item's handlers are skipped for now
BOOL func_ov093_021e03e8(BattleEventItem *item, s32 *work);

void BattleEvent_RemoveIsolatedItems(void);
void BattleEventVar_Pop(u32 line);
void BattleEventVar_SetValue(u32 key, s32 value);
void BattleEventVar_SetConstValue(u32 key, intptr_t value);
void BattleEventVar_SetRewriteOnceValue(u32 key, s32 value);
void BattleEventVar_SetMulValue(u32 key, s32 value, s32 minValue, s32 maxValue);
void BattleEvent_ForceCallHandlers(BtlServerFlow *flow, u32 event);
void BattleEvent_CallHandlers(BtlServerFlow *flow, u32 event);
void func_ov093_021d6eac(void);
void func_ov093_021d6ee8(void);
void func_ov093_021d70fc(BtlServerFlow *flow, u32 event, u32 factorType);
void func_ov093_021d7394(u8 monId);
void func_ov093_021d73d4(u8 monId);

// Black 2's; twinned code confirms the offsets of prev, next, handlers, skipCheck, factorType, priority, callDepth,
// numHandlers, running, unk25, tempItem, removeReserved, recallEnable, active, sleeping, work, subId, monId and
// dependMonId in Black
// A registered handler table for an ability, item, move or other effect, 0x3c bytes; 0x84 are pooled
struct BattleEventItem {
    BattleEventItem *prev;
    BattleEventItem *next;
    const BattleEventHandlerEntry *handlers;
    BattleEventSkipCheckFn skipCheck;
    u32 factorType;
    // The priority level (7 minus the one it was added with) in the top byte, then the sub priority
    u32 priority;
    // The event call depth it was added at, which it isn't called at; its handlers aren't called while this is set
    // and deeper than the call
    u32 callDepth : 16;
    u32 numHandlers : 8;
    // Set while one of its handlers runs
    u32 running : 1;
    // Set until the turn's end, which func_ov167_021bc6f8 clears
    u32 unk25 : 1;
    u32 tempItem : 1;
    // Set when it was removed while running, to remove it once its handler returns
    u32 removeReserved : 1;
    // Lets its handlers be called again while one runs
    u32 recallEnable : 1;
    u32 active : 1;
    // Set while its mon is rotated out
    u32 sleeping : 1;
    u32 unk31 : 1;
    u32 work[7];
    u16 subId;
    u8 monId;
    u8 dependMonId;
};

// The event variables that handlers read and rewrite, a stack of frames that BattleEventVar_Push starts and
// BattleEventVar_Pop ends; keys[] holds 1 at the start of each frame
struct BattleEventVarStack {
    u32 sp;
    u16 keys[0x60];
    // A number, or an address: 0x3f is the BattleHandlerString * that handlers set a message in
    intptr_t values[0x60];
    s32 maxValues[0x60];
    s32 minValues[0x60];
    // 0, or 1 to be rewritten once (then 2), 3 to be multiplied and clamped, 4 constant
    u8 kinds[0x60];
};

BOOL BattleEventItem_IsIsolated(BattleEventItem *item);
void func_ov093_021d7108(BtlServerFlow *flow, u32 event, u32 factorType, BOOL checkSkip);
void func_ov093_021d713c(BtlServerFlow *flow, u32 event, u32 factorType, BOOL checkSkip);
BOOL func_ov093_021d7240(BtlServerFlow *flow, BattleEventItem *item, u32 event);
BattleEventItem *func_ov093_021d7474(void);
void func_ov093_021d7494(BattleEventItem *item);
void func_ov093_021d74bc(BattleEventItem *item);
void func_ov093_021d74c8(void);
BOOL func_ov093_021d778c(u16 key, s32 *value);
u32 func_ov093_021d77c4(BattleEventVarStack *vars, u32 key);
s32 func_ov093_021d77e8(BattleEventVarStack *vars, u16 key);
s32 func_ov093_021d780c(BattleEventVarStack *vars, u32 index, s32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void BattleEventItem_AttachSkipCheckHandler(BattleEventItem *item, BattleEventSkipCheckFn handler);
// The line is the caller's source line, which they ignore
void BattleEventVar_Push(u32 line);

void func_ov093_021d6f08(void);

#endif // BWPORT_BATTLE_BTL_EVENT_H
