#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "unk_0200B150.h"
#include "sys_task.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

s32 ov41_022468FC(void *);

s32 ov41_02246A94(void *);

void ov41_02246670(struct UnkStruct_ov22_0225A0E4 *arg0);

void ov41_02246B5C(UnkStruct_ov22_0225A0E4 *param0);
void ov41_022466C8(UnkStruct_ov22_0225A0E4 *param0);

void ov41_02246670(UnkStruct_ov22_0225A0E4 *arg0) {
    arg0->unk_5C = NARC_New(NARC_a_0_2_6, HEAP_ID_14);
    ov41_02246A94(arg0);
    arg0->unk_40 = BgConfig_Alloc(HEAP_ID_14);
    ov41_022468FC(arg0);
}

s32 ov41_02246A20(UnkStruct_ov22_0225A0E4 *param0);

s32 ov41_02246B34(void *);

void ov41_02246698(struct UnkStruct_ov22_0225A0E4 *arg0);

void ov41_02246698(UnkStruct_ov22_0225A0E4 *arg0) {
    ov41_02246A20(arg0);
    Heap_Free(*((void * *)&arg0->unk_40));
    NARC_Delete(arg0->unk_5C);
    ov41_02246B34(arg0);
}

void ov41_022466B8(void *arg0);

void ov41_022466B8(void *arg0) {
    DoScheduledBgGpuUpdates(*(BgConfig **)((u8 *)arg0 + 0x40));
    OamManager_ApplyAndResetBuffers();
}

void ov41_022466C8(UnkStruct_ov22_0225A0E4 *param0) {
    ov41_02246B5C(param0);
}
