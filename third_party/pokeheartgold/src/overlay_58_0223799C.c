#include "global.h"

#include "heap.h"
#include "overlay_58.h"
#include "overlay_manager.h"

void ov58_02237B94(Ov58ApricornBoxWork *work);

s32 ov58_02237CCC(void);

BOOL ApricornBox_Exit(OverlayManager *man, int *state);

BOOL ApricornBox_Exit(OverlayManager *man, int *state) {
    Ov58ApricornBoxWork *v0;

    v0 = OverlayManager_GetData(man);
    ov58_02237CCC();
    ov58_02237B94(v0);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_133);
    return 1;
}
