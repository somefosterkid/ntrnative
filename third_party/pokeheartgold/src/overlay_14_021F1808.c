#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

s32 ov14_021F396C(struct UnkStruct_ov14_boxsys *, u32, s32);

u32 ov14_021E6070(void *, s32, s32, void *);
s32 ov14_021E8234(void *);
s32 ov14_021E8294(void *);
s32 ov14_021E8314(void *);
s32 ov14_021E8434(void *);
s32 ov14_021E88BC(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);
s32 ov14_021F5FBC(void *, s32);
void ov14_021F1808(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);
struct UnkStruct_ov14_boxapp;

s32 ov14_021EA408(UnkStruct_ov14_boxapp *app);

void ov14_021F1808(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    UnkStruct_ov14_boxsys *v0;
    UnkStruct_ov14_boxsys *v1;

    arg0->unk21 = arg1;
    arg0->sys->unk_88C8 = ov14_021E6070(arg0, arg0->unk21, 6, 0);
    ov14_021F5FBC(arg0, arg0->sys->unk_88C8);
    ov14_021E8434(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8234(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8294(*(void **)&arg0->sys->unk_2F0);
    ov14_021E8314(*(void **)&arg0->sys->unk_2F0);
    v0 = arg0->sys;
    if (v0->unk_88C8 != 0) {
        ov14_021F2A18(v0, 0xB, 0);
        ov14_021F396C(((void *)arg0->sys), arg0->unk21, 0);
        v1 = arg0->sys;
        ov14_021F3844(v1, v1->unk_88C8);
        ov14_021F39D0(((void *)arg0->sys));
        ov14_021E88BC(*(void **)&arg0->sys->unk_2F0);
    }
    ov14_021F0234(arg0, ov14_021EA408, 0x76);
}
