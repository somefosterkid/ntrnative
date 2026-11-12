// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov167/btl_handler_work.c, ported by tools/port_file.py.
#include "types.h"
#include "battle/btl_handler.h"
#include "battle/btl_handler_work.h"
#include "gfl/std.h"

void func_ov093_021cc8c8(BtlActionState *state) {
    state->raw = 0;
    MI_CpuFill8(state->work, 0, sizeof(state->work));
}

// Function name from swan.
u32 PushState(BtlActionState *state, u32 command) {
    u32 prev = state->raw;

    state->useItemNo = 0;
    state->savedPos = state->workPos;
    state->result = 0;
    state->prevResult = 0;
    state->used = 0;
    return prev;
}

// Function name from swan.
u32 PushStateUseItem(BtlActionState *state, u16 item, u32 command) {
    u32 prev = state->raw;

    state->useItemNo = item;
    state->savedPos = state->workPos;
    state->result = 0;
    state->prevResult = 0;
    state->used = 0;
    return prev;
}

// Function names from swan.
void PopState(BtlActionState *state, u32 value, u32 command) {
    state->raw = value;
}

u16 GetUseItemNo(BtlActionState *state) {
    return state->useItemNo;
}

BOOL IsUsed(BtlActionState *state) {
    return state->used;
}

void SetResult(BtlActionState *state, BOOL result) {
    if (result) {
        state->prevResult = 1;
        state->result = 1;
    } else {
        state->prevResult = 0;
    }
    state->used = 1;
}

BOOL GetPrevResult(BtlActionState *state) {
    return state->prevResult;
}

BOOL func_ov093_021cc9a4(BtlActionState *state) {
    return state->result;
}

// The size of each handler command's work: the parameters that BattleHandler_Execute's handler for it takes (none
// holds a pointer: the same sizes on a PC target)
typedef struct {
    u8 command;
    u8 size;
} BtlHandlerWorkSize;

