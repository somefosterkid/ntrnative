// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/el_scoreboard.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_EL_SCOREBOARD_H
#define BWPORT_FIELD_EL_SCOREBOARD_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's ElScoreboard_Create
ElScoreboard *func_ov020_021875f4(void *texture, const char *texName, const char *plName, const StrBuf *text, u16 a4, u16 a5,
                                  HeapID heapId);
// pokebw2's ElScoreboard_Update
void func_ov020_0218768c(ElScoreboard *board);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ElScoreboard_Free
void func_ov020_02187684(ElScoreboard *board);

#endif // BWPORT_FIELD_EL_SCOREBOARD_H
