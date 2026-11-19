#include "global.h"
#include "heap.h"
#include "sys_task_api.h"

void ov96_021E8884(SysTask *task, void *param);

void ov96_021E88FC(void *arg0);

void ov96_021E8884(SysTask *task, void *param) {
    s32 *param_2 = param;
    s32 iVar1;
    s32 *piVar2;
    s32 iStack_20;
    iStack_20 = 0;
    piVar2 = param_2;
    do {
        if ((*piVar2) == 1) {
            iVar1 = piVar2[0xc];
            if (piVar2[0xc] <= (0x27 - 1)) {
                (*(*((void (**)())(param_2[0x70] + (piVar2[1] * 0xc)))))(0, piVar2[0xc], piVar2 + 2, param_2[0x72]);
            } else {
                (*(*((void (**)())(param_2[0x70] + (piVar2[1] * 0xc)))))(0, iVar1, piVar2[0xd], param_2[0x72]);
            }
            memset(piVar2, 0, 0x38);
        }
        piVar2 = piVar2 + 0xe;
        iStack_20 = iStack_20 + 1;
    } while (iStack_20 < 8);
    return;
}

void ov96_021E88FC(void *ptr) {
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 0x1CC));
    Heap_Free(ptr);
}
