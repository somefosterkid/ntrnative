#include "global.h"

#include "field_communication_manager.h"
#include "unk_020379A0.h"

s32 sub_02036010(void);

s32 sub_020582F4(s32 *, u16);

extern s32 sub_0205876C;

void sub_020587E8(void);

void sub_020587E8(void) {
    u16 v0;

    if (sub_02036010() != 0) {
        v0 = _021D41C8->timer;
        if (v0 != 0) {
            _021D41C8->timer = (u16)(v0 - 1);
        }
        if (_021D41C8->timer == 0x5A) {
            sub_02037AC0(4U);
        }
        if (sub_02037B38(4U) != 0) {
            sub_020582F4(&sub_0205876C, 0);
        }
    }
}
