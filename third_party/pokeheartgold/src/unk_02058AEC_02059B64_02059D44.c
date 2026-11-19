#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"
#include "unk_02035900.h"
#include "union_room_card_task.h"

u16 sub_02057A34(u32);

u16 sub_02057A88(u32);

s32 sub_02057ADC(s32 arg0);

s32 sub_02057B14(u32);

BOOL sub_02059B64(TaskManager *param_1);

void sub_02059D44(FieldSystem *fieldSystem);

void sub_02059D44(FieldSystem *fieldSystem) {
    u16 uVar1;
    s32 iVar2;
    s32 iVar3;
    s32 iVar4;
    UnionRoomPeerCardTask *env;
    u32 uVar5;

    uVar1 = sub_0203769C();
    iVar2 = sub_02057ADC(uVar1);
    iVar3 = sub_02057B14((u32)uVar1);
    uVar5 = 0;
    iVar4 = sub_02037454();
    if (0 < iVar4) {
        do {
            if (((uVar5 != uVar1) && (iVar4 = sub_02057A34(uVar5), iVar2 == iVar4)) && (iVar4 = sub_02057A88(uVar5), iVar3 == iVar4)) {
                env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnionRoomPeerCardTask));
                env->netId = uVar5;
                env->state = 0;
                FieldSystem_CreateTask(fieldSystem, sub_02059B64, env);
                sub_0203E2F4();
                return;
            }
            uVar5 = uVar5 + 1;
            iVar4 = sub_02037454();
        } while ((s32)uVar5 < iVar4);
    }
    return;
}
