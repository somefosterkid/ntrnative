#include "global.h"

#include "heap.h"
#include "unk_0200A090.h"
#include "overlay_96_unk_struct_ov96_021e9a78.h"

void ov96_021E9D10(UnkStruct_ov96_021E9A78 *res);

UnkStruct_ov96_021E9A78 *ov96_021E9A78(enum HeapID heapId, u32 flags, u32 param2);

UnkStruct_ov96_021E9A78 *ov96_021E9A78(enum HeapID heapId, u32 flags, u32 param2) {
    u8 counts[4];
    UnkStruct_ov96_021E9A78 *work;
    int i;

    for (i = 0; i < 4; i++) {
        counts[i] = 0;
    }
    if (flags & 0x1) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x4) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x2) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x8) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x10) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x20) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x40) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x80) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x100) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x200) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x400) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    if (flags & 0x800) {
        counts[0]++;
        counts[2]++;
        counts[3]++;
    }
    counts[1] = 2;
    work = Heap_Alloc(heapId, sizeof(UnkStruct_ov96_021E9A78));
    work->heapId = heapId;
    work->flags = flags;
    work->vram = param2;
    for (i = 0; i < 4; i++) {
        work->resMan[i] = Create2DGfxResObjMan(counts[i], (GfGfxResType)i, work->heapId);
    }
    ov96_021E9D10(work);
    return work;
}
