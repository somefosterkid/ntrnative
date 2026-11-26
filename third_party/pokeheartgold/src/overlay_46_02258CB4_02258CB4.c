#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "bg_window.h"
#include "save.h"

void ov46_02258F78(u32 *, u32);

void ov46_022592EC(u32 *, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

void ov46_022594E0(u32 *, u32);

void ov46_02258F70();

u32 ov46_02258CB4(OverlayManager *param_1);

u32 ov46_02258CB4(OverlayManager *param_1) {
    u32 *arg;
    u32 *puVar1;

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x20000);
    arg = OverlayManager_CreateAndGetData(param_1, 0x404, HEAP_ID_119);
    memset(arg, 0, 0x404);
    puVar1 = OverlayManager_GetArgs(param_1);
    *arg = *puVar1;
    ov46_02258F78(arg, 0x77);
    ov46_022592EC(arg + 0x10, arg[3], 1, 800, 2, 0x13, 0x1b, 4, 0x28, *arg, 0x77);
    ov46_022592EC(arg + 0x28, arg[3], 0, 800, 4, 4, 0x17, 0x10, 0x94, *arg, 0x77);
    ov46_022592EC(arg + 4, arg[3], 1, 0x30a, 5, 1, 0x16, 2, 0x204, *arg, 0x77);
    ov46_022594E0(arg + 4, 0x15);
    sub_0203A880();
    Main_SetVBlankIntrCB(ov46_02258F70, arg);
    HBlankInterruptDisable();
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
    return 1;
}
