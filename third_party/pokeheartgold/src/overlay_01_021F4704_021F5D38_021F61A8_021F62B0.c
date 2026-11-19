#include "global.h"

#include "field/map_load_manager.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "heap.h"
#include "overlay_01_021FAC44.h"
#include "overlay_01_map_load_manager.h"

void MapLoadManager_FreeNARCAndLoadedMapBuffers(MapLoadManager *mapLoadManager) {
    NARC_Delete(mapLoadManager->narc);
    ov01_021FACB4(mapLoadManager->buffers);
    Heap_Free(mapLoadManager);
}
