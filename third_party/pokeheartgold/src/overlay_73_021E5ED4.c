#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "overlay_73_unk_struct_020961_e8_t.h"

s32 ov73_021E77E8(UnkStruct_020961E8 *arg0);

void ov73_021E5ED4(UnkStruct_020961E8 *arg0);

void ov73_021E5ED4(UnkStruct_020961E8 *arg0) {
    s32 v0;

    ov73_021E77E8(arg0);
    Heap_Free(arg0->unk_0C.unk_0C);
    Heap_Free(arg0->unk_40C);
    v0 = 0;
    do {
        String_Delete(arg0->unk_2C[v0]);
        v0 += 1;
    } while (v0 < 5);
    String_Delete(arg0->unk_48);
    String_Delete(arg0->unk_44);
}

void ov73_021E5F0C(BgConfig *arg0);

void ov73_021E5F0C(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    FreeBgTilemapBuffer(bgConfig, 1U);
    FreeBgTilemapBuffer(bgConfig, 0U);
    Heap_Free(bgConfig);
}
