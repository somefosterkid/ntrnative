// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/beacon_view_common.h: the lines in use
// so far.
#ifndef BWPORT_FIELD_BEACON_VIEW_COMMON_H
#define BWPORT_FIELD_BEACON_VIEW_COMMON_H

#include "types.h"
#include "system/printsys.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
// A window whose text goes through the print queue. The window, the font, the queue and the task manager belong to
// the caller
typedef struct {
    BmpWin *window;
    GFLBitmap *bitmap;
    PrintWindow printWindow;
} BeaconViewWin;

// Black 2's, not checked against Black
// A bitmap shown as OAM, whose text goes through the print queue
typedef struct {
    BmpOamActor *actor;
    GFLBitmap *bitmap;
    Font *font;
    PrintQueue *queue;
    TCBExManager *tcbManager;
} BeaconViewOam;

#endif // BWPORT_FIELD_BEACON_VIEW_COMMON_H
