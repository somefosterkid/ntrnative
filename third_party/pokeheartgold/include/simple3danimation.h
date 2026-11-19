#ifndef POKEHEARTGOLD_SIMPLE3DANIMATION_H
#define POKEHEARTGOLD_SIMPLE3DANIMATION_H

// Simple3DAnimation: merged by tools/hg/promote.py from 11 m2c local copies (1 variants, base
// src/unk_02015DD8_02015FFC.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

struct NNSFndAllocatorFunc;
struct NNSG3dAnmObj_;

typedef struct Simple3DAnimation {
    u32 flags;
    fx32 currFrame;
    void *animSet;
    void *g3DAnim;
    struct NNSG3dAnmObj_ *animObj;
    struct { struct NNSFndAllocatorFunc *pFunc; void *pHeap; u32 heapParam1; u32 heapParam2; } allocator;
} Simple3DAnimation;

// sizeof(Simple3DAnimation) == 0x24

#endif // POKEHEARTGOLD_SIMPLE3DANIMATION_H
