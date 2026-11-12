// Black's code at ov021 0x021b2ebc-0x021b2fb0. Placed by tools/gap_files.py from 1 name, 1 pokebw2.

#include "types.h"
#include "field/field.h"
#include "gfl/fade.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "struct_decls.h"

// The work of a field fade (pokebw2's FieldFadeWork)
typedef struct FieldFadeWork {
    GameSystem *gsys;
    Field *field;
    u32 slow;
    u32 waitDone;
    u32 type;
    s32 stage;
} FieldFadeWork;

void func_ov021_021b2f18(TCB *param_1, void *param_2);
void Field_SetFadeFlag(Field *field, BOOL flag);

void FieldFadeTCB_Start(GameSystem *gsys, Field *field, u32 a2, u32 a3, u32 a4) {
    TCBManager *manager;
    FieldFadeWork *data;

    manager = Field_GetTCBMgr(field);
    data = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(FieldFadeWork), 1, "scrcmd_screeneffects.c", 0x369);
    GFL_TCBMgrAddTask(manager, func_ov021_021b2f18, data, 0);
    Field_SetFadeFlag(field, 1);
    data->stage = 0;
    data->gsys = gsys;
    data->field = field;
    data->slow = a3;
    data->waitDone = a4;
    data->type = a2;
}

void func_ov021_021b2f18(TCB *param_1, void *param_2) {
    FieldFadeWork *wk = param_2;
    s32 startBright;
    s32 state;
    s32 done;
    s32 fadeStep;
    s32 endBright;
    u32 mode;
    u32 savedBGs;
    u32 targetScreens;

    state = wk->stage;
    switch (state) {
    case 0:
        mode = wk->type;
        if (mode <= 1U) {
            fadeStep = -1;
        } else {
            fadeStep = 4;
        }
        if ((mode == 0) || (mode == 2)) {
            startBright = 0x10;
            endBright = 0;
            savedBGs = GFL_BGSysGetEnabledBGsA();
            FieldG2D_SetLCDConfig();
            GFL_BGSysSetEnabledBGsA(savedBGs);
            FieldG2D_Prepare3DSurface(wk->field);
        } else {
            startBright = 0;
            endBright = 0x10;
        }
        if (wk->slow == 0) {
            targetScreens = 3;
        } else {
            targetScreens = 0xC;
        }
        GFL_FadeSet(targetScreens, startBright, endBright, fadeStep);
        wk->stage++;
        return;
    case 1:
        done = 0;
        if (wk->waitDone == 1) {
            done = 1;
        }
        if (GFL_FadeIsRunning() == 0) {
            done = 1;
        }
        if (done != 0) {
            Field_SetFadeFlag(wk->field, 0);
            GFL_TCBRemove(param_1);
            GFL_HeapFree(param_2);
        }
        return;
    }
}
