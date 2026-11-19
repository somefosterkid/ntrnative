#include "global.h"

#include "field_communication_manager.h"
#include "party.h"
#include "unk_02035900.h"

s32 sub_020582F4(s32 *, u16);

extern s32 sub_02058608;

extern s32 sub_02058870;

void sub_02058640(s32 arg0, Party *arg1);

void sub_02058640(s32 arg0, Party *src) {
    u8 v0;

    if (src != NULL) {
        _021D41C8->party = SaveArray_Party_Alloc(HEAP_ID_FIELD2);
        Party_Copy(src, _021D41C8->party);
    }
    if (arg0 != 0) {
        sub_020582F4(&sub_02058870, 3);
        return;
    }
    v0 = 3;
    sub_020376E0(0x5E, &v0);
    sub_020582F4(&sub_02058608, 0);
}
