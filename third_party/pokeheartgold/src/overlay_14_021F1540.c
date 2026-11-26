#include "global.h"

#include "overlay_14_unk_struct_ov14_boxsys.h"
struct UnkStruct_ov14_boxapp;

#include "pokemon_storage_system.h"
#include "unk_02005D10.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F2270(void *, s32, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
void ov14_021F1540(struct UnkStruct_ov14_boxapp *arg0);
s32 ov14_021E8824(UnkStruct_ov14_boxsys *);
s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F3190(void *, u32, s32);
void ov14_021F1580(struct UnkStruct_ov14_boxapp *arg0, u8 arg1);

void ov14_021F1540(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F29E4(arg0->sys, 9, 8);
    if (PCStorage_CountMonsAndEggsInBox(arg0->storage, (u32) arg0->unk25) == 0x1E) {
        PlaySE(0x5F3U);
    } else {
        PlaySE(0x5DDU);
    }
    ov14_021F2270(arg0, 0xC, 0x66);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov14_021F40DC(void *);
#else
s32 ov14_021F40DC(void *);
#endif

s32 ov14_021EA254(UnkStruct_ov14_boxapp *app);

void ov14_021F1580(UnkStruct_ov14_boxapp *arg0, u8 arg1) {
    arg0->unk21 = arg1;
    arg0->sys->unk_44B = 1;
    ov14_021F3190(((void *)arg0->sys), arg0->unk21, 0);
    ov14_021F40DC(arg0);
    if (arg0->unk2A == 0) {
        ov14_021E8824(((void *)arg0->sys));
    }
    ov14_021F0234(arg0, ov14_021EA254, 0x73);
}