static const BtlHandlerWorkSize data_ov093_021efe9c[59] = {
    { 0x00, sizeof(BattleHandlerUseHeldItemParam) },
    { 0x01, sizeof(BattleHandlerHeader) },
    { 0x05, sizeof(BattleHandlerRecoverHPParam) },
    { 0x06, sizeof(BattleHandlerDrainParam) },
    { 0x08, sizeof(BattleHandlerChangeHPParam) },
    { 0x09, sizeof(BattleHandlerPPParam) },
    { 0x0a, sizeof(BattleHandlerPPParam) },
    { 0x0b, sizeof(BattleHandlerCureConditionParam) },
    { 0x0c, sizeof(BattleHandlerAddConditionParam) },
    { 0x0e, sizeof(BattleHandlerStatChangeParam) },
    { 0x0f, sizeof(BattleHandlerSetStatStageParam) },
    { 0x12, sizeof(BattleHandlerRecoverStatStageParam) },
    { 0x10, sizeof(BattleHandlerResetStatStageParam) },
    { 0x11, sizeof(BattleHandlerSetStatusParam) },
    { 0x07, sizeof(BattleHandlerDamageParam) },
    { 0x13, sizeof(BattleHandlerFaintParam) },
    { 0x14, sizeof(BattleHandlerChangeTypeParam) },
    { 0x04, sizeof(BattleHandlerMessageParam) },
    { 0x02, sizeof(BattleHandlerHeader) },
    { 0x03, sizeof(BattleHandlerPopupParam) },
    { 0x15, sizeof(BattleHandlerFlagParam) },
    { 0x16, sizeof(BattleHandlerFlagParam) },
    { 0x17, sizeof(BattleHandlerFlagParam) },
    { 0x18, sizeof(BattleHandlerFlagParam) },
    { 0x19, sizeof(BattleHandlerAddSideEffectParam) },
    { 0x1a, sizeof(BattleHandlerRemoveSideEffectParam) },
    { 0x1b, sizeof(BattleHandlerAddFieldEffectParam) },
    { 0x1d, sizeof(BattleHandlerChangeWeatherParam) },
    { 0x1c, sizeof(BattleHandlerRemoveFieldEffectParam) },
    { 0x1e, sizeof(BattleHandlerPosEffectParam) },
    { 0x1f, sizeof(BattleHandlerAbilityChangeParam) },
    { 0x20, sizeof(BattleHandlerSetItemParam) },
    { 0x21, sizeof(BattleHandlerCheckHeldItemParam) },
    { 0x22, sizeof(BattleHandlerForceUseItemParam) },
    { 0x23, sizeof(BattleHandlerConsumeItemParam) },
    { 0x24, sizeof(BattleHandlerSwapItemParam) },
    { 0x25, sizeof(BattleHandlerUpdateMoveParam) },
    { 0x26, sizeof(BattleHandlerSetCounterParam) },
    { 0x27, sizeof(BattleHandlerDelayMoveDamageParam) },
    { 0x28, sizeof(BattleHandlerQuitBattleParam) },
    { 0x29, sizeof(BattleHandlerSwitchParam) },
    { 0x2a, sizeof(BattleHandlerBatonPassParam) },
    { 0x2b, sizeof(BattleHandlerFlinchParam) },
    { 0x2c, sizeof(BattleHandlerReviveParam) },
    { 0x2d, sizeof(BattleHandlerSetWeightParam) },
    { 0x2e, sizeof(BattleHandlerForceSwitchParam) },
    { 0x2f, sizeof(BattleHandlerInterruptParam) },
    // BattleHandler_InterruptMove reads the move alone
    { 0x30, offsetof(BattleHandlerInterruptParam, string) },
    { 0x31, sizeof(BattleHandlerInterruptParam) },
    { 0x32, sizeof(BattleHandlerSwapPokeParam) },
    { 0x33, sizeof(BattleHandlerTransformParam) },
    { 0x34, sizeof(BattleHandlerIllusionBreakParam) },
    { 0x35, sizeof(BattleHandlerGravityCheckParam) },
    { 0x36, sizeof(BattleHandlerHideTurnParam) },
    { 0x37, sizeof(BattleHandlerEffectAtPosParam) },
    { 0x38, sizeof(BattleHandlerHeader) },
    { 0x39, sizeof(BattleHandlerChangeFormParam) },
    { 0x3a, sizeof(BattleHandlerMoveEffectParam) },
    { 0x3b, sizeof(BattleHandlerHeader) },
};

// Allocates a handler command's work on the stack, zeroed and with its header set up
void *func_ov093_021cc9ac(BtlActionState *state, u32 command, u32 monId) {
    u32 i;
    u8 *work;
    u32 pos;
    u32 size;
    BattleHandlerHeader *header;

    for (i = 0, size = 0; i < 59; i++) {
        if (command == data_ov093_021efe9c[i].command) {
            size = data_ov093_021efe9c[i].size;
            break;
        }
    }
    if (size != 0) {
        while (size & 3) {
            size++;
        }
        pos = state->workPos;
        if (pos + size <= sizeof(state->work)) {
            work = state->work;
            for (i = 0; i < size; i++) {
                state->work[state->workPos + i] = 0;
            }
            header = (BattleHandlerHeader *)&work[pos];
            header->command = command;
            header->size = size;
            header->monId = monId;
            header->unk23 = 0;
            header->unk26 = 1;
            state->workPos += size;
            return header;
        }
    }
    return NULL;
}

// Function name from swan.
void PopWork(BtlActionState *state, void *work) {
    BattleHandlerHeader *header = work;
    u32 pos = state->workPos;

    if (header->size <= pos) {
        u32 start = (u8 *)work - state->work;
        u32 end = start + header->size;
        if (end == pos) {
            state->workPos = pos - header->size;
        }
    }
}

