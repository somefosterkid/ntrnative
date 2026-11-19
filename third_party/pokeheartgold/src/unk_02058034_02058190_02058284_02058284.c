#include "global.h"

#include "field_communication_manager.h"
#include "unk_020379A0.h"
#include "unk_02037C94.h"
#include "unk_02058034.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058A60;

void sub_02058284(void) {
    if (_021D41C8 != 0) {
        sub_020398D4(0, 0);
        sub_020582F4(&sub_02058A60, 5);
    }
}

extern s32 sub_02058A38;

void sub_020582A8(void) {
    if (_021D41C8 != 0) {
        sub_02037AC0(0x5BU);
        sub_020582F4(&sub_02058A38, 5);
    }
}
