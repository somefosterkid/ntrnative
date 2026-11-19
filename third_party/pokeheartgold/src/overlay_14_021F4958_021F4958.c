#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "overlay_14_box_funcs.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"

u32 ov14_021E7930(UnkStruct_ov14_boxapp *param_1, u32 param_2);
void ov14_021F46B0(UnkStruct_ov14_boxapp *param_1, u8 *param_2, u32 param_3, u32 param_4, u32 param_5);
void ov14_021F4A64(UnkStruct_ov14_boxapp *param_1, u32 param_2, u8 *param_3);
void ov14_021F4958(UnkStruct_ov14_boxapp *param_1, u32 param_2);

void ov14_021F4958(UnkStruct_ov14_boxapp *param_1, u32 param_2) {
    void *pNcgrFile;
    u32 uVar1;
    s32 iVar2;
    NNSG2dCharacterData *apNStack_1c;

    pNcgrFile = AllocAtEndAndReadWholeNarcMemberByIdPair(NARC_a_0_1_9, 0x46, HEAP_ID_10);
    NNS_G2dGetUnpackedCharacterData(pNcgrFile, &apNStack_1c);
    iVar2 = param_2 * 0x400;
    MI_CpuCopy8(apNStack_1c->pRawData, param_1->sys->unk_40C8 + iVar2, 0x400);
    Heap_Free(pNcgrFile);
    uVar1 = ov14_021E7930(param_1, param_2);
    ov14_021F46B0(param_1, param_1->sys->unk_40C8 + iVar2, uVar1, 8, 0x400);
    ov14_021F4A64(param_1, param_2, param_1->sys->unk_40C8 + iVar2);
    return;
}
