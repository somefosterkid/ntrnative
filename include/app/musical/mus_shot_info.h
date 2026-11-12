// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/mus_shot_info.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUS_SHOT_INFO_H
#define BWPORT_APP_MUSICAL_MUS_SHOT_INFO_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's MusShotInfo_Create
MusShotInfo *func_ov112_021f4284(MusicalShot *shot, MusicalSave *save, BOOL askSave, void *comm, HeapID heapId);
// pokebw2's MusShotInfo_Delete
void func_ov112_021f4300(MusShotInfo *info);
// pokebw2's MusShotInfo_Main
void func_ov112_021f4350(MusShotInfo *info);
// pokebw2's MusShotInfo_IsFinished
BOOL func_ov112_021f44e8(MusShotInfo *info);

#endif // BWPORT_APP_MUSICAL_MUS_SHOT_INFO_H
