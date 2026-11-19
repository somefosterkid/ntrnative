#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "overlay_40.h"
#include "unk_02005D10.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_02235FA0_v0 {
    u8 unk_0[0x1DC];
    void *unk_1DC;
    MsgData *unk_1E0;
    s32 unk_1E4;
    void *unk_1E8;
} UnkStruct_ov40_02235FA0_v0;

void ov40_02235FA0(void *arg0) {
    UnkStruct_ov40_02235FA0_v0 *v0;

    v0 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    DestroyMsgData(v0->unk_1E0);
    Heap_Free(v0->unk_1E8);
    Heap_Free(v0->unk_1DC);
    v0->unk_1E4 = 0;
}

s32 ov40_02235DAC(UnkStruct_0208C06C *, s32);

s32 ov40_02235E34(void *, s32);

void ov40_02235FD0(s32 arg0, s32 arg1, void *arg2);

void ov40_02235FD0(s32 arg0, s32 arg1, void *arg2) {
    if (arg1 == 0) {
        if (ov40_02235DAC(arg2, arg0) == 0) {
            PlaySE(0x57CU);
            return;
        }
        ov40_02235E34(arg2, arg0);
    }
}
