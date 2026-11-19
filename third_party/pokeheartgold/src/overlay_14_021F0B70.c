#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E7E78(void *);
s32 ov14_021E81A8(void *, s32);
s32 ov14_021E82DC(void *);
s32 ov14_021E8368(void *);
s32 ov14_021F30B0(void *);
s32 ov14_021F63A8(void *);
s32 ov14_021F63F0(UnkStruct_ov14_boxsys *);
s32 ov14_021E7E88(void *);
s32 ov14_021F311C(void *);
s32 ov14_021F63B8(void *);
void ov14_021F0BF4(void *arg0);

void ov14_021F0B70(void *arg0) {
    ov14_021E81A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0), 1);
    ov14_021F63F0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F63A8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021E8368(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E82DC(arg0);
    ov14_021E7E78(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F30B0(arg0);
}

void ov14_021F0BB4(void *arg0) {
    ov14_021E81A8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0), 1);
    ov14_021F63F0(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F63B8(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021E8368(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021E7E88(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F311C(arg0);
}

void ov14_021F0BF4(void *arg0) {
    ov14_021F30B0(arg0);
    ov14_021E7E78(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
}
