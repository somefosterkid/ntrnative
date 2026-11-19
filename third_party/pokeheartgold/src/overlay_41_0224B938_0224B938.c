#include "global.h"

#include "overlay_41_unk_struct_ov22_0225a0e4_head.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "overlay_41_unk_struct_ov22_0225b4e4.h"

s32 ov41_022464AC(void *, s32);

void ov41_0224B958(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID);

void ov41_0224B938(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID);

void ov41_0224B938(UnkStruct_ov22_0225A0E4_Head *param0, UnkStruct_ov22_02255CB8 *param1, const UnkStruct_ov22_0225B4E4 *param2, enum HeapID heapID) {
    ov41_022464AC(param1, heapID);
    ov41_0224B958(param0, param1, param2, heapID);
}
