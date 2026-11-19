#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "render_window.h"

void ov92_0225E360(s32 *param_1);

void ov92_0225E360(s32 *param_1) {
    BOOL BVar1;
    s32 iVar2;
    Window *window;

    iVar2 = 0;
    if (0 < param_1[1]) {
        window = (Window *)(param_1 + 0x7fc);
        do {
            if (iVar2 != *param_1) {
                BVar1 = WindowIsInUse(window);
                if (BVar1 == 1) {
                    ClearWindowPixelBuffer(window, 1);
                    ClearWindowTilemapAndCopyToVram(window);
                    RemoveWindow(window);
                }
                window = window + 1;
            }
            iVar2 = iVar2 + 1;
        } while (iVar2 < param_1[1]);
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(NARC **)param_1[5], 0x4e, (BgConfig *)((void **)param_1[5])[4], GF_BG_LYR_SUB_3, 0, 0, 0, HEAP_ID_113);
    return;
}
