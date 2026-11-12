// Black's code at ov021 0x021b5a18-0x021b5b4c. Placed by tools/gap_files.py from 2 asserts.

#include "types.h"

#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "struct_decls.h"

u16 *func_ov021_0218ed50(FieldCamera *camera);

static u16 data_ov021_021ee724;

typedef struct {
    u32 base;
    s32 range;
    s32 step;
    s32 steps;
} Ov021ValueTween;

#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_script.h"
#include "system/game_system.h"

void func_ov021_021b5b0c(TCB *tcb, Ov021ValueTween *tween);
u32 func_ov021_021b5a18(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;
    Field *field;
    TCBManager *tcbMgr;
    HeapID heapId;
    FieldCamera *camera;
    MMSys *actorSys;
    Ov021ValueTween *tween;
    u32 steps;
    u32 rem;
    FieldEvCameraAnimationCoords *coords;

    FieldScriptEnv_GetScriptWork(env);
    gsys = FieldScriptEnv_GetGameSystem(env);
    FieldScriptEnv_GetGameData(env);
    field = GSYS_GetField(gsys);
    tcbMgr = Field_GetTCBMgr(field);
    heapId = Field_GetHeapID(field);
    camera = Field_GetCameraSystem(field);
    actorSys = Field_GetActorSystem(field);
    steps = ScriptReadAny(vm, env);
    tween = GFL_HeapAllocate(heapId, 0x10, TRUE, "scrcmd_c03center.c", 93);
    data_ov021_021ee724 = *func_ov021_0218ed50(camera);
    func_ov010_0216cdb8(actorSys, &data_ov021_021ee724);
    coords = func_ov021_0218fe70(camera);
    tween->base = data_ov021_021ee724;
    tween->range = coords->yaw;
    rem = tween->range % 0x4000;
    if (rem >= 0x2000) {
        tween->range = tween->range + (0x4000 - rem);
    } else {
        tween->range = tween->range - rem;
    }
    tween->range = tween->range - tween->base;
    if ((tween->range < 0 ? -tween->range : tween->range) >= 0x8000) {
        if (tween->range > 0) {
            tween->range = tween->range - 0x10000;
        } else {
            tween->range = tween->range + 0x10000;
        }
    }
    tween->step = 0;
    tween->steps = steps;
    GFL_TCBMgrAddTask(tcbMgr, (TCBFunc)func_ov021_021b5b0c, tween, 0);
    return 0;
}

void func_ov021_021b5b0c(TCB *tcb, Ov021ValueTween *tween) {
    tween->step++;
    data_ov021_021ee724 = tween->range * tween->step / tween->steps;
    data_ov021_021ee724 += (u16)tween->base;
    if (tween->step >= tween->steps) {
        GFL_HeapFree(tween);
        GFL_TCBRemove(tcb);
    }
}

