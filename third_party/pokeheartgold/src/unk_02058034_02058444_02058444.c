#include "global.h"

#include "field_communication_manager.h"
#include "unk_02035900.h"
#include "unk_020379A0.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058608;

void sub_02058444(void);

void sub_02058444(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    if (sub_02037B38(0x1EU) != 0) {
        sub_020374E4();
        sub_020582F4(&sub_02058608, 0);
    }
}
