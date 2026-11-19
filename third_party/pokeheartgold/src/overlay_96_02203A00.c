#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_96_ov96_pic_panel.h"

Ov96PicPanel *ov96_02203A00(enum HeapID heapID, BgConfig *bgConfig, struct PokeathlonCourseData *course) {
    Ov96PicPanel *panel;

    panel = Heap_Alloc(heapID, sizeof(Ov96PicPanel));
    MI_CpuFill8(panel, 0, sizeof(Ov96PicPanel));
    panel->heapID = heapID;
    panel->bgConfig = bgConfig;
    panel->course = course;
    ov96_02203FBC(panel);
    ov96_02203FFC(panel);
    return panel;
}

void ov96_02203A30(Ov96PicPanel *panel) {
    s32 i;

    i = 0;
    do {
        Heap_Free(panel->pics[i].charData);
        Heap_Free(panel->pics[i].plttData);
        i = i + 1;
    } while (i < 3);
    Heap_Free(panel->scrnBuf);
    RemoveWindow(&panel->window);
    Heap_Free(panel);
}
