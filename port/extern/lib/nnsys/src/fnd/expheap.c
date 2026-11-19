// The PC build compiles lib/nnsys/src/fnd/expheap.c through this file (docs/nnsys-port.md; the DS build never sees
// it). A block freed to an expanded heap frees what the 2D loaders built from the files in it (port/src/nnsys_load64.c:
// host copies of the files' structs, which on the DS are the file itself and die with it); a child heap's memory going
// back to its parent (GFL_HeapDelete) frees the copies of every file that was in the child. libntrsystem's expheap did
// the same (its load64 patch).
// The heap checker (BWPORT_HEAPCHECK, port/src/nnsys_heapcheck.c) hooks the same way: every call that makes, changes or
// destroys an expanded heap goes through the wrappers below, which are one test of an int when it is off. The checker's
// lock makes each operation and its record one step to the game's other threads.
#pragma push_macro("NNS_FndFreeToExpHeap")
#pragma push_macro("NNS_FndCreateExpHeapEx")
#pragma push_macro("NNS_FndDestroyExpHeap")
#pragma push_macro("NNS_FndAllocFromExpHeapEx")
#pragma push_macro("NNS_FndResizeForMBlockExpHeap")
#define NNS_FndFreeToExpHeap NNS_FndFreeToExpHeap_nns
#define NNS_FndCreateExpHeapEx NNS_FndCreateExpHeapEx_nns
#define NNS_FndDestroyExpHeap NNS_FndDestroyExpHeap_nns
#define NNS_FndAllocFromExpHeapEx NNS_FndAllocFromExpHeapEx_nns
#define NNS_FndResizeForMBlockExpHeap NNS_FndResizeForMBlockExpHeap_nns
#include "../../../../../../lib/nnsys/src/fnd/expheap.c"
#pragma pop_macro("NNS_FndFreeToExpHeap")
#pragma pop_macro("NNS_FndCreateExpHeapEx")
#pragma pop_macro("NNS_FndDestroyExpHeap")
#pragma pop_macro("NNS_FndAllocFromExpHeapEx")
#pragma pop_macro("NNS_FndResizeForMBlockExpHeap")

void WIN_G2dFreeUnpacked(void *start, void *end);   // port/src/nnsys_load64.c
NNSFndHeapHandle NNS_FndCreateExpHeapEx(void *startAddress, u32 size, u16 optFlag);
void NNS_FndDestroyExpHeap(NNSFndHeapHandle heap);
void *NNS_FndAllocFromExpHeapEx(NNSFndHeapHandle heap, u32 size, int alignment);
u32 NNS_FndResizeForMBlockExpHeap(NNSFndHeapHandle heap, void *memBlock, u32 size);
void NNS_FndFreeToExpHeap(NNSFndHeapHandle heap, void *memBlock);

// port/src/nnsys_heapcheck.c (PORT_HeapCheckLevel: -1 until the env var is read, then 0 off or 1, 2 on)
extern int PORT_HeapCheckLevel;
void PORT_HeapCheckInit(void);
void PORT_HeapCheckLock(void);
void PORT_HeapCheckUnlock(void);
void PORT_HeapCheckCreated(NNSFndHeapHandle heap);
void PORT_HeapCheckDestroyed(NNSFndHeapHandle heap);
void PORT_HeapCheckBefore(NNSFndHeapHandle heap);
void PORT_HeapCheckAllocated(NNSFndHeapHandle heap, void *data, u32 size, void *caller);
void PORT_HeapCheckBeforeFree(NNSFndHeapHandle heap, void *data, void *caller);
void PORT_HeapCheckResized(NNSFndHeapHandle heap, void *data, u32 size, u32 got);

static inline int HeapCheckOn(void) {
    if (PORT_HeapCheckLevel < 0) {
        PORT_HeapCheckInit();
    }
    return PORT_HeapCheckLevel > 0;
}

NNSFndHeapHandle NNS_FndCreateExpHeapEx(void *startAddress, u32 size, u16 optFlag) {
    NNSFndHeapHandle heap;

    if (!HeapCheckOn()) {
        return NNS_FndCreateExpHeapEx_nns(startAddress, size, optFlag);
    }
    PORT_HeapCheckLock();
    heap = NNS_FndCreateExpHeapEx_nns(startAddress, size, optFlag);
    PORT_HeapCheckCreated(heap);
    PORT_HeapCheckUnlock();
    return heap;
}

void NNS_FndDestroyExpHeap(NNSFndHeapHandle heap) {
    if (HeapCheckOn()) {
        PORT_HeapCheckLock();
        PORT_HeapCheckDestroyed(heap);
        PORT_HeapCheckUnlock();
    }
    NNS_FndDestroyExpHeap_nns(heap);
}

void *NNS_FndAllocFromExpHeapEx(NNSFndHeapHandle heap, u32 size, int alignment) {
    void *data;

    if (!HeapCheckOn()) {
        return NNS_FndAllocFromExpHeapEx_nns(heap, size, alignment);
    }
    PORT_HeapCheckLock();
    PORT_HeapCheckBefore(heap);
    data = NNS_FndAllocFromExpHeapEx_nns(heap, size, alignment);
    PORT_HeapCheckAllocated(heap, data, size, __builtin_return_address(0));
    PORT_HeapCheckUnlock();
    return data;
}

u32 NNS_FndResizeForMBlockExpHeap(NNSFndHeapHandle heap, void *memBlock, u32 size) {
    u32 got;

    if (!HeapCheckOn()) {
        return NNS_FndResizeForMBlockExpHeap_nns(heap, memBlock, size);
    }
    PORT_HeapCheckLock();
    PORT_HeapCheckBefore(heap);
    got = NNS_FndResizeForMBlockExpHeap_nns(heap, memBlock, size);
    PORT_HeapCheckResized(heap, memBlock, size, got);
    PORT_HeapCheckUnlock();
    return got;
}

void NNS_FndFreeToExpHeap(NNSFndHeapHandle heap, void *memBlock) {
    WIN_G2dFreeUnpacked(memBlock, (u8 *)memBlock + NNS_FndGetSizeForMBlockExpHeap(memBlock));
    if (!HeapCheckOn()) {
        NNS_FndFreeToExpHeap_nns(heap, memBlock);
        return;
    }
    PORT_HeapCheckLock();
    PORT_HeapCheckBeforeFree(heap, memBlock, __builtin_return_address(0));
    NNS_FndFreeToExpHeap_nns(heap, memBlock);
    PORT_HeapCheckUnlock();
}
