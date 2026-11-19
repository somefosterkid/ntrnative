#include "global.h"

#include "battle/battle_system.h"

#include "heap.h"
#include "sys_task_api.h"
#include "unk_02033AE0.h"

typedef struct UnkStruct_sub_02074E5C_Heap_Alloc_2 {
    BattleSystem *unk_0;
    s8 unk_4;
    u8 unk_5[0x3];
} UnkStruct_sub_02074E5C_Heap_Alloc_2;

typedef struct UnkStruct_sub_02074E5C_Heap_Alloc {
    BattleSystem *unk_0;
    s8 unk_4;
    u8 unk_5[0x3];
} UnkStruct_sub_02074E5C_Heap_Alloc;

void sub_02075434(SysTask *task, void *data);

void sub_020754C0(SysTask *task, void *data);

extern s32 _020FFE30;

void sub_02074E5C(BattleSystem *arg0);

void sub_02074E5C(BattleSystem *battleSystem) {
    UnkStruct_sub_02074E5C_Heap_Alloc *v0;
    UnkStruct_sub_02074E5C_Heap_Alloc_2 *v1;

    if (!(BattleSystem_GetBattleSpecial(battleSystem) & 0x10)) {
        v0 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_sub_02074E5C_Heap_Alloc));
        v1 = Heap_Alloc(HEAP_ID_BATTLE, sizeof(UnkStruct_sub_02074E5C_Heap_Alloc_2));
        sub_0203410C(&_020FFE30, 0xC, battleSystem);
        v0->unk_0 = battleSystem;
        v0->unk_4 = 0;
        v1->unk_0 = battleSystem;
        v1->unk_4 = 0;
        ov12_0223BBFC(battleSystem, (u8 *)&v0->unk_4);
        ov12_0223BC08(battleSystem, (u8 *)&v1->unk_4);
        SysTask_CreateOnMainQueue(sub_02075434, v0, 0U);
        SysTask_CreateOnMainQueue(sub_020754C0, v1, 0U);
    }
}
