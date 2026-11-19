#include "global.h"

#include "field_communication_manager.h"
#include "unk_02035900.h"

s32 sub_020582F4(s32 *, u16);

s32 sub_0205857C(void);

extern s32 sub_020586A0;

void sub_020586EC(void);

void sub_020586EC(void) {
    u8 v0;

    if (_021D41C8->battleRoomMovement != 0) {
        sub_020582F4(&sub_020586A0, 5);
        v0 = 0;
        sub_020376E0(0x5E, &v0);
    }
    sub_0205857C();
}
