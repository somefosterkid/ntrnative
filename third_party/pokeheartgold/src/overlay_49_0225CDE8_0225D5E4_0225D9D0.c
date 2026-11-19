#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "heap.h"

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);

typedef struct {
    Field3dModel unk_00[2];
    void *unk_20[2][3];
    Field3DModelAnimation unk_38[2][4];
} UnkStruct_ov70_022618C8;

void ov49_0225D9D0(UnkStruct_ov70_022618C8 *param0, NNSFndAllocator *param1);

void ov49_0225D9D0(UnkStruct_ov70_022618C8 *param0, NNSFndAllocator *param1) {
    int v0, v1;

    {
        for (v0 = 0; v0 < 2; v0++) {
            for (v1 = 0; v1 < 4; v1++) {
                sub_020180F8(&param0->unk_38[v0][v1], param1);
            }
        }
    }

    {
        for (v0 = 0; v0 < 2; v0++) {
            Heap_Free(param0->unk_00[v0].dataRaw);
        }
    }

    {
        NNSG3dTexKey v2;
        NNSG3dTexKey v3;
        NNSG3dPlttKey v4;
        NNSG3dResTex *v5;

        for (v0 = 0; v0 < 2; v0++) {
            for (v1 = 0; v1 < 3; v1++) {
                v5 = NNS_G3dGetTex(param0->unk_20[v0][v1]);

                NNS_G3dTexReleaseTexKey(v5, &v2, &v3);
                NNS_GfdFreeTexVram(v2);
                NNS_GfdFreeTexVram(v3);

                v4 = NNS_G3dPlttReleasePlttKey(v5);
                NNS_GfdFreePlttVram(v4);

                Heap_Free(param0->unk_20[v0][v1]);
            }
        }
    }
}
