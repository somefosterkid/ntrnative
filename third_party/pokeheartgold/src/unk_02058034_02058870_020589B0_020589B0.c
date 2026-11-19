#include "global.h"

#include "field_communication_manager.h"
#include "unk_02035900.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_0205896C;

void sub_020589B0(void);

void sub_020589B0(void) {
    u16 v0;

    v0 = _021D41C8->timer;
    if (v0 != 0) {
        _021D41C8->timer = (u16)(v0 - 1);
        return;
    }
    sub_02035FE4();
    sub_020582F4(&sub_0205896C, 0x78);
}
