// The PC build compiles lib/nnsys/src/fnd/expheap.c through this file (docs/nnsys-port.md; the DS build never sees
// it). A block freed to an expanded heap frees what the 2D loaders built from the files in it (port/src/nnsys_load64.c:
// host copies of the files' structs, which on the DS are the file itself and die with it); a child heap's memory going
// back to its parent (GFL_HeapDelete) frees the copies of every file that was in the child. libntrsystem's expheap did
// the same (its load64 patch).
#pragma push_macro("NNS_FndFreeToExpHeap")
#define NNS_FndFreeToExpHeap NNS_FndFreeToExpHeap_nns
#include "../../../../../../lib/nnsys/src/fnd/expheap.c"
#pragma pop_macro("NNS_FndFreeToExpHeap")

void WIN_G2dFreeUnpacked(void *start, void *end);   // port/src/nnsys_load64.c
void NNS_FndFreeToExpHeap(NNSFndHeapHandle heap, void *memBlock);

void NNS_FndFreeToExpHeap(NNSFndHeapHandle heap, void *memBlock) {
    WIN_G2dFreeUnpacked(memBlock, (u8 *)memBlock + NNS_FndGetSizeForMBlockExpHeap(memBlock));
    NNS_FndFreeToExpHeap_nns(heap, memBlock);
}
