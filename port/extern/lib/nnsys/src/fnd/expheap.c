// The PC build compiles lib/nnsys/src/fnd/expheap.c through this file (docs/nnsys-port.md; the DS build never sees
// it). A block freed to an expanded heap frees what the 2D loaders built from the files in it (port/src/nnsys_load64.c:
// host copies of the files' structs, which on the DS are the file itself and die with it); a child heap's memory going
// back to its parent (GFL_HeapDelete) frees the copies of every file that was in the child. libntrsystem's expheap did
// the same (its load64 patch).
// The heap checker (BWPORT_HEAPCHECK, port/src/nnsys_heapcheck.c) hooks the same way: every call that makes, changes or
// destroys an expanded heap goes through the wrappers below, which are one test of an int when it is off. The checker's
// lock makes each operation and its record one step to the game's other threads.
// Every block an expanded heap hands out (and the part a resize grows) starts zeroed on the PC, as if every heap were
// made with NitroSystem's NNS_FND_HEAP_OPT_0_CLEAR (sv1, 11-19). The games read memory they never wrote and keep it:
// Platinum's and HG/SS's Mail_Reset leaves each EasyChatSentence's id unset (EasyChatSentence_Init sets the type and
// the words), and the party mon's mail carries it into every save. On the DS those bytes are whatever the block held
// before, DS addresses, the same on every console; on the PC they are 64-bit host addresses (a freed block's header,
// a pointer the last owner kept) that move with the exe's layout: the same leg gave other save bytes on another build or
// machine (a 64 KB shift of s_HW_MAIN_MEM changed sentence 2's id, bits 16-31 of a pointer, by one), so a chain split
// at a trusted save never matched. Zeroed, a block's unwritten bytes are the same on every build, machine and run.
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

// The whole block (its size rounded up as the heap keeps it), not only the bytes asked for
static void *ClearNewBlock(void *data) {
    if (data != NULL) {
        memset(data, 0, NNS_FndGetSizeForMBlockExpHeap(data));
    }
    return data;
}

// The bytes a resize added past the block's old size
static u32 ClearGrownPart(void *memBlock, u32 old, u32 got) {
    if (got > old) {
        memset((u8 *)memBlock + old, 0, got - old);
    }
    return got;
}

void *NNS_FndAllocFromExpHeapEx(NNSFndHeapHandle heap, u32 size, int alignment) {
    void *data;

    if (!HeapCheckOn()) {
        return ClearNewBlock(NNS_FndAllocFromExpHeapEx_nns(heap, size, alignment));
    }
    PORT_HeapCheckLock();
    PORT_HeapCheckBefore(heap);
    data = ClearNewBlock(NNS_FndAllocFromExpHeapEx_nns(heap, size, alignment));
    PORT_HeapCheckAllocated(heap, data, size, __builtin_return_address(0));
    PORT_HeapCheckUnlock();
    return data;
}

u32 NNS_FndResizeForMBlockExpHeap(NNSFndHeapHandle heap, void *memBlock, u32 size) {
    u32 got;
    u32 old = NNS_FndGetSizeForMBlockExpHeap(memBlock);

    if (!HeapCheckOn()) {
        return ClearGrownPart(memBlock, old, NNS_FndResizeForMBlockExpHeap_nns(heap, memBlock, size));
    }
    PORT_HeapCheckLock();
    PORT_HeapCheckBefore(heap);
    got = ClearGrownPart(memBlock, old, NNS_FndResizeForMBlockExpHeap_nns(heap, memBlock, size));
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
