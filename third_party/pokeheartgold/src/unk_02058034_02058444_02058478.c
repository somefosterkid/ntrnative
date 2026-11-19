#include "global.h"

#include "comm_player_manager.h"
#include "field_communication_manager.h"
#include "heap.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"

s32 sub_02057E50(void);

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058444;

void sub_02058478(void);

void sub_02058478(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    if (sub_0203769C() == 0) {
        if (sub_02057E50() != 0) {
            sub_02037AC0(0x1EU);
            goto block_6;
        }
    } else {
        sub_02037AC0(0x1EU);
block_6:
        sub_020582F4(&sub_02058444, 0x14);
    }
}

s32 sub_02057F80(void);

extern s32 sub_02058518;

void sub_020584BC(void);

void sub_020584BC(void) {
    u16 v0;

    if (sub_02037B38(0x62U) != 0) {
        sub_02056D7C(Heap_Alloc(HEAP_ID_15, sub_02057180()), _021D41C8->fieldSystem, 0);
        sub_02057F80();
        sub_02037AC0(0x5CU);
        sub_020582F4(&sub_02058518, 0);
        return;
    }
    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    _021D41C8->timer = 0x1EU;
    sub_02037AC0(0x62U);
}
