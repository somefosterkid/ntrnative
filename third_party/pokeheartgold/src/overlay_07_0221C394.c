#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

BOOL ov07_0221C3DC(BattleAnimSystem *system);

BOOL ov07_0221C394(BattleAnimSystem *system);

struct BattleAnimSystem;
enum BattleAnimTaskKind {
    BATTLE_ANIM_TASK_KIND_EFFECT = 1,
    BATTLE_ANIM_TASK_KIND_SOUND,
};
void ov07_0221BEA4(s32 kind, BattleAnimSystem *system, SysTask *task);
void ov07_0221C458(BattleAnimSystem *system, SysTask *task);

BOOL ov07_0221C394(BattleAnimSystem *system) {
    if (ov07_0221C3DC(system) == 0) {
        return 0;
    }
    system->executeAnimScriptFunc(system);
    return 1;
}

BOOL ov07_0221C3B0(BattleAnimSystem *system);

BOOL ov07_0221C3B0(BattleAnimSystem *system) {
    if (system->moveActive == 1) {
        return 1;
    }
    return 0;
}

BOOL ov07_0221C3C0(BattleAnimSystem *system);

BOOL ov07_0221C3C0(BattleAnimSystem *system) {
    void *ptr;

    ptr = system->scriptData;
    if (ptr == NULL) {
        if (ptr == NULL) {
            GF_AssertFail();
        }
        return 0;
    }
    Heap_Free(ptr);
    return 1;
}

BOOL ov07_0221C3DC(BattleAnimSystem *system) {
    if (system == NULL) {
        return 0;
    }
    if (system->isActive == 1) {
        return 1;
    }
    return 0;
}

SysTask *ov07_0221BE68(u8 kind, BattleAnimSystem *system, SysTaskFunc func, void *param, u32 priority);

SysTask *ov07_0221C3F4(BattleAnimSystem *arg0, SysTaskFunc arg1, void *arg2, s32 arg3) {
    return ov07_0221BE68(1, arg0, arg1, arg2, arg3);
}

SysTask *ov07_0221C410(BattleAnimSystem *arg0, SysTaskFunc arg1, void *arg2) {
    return ov07_0221BE68(1, arg0, arg1, arg2, 0x44C);
}

SysTask *ov07_0221C42C(BattleAnimSystem *arg0, SysTaskFunc arg1, void *arg2, s32 arg3) {
    return ov07_0221BE68(2, arg0, arg1, arg2, arg3);
}

void ov07_0221C448(BattleAnimSystem *system, SysTask *task) {
    ov07_0221BEA4(BATTLE_ANIM_TASK_KIND_EFFECT, system, task);
}

void ov07_0221C458(BattleAnimSystem *system, SysTask *task) {
    ov07_0221BEA4(BATTLE_ANIM_TASK_KIND_SOUND, system, task);
}
