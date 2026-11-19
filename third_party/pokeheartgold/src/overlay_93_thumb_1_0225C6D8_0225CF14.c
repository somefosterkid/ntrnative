#include "global.h"

#include "gf_3d_vramman.h"
#include "overlay_93_thumb_1.h"

void ov93_0225CF34(void);

void ov93_0225CF14(enum HeapID heapID) {
    GF_3DVramMan_Create(heapID, 0, 1, 0, 2, ov93_0225CF34);
}
