// Black's code at ov114 0x021b9b4c-0x021b9c6c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/nhttp_internal.h"

#pragma thumb off
void func_ov114_021b9b4c(OSMutex *mutex) {
    OS_InitMutex(mutex);
}

void func_ov114_021b9b58(OSMutex *mutex) {
    OS_LockMutex(mutex);
}

// Starts the thread, making its message queue the first time
BOOL func_ov114_021b9b64(NHttpThreadInfo *info, int priority, void *stack) {
    if (!func_ov114_021bbeb8(info)) {
        OS_InitMessageQueue(&info->queue, info->messages, 3);
        func_ov114_021bbea0(info);
    }
    OS_CreateThread(&info->thread, func_ov114_021b9c60, NULL, (u8 *)stack + 0x2000, 0x2000, priority);
    OS_WakeupThreadDirect(&info->thread);
    return TRUE;
}

// Asks the thread to end, wakes it and waits for it
void func_ov114_021b9bd4(NHttpThreadInfo *info, NHttpState *state) {
    state->quit = TRUE;
    func_ov114_021b9c08(&info->queue);
    OS_JoinThread(&info->thread);
}

// Waits for a message, which is the thread's way to sleep until there is a request
BOOL func_ov114_021b9bf4(OSMessageQueue *queue) {
    OSMessage message;

    return OS_ReceiveMessage(queue, &message, OS_MESSAGE_BLOCK);
}

void func_ov114_021b9c08(OSMessageQueue *queue) {
    OS_SendMessage(queue, NULL, OS_MESSAGE_NOBLOCK);
}

// Stops the program if it is not running where it has to: on the thread when outside is FALSE, anywhere but there when
// it is TRUE
void func_ov114_021b9c1c(NHttpThreadInfo *info, BOOL outside) {
    OSThread *thread = &info->thread;
    OSThread *current = OSi_ThreadInfo.current;

    if (current != NULL) {
        if ((!outside && current != thread) || (outside && current == thread)) {
            OS_Terminate();
        }
    }
}

void func_ov114_021b9c60(void *arg) {
    func_ov114_021be4d4();
}
