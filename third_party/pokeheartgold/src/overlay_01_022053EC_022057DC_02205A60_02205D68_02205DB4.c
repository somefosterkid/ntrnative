#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "overlay_01_022031C0.h"
#include "task.h"
#include "unk_02062108.h"
#include "vram_transfer_manager.h"

s32 ov01_02206088(u32 spriteId);

s32 sub_02023FB0(void *arg0);

Billboard *ov01_021F771C(MapObjectManager *);

void sub_02023E78(void *, u32 *);

extern VecFx32 ov01_022096F8;

BOOL ov01_02205DB4(TaskManager *param_1);

BOOL ov01_02205DB4(TaskManager *param_1) {
    FieldSystem *fieldSystem;
    s32 *ptr;
    u32 *puVar1;
    BOOL BVar2;
    u32 file_id;
    NARC *narc;
    NNSG3dResFileHeader *header;
    NNSG3dResTex *pNVar3;
    s32 iVar4;
    void *arg0;
    u32 uVar5;
    u32 uVar6;
    LocalMapObject *pLVar7;
    VecFx32 uStack_20;

    fieldSystem = TaskManager_GetFieldSystem(param_1);
    ptr = TaskManager_GetEnvironment(param_1);
    puVar1 = TaskManager_GetStatePtr(param_1);
    switch (*puVar1) {
    case 0:
        pLVar7 = FollowMon_GetMapObject(fieldSystem);
        MapObject_UnpauseMovement(pLVar7);
        *puVar1 = *puVar1 + 1;
    case 1:
        pLVar7 = FollowMon_GetMapObject(fieldSystem);
        BVar2 = MapObject_IsMovementPaused(pLVar7);
        if (BVar2 != 0) {
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 2:
        pLVar7 = FollowMon_GetMapObject(fieldSystem);
        file_id = ov01_02206088(MapObject_GetSpriteID(pLVar7));
        narc = MapObjectManager_GetMapModelNarc(fieldSystem->mapObjectManager);
        header = NARC_AllocAndReadWholeMember(narc, file_id, HEAP_ID_FIELD2);
        pNVar3 = NNS_G3dGetTex(header);
        MIi_CpuCopy16((u8 *)pNVar3 + (pNVar3->plttInfo).ofsPlttData, ptr + 1, 0x40);
        Heap_Free(header);
        ov01_0220329C(pLVar7, 1);
        *puVar1 = *puVar1 + 1;
        break;
    case 3:
        iVar4 = *ptr;
        *ptr = iVar4 + 1;
        if (iVar4 + 1 >= 0x14) {
            ov01_02205790(fieldSystem, 0);
            uStack_20 = ov01_022096F8;
            arg0 = (void *)ov01_021F771C(fieldSystem->mapObjectManager);
            sub_02023E78(arg0, (u32 *)&uStack_20);
            uVar5 = sub_02023FB0(arg0);
            uVar6 = sub_02023FB0(arg0);
            GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, (uVar6 & 0xffff) << 3, ptr + 1, ((uVar5 & 0xffff0000) >> 0x10) << 3);
            sub_0206A054(fieldSystem);
            pLVar7 = FollowMon_GetMapObject(fieldSystem);
            sub_02069E28(pLVar7, 0);
            *puVar1 = *puVar1 + 1;
        }
        break;
    case 4:
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
