#include "global.h"

#include "field_communication_manager.h"
#include "unk_02035900.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_020587E8;

void sub_0205882C(void);

void sub_0205882C(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    sub_02035FD8();
    sub_020582F4(&sub_020587E8, 0x78);
}
