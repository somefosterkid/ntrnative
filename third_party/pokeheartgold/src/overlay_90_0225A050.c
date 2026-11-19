#include "global.h"

#include "overlay_90.h"
#include "bg_window.h"
#include "error_handling.h"
#include "system.h"
#include "constants/heap.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"

extern u32 ov90_0225C284;

void ov90_0225A1B8(void *);
BufferManager *sub_02014A08(enum HeapID heapID, void *buffer1, void *buffer2);
void sub_02014A78(BufferManager *bufferManager, u8 mode);
void sub_02014A38(BufferManager *bufferManager);
void ov90_0225A108(UnkStruct_ov114_0225DFFC *arg0);

u32 ov90_0225A050(u32 param0, u32 param1, u32 param2) {
    u32 uVar1;
    u32 in_r3;
    u32 uVar2;
    u8 *puVar3;

    uVar2 = 0;
    uVar1 = param0;
    if (param0 != 0) {
        puVar3 = (unsigned char *)(&ov90_0225C284 + param0);
        do {
            if (param1 == (u8)puVar3[-4]) {
                in_r3 = uVar2;
            }
            if (param2 == (u8)puVar3[-4]) {
                uVar1 = uVar2;
            }
            uVar2 = uVar2 + 1;
            puVar3 = puVar3 + 1;
        } while (uVar2 < param0);
    }
    uVar1 = in_r3 - uVar1;
    if ((s32)uVar1 < 0) {
        uVar1 = uVar1 + param0;
    }
    return uVar1;
}

void ov90_0225A088(UnkStruct_ov114_0225DFFC *arg, UnkStruct_ov114_0225CEF0 *arg1, enum HeapID heapID) {
    int v1;

    if (arg->bufferManager != NULL) {
        GF_AssertFail();
    }
    arg->unk_00 = arg1;
    BgSetPosTextAndCommit(arg1->unk_00, 0U, BG_POS_OP_SET_ROT, -0xFF);
    BgSetPosTextAndCommit(arg->unk_00->unk_00, 1U, BG_POS_OP_SET_ROT, -0xFF);
    for (v1 = 0; v1 < 2; v1++) {
        MIi_CpuClear16(0xFF01U, arg->unk_04[v1], sizeof(arg->unk_04[v1]));
    }
    arg->bufferManager = sub_02014A08(heapID, arg->unk_04[0], arg->unk_04[1]);
    sub_02014A78(arg->bufferManager, 0);
    Main_SetHBlankIntrCB(ov90_0225A1B8, arg);
}

void ov90_0225A108(UnkStruct_ov114_0225DFFC *arg0) {
    if (arg0->bufferManager != NULL) {
        Main_SetHBlankIntrCB(NULL, NULL);
        sub_02014A38(arg0->bufferManager);
        arg0->bufferManager = NULL;
    }
}
