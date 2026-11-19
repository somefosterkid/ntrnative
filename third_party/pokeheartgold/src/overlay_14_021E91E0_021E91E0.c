#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "party.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E65C4(void *);
s32 ov14_021E6CF8(void *, u8, s32);
s32 ov14_021E7148(void *, void *);
s32 ov14_021F40E8(void *, s32);
s32 ov14_021E91E0(UnkStruct_ov14_boxapp *app);
s32 ov14_021E91E0(UnkStruct_ov14_boxapp *arg0) {
    u16 temp_r1;
    Party *new_var;
    unsigned int new_var2;
    void *temp_r4;
    temp_r4 = *((void * *)&arg0->sys);
    temp_r1 = ((UnkStruct_ov14_boxsys *)temp_r4)->unk_10;
    switch (temp_r1) {
    case 0:
        new_var = arg0->party;
        new_var2 = Party_GetCount(new_var) + 0x1E;
        ov14_021E6CF8(arg0, arg0->unk21, new_var2);
        ov14_021F40E8(arg0, 0);
        ((UnkStruct_ov14_boxsys *)temp_r4)->unk_10 = (u16)((((UnkStruct_ov14_boxsys *)temp_r4)->unk_10) + 1);
        break;

    case 1:
        if (ov14_021E65C4(arg0) == 0) {
            ov14_021E7148(arg0, *((void **)(((u8 *)temp_r4) + 0xC)));
            ((UnkStruct_ov14_boxsys *)temp_r4)->unk_10 = 0U;
            return 0;
        }
        break;
    }

    return 1;
}
