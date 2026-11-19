#include "global.h"

#include "error_handling.h"
#include "sys_task.h"
#include "overlay_96_unk_struct_ov96_022044a0.h"

void Heap_Free(void *ptr);

void ov96_022043B8(void *ptr);

void ov96_022043B8(void *ptr) {
    Heap_Free(ptr);
}

s32 ov96_022044A0(void *, u8, u8, s32);

u8 ov96_022043C0(void *arg0, s32 arg1);

u8 ov96_022043C0(void *arg0, s32 arg1) {
    s32 v1;
    void *new_var;
    s32 v2;
    s32 v3;
    u8 v0;
    u32 v4;
    u8 v5;
    v5 = 0xC;
    if ((((UnkStruct_ov96_022044A0 *)arg0)->unk_C9) == 4) {
        return 0U;
    }
    ((UnkStruct_ov96_022044A0 *)arg0)->unk_C6 = (u8)((((UnkStruct_ov96_022044A0 *)arg0)->unk_C6) + 1);
    if (((u32)(((UnkStruct_ov96_022044A0 *)arg0)->unk_C6)) >= ((u32)(((UnkStruct_ov96_022044A0 *)arg0)->unk_C7))) {
        *((s32 *)&((UnkStruct_ov96_022044A0 *)arg0)->unk_C0) = 1;
        ((UnkStruct_ov96_022044A0 *)arg0)->unk_C6 = 0U;
    }
    if ((*((s32 *)&((UnkStruct_ov96_022044A0 *)arg0)->unk_C0)) != 0) {
        v0 = ((UnkStruct_ov96_022044A0 *)arg0)->unk_C8;
        v4 = v0 + (((UnkStruct_ov96_022044A0 *)arg0)->unk_C5);
        v3 = (((UnkStruct_ov96_022044A0 *)arg0)->unk_CA) + (v4 * 3);
        if (v3 < ((s32)v0)) {
            GF_AssertFail();
        }
        v1 = v3 * 4;
        v2 = *((s32 *)(((u8 *)(*((void **)(((u8 *)((new_var = arg0) + v1)) + 0x30)))) + 0xC));
        if (((v2 == (0xC * 0)) || (v2 == 2)) && ((*((u8 *)(((u8 *)(*((void **)(arg0 + v1)))) + 0x19))) != 1)) {
            ov96_022044A0(arg0, v4, (u8)v3, arg1);
        }
        v5 = (u8)v3;
        ((UnkStruct_ov96_022044A0 *)arg0)->unk_C5 = (u8)((((UnkStruct_ov96_022044A0 *)arg0)->unk_C5) + 1);
        if (((u32)(((UnkStruct_ov96_022044A0 *)arg0)->unk_C5)) >= ((u32)(((UnkStruct_ov96_022044A0 *)arg0)->unk_C4))) {
            ((UnkStruct_ov96_022044A0 *)arg0)->unk_C5 = 0U;
            ((UnkStruct_ov96_022044A0 *)arg0)->unk_CA = (u8)(((s32)((((UnkStruct_ov96_022044A0 *)arg0)->unk_CA) + 1)) % 3);
            *((s32 *)&((UnkStruct_ov96_022044A0 *)arg0)->unk_C0) = 0;
        }
    }
    return v5;
}
