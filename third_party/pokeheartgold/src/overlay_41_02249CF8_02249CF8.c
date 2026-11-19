#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"

typedef enum HeapID HeapID;

void ov41_02249F7C(u32, u32, NNSG2dScreenData *, u32, u32, u32, u32, u32, u32);

void ov41_02249CF8(s32 *param_1, s32 param_2);

void ov41_02249CF8(s32 *param_1, s32 param_2) {
    s32 iVar1;
    void *ptr;
    s32 memberNo;
    u32 *puVar2;
    NNSG2dScreenData *apNStack_14;

    iVar1 = param_1[3];
    param_1[3] = iVar1 + param_2;
    if (iVar1 + param_2 > param_1[2]) {
        param_1[3] = 0;
        if (param_1[4] == 0) {
            memberNo = param_1[1];
            param_1[4] = 1;
        } else {
            memberNo = *(s32 *)(*param_1 + 8);
            param_1[4] = 0;
        }
        ptr = (void *)(GfGfxLoader_GetScrnData(*(NarcId *)(*param_1 + 4), memberNo, 0, &apNStack_14, (HeapID)(param_1[5])));
        puVar2 = (u32 *)*param_1;
        ov41_02249F7C(*puVar2, puVar2[7], apNStack_14, puVar2[5], puVar2[6], puVar2[3], puVar2[4], puVar2[10], puVar2[8]);
        Heap_Free(ptr);
    }
    return;
}
