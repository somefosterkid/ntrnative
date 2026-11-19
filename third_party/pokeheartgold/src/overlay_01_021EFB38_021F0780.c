#include "global.h"
#include "sys_task.h"

struct SysTask;
struct SysTaskQueue;
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
void Heap_Free(void *ptr);

void ov01_021F0780(void *ptr);

void ov01_021F0780(void *ptr) {
    Heap_Free(ptr);
}
