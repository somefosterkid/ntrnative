// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync/pdwacc_disp.h: the lines in use so
// far.
#ifndef BWPORT_APP_GSYNC_PDWACC_DISP_H
#define BWPORT_APP_GSYNC_PDWACC_DISP_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PdwAccDisp_Create
PdwAccDisp *func_ov123_021d9958(HeapID heapId);
// pokebw2's PdwAccDisp_Main
void func_ov123_021d9a14(PdwAccDisp *disp);
// pokebw2's PdwAccDisp_Free
void func_ov123_021d9a1c(PdwAccDisp *disp);

#endif // BWPORT_APP_GSYNC_PDWACC_DISP_H
