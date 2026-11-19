#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_102_app_021eb1e0.h"

BgConfig *ov102_021EA268(void *);

void ov102_021EB29C(Ov102App_021EB1E0 *param_1);

void ov102_021EB29C(Ov102App_021EB1E0 *param_1) {
    BgConfig *bgConfig;
    Window *window;
    s32 iVar2;

    bgConfig = ov102_021EA268(param_1->unk_0);
    if (param_1->unk_C != NULL) {
        Sprite_Delete(param_1->unk_C);
    }
    iVar2 = 0;
    window = param_1->unk_24;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        String_Delete(param_1->unk_18[iVar2]);
        window = window + 1;
        iVar2 = iVar2 + 1;
    } while (iVar2 < 3);
    Heap_Free(param_1->unk_58);
    Heap_Free(param_1->unk_54);
    BgCommitTilemapBufferToVram(bgConfig, 3);
    return;
}
