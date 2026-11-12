// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/comm_tvt/ctvt_talk.h: the lines in use
// so far.
#ifndef BWPORT_APP_COMM_TVT_CTVT_TALK_H
#define BWPORT_APP_COMM_TVT_CTVT_TALK_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// pokebw2's CtvtTalk_Delete
void func_ov172_021b8cb0(CommTvtWork *sys, CtvtTalk *talk);

// pokebw2's CtvtTalk_Draw
void func_ov172_021b9574(CommTvtWork *sys, CtvtTalk *talk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CtvtTalk_GetMic
CtvtMic *func_ov172_021b9f98(CommTvtWork *sys, CtvtTalk *talk);
// pokebw2's CtvtTalk_Create
CtvtTalk *func_ov172_021b8c3c(CommTvtWork *sys, HeapID heapId);
// pokebw2's CtvtTalk_Enter
void func_ov172_021b8cc8(CommTvtWork *sys, CtvtTalk *talk);
// pokebw2's CtvtTalk_Leave
void func_ov172_021b8fb8(CommTvtWork *sys, CtvtTalk *talk);
// pokebw2's CtvtTalk_Main
// Returns the mode to go on to, COMM_TVT_MODE_TALK to stay
int func_ov172_021b9044(CommTvtWork *sys, CtvtTalk *talk);

#endif // BWPORT_APP_COMM_TVT_CTVT_TALK_H
