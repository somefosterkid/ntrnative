#include "global.h"

#include "code_entry_args.h"
#include "overlay_75.h"
#include "pm_string.h"
#include "overlay_75_unk_struct_ov98_02246_e88.h"

s32 ov75_02246BCC(void *, s32, s32);

s32 ov75_02246CF0(void *, s32);

s32 ov75_02246CF8(void *);

s32 ov75_02249838(String *, s32);

void ov75_02246D40(UnkStruct_ov98_02246E88 *arg0) {
    CodeEntryArgs *v0;

    v0 = arg0->unk_98;
    if (ov75_02249838(v0->unk_1C, 0x73) != 0) {
        ov75_02246CF0(arg0, 1);
    } else if (ov75_02246CF8(arg0) == 2) {
        if (String_Compare(arg0->unk_114, v0->unk_1C) != 0) {
            ov75_02246CF0(arg0, 3);
        } else {
            ov75_02246CF0(arg0, 0);
        }
    } else {
        String_Copy(arg0->unk_114, v0->unk_1C);
        ov75_02246CF0(arg0, 2);
    }
    sub_02085438(v0);
    ov75_02246BCC(arg0, 0, 0);
}
