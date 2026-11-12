// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/dwc_rapcommon.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_GFL_DWC_RAPCOMMON_H
#define BWPORT_GFL_DWC_RAPCOMMON_H

#include "types.h"
#include "gfl/heap.h"

// pokebw2's DWCRapCommon_SetSubHeap
// Gives the library a second heap of size bytes in heapId, which allocations named name and larger than minSize use
void func_ov009_02156e24(u32 name, u32 size, HeapID heapId);
// pokebw2's DWCRapCommon_SetSubHeapOwner
// The heap to delete with the second heap
void func_ov009_02156f50(HeapID heapId);
// pokebw2's DWCRapCommon_Alloc
// The library's allocator and deallocator
void *func_ov009_02156f5c(u32 name, u32 size, int align);
// pokebw2's DWCRapCommon_Free
void func_ov009_02156fdc(u32 name, void *ptr, u32 size);
// pokebw2's DWCRapCommon_ResetUserData
// Clears the user data and creates it if it is not valid
void func_ov009_02157138(void *userData);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DWCRapCommon_StartLibrary
// Starts the DWC library with a heap of its own in heapId, and shows the user info warning if that returns 3
void func_ov009_02156dfc(u32 heapId);
// pokebw2's DWCRapCommon_SetSubHeapEx
void func_ov009_02156e30(u32 name, u32 size, u32 minSize, HeapID heapId);
// pokebw2's DWCRapCommon_EndSubHeap
// Ends the second heap, now or once its blocks are freed
void func_ov009_02156ed0(void);
// pokebw2's DWCRapCommon_Create
// Creates and destroys the work and the library's heap
void func_ov009_02157048(HeapID heapId, u32 size);
// pokebw2's DWCRapCommon_Delete
void func_ov009_021570b8(void);

#endif // BWPORT_GFL_DWC_RAPCOMMON_H
