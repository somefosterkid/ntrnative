#include "global.h"

#include "comm_player_manager.h"
#include "field_communication_manager.h"
#include "heap.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"

void sub_02058388(void);

void sub_02058388(void) {
}

s32 sub_02057F80(void);

s32 sub_020582F4(s32 *, u16);

extern s32 sub_020583EC;

void sub_0205838C(void);

void sub_0205838C(void) {
    u16 v0;

    if (sub_02037B38(0x62U) != 0) {
        sub_02056D7C(Heap_Alloc(HEAP_ID_15, sub_02057180()), _021D41C8->fieldSystem, 0);
        sub_02057F80();
        sub_02037504();
        sub_02037AC0(0x5CU);
        sub_020582F4(&sub_020583EC, 0);
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
