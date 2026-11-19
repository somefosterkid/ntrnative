#include "global.h"

#include "overlay_49.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

s32 ov49_0225A08C(UnkStruct_ov70_0225DEE8 *, s32);

s32 ov49_0225A30C(UnkStruct_ov70_0225DEE8 *, s32, s32);

s32 ov49_0225EF8C(s32, s32);

void ov49_02262BF8(UnkStruct_ov70_02266AF0 *work, s32 slotMgr, UnkStruct_ov70_0225DEE8 *plaza, s32 msgId, s32 state) {
    ov49_0225A08C(plaza, ov49_0225A30C(plaza, 0, msgId));
    work->unk_04 = (s8)state;
    ov49_0225EF8C(slotMgr, 0x19);
}

void ov49_02262C20(UnkStruct_ov70_02266AF0 *work, s32 slotMgr, s8 state, s8 arg3) {
    work->unk_04 = state;
    work->unk_06 = arg3;
    work->unk_05 = 0;
    work->unk_0A = 0;
    ov49_0225EF8C(slotMgr, 0x1A);
}
