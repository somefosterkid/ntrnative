#include "global.h"

#include "field_communication_manager.h"
#include "unk_02058034.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_0205834C;

void sub_02058164(u16 arg0) {
    _021D41C8->connectID = arg0;
    sub_020582F4(&sub_0205834C, 0);
}
