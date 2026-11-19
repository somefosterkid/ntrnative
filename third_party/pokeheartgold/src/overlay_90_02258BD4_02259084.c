#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "gf_gfx_planes.h"

struct BgConfig;

void FreeBgTilemapBuffer(BgConfig *bgConfig, u8 bgLayer);

void ov90_02259084(UnkStruct_ov114_0225CEF0 *param0);

void ov90_02259084(UnkStruct_ov114_0225CEF0 *param0) {
    BgSetPosTextAndCommit(param0->unk_00, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_Y, 0);

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_08; v0++) {
            FreeBgTilemapBuffer(param0->unk_00, param0->unk_04[v0].unk_00);
        }
    }

    Heap_Free(param0->unk_00);
}

void ov90_022590C0(UnkStruct_ov114_0225CEF0 *param0);

void ov90_022590C0(UnkStruct_ov114_0225CEF0 *param0) {
    DoScheduledBgGpuUpdates(param0->unk_00);
}
