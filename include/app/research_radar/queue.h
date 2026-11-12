// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/research_radar/queue.h: the lines in use
// so far.
#ifndef BWPORT_APP_RESEARCH_RADAR_QUEUE_H
#define BWPORT_APP_RESEARCH_RADAR_QUEUE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's Queue_Create
Queue *func_ov217_021c0e08(int size, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Queue_Delete
void func_ov217_021c0e3c(Queue *queue);
// pokebw2's Queue_Push
void func_ov217_021c0e4c(Queue *queue, u32 value);
// pokebw2's Queue_Pop
u32 func_ov217_021c0e54(Queue *queue);
// pokebw2's Queue_Clear
void func_ov217_021c0e5c(Queue *queue);
// pokebw2's Queue_IsEmpty
BOOL func_ov217_021c0e64(Queue *queue);

#endif // BWPORT_APP_RESEARCH_RADAR_QUEUE_H
