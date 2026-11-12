// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov203/wf2dmap_scroll.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/wf2dmap/wf2dmap_common.h"
#include "app/wf2dmap/wf2dmap_obj.h"
#include "app/wf2dmap/wf2dmap_scroll.h"
#include "gfl/std.h"

// Where the Wi-Fi Club's 2D map is scrolled to. The ROM has no name for this file: it is named after the older games'
// wf2dmap_scroll.c, and its first function is their wf2dmap_scrcont.c's

// Where on the screen the object that the map follows is drawn
#define WF2DMAP_SCRCONT_CENTER_X 112
#define WF2DMAP_SCRCONT_CENTER_Y 96

// pokebw2's WF2DMAP_SCRContSysMain
void func_ov127_021c5994(WF2DMAP_SCROLL *scroll, const WF2DMAP_OBJWK *wk) {
    WF2DMAP_POS pos;

    pos = func_ov127_021c4adc(wk);
    func_ov127_021c59dc(scroll, pos.y - WF2DMAP_SCRCONT_CENTER_Y, pos.x - WF2DMAP_SCRCONT_CENTER_X);
}

// pokebw2's WF2DMAP_SCROLLSysDataInit
void func_ov127_021c59d0(WF2DMAP_SCROLL *scroll) {
    MI_CpuFill8(scroll, 0, sizeof(WF2DMAP_SCROLL));
}

// pokebw2's WF2DMAP_SCROLLSysDataSet
void func_ov127_021c59dc(WF2DMAP_SCROLL *scroll, s16 top, s16 left) {
    scroll->top = top;
    scroll->left = left;
}

// pokebw2's WF2DMAP_SCROLLSysTopGet
s16 func_ov127_021c59e4(const WF2DMAP_SCROLL *scroll) {
    return scroll->top;
}

// pokebw2's WF2DMAP_SCROLLSysLeftGet
s16 func_ov127_021c59ec(const WF2DMAP_SCROLL *scroll) {
    return scroll->left;
}
