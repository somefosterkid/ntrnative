#include "global.h"

#include "frontier/overlay_80_02239960.h"

#include "camera.h"
#include "error_handling.h"
#include "heap.h"
#include "unk_02014DA0.h"
#include "unk_02026E30.h"

struct Camera;

struct SPLEmitter;

typedef struct {
    ParticleSystem *slots[8];
    u16 heapId;
} UnkStruct_ov80_02239960;

u32 ov80_02239AB0(u32 szByte, BOOL is4x4comp);

u32 ov80_02239AD4(u32 szByte, BOOL is4pltt);

UnkStruct_ov80_02239960 *ov80_02239960(enum HeapID heapId);

ParticleSystem *ov80_022399A4(UnkStruct_ov80_02239960 *manager, u32 idx, int fileId, int a3);

ParticleSystem *ov80_022399A4(UnkStruct_ov80_02239960 *manager, u32 idx, int fileId, int a3) {
    ParticleSystem *emitter;
    void *particleHeap;
    GF_ASSERT(manager->slots[idx] == NULL);
    particleHeap = Heap_Alloc((enum HeapID)manager->heapId, 0x12 << 0xA);
    emitter = sub_02014DB4(ov80_02239AB0, ov80_02239AD4, particleHeap, 0x12 << 0xA, 1, (enum HeapID)manager->heapId);
    Camera_SetPerspectiveClippingPlane(1 << 0xC, 0xE1 << 0xE, sub_02015524(emitter));
    sub_02015528(emitter, a3);
    sub_0201526C(emitter, sub_02015264((NarcId)0xBC, fileId, (enum HeapID)manager->heapId), 0xA, 1);
    manager->slots[idx] = emitter;
    return emitter;
}
