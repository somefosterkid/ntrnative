#ifndef POKEBW2_SYSTEM_RINGTONE_SYS_H
#define POKEBW2_SYSTEM_RINGTONE_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The field sound system's ringtone (ringtone_sys.c): rings SEQ_SE_SYS_35 for up to 900 frames with the other sound
// players silenced, and follows the lid: closing it while nothing rings sets the master volume to 0.
// RingtoneSys_Create is swan's (https://github.com/ds-pokemon-hacking/swan, GPL-3.0); the other names are ours

RingtoneSys *RingtoneSys_Create(HeapID heapId, PlayerVolumeFader *fader);
void RingtoneSys_Free(RingtoneSys *sys);
// Called every frame: stops the ringtone once it has rung its time
void RingtoneSys_Update(RingtoneSys *sys);
void RingtoneSys_Ring(RingtoneSys *sys);
void RingtoneSys_Stop(RingtoneSys *sys);
// Sets the lid callbacks back to the ringtone's, after something else replaced them
void RingtoneSys_RestoreLidCallbacks(void);

#endif // POKEBW2_SYSTEM_RINGTONE_SYS_H
