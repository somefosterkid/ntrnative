#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_40.h"
#include "poke_overlay.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223414C_Heap_Alloc {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0x213];
    BgConfig *unk_218;
    s32 unk_21C;
    s32 unk_220;
    s32 unk_224;
    u8 unk_228[0x10];
    void *unk_238;
    u8 unk_23C[0x14];
    void *unk_250;
    u8 unk_254[0x94];
} UnkStruct_ov40_0223414C_Heap_Alloc;

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222D9E8(void *, void *, s32);

void *sub_0202B998(u32 heapID);

void *sub_020314A4(s32);

FS_EXTERN_OVERLAY(OVY_41);

s32 ov40_0223414C(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_0223414C_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_109, sizeof(UnkStruct_ov40_0223414C_Heap_Alloc));
    memset(v0, 0, sizeof(UnkStruct_ov40_0223414C_Heap_Alloc));
    arg0->unk_860 = v0;
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_41), OVY_LOAD_ASYNC);
    v0->unk_218 = (BgConfig *)arg0->unk_14.unk_10;
    v0->unk_21C = 0x48;
    v0->unk_220 = 0x10;
    v0->unk_224 = 0x6D;
    v0->unk_238 = sub_0202B998(109);
    v0->unk_250 = sub_020314A4(0x6D);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 2U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 3U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 6U);
    BgClearTilemapBufferAndCommit(arg0->unk_14.unk_10, 7U);
    ov40_0222D9E8(v0, (void *)&v0->unk_4, 0);
    ov40_0222BF80(arg0, 1);
    return 0;
}
