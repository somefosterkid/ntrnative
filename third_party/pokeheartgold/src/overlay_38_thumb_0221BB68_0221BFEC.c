#include "global.h"

#include "overlay_38_thumb.h"

void DWC_Free(int name, void *ptr, u32 size);

void DWC_ShutdownGHTTP();

extern u32 _02225040;

void ov38_0221BFEC(void) {
    if (((void **)&_02225040)[9] != NULL) {
        DWC_Free(10, ((void **)&_02225040)[9], 0);
        ((void **)&_02225040)[9] = NULL;
    }
    DWC_ShutdownGHTTP();
    _02225040 = 1;
    return;
}
