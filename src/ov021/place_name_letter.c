// Black's code at ov021 0x021bd214-0x021bd2b8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/ov021_021bd068.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "struct_decls.h"
#include "system/bmp_oam.h"


Ov021Work *func_ov021_021bd214(HeapID param_1) {
    return GFL_HeapAllocate(param_1, sizeof(Ov021Work), 0, "place_name_letter.c", 0x170);
}

void func_ov021_021bd22c(void *ptr) {
    GFL_HeapFree(ptr);
}


void func_ov021_021bd234(Ov021Work *work) {
    MI_CpuFill8(work, 0, sizeof(Ov021Work));
    work->active = 0;
    work->letter = GFL_StrBufGetTerminator();
}

void func_ov021_021bd250(Ov021Work *work) {
    GFLBitmap *pGVar1;

    pGVar1 = GFL_BitmapCreate(2, 2, 0x20, work->heapId);
    work->bitmap = pGVar1;
}

void func_ov021_021bd264(Ov021Work *work) {
    if (work->bitmap != NULL) {
        GFL_BitmapFree(work->bitmap);
        work->bitmap = NULL;
    }
}

void func_ov021_021bd278(Ov021Work *work, BmpOamSys *sys, u16 surface, u32 palette) {
    BmpOamActorSetup setup;

    setup.bitmap = work->bitmap;
    setup.x = 0;
    setup.y = 0;
    setup.palette = palette;
    setup.paletteOffset = 0;
    setup.priority = 0;
    setup.bgPriority = 0;
    setup.surface = surface;
    setup.vramType = 0;
    work->actor = func_020255a0(sys, &setup);
}


void func_ov021_021bd2a4(Ov021Work *work) {
    if (work->actor != NULL) {
        func_0202570c(work->actor);
        work->actor = NULL;
    }
}
