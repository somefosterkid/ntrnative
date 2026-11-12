// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_scroll.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_SCROLL_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_SCROLL_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_obj.h"

// The screen's top left corner on the map, in pixels
typedef struct {
    s16 top;
    s16 left;
} WF2DMAP_SCROLL;

// pokebw2's WF2DMAP_SCRContSysMain
// Centers the screen on an object
void func_ov127_021c5994(WF2DMAP_SCROLL *scroll, const WF2DMAP_OBJWK *wk);
// pokebw2's WF2DMAP_SCROLLSysDataInit
void func_ov127_021c59d0(WF2DMAP_SCROLL *scroll);
// pokebw2's WF2DMAP_SCROLLSysDataSet
void func_ov127_021c59dc(WF2DMAP_SCROLL *scroll, s16 top, s16 left);
// pokebw2's WF2DMAP_SCROLLSysTopGet
s16 func_ov127_021c59e4(const WF2DMAP_SCROLL *scroll);
// pokebw2's WF2DMAP_SCROLLSysLeftGet
s16 func_ov127_021c59ec(const WF2DMAP_SCROLL *scroll);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_SCROLL_H
