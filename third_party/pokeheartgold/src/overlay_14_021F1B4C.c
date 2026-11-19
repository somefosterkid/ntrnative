#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);
s32 ov14_021F1B4C(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);

s32 ov14_021F39A0(struct UnkStruct_ov14_boxsys *, u32, s32);

u32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E88BC(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021EAA04(UnkStruct_ov14_boxapp *app);

s32 ov14_021F1B4C(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    void *v0;

    arg0->unk21 = arg1;
    arg0->sys->unk_88CA = (s16)arg1;
    arg0->sys->unk_88C8 = ov14_021E6070(arg0, arg0->unk21, 6, 0);
    if (arg0->sys->unk_88C8 == 0) {
        return 0x82;
    }
    PlaySE(0x5EBU);
    arg0->sys->unk_44B = 1;
    ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
    ov14_021F39A0(((void *)arg0->sys), arg0->unk21, 2);
    v0 = ((void *)arg0->sys);
    ov14_021F3844(v0, ((UnkStruct_ov14_boxsys *)v0)->unk_88C8);
    ov14_021F34C8(((void *)arg0->sys), arg0->unk21, 1);
    ov14_021E88BC(*(void **)&arg0->sys->unk_2F0);
    if (ov14_021E8648(arg0->sys->unk_2F0) == 1) {
        ov14_021E8634(arg0->sys->unk_2F0);
    }
    return ov14_021F0234(arg0, ov14_021EAA04, 0x88);
}
