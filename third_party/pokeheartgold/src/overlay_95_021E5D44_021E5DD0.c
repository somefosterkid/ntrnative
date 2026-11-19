#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "camera.h"
#include "heap.h"
#include "unk_02014DA0.h"

u32 ov95_021E5D98(u32 szByte, BOOL is4x4comp);

u32 ov95_021E5DB4(u32 szByte, BOOL is4pltt);

ParticleSystem *ov95_021E5DD0(enum HeapID arg0);

ParticleSystem *ov95_021E5DD0(enum HeapID arg0) {
    Camera *temp_r0_2;
    ParticleSystem *temp_r0;

    temp_r0 = sub_02014DB4(ov95_021E5D98, ov95_021E5DB4, Heap_Alloc(arg0, 0x4800U), 0x4800U, 1, arg0);
    temp_r0_2 = sub_02015524(temp_r0);
    if (temp_r0_2 != NULL) {
        Camera_SetPerspectiveClippingPlane(0x1000, 0x384000, temp_r0_2);
    }
    return temp_r0;
}
