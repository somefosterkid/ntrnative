#ifndef POKEBW2_APP_RESEARCH_RADAR_QUEUE_H
#define POKEBW2_APP_RESEARCH_RADAR_QUEUE_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// A ring buffer of values, which the Research Radar's screens use for the sequences they run next (queue.c)
// The names of these functions and types are ours

Queue *Queue_Create(int size, HeapID heapId);
void Queue_Delete(Queue *queue);
void Queue_Push(Queue *queue, u32 value);
u32 Queue_Pop(Queue *queue);
void Queue_Clear(Queue *queue);
BOOL Queue_IsEmpty(Queue *queue);

#endif // POKEBW2_APP_RESEARCH_RADAR_QUEUE_H
