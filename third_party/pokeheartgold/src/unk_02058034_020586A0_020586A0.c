#include "global.h"

#include "field_communication_manager.h"
#include "party.h"
#include "unk_02035900.h"
#include "unk_02058034.h"

s32 sub_02057F58(void);

s32 sub_020582F4(s32 *, u16);

void sub_02059538(FieldSystem *, void (*)(s32, Party *));

void sub_02058640(s32 arg0, Party *arg1);

extern s32 sub_02058690;

void sub_020586A0(void);

void sub_020586A0(void) {
    u16 v0;

    if ((sub_02037958() == 0) && (sub_02057F18((s32)sub_0203769C()) == 0)) {
        v0 = _021D41C8->timer;
        if (v0 != 0) {
            _021D41C8->timer = (u16)(v0 - 1);
            return;
        }
        sub_02057F58();
        sub_02059538(_021D41C8->fieldSystem, sub_02058640);
        sub_020582F4(&sub_02058690, 0);
    }
}
