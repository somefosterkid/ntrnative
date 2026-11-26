#include "global.h"
struct UnkStruct_ov14_boxapp;

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void ov14_021F46B0(void *, void *, s32, u32, u32);
void ov14_021F462C(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F462C(UnkStruct_ov14_boxapp *arg0) {
    NNSG2dCharacterData *spC;
    void *pNcgrFile;
    void *dest;
    u32 sp4;
    s32 var_r6;
    s32 var_r5;

    var_r6 = 0x15;
    {
        u32 t = *((u8 *)&arg0->sys->unk_44D);
        var_r5 = (u8)(t >> 2) * 4;
    }
    dest = Heap_AllocAtEnd(HEAP_ID_10, 0x240);
    pNcgrFile = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_0_1_9, 0x4A, HEAP_ID_10);
    NNS_G2dGetUnpackedCharacterData(pNcgrFile, &spC);
    sp4 = 0;
    do {
        MI_CpuCopy8(spC->pRawData, dest, 0x240);
        ov14_021F46B0(arg0, dest, var_r5, 0x1E, 0x240);
        ov14_021F2C1C(arg0->sys, var_r6, dest, 0x240);
        var_r6 += 1;
        var_r5 += 1;
        sp4 += 1;
    } while (sp4 < 4);
    Heap_Free(pNcgrFile);
    Heap_Free(dest);
}
