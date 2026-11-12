// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_handler_work.h: the lines in use
// so far.
#ifndef BWPORT_BATTLE_BTL_HANDLER_WORK_H
#define BWPORT_BATTLE_BTL_HANDLER_WORK_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of raw, useItemNo, workPos, savedPos, prevResult, result, used and work
// in Black
struct BtlActionState {
    union {
        u32 raw;
        struct {
            u32 useItemNo : 10;
            // The top of the work stack, and where PushState saved it
            u32 workPos : 9;
            u32 savedPos : 9;
            u32 prevResult : 1;
            u32 result : 1;
            u32 used : 1;
            u32 unk31 : 1;
        };
    };
    // The stack of the handler commands' work
    u8 work[500];
};

u32 PushState(BtlActionState *state, u32 command);
u32 PushStateUseItem(BtlActionState *state, u16 item, u32 command);
u16 GetUseItemNo(BtlActionState *state);
BOOL IsUsed(BtlActionState *state);
void SetResult(BtlActionState *state, BOOL result);
BOOL GetPrevResult(BtlActionState *state);
void PopWork(BtlActionState *state, void *work);
void func_ov093_021cc8c8(BtlActionState *state);
BOOL func_ov093_021cc9a4(BtlActionState *state);
void *func_ov093_021cc9ac(BtlActionState *state, u32 command, u32 monId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void PopState(BtlActionState *state, u32 value, u32 command);

#endif // BWPORT_BATTLE_BTL_HANDLER_WORK_H
