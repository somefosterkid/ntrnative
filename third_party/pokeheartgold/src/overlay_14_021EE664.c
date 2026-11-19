#include "global.h"
struct UnkStruct_ov14_boxapp;
#include "heap.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_unk_0201956C.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F0234(UnkStruct_ov14_boxapp *app, UnkFuncPtr_ov14_boxtask task, s32 nextState);
s32 ov14_021F08BC(void *);
s32 ov14_021E637C(void *);
s32 ov14_021E6548(void *, s32, s32);
s32 ov14_021E8094(void *);
s32 ov14_021F08F0(void *);
s32 ov14_021F5EB4(void *, s32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F29E4(UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F685C(void *, s32, s32, s32);
s32 ov14_021F6AC0(void *, s32, s32);
s32 ov14_021F6678(void *, s32);

s32 ov14_021E9234(UnkStruct_ov14_boxapp *app);

s32 ov14_021E954C(UnkStruct_ov14_boxapp *app);
s32 ov14_021E95B4(UnkStruct_ov14_boxapp *app);

void ov14_021EE664(void *arg0) {
    *(s8 *)((u8 *)arg0 + 0x22) = 2;
    ov14_021F08BC(arg0);
    ov14_021F0234(arg0, ov14_021E9234, 0x6B);
}

void ov14_021EE684(UnkStruct_ov14_boxapp *arg0) {
    void *v0;

    v0 = arg0->sys->unk_C;
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    ov14_021E6548(arg0, *(s32 *)((u8 *)v0 + 0xE4), *(s32 *)((u8 *)v0 + 0xE8));
    ov14_021F08F0(arg0);
    ov14_021E8094(((u8 *)arg0->sys->unk_2F0));
    ov14_021F0234(arg0, ov14_021E954C, 0x6C);
}

void ov14_021EE6D0(void *arg0) {
    ov14_021F5EB4(arg0, 0);
    ov14_021E8328(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_2F0);
    ov14_021F0234(arg0, ov14_021E95B4, 0x6D);
}

s32 ov14_021EE6F8(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F6AC0(arg0, 0, 0);
    ov14_021F685C(arg0, 0, 0, 0x27);
    ov14_021E7588(arg0, 0x1E);
    ov14_021F29E4(*((void **)&arg0->sys), 9, 8);
    return 0x5B;
}

s32 ov14_021EE728(UnkStruct_ov14_boxapp *arg0) {
    u8 v0;
    void *v1;

    v1 = arg0->sys->unk_C;
    PlaySE(0x5EAU);
    ov14_021E637C(arg0);
    ov14_021E6548(arg0, *(s32 *)((u8 *)v1 + 0xE4), *(s32 *)((u8 *)v1 + 0xE8));
    ov14_021F08F0(arg0);
    ov14_021F6678(*((void * *)&arg0->sys), 0x28);
    v0 = arg0->unk21;
    if (v0 == 0xFF) {
        ov14_021F685C(arg0, 0U, 0, 0x27);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 0);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
        ov14_021E7588(arg0, 0x1E);
    } else {
        ov14_021F685C(arg0, v0, 1, 0x27);
        GridInputHandler_SetNextInput(arg0->sys->unk_2C, 7);
        GridInputHandler_SetButtonInputMode(arg0->sys->unk_2C, 1);
    }
    return 0x5B;
}

s32 ov14_021EE7B4(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0xC;
}

s32 ov14_021EE7C4(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x29;
}

s32 ov14_021EE7D4(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x24;
}

s32 ov14_021EE7E4(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x5B;
}

s32 ov14_021EE7F4(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    ov14_021F685C(arg0, 0, 0, 0x27);
    return 0x5B;
}

s32 ov14_021EE810(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x61;
}

s32 ov14_021EE820(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x16;
}

s32 ov14_021EE830(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x3D;
}

s32 ov14_021EE840(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x42;
}

s32 ov14_021EE850(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    return 0x51;
}

s32 ov14_021EE860(void *arg0) {
    Heap_Free(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_C);
    ov14_021F685C(arg0, 0, 0, 0x27);
    return 0x51;
}
