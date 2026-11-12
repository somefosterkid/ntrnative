// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/ringtone_sys.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_RINGTONE_SYS_H
#define BWPORT_SYSTEM_RINGTONE_SYS_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

RingtoneSys *RingtoneSys_Create(HeapID heapId, PlayerVolumeFader *fader);

// pokebw2's RingtoneSys_RestoreLidCallbacks
// Sets the lid callbacks back to the ringtone's, after something else replaced them
void func_02029f6c(void);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's RingtoneSys_Free
void func_02029f44(RingtoneSys *sys);
// pokebw2's RingtoneSys_Update
// Called every frame: stops the ringtone once it has rung its time
void func_02029f4c(RingtoneSys *sys);
// pokebw2's RingtoneSys_Ring
void func_02029f54(RingtoneSys *sys);
// pokebw2's RingtoneSys_Stop
void func_02029f60(RingtoneSys *sys);

#endif // BWPORT_SYSTEM_RINGTONE_SYS_H
