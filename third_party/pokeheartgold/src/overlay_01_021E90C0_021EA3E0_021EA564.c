#include "global.h"

#include "field/area_light.h"

#include "heap.h"

void ov01_021EA564(AreaLightTemplate **templates) {
    Heap_FreeExplicit(HEAP_ID_FIELD1, *templates);
    *templates = NULL;
}
