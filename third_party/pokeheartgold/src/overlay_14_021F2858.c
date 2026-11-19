#include "global.h"
struct UnkStruct_ov14_boxapp;
struct UnkStruct_ov70_022630A4_t_14;

#include "sys_task.h"
#include "overlay_14_unk_struct_ov70_022630_a4_t_14.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E76B8();
void ov14_021F0AD8(UnkStruct_ov14_boxapp *arg0);
s32 ov14_021F3488(void *, s32, s32);
void ov14_021F2858(UnkStruct_ov14_boxapp *arg0);
void ov14_021F0C88(UnkStruct_ov14_boxapp *param_1);
void ov14_021F2874(UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E8248(void *);
s32 ov14_021E82A8(void *);
s32 ov14_021E88F8(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F0F0C(void *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F2914(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F2858(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E76B8();
    ov14_021F3488(arg0, 0x82, 1);
    ov14_021F0AD8(arg0);
}

void ov14_021F2874(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E76B8();
    ov14_021F3488(arg0, 0x82, 1);
    ov14_021F0C88(arg0);
}

void ov14_021F2890(void *param_1) {
    ov14_021F1EB8(param_1);
}

s32 ov14_021F40DC(void *);

s32 ov14_021EA674(UnkStruct_ov14_boxapp *app);

void ov14_021F2898(UnkStruct_ov14_boxapp *arg0) {
    UnkStruct_ov14_boxsys *v0;

    ov14_021E84A4(arg0->sys->unk_2F0);
    ov14_021E8248(((u8 *)arg0->sys->unk_2F0));
    ov14_021E82A8(((u8 *)arg0->sys->unk_2F0));
    ov14_021E8328(arg0->sys->unk_2F0);
    ov14_021F40DC(arg0);
    v0 = arg0->sys;
    if (v0->unk_88C8 != 0) {
        ov14_021E88F8(v0->unk_2F0);
    }
    ov14_021F0234(arg0, ov14_021EA674, 0x76);
}

void ov14_021F28FC(UnkStruct_ov70_022630A4_t_14 *arg0) {
    ov14_021F2A18(*((void **)&arg0->unk_08[1].unk_04.unk_04), 9, 0);
    ov14_021F0F0C(arg0, 4);
}

void ov14_021F2914(UnkStruct_ov14_boxapp *arg0) {
    ov14_021E76B8();
    ov14_021F2A18(*((void **)&arg0->sys), 0xB, 0);
    ov14_021F0C0C(arg0);
}
