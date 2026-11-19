#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "palette.h"
#include "task.h"
#include "vram_transfer_manager.h"

s32 ov01_021F771C(MapObjectManager *);

u32 ov01_02206088(u32 spriteId);

s32 sub_02023FB0();

s32 ov01_0220610C(TaskManager *arg0);

s32 ov01_0220610C(TaskManager *arg0) {
    FieldSystem *temp_r6;
    NNSG3dResFileHeader *temp_r0_3;
    NNSG3dResTex *temp_r0_4;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r6_2;
    s32 temp_r6_3;
    u32 *temp_r0;
    u32 temp_r0_2;
    u32 temp_r7;
    u8 temp_r0_5;
    u8 temp_r0_6;
    FollowMonFadeTask *temp_r4;

    temp_r6 = TaskManager_GetFieldSystem(arg0);
    temp_r4 = TaskManager_GetEnvironment(arg0);
    temp_r0 = TaskManager_GetStatePtr(arg0);
    temp_r0_2 = *temp_r0;
    switch (temp_r0_2) {
    case 0:
        temp_r7 = ov01_02206088(MapObject_GetSpriteID(FollowMon_GetMapObject(temp_r6)));
        temp_r0_3 = NARC_AllocAndReadWholeMember(MapObjectManager_GetMapModelNarc(temp_r6->mapObjectManager), temp_r7, HEAP_ID_FIELD2);
        temp_r0_4 = NNS_G3dGetTex(temp_r0_3);
        MIi_CpuCopy16(((u8 *)temp_r0_4) + temp_r0_4->plttInfo.ofsPlttData, temp_r4->palette, 0x40U);
        Heap_Free(temp_r0_3);
        *temp_r0 += 1;
        break;
    case 1:
        temp_r0_5 = temp_r4->timer;
        if (temp_r0_5 != 0) {
            temp_r4->timer = (u8)(temp_r0_5 - 1);
        } else {
            temp_r4->level = (s8)(temp_r4->level + temp_r4->step);
            if ((s32) temp_r4->level >= 0x10) {
                temp_r4->level = 0x10;
                *temp_r0 += 1;
            }
            BlendPalette(temp_r4->palette, temp_r4->blended, 0x20U, (u8) temp_r4->level, 0xFFFFU);
            temp_r6_2 = ov01_021F771C(temp_r6->mapObjectManager);
            temp_r5 = sub_02023FB0();
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u32)(sub_02023FB0(temp_r6_2) << 0x10) >> 0xD, temp_r4->blended, ((u32)(0xFFFF0000 & temp_r5) >> 0x10) * 8);
            temp_r4->timer = (u8) temp_r4->delay;
        }
        break;
    case 2:
        temp_r0_6 = temp_r4->timer;
        if (temp_r0_6 != 0) {
            temp_r4->timer = (u8)(temp_r0_6 - 1);
        } else {
            temp_r4->level = (s8)(temp_r4->level - temp_r4->step);
            if ((s32) temp_r4->level <= 0) {
                *temp_r0 += 1;
            }
            BlendPalette(temp_r4->palette, temp_r4->blended, 0x20U, (u8) temp_r4->level, 0xFFFFU);
            temp_r6_3 = ov01_021F771C(temp_r6->mapObjectManager);
            temp_r5_2 = sub_02023FB0();
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (u32)(sub_02023FB0(temp_r6_3) << 0x10) >> 0xD, temp_r4->blended, ((u32)(0xFFFF0000 & temp_r5_2) >> 0x10) * 8);
            temp_r4->timer = (u8) temp_r4->delay;
        }
        break;
    case 3:
        Heap_Free(temp_r4);
        return 1;
    }
    return 0;
}
