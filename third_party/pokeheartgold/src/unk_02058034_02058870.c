#include "global.h"

#include "field_communication_manager.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"

s32 sub_02056EA0(s32);

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058854;

void sub_02058870(void);

extern s32 sub_020588B4;
void sub_020588A0(void);

void sub_02058870(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    sub_02056EA0(0);
    sub_02037AC0(3U);
    sub_020582F4(&sub_02058854, 0);
}

void sub_020588A0(void) {
    sub_02037E38();
    sub_020582F4(&sub_020588B4, 2);
}
