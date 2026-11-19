#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "map_object.h"

void *ReadMModelFromNarcInternal(MapObjectManager *arg0, u32 arg1, s32 arg2);

void *ReadMModelFromNarcInternal(MapObjectManager *manager, u32 file_id, s32 arg2) {
    NARC *narc;
    u32 size;
    void *dest;

    narc = MapObjectManager_GetMapModelNarc(manager);
    size = NARC_GetMemberSize(narc, file_id);
    if (arg2 == 1) {
        dest = Heap_Alloc(HEAP_ID_FIELD1, size);
    } else {
        dest = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
    }
    NARC_ReadWholeMember(narc, file_id, dest);
    return dest;
}
