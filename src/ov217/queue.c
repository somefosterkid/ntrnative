// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov310/queue.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/research_radar/queue.h"
#include "gfl/heap.h"

struct Queue {
    u32 *buffer;
    int size;
    int head;
    int tail;
};

static void func_ov217_021c0e6c(Queue *queue);
static void func_ov217_021c0e78(Queue *queue, int size, HeapID heapId);
static void func_ov217_021c0e98(Queue *queue);
static void func_ov217_021c0ea4(Queue *queue, u32 value);
static u32 func_ov217_021c0ec0(Queue *queue);
static void func_ov217_021c0edc(Queue *queue);
static BOOL func_ov217_021c0ee4(Queue *queue);

// pokebw2's Queue_Create
Queue *func_ov217_021c0e08(int size, HeapID heapId) {
    Queue *queue = GFL_HeapAllocate(heapId, sizeof(Queue), FALSE, "queue.c", 60);

    func_ov217_021c0e6c(queue);
    func_ov217_021c0e78(queue, size, heapId);
    return queue;
}

// pokebw2's Queue_Delete
void func_ov217_021c0e3c(Queue *queue) {
    func_ov217_021c0e98(queue);
    GFL_HeapFree(queue);
}

// pokebw2's Queue_Push
void func_ov217_021c0e4c(Queue *queue, u32 value) {
    func_ov217_021c0ea4(queue, value);
}

// pokebw2's Queue_Pop
u32 func_ov217_021c0e54(Queue *queue) {
    return func_ov217_021c0ec0(queue);
}

// pokebw2's Queue_Clear
void func_ov217_021c0e5c(Queue *queue) {
    func_ov217_021c0edc(queue);
}

// pokebw2's Queue_IsEmpty
BOOL func_ov217_021c0e64(Queue *queue) {
    return func_ov217_021c0ee4(queue);
}

// pokebw2's Queue_Init
static void func_ov217_021c0e6c(Queue *queue) {
    queue->buffer = NULL;
    queue->head = 0;
    queue->tail = 0;
}

// pokebw2's Queue_AllocBuffer
static void func_ov217_021c0e78(Queue *queue, int size, HeapID heapId) {
    queue->buffer = GFL_HeapAllocate(heapId, sizeof(u32) * size, FALSE, "queue.c", 211);
    queue->size = size;
}

// pokebw2's Queue_FreeBuffer
static void func_ov217_021c0e98(Queue *queue) {
    GFL_HeapFree(queue->buffer);
}

// pokebw2's Queue_PushCore
static void func_ov217_021c0ea4(Queue *queue, u32 value) {
    queue->buffer[queue->tail] = value;
    queue->tail = (queue->tail + 1) % queue->size;
}

// pokebw2's Queue_PopCore
static u32 func_ov217_021c0ec0(Queue *queue) {
    u32 value = queue->buffer[queue->head];

    queue->head = (queue->head + 1) % queue->size;
    return value;
}

// pokebw2's Queue_ClearCore
static void func_ov217_021c0edc(Queue *queue) {
    queue->head = 0;
    queue->tail = 0;
}

// pokebw2's Queue_IsEmptyCore
static BOOL func_ov217_021c0ee4(Queue *queue) {
    if (queue->head == queue->tail) {
        return TRUE;
    }
    return FALSE;
}
