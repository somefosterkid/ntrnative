#include "global.h"
#include "unk_struct_02089688.h"

void sub_02085604(void *arg0, s32 arg1);

void sub_02085604(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;

    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D8) = (s32) *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D4);
    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D4) = arg1;
    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2DC) = 0;
    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E0) = 0;
    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E4) = 0;
    *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E8) = 0;
    v0 = *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D4);
    if (v0 != 0) {
        *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2DC) = (s32) * (u16 *)((u8 *)(arg0 + ((v0 - 1) * 4)) + 0x2AA);
        *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E0) = (s32) * (u16 *)((u8 *)(arg0 + ((*((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D4) - 1) * 4)) + 0x2AC);
    }
    v1 = *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D8);
    if (v1 != 0) {
        *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E4) = (s32) * (u16 *)((u8 *)(arg0 + ((v1 - 1) * 4)) + 0x2AA);
        *((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2E8) = (s32) * (u16 *)((u8 *)(arg0 + ((*((s32 *)&((UnkStruct_02089688 *)arg0)->unk_2D8) - 1) * 4)) + 0x2AC);
    }
}
