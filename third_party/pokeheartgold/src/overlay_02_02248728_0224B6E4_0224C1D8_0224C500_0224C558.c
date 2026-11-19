#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "overlay_01_022053EC.h"
#include "overlay_02.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sound_chatot.h"
#include "task.h"
#include "unk_02005D10.h"

extern s32 (*ov02_02253724[6])(TaskManager *, FieldSystem *, FieldMoveTaskEnvironment *);

extern s32 (*ov02_0225373C[6])(TaskManager *, FieldSystem *, FieldMoveTaskEnvironment *);

BOOL Task_FieldTeleport(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    FieldMoveTaskEnvironment *ptr;
    u32 *puVar1;
    u32 uVar2;
    u32 uVar3;
    BOOL BVar4;
    u8 a1;
    s32 iVar5;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    puVar1 = TaskManager_GetStatePtr(taskManager);
    switch (*puVar1) {
    case 0:
        if (ptr->followMonActive != 0) {
            TaskManager_Call(taskManager, ov01_02205A60, (void *)0x0);
        }
        *puVar1 = *puVar1 + 1;
        break;
    case 1:
        *puVar1 = *puVar1 + 1;
        if (ptr->followMonActive != 0) {
            BVar4 = ov02_02250780(fieldSystem, 0xe);
            if (BVar4 != 0) {
                a1 = 2;
                FieldSystem_UnkSub108_AddMonMood(fieldSystem->unk108, 1);
            } else {
                a1 = 1;
            }
            ov02_022507B4(fieldSystem, a1);
            break;
        }
    case 2:
        if (ptr->followMonActive != 0) {
            uVar2 = GetMonData(ptr->mon, 5, (void *)0x0);
            uVar3 = GetMonData(ptr->mon, 0x70, (void *)0x0);
            PlayCry((u16)uVar2, (u8)uVar3);
        }
        *puVar1 = *puVar1 + 1;
    case 3:
        do {
            if (ptr->followMonActive != 0) {
                BVar4 = IsCryFinished();
                if (BVar4 != 0) {
                    iVar5 = 0;
                } else {
                    iVar5 = ov02_0225373C[ptr->state](taskManager, fieldSystem, ptr);
                }
            } else {
                iVar5 = ov02_02253724[ptr->state](taskManager, fieldSystem, ptr);
            }
            if (iVar5 == 2) {
                Heap_Free(ptr);
            }
        } while (iVar5 == 1);
    }
    return 0;
}
