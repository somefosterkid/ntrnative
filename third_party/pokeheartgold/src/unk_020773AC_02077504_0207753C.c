#include "global.h"

#include "camera.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "unk_02014DA0.h"

struct Camera;

struct SPLEmitter;

u32 sub_02077504(u32 szByte, BOOL is4x4comp);

u32 sub_02077520(u32 szByte, BOOL is4pltt);

ParticleSystem *sub_0207753C(int heapID);

ParticleSystem *sub_0207753C(int heapID) {
    void *particleHeap = Heap_Alloc((enum HeapID)heapID, 0x12 << 10);
    ParticleSystem *emitter = sub_02014DB4(sub_02077504, sub_02077520, particleHeap, 0x12 << 10, 1, (enum HeapID)heapID);
    Camera *camera = sub_02015524(emitter);
    if (camera != NULL) {
        Camera_SetPerspectiveClippingPlane(1 << 12, 0xe1 << 14, camera);
    }
    return emitter;
}
