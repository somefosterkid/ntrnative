#include "global.h"
#include "constants/heap.h"
#include "gf_3d_loader.h"
#include "overlay_01_021FC4C4.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void ov01_021F9808(UnkStruct_ov5_021ED0A4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 num, s32 arg6) {
    arg0->unk_F8 = ov01_021FC4C4(HEAP_ID_FIELD1, HEAP_ID_63, arg1 << 0xC, arg1);
    ov01_021F9B00(arg0);
    ov01_021FA208(arg0, arg2);
    ov01_021FA210(arg0, arg1 - arg2);
    arg0->unk_FC = ov01_021FC4C4(HEAP_ID_FIELD1, HEAP_ID_64, arg3 << 7, arg3);
    ov01_021F9C24(arg0);
    ov01_021FA218(arg0, arg4);
    ov01_021FA220(arg0, arg3 - arg4);
    ov01_021FA1EC(arg0, GF3dGfxRawResMan_Create(num, HEAP_ID_FIELD1));
    ov01_021F9D48(arg0);
    ov01_021FA228(arg0, arg6);
    ov01_021FA230(arg0, num - arg6);
}

s32 ov01_021FC520(struct ResourceHeap *);

void ov01_021F9890(UnkStruct_ov5_021ED0A4 *arg0) {
    ov01_021FC520(arg0->unk_F8);
    ov01_021FC520(arg0->unk_FC);
    GF3dGfxRawResMan_Destroy(ov01_021FA1F4(arg0));
}
