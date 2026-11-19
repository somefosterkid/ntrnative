#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_96_app_0221040c.h"

void ov96_022102D4(Ov96App_0221040C *param_1) {
    s32 iVar1;
    Window *window;
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    Heap_Free(param_1->scrnBuf);
    iVar1 = 0;
    window = param_1->windows;
    do {
        RemoveWindow(window);
        iVar1 = iVar1 + 1;
        window = window + 1;
    } while (iVar1 < 4);
    iVar1 = 0;
    do {
        if (param_1->sprites[iVar1] == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(param_1->sprites[iVar1]);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0x1f);
    Heap_Free(param_1);
    return;
}

void ov96_0221031C(Ov96App_0221040C *param_1) {
    ov96_022107D4(param_1);
}
