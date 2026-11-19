#include "global.h"

#include "field_communication_manager.h"
#include "unk_02037C94.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058098;

void sub_02058A78(void);

void sub_02058A78(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    sub_02037E9C();
    sub_020582F4(&sub_02058098, 0);
}
