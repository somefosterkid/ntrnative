#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "overlay_01.h"
#include "overlay_01_021FCE98.h"
#include "overlay_01_022053EC.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "pokemon_types_def.h"
#include "task.h"

s32 ov01_022062CC(FieldSystem *);

BOOL Task_UseSweetScentInField(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    FieldMoveEnvironment *ptr;
    u32 *puVar1;
    s32 sVar2;
    s32 iVar3;
    u32 uVar4;
    void *pvVar5;
    BOOL BVar6;
    u8 a1;
    SweetScentTaskData *ptr_00;
    u32 uVar7;
    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    puVar1 = TaskManager_GetStatePtr(taskManager);
    ptr_00 = (SweetScentTaskData *)ptr->moveData;
    switch (*puVar1) {
    case 0:
        uVar7 = ((FieldMoveData *)ptr_00)->partySlot;
        Heap_Free(ptr_00);
        ptr_00 = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(SweetScentTaskData));
        ptr->moveData = (FieldMoveData *)ptr_00;
        ptr_00->partySlot = uVar7;
        *puVar1 = (*puVar1) + 1;
        break;

    case 1:
        uVar4 = GetHoneySweetScentWorkSize();
        pvVar5 = Heap_AllocAtEnd(HEAP_ID_FIELD2, uVar4);
        ptr_00->effectWork = pvVar5;
        memset(pvVar5, 0, uVar4);
        sVar2 = PlayerAvatar_GetState(fieldSystem->playerAvatar);
        if ((sVar2 - 1U) <= (2 - 1)) {
            *puVar1 = (*puVar1) + 1;
        } else {
            iVar3 = ov01_022062CC(fieldSystem);
            if ((ptr_00->partySlot == iVar3) && (BVar6 = FollowMon_IsVisible(fieldSystem), BVar6 != 0)) {
                TaskManager_Call(taskManager, ov01_02205A60, (void *)0x0);
                *puVar1 = 4;
            } else {
                *puVar1 = (*puVar1) + 1;
            }
        }
        break;

    case 2:
        uVar4 = PlayerAvatar_GetGender(fieldSystem->playerAvatar);
        ptr_00->cutInTask = ov02_02249458(fieldSystem, 0, ptr->mon, uVar4);
        *puVar1 = (*puVar1) + 1;
        break;

    case 3:
        BVar6 = ov02_0224953C(ptr_00->cutInTask);
        if (BVar6 != 0) {
            ov02_02249548(ptr_00->cutInTask);
            *puVar1 = 6;
        }
        break;

    case 4:
        BVar6 = ov02_02250780(fieldSystem, 0xc);
        if (BVar6 != 0) {
            a1 = 2;
            FieldSystem_UnkSub108_AddMonMood((FieldSystemUnkSub108 *)fieldSystem->unk108, 1);
        } else {
            a1 = 1;
        }
        ov02_022507B4(fieldSystem, a1);
        *puVar1 = (*puVar1) + 1;
        break;

    case 5:
        ov01_021FCFEC(fieldSystem);
        *puVar1 = (*puVar1) + 1;
        break;

    case 6:
        TaskManager_Call(taskManager, Task_HoneyOrSweetScent, ptr_00->effectWork);
        *puVar1 = (*puVar1) + 1;
        break;

    case 7:
        Heap_Free(ptr_00);
        Heap_Free(ptr);
        return 1;
    }

    return 0;
}
