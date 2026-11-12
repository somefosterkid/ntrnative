// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_call.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_CALL_H
#define BWPORT_APP_COMM_TVT_CTVT_CALL_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's CtvtCall_Leave
void func_ov172_021bbe5c(CommTvtWork *sys, CtvtCall *call);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtCall_Create
CtvtCall *func_ov172_021bba6c(CommTvtWork *sys, HeapID heapId);
// pokebw2's CtvtCall_Delete
void func_ov172_021bba88(CommTvtWork *sys, CtvtCall *call);
// pokebw2's CtvtCall_Enter
void func_ov172_021bba94(CommTvtWork *sys, CtvtCall *call);

#endif // BWPORT_APP_COMM_TVT_CTVT_CALL_H
