// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/mus_shot_photo.h, the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUS_SHOT_PHOTO_H
#define BWPORT_APP_MUSICAL_MUS_SHOT_PHOTO_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Overlay 112's mus_shot_photo.c (Black 2's overlay 209): the photo of a musical's finale, drawn on the main screen
// with the stage's actors. pokebw2's guessed names are in the comments

// pokebw2's MusShotPhoto_Create, MusShotPhoto_Delete and MusShotPhoto_Main
MusShotPhoto *func_ov112_021f3af0(MusicalShot *shot, HeapID heapId);
void func_ov112_021f3bf0(MusShotPhoto *photo);
void func_ov112_021f3c50(MusShotPhoto *photo);

#endif // BWPORT_APP_MUSICAL_MUS_SHOT_PHOTO_H
