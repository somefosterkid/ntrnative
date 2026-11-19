#include "global.h"

#include "field/map_prop_animation.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01.h"
#include "heap.h"


void *ov01_021E9C30(void);

void ov01_021E9C20(FieldSystem *fieldSystem, u8 a1) {
    MapPropOneShotAnimationManager_UnloadAnimation(fieldSystem->mapPropAnimationManager, fieldSystem->mapPropOneShotAnimationManager, (s32)a1);
}

void *ov01_021E9C30(void) {
    s8 *v0;

    v0 = Heap_AllocAtEnd(HEAP_ID_FIELD1, 8U);
    *v0 = 0;
    return v0;
}
