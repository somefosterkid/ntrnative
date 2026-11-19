#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "unk_02058AEC.h"
#include "heap.h"
#include "player_data.h"
#include "sys_task_api.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "union_room_manager.h"

void sub_0205A034(UnionRoomManager *manager, void (*task)(UnionRoomManager *manager), int timer);

void sub_0205A07C(UnionRoomManager *manager);

void sub_0205A03C(SysTask *task, void *data);
void sub_0205ABBC(UnionRoomManager *manager);
UnionRoomManager *sub_02059E1C(FieldSystem *fieldSystem);

void sub_02059E04(FieldSystem *fieldSystem) {
    if (fieldSystem->unk80 != NULL) {
        sub_0205A034(fieldSystem->unk80, sub_0205A07C, 5);
    }
    return;
}

UnionRoomManager *sub_02059E1C(FieldSystem *fieldSystem) {
    SaveData *saveData;
    UnionRoomManager *manager;

    if (fieldSystem->unk80 != NULL) {
        return NULL;
    }
    saveData = FieldSystem_GetSaveData(fieldSystem);
    sub_02037F18(saveData);
    manager = Heap_Alloc(HEAP_ID_31, sizeof(UnionRoomManager));
    MI_CpuFill8(manager, 0U, sizeof(UnionRoomManager));
    manager->task = NULL;
    manager->timer = 0x28;
    manager->sysTask = SysTask_CreateOnMainQueue(sub_0205A03C, manager, 0xAU);
    manager->fieldSystem = fieldSystem;
    manager->saveData = saveData;
    manager->trainerInfo = Save_PlayerData_GetProfile(saveData);
    sub_0205ABBC(manager);
    sub_0203778C(&manager->rand);
    return manager;
}
