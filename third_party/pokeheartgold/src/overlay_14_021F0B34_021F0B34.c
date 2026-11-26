#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

void ov14_021EC710(void *arg0);
void ov14_021E76B8(void *);
s32 ov14_021F1100(void *, u8);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F0B34(struct UnkStruct_ov14_boxapp *arg0);
struct UnkStruct_ov14_boxapp;

void ov14_021F0B34(UnkStruct_ov14_boxapp *arg0) {
    *((s8 *)&arg0->unk23[0]) = 0;
    ov14_021F2A18(((void *)arg0->sys), 9, 0);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 0) {
        ov14_021EC710(arg0);
        return;
    }
    ov14_021E76B8(arg0);
    ov14_021F1100(arg0, 0x25);
}
