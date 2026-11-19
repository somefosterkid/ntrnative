#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E8588(void *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
void ov14_021ED920(void *arg0);
s32 ov14_021E81D8(void *);
void ov14_021ED940(void *arg0);
s32 ov14_021F1058(UnkStruct_ov14_boxapp *, s32);

s32 ov14_021E9604(UnkStruct_ov14_boxapp *app);

void ov14_021ED920(void *arg0) {
    ov14_021E8588(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E9604, 0x44);
}

s32 ov14_021E96A8(UnkStruct_ov14_boxapp *app);

void ov14_021ED940(void *arg0) {
    ov14_021E81D8(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E96A8, 0x45);
}

void ov14_021ED960(void *arg0) {
    ov14_021F1058(arg0, 0x37);
}
