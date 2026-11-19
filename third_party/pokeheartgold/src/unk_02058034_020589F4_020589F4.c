#include "global.h"

#include "field_communication_manager.h"
#include "unk_020379A0.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_0205838C;

extern s32 sub_020584BC;

void sub_020589F4(void);

void sub_020589F4(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    sub_02037AC0(0x62U);
    if (_021D41C8->isReturningFromBattle != 0) {
        sub_020582F4(&sub_0205838C, 0x1E);
        return;
    }
    sub_020582F4(&sub_020584BC, 0x1E);
}
