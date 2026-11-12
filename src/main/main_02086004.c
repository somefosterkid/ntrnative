// Black's code at main 0x02086004-0x02086088. No name is known: the module and address.

#include "types.h"
#include "nitro/os.h"

void func_02086164(OSMutex *param_1, u32 param_2);

#pragma thumb off
void OS_InitMutex(OSMutex *mutex) {
    s16 val;
    mutex->queue.tail = NULL;
    mutex->queue.head = NULL;
    mutex->thread = NULL;
    val = mutex->count & 0xFF000000;
    mutex->count = val & ~0xFF000000;
}
#pragma thumb on

#pragma thumb off
void OS_LockMutex(OSMutex *mutex) {
    u32 state;
    OSThread *current;


    state = OS_DisableInterrupts();
    current = OSi_ThreadInfo.current;
loop_1:
    if (OS_TryLockMutex(mutex) == 0) {
        current->mutex = mutex;
        OS_SleepThread(&mutex->queue);
        current->mutex = NULL;
        goto loop_1;
    }
    OS_RestoreInterrupts(state);
}
#pragma thumb on

#pragma thumb off
void OS_UnlockMutex(OSMutex *mutex) {
    func_02086164(mutex, 0x10000000);
}
#pragma thumb on
