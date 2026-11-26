#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_40.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222BF80(void *, u32);

void ov40_0222D9E8(void *, void *, u32);

u32 ov40_0223B5B0(UnkStruct_0208C06C *param_1) {
    void *pvVar1;
    s32 iVar2;

    pvVar1 = Heap_Alloc(HEAP_ID_109, 0x217c);
    memset(pvVar1, 0, 0x217c);
    param_1->unk_860 = pvVar1;
    iVar2 = param_1->unk_86C;
    if (iVar2 == 300) {
        ((u32 *)pvVar1)[3] = 0;
    } else if (iVar2 == 0x12d) {
        ((u32 *)pvVar1)[3] = 1;
    } else if (iVar2 == 200) {
        ((u32 *)pvVar1)[3] = 2;
    }
    BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 2);
    BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 3);
    BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 6);
    BgClearTilemapBufferAndCommit(param_1->unk_14.unk_10, 7);
    ov40_0222D9E8(pvVar1, (u8 *)pvVar1 + 4, 0);
    ov40_0222BF80(param_1, 1);
    return 0;
}
