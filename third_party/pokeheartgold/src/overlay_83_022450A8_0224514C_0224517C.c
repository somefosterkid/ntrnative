#include "global.h"

#include "unk_02035900.h"

void ov83_0224517C(void *arg0, u16 arg1, u16 arg2);

void ov83_0224517C(void *arg0, u16 arg1, u16 arg2) {
    *(s16 *)((u8 *)arg0 + 0x564) = arg1;
    *(s16 *)((u8 *)arg0 + 0x566) = arg2;
    if ((sub_0203769C() == 0) && (*(u8 *)((u8 *)arg0 + 0x11) == 0xFF)) {
        *(u8 *)((u8 *)arg0 + 0x11) = (u8)arg2;
    }
    *(s16 *)((u8 *)arg0 + 0x568) = (s16) * (u8 *)((u8 *)arg0 + 0x11);
    *(s16 *)((u8 *)arg0 + 0x56C) = (s16) * (u8 *)((u8 *)arg0 + 0x12);
    *(s16 *)((u8 *)arg0 + 0x56E) = (s16) * (u8 *)((u8 *)arg0 + 0x13);
}
