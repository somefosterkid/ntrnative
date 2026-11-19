#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "sys_task.h"
#include "error_handling.h"
#include "overlay_40_unk_struct_ov62_02246bf4.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef struct {
    int unk_00;
    int unk_04[4];
    u8 padding_14[4];
    String *unk_18;
    Options *options;
} UnkStruct_ov62_02246B00;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

void ov40_02242FF8(void *arg0);
void ov40_0224301C(void *arg0, s32 arg1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

extern UnkStruct_ov62_02246B00 *ov40_02242FAC(enum HeapID heapID, int param1, int param2[], Options *options);

UnkStruct_ov62_02246B00 *ov40_02242FAC(enum HeapID heapID, int param1, int param2[], Options *options) {
    int v0;
    UnkStruct_ov62_02246B00 *v1 = NULL;

    v1 = Heap_Alloc(heapID, sizeof(UnkStruct_ov62_02246B00));
    MI_CpuFill8(v1, 0, sizeof(UnkStruct_ov62_02246B00));
    v1->unk_00 = param1;
    v1->unk_18 = String_New(param1 + 1, heapID);
    v1->options = options;

    for (v0 = 0; v0 < 3; v0++) {
        v1->unk_04[v0] = param2[v0];
    }

    v1->unk_04[v0] = param2[v0 - 1];

    return v1;
}

void ov40_02242FF8(void *ptr) {
    if (*(String **)((u8 *)ptr + 0x18) == NULL) {
        GF_AssertFail();
    }
    if (ptr == NULL) {
        GF_AssertFail();
    }
    String_Delete(*(String **)((u8 *)ptr + 0x18));
    Heap_Free(ptr);
}

void ov40_0224301C(void *arg0, s32 arg1) {
    s32 v0;
    s32 v1;

    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_20C) = (s32) *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_208);
    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_208) = arg1;
    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_210) = 0;
    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_214) = 0;
    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_218) = 0;
    *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_21C) = 0;
    v0 = *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_208);
    if (v0 != 0) {
        *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_210) = (s32) * (u16 *)((u8 *)(arg0 + ((v0 - 1) * 4)) + 0x1E4);
        *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_214) = (s32) * (u16 *)((u8 *)(arg0 + ((*((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_208) - 1) * 4)) + 0x1E6);
    }
    v1 = *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_20C);
    if (v1 != 0) {
        *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_218) = (s32) * (u16 *)((u8 *)(arg0 + ((v1 - 1) * 4)) + 0x1E4);
        *((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_21C) = (s32) * (u16 *)((u8 *)(arg0 + ((*((s32 *)&((UnkStruct_ov62_02246BF4 *)arg0)->unk_20C) - 1) * 4)) + 0x1E6);
    }
}
