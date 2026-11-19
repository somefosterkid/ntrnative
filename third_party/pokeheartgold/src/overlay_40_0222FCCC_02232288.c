#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222D9E8(void *, void *, s32);

s32 ov40_02232288(UnkStruct_0208C06C *arg0) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_109, 0x19CU);
    memset(v0, 0, 0x19CU);
    arg0->unk_860 = v0;
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 3U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 6U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 7U);
    ov40_0222D9E8(v0, v0 + 4, 0);
    ov40_0222BF80(arg0, 1);
    return 0;
}
