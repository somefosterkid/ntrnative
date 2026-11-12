// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_draw.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_DRAW_H
#define BWPORT_APP_COMM_TVT_CTVT_DRAW_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's CtvtDraw_Leave
void func_ov172_021ba8c4(CommTvtWork *sys, CtvtDraw *draw);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtDraw_Create
CtvtDraw *func_ov172_021ba280(CommTvtWork *sys, HeapID heapId);
// pokebw2's CtvtDraw_Delete
void func_ov172_021ba2b8(CommTvtWork *sys, CtvtDraw *draw);
// pokebw2's CtvtDraw_Enter
void func_ov172_021ba2c4(CommTvtWork *sys, CtvtDraw *draw);
// pokebw2's CtvtDraw_Main
// Returns the mode to go on to, COMM_TVT_MODE_DRAW to stay
int func_ov172_021ba9ec(CommTvtWork *sys, CtvtDraw *draw);

#endif // BWPORT_APP_COMM_TVT_CTVT_DRAW_H
