#include "global.h"

#include "field_communication_manager.h"
#include "field_types_def.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "sys_task_api.h"
#include "unk_02035900.h"
#include "unk_02091564.h"
#include "sys_task.h"

static void sub_02058034(void);

void sub_02058098(void);

static void sub_02058034(void) {
}

void sub_020582CC(SysTask *task, void *data);

void sub_02058038(FieldSystem *fieldSystem);

void sub_02058038(FieldSystem *fieldSystem) {
    FieldCommunicationManager *dest;

    if (_021D41C8 == NULL) {
        sub_02091574(fieldSystem);
        dest = Heap_Alloc(HEAP_ID_15, sizeof(FieldCommunicationManager));
        _021D41C8 = dest;
        MI_CpuFill8(dest, 0U, sizeof(FieldCommunicationManager));
        _021D41C8->timer = 0x32;
        _021D41C8->sysTask = SysTask_CreateOnMainQueue(sub_020582CC, NULL, 0xAU);
        _021D41C8->fieldSystem = fieldSystem;
        _021D41C8->party = NULL;
        sub_02058034();
        sub_0203778C(&_021D41C8->rand);
    }
}

void sub_02058098(void) {
    s32 i;
    void *ptr;
    Party *party;

    if (_021D41C8 != NULL) {
        SysTask_Destroy(_021D41C8->sysTask);
        i = 0;
        do {
            ptr = _021D41C8->trainerCase[i];
            if (ptr != NULL) {
                Heap_Free(ptr);
            }
            i += 1;
        } while (i < FIELD_COMM_MAX_PLAYERS);
        party = _021D41C8->party;
        if (party != NULL) {
            Heap_Free(party);
        }
        Heap_Free(_021D41C8);
        _021D41C8 = NULL;
    }
}
