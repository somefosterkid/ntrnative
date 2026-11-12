// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_common.h: the lines in use so
// far.
#ifndef BWPORT_APP_MANUAL_COMMON_H
#define BWPORT_APP_MANUAL_COMMON_H

#include "types.h"
#include "app/manual_data.h"
#include "app/manual_graphic.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct ManualTouchBar ManualTouchBar;

typedef struct {
    GameData *gameData;
    HeapID heapId;
    ManualGraphic *graphic;
    Font *font;
    // The queues of the explanation's title, of the buttons and the explanation's text, and of the touch bar
    PrintQueue *titleQueue;
    PrintQueue *queue;
    PrintQueue *touchBarQueue;
    MsgData *msgSystem;
    MsgData *msgManual;
    ArcTool *arc;
    ArcTool *imageArc;
    ManualData *data;
    // The picture on the upper screen, or MANUAL_IMAGE_NONE
    u16 image;
    ManualTouchBar *touchBar;
} ManualCommon;

// Black 2's; twinned code confirms the offsets of bg, x, y, width, height, palette and fromEnd in Black
// A window of the lower screen, and where its text goes
typedef struct {
    u8 bg;
    u8 x;
    u8 y;
    u8 width;
    u8 height;
    u8 palette;
    u8 fromEnd;
    u8 textX;
    u8 textY;
} ManualWindowSetup;

// pokebw2's ManualCommon_LoadImage
// Shows a picture of ARCID_MANUAL_IMAGE on the upper screen, or clears it with MANUAL_IMAGE_NONE
void func_ov225_021b9e80(ManualCommon *common, u16 image);

// pokebw2's ManualCommon_Free
void func_ov225_021b9dec(ManualCommon *common);
// pokebw2's ManualCommon_Main
void func_ov225_021b9e60(ManualCommon *common);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualCommon_Create
ManualCommon *func_ov225_021b9d18(GameData *gameData, HeapID heapId);

#endif // BWPORT_APP_MANUAL_COMMON_H
