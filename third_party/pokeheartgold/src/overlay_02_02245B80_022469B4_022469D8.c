#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "pokemon.h"

FollowMonCutInTask *ov02_022469D8(FieldSystem *fieldSystem, LocalMapObject *playerObject, LocalMapObject *followMonObject) {
    Pokemon *mon;
    FollowMonCutInTask *dest;

    dest = Heap_Alloc(HEAP_ID_FIELD1, sizeof(FollowMonCutInTask));
    if (dest == NULL) {
        GF_AssertFail();
        return NULL;
    }
    MI_CpuFill8(dest, 0U, sizeof(FollowMonCutInTask));
    dest->fieldSystem = fieldSystem;
    dest->playerObject = playerObject;
    dest->followMonObject = followMonObject;
    dest->heapID = HEAP_ID_FIELD1;
    mon = AllocMonZeroed(HEAP_ID_FIELD1);
    dest->mon = mon;
    ZeroMonData(mon);
    CreateMon(dest->mon, 0x95, 0x64, 0x20, 0, 0, 2, 0);
    return dest;
}
