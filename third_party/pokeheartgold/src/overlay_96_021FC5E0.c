#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_ov96_icon_work.h"
#include "sprite.h"

void ov96_021FC5E0(SpriteTemplate *template, Ov96IconWork *work, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE vram) {
    template->spriteList = work->spriteList;
    template->header = &work->resHeader;
    template->position.x = 0xf0000;
    template->position.y = 0xe000;
    template->position.z = 0;
    template->scale.x = 0x1000;
    template->scale.y = 0x1000;
    template->scale.z = 0x1000;
    template->rotation = 0;
    template->drawPriority = 2;
    template->whichScreen = vram;
    template->heapID = work->heapID;
}

void *ov96_021FC618(enum HeapID param_1);

void *ov96_021FC618(enum HeapID param_1) {
    void *dest;

    dest = Heap_Alloc(param_1, 0x84);
    MI_CpuFill8(dest, 0, 0x84);
    return dest;
}
