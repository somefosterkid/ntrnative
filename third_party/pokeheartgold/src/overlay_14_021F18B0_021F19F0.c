#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);
s32 ov14_021F19F0(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);

s32 ov14_021F396C(struct UnkStruct_ov14_boxsys *, u32, s32);

u32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E85D0(void *);
s32 ov14_021E88BC(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F1F24(void *);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F2F88(u32, void *, s32 *, s32);
s32 ov14_021F34C8(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);

s32 ov14_021E99A0(UnkStruct_ov14_boxapp *app);

s32 ov14_021EA778(UnkStruct_ov14_boxapp *app);

s32 ov14_021F19F0(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    u8 sp0[4];
    UnkStruct_ov14_boxsys *temp_r1;

    arg0->unk21 = arg1;
    arg0->sys->unk_88C8 = ov14_021E6070(arg0, arg0->unk21, 6, 0);
    ov14_021F6408(arg0, 0);
    temp_r1 = arg0->sys;
    if (temp_r1->unk_88C8 == 0) {
        GridInputHandler_SetNextInput(temp_r1->unk_2C, (s32) arg0->unk21);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        if (ov14_021E8648(arg0->sys->unk_2F0) == 1) {
            return 0x82;
        }
        if (ov14_021E85E4(arg0->sys->unk_2F0) == 1) {
            ov14_021E85D0(*(void **)&arg0->sys->unk_2F0);
        }
        return ov14_021F0234(arg0, ov14_021E99A0, 0x82);
    }
    PlaySE(0x5EBU);
    ov14_021F2A18(((void *)arg0->sys), 0xB, 0);
    ov14_021F396C(((void *)arg0->sys), arg0->unk21, 2);
    ov14_021F3844(((void *)arg0->sys), arg0->sys->unk_88C8);
    ov14_021F39D0(((void *)arg0->sys));
    ov14_021F34C8(((void *)arg0->sys), arg0->unk21, 1);
    ov14_021E88BC(*(void **)&arg0->sys->unk_2F0);
    if (ov14_021E85E4(arg0->sys->unk_2F0) == 1) {
        ov14_021E85D0(*(void **)&arg0->sys->unk_2F0);
    } else if (ov14_021E8648(arg0->sys->unk_2F0) == 1) {
        ov14_021E8634(arg0->sys->unk_2F0);
    }
    ov14_021F2F88(arg0->unk21, &sp0[2], (s32 *)sp0, 2);
    arg0->sys->unk_40B8 = (s32)(*(s16 *)((u8 *)((s32 *)sp0) + 2) + 8);
    arg0->sys->unk_40BC = (s32)(*(s16 *)((s32 *)sp0) + 8);
    ov14_021F1F24(arg0);
    return ov14_021F0234(arg0, ov14_021EA778, 0x84);
}
