#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "item.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

s32 ov14_021F2A44(UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F396C(struct UnkStruct_ov14_boxsys *, u32, s32);

u32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E8314(void *);
s32 ov14_021E83F4(void *);
s32 ov14_021E88BC(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1F24(void *);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2F88(u32, void *, s32 *, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);
s32 ov14_021F5FBC(void *, s32);
s32 ov14_021F6928(void *, s32, s32);
void ov14_021F1D6C(struct UnkStruct_ov14_boxapp *arg0, u16 arg1);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021EACD4(UnkStruct_ov14_boxapp *app);

void ov14_021F1D6C(UnkStruct_ov14_boxapp *arg0, u16 arg1) {
    s16 sp0[2];
    u16 temp_r0;
    u16 temp_r4;
    UnkStruct_ov14_boxsys *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r0_3;
    void *temp_r1;
    arg0->unk21 = arg1;
    temp_r4 = arg0->sys->unk_88C8;
    arg0->sys->unk_88C8 = ov14_021E6070(arg0, arg0->unk21, 6, 0);
    if (ov14_021E8544(arg0->sys->unk_2F0) == 1) {
        if ((temp_r4 != 0) || (temp_r1 = ((void *)arg0->sys), temp_r0 = ((UnkStruct_ov14_boxsys *)temp_r1)->unk_88C8, temp_r0 != 0)) {
            ov14_021E83F4(*(void **)&arg0->sys->unk_2F0);
        } else if (temp_r0 != 0) {
            ov14_021E84A4(*((void **)&((UnkStruct_ov14_boxsys *)temp_r1)->unk_2F0));
        }
    } else {
        ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    }
    ov14_021F5FBC(arg0, arg0->sys->unk_88C8);
    temp_r0_2 = arg0->sys;
    if ((temp_r0_2->unk_88C8) != 0) {
        ov14_021F2A18(temp_r0_2, 0xB, 0);
        ov14_021F396C(((void *)arg0->sys), arg0->unk21, 1);
        temp_r0_3 = arg0->sys;
        ov14_021F3844(temp_r0_3, temp_r0_3->unk_88C8);
        ov14_021F39D0(((void *)arg0->sys));
        ov14_021F34C8(((void *)arg0->sys), arg0->unk21, 1);
        ov14_021E88BC(*(void **)&arg0->sys->unk_2F0);
        if (ItemIdIsMail(arg0->sys->unk_88C8) == 1) {
            ov14_021F6928(arg0, 0x28, 9);
        } else {
            ov14_021F6928(arg0, 0x28, 0xA);
        }
    } else if (ov14_021F2A44(temp_r0_2, 0xB) == 1) {
        ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
        ov14_021F40DC(arg0);
    }
    ov14_021F2F88(arg0->unk21, &sp0[1], (s32 *)sp0, 1);
    arg0->sys->unk_40B8 = (s32)(sp0[1] + 8);
    arg0->sys->unk_40BC = (s32)(sp0[0] + 8);
    ov14_021F1F24(arg0);
    ov14_021F0234(arg0, ov14_021EACD4, 0x8D);
}
