// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_scrdraw.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_SCRDRAW_H
#define BWPORT_APP_WF2DMAP_WF2DMAP_SCRDRAW_H

#include "types.h"
#include "app/wf2dmap/wf2dmap_scroll.h"
#include "gfl/clact.h"

typedef struct {
    // 0 for the main screen's surface of the renderer, or else the sub screen's
    u8 surface;
    u8 bg;
    // The BG's BGSetup values
    u8 colorMode;
    u8 screenBase;
    u8 charBase;
    u8 extPaletteSlot;
    u8 priority;
    u8 mosaic;
    // The map's screen (NSCR)
    u8 arcId;
    u8 fileId;
    u8 compressed;
} WF2DMAP_SCRDRAWINIT;

typedef struct WF2DMAP_SCRDRAW WF2DMAP_SCRDRAW;

// pokebw2's WF2DMAP_SCRDrawSysMain
void func_ov127_021c5a60(WF2DMAP_SCRDRAW *sys, const WF2DMAP_SCROLL *scroll);
// pokebw2's WF2DMAP_SCRDrawSysInit
// unk1 is never used
WF2DMAP_SCRDRAW *func_ov127_021c59f4(ClActRenderer *renderer, void *unk1, const WF2DMAP_SCRDRAWINIT *init,
                                        u32 heapId);

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_SCRDRAW_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wf2dmap/wf2dmap_scrdraw.h: the lines in
// use so far.
#ifndef BWPORT_APP_WF2DMAP_WF2DMAP_SCRDRAW_H


// pokebw2's WF2DMAP_SCRDrawSysInit
// unk1 is never used
WF2DMAP_SCRDRAW *func_ov127_021c59f4(ClActRenderer *renderer, void *unk1, const WF2DMAP_SCRDRAWINIT *init,
                                        u32 heapId);
// pokebw2's WF2DMAP_SCRDrawSysMain

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_SCRDrawSysExit
void func_ov127_021c5a4c(WF2DMAP_SCRDRAW *sys);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WF2DMAP_SCRDrawSysExit

#endif // BWPORT_APP_WF2DMAP_WF2DMAP_SCRDRAW_H
