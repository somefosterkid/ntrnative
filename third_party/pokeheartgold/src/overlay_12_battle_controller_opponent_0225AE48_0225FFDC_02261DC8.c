#include "global.h"

#include "heap.h"
#include "overlay_12_trainer_encounter_data.h"
#include "sprite_system.h"
#include "sys_task_api.h"

void ov12_02261E40(SysTask *task, void *data);

BOOL ov12_02261DC8(TrainerEncounterData *arg0, ManagedSprite *unused) {
    s32 v4;
    u32 v0;
    u8 v3;
    void *v1;

    v4 = 0;
    v0 = ManagedSprite_GetUserAttrForCurrentAnimFrame(arg0->managedSprite);
    switch (v0) {
    case 0x1:
        if (arg0->ballFlashStarted == 0) {
            arg0->ballFlashStarted = 1;
            v1 = Heap_Alloc(HEAP_ID_BATTLE, 8U);
            *(s32 *)v1 = 0;
            *(s32 *)((u8 *)v1 + 4) = 0;
            SysTask_CreateOnMainQueue(ov12_02261E40, v1, 0U);
        }
        break;
    case 0xFFF:
        v4 = 1;
        break;
    default:
        if ((v0 & 0xF00) == 0x100) {
            v3 = (u8)v0;
            if (v3 != 0) {
                ManagedSprite_SetAnimationFrame(arg0->managedSprite, 0U);
                ManagedSprite_SetAnim(arg0->managedSprite, v3 - 1);
                v4 = 1;
            }
        }
        break;
    }
    return v4;
}
