// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/wifilogin_disp.h: the lines in use so
// far.
#ifndef BWPORT_APP_WIFILOGIN_DISP_H
#define BWPORT_APP_WIFILOGIN_DISP_H

#include "types.h"
#include "gfl/heap.h"

// The graphics the display is made of
#define WIFILOGIN_DISP_STYLE_PDWACC 0

#define WIFILOGIN_DISP_STYLE_GSYNC 1
typedef struct WifiLoginDisp WifiLoginDisp;

// pokebw2's WifiLoginDisp_Create
WifiLoginDisp *func_ov115_021d4d38(HeapID heapId, u32 style, u32 unused);
// pokebw2's WifiLoginDisp_Main
void func_ov115_021d4df4(WifiLoginDisp *disp);
// pokebw2's WifiLoginDisp_Free
void func_ov115_021d4e04(WifiLoginDisp *disp);
// pokebw2's WifiLoginDisp_SetUnk9C4
void func_ov115_021d592c(WifiLoginDisp *disp);
// pokebw2's WifiLoginDisp_ResetBlend
void func_ov115_021d5938(WifiLoginDisp *disp);

#endif // BWPORT_APP_WIFILOGIN_DISP_H
