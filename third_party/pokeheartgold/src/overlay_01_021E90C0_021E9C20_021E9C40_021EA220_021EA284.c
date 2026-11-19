#include "global.h"

#include "constants/heap.h"

#include "field/area_light.h"

#include "error_handling.h"
#include "heap.h"

void AreaLightManager_Free(AreaLightManager **areaLightManager) {
    if (areaLightManager == NULL) {
        GF_AssertFail();
    }
    ov01_021EA564(&(*areaLightManager)->templates);
    Heap_FreeExplicit(HEAP_ID_FIELD1, *areaLightManager);
    *areaLightManager = NULL;
}
