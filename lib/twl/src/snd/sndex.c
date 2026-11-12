// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/snd/sndex.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK's sound extensions for the DSi (SNDEX), ARM9 side (library code in the DSi-only LTD autoload). The ARM9 sends
// the ARM7 one request at a time over the PXI tag 0x18, as words of a command and a value, and the ARM7 answers with a
// word of the same command and a code; a request's callback, or the thread that waits on a message queue, gets the
// result. Only the code that the game keeps is here: the setup, the request that sets the microphone's filter and the
// two that begin and end the exclusive state, with what they share. The ROM names none of it; the file name and those
// of the types and data are descriptive guesses, and the functions have the SDK's names where the code shows them and
// keep their default names otherwise (twl/sndex.h says what the public ones do).

#include "types.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"
#include "nitro/pxi.h"
#include "nitro/os/common/systemCall.h"
#include "twl/snd/ARM9/sndex.h"

// The priority of the library's callbacks in the lists of the callbacks that run before the console sleeps or exits
#define SNDEX_PM_PRIORITY 0xfffd

// How long a wait spins between two looks at the state, in cycles of the system clock
#define SNDEX_SPIN_CYCLES 0x82dc

// The commands of the words (bits 16 to 23), as far as the code shows them. The ARM7 answers a command with its own
// number. The answers to 1 to 5 and 0xfe carry a value (bits 0 to 7), which goes to where the request pointed at, a
// byte for 3 and 4 and a word for the others; the requests that send them were left out of the ROM
#define SNDEX_COMMAND_NONE 0
#define SNDEX_COMMAND_84 0x84                   // a value of at most 31, or at most 7 with bit 6 of the value set
#define SNDEX_COMMAND_FILTER_TARGET 0xc1        // the filter request: the target, five coefficients and the end
#define SNDEX_COMMAND_FILTER_COEFFICIENT_0 0xc2 // to 0xc6, for the five coefficients
#define SNDEX_COMMAND_FILTER_COEFFICIENT_1 0xc3
#define SNDEX_COMMAND_FILTER_COEFFICIENT_2 0xc4
#define SNDEX_COMMAND_FILTER_COEFFICIENT_3 0xc5
#define SNDEX_COMMAND_FILTER_COEFFICIENT_4 0xc6
#define SNDEX_COMMAND_FILTER_END 0xc7 // the one that the callback waits for
#define SNDEX_COMMAND_EXCLUSIVE_BEGIN 0xfb
#define SNDEX_COMMAND_EXCLUSIVE_END 0xfc
#define SNDEX_COMMAND_NOTIFY 0xfd // sent by the ARM7 of its own

// The codes of the answers (bits 8 to 15)
enum {
    SNDEX_ANSWER_OK = 0,
    SNDEX_ANSWER_ERROR_2 = 2,
    SNDEX_ANSWER_ERROR_3 = 3,
    SNDEX_ANSWER_ERROR_4 = 4,
    SNDEX_ANSWER_ERROR_5 = 5, // for the command 4, the byte to write is cleared
};

// The library does one request at a time, and is in the request state from the moment that one starts to that of its
// answer. The exclusive state follows the answer to the request 0xfb, and lasts until the request 0xfc is answered;
// every request but those two is refused in it
typedef enum SndexState {
    SNDEX_STATE_UNINITIALIZED,
    SNDEX_STATE_INITIALIZING,
    SNDEX_STATE_IDLE,
    SNDEX_STATE_REQUEST,
    SNDEX_STATE_EXCLUSIVE,
    SNDEX_STATE_EXCLUSIVE_ENDING,
} SndexState;

// The notification that the ARM7 sends of its own: the code of its word, and the argument that was set with it
typedef void (*SndexNotifyCallback)(u32 code, void *arg);

// The request in progress
typedef struct SndexRequest {
    SNDEXCallback callback; // 0x00
    void *arg;              // 0x04
    void *result;           // 0x08: where the answer's value goes, if the request has one
    u8 command;             // 0x0c
} SndexRequest;

// The notification that the ARM7 sends of its own, and the argument that goes with it
typedef struct SndexNotify {
    SndexNotifyCallback callback; // 0x00
    void *arg;                    // 0x04
    u32 unk_8[2];                 // 0x08
} SndexNotify;

// The library's state, in the bss: TwlSDK's separate statics, which MWCC lays out by size (the order of the
// definitions below gives the ROM's layout). The state is volatile because the PXI callback, which runs in the FIFO's
// interrupt, changes it while a thread looks at it. The volume and its counters belong to the code that sets the
// volume before the console sleeps or exits (func_02704e04), which nothing in the game starts: it sends the command
// 0x84 with the volume, and again, up to five times, from the callback of its answer
static u8 data_02734450;                 // the volume
static BOOL data_02734468;               // the last word of the filter request is on its way
static BOOL data_02734454;               // the filter request is in progress
static int data_02734458;                // the volume's retries
static volatile SndexState data_0273445c; // the state
static u32 data_02734460;                // the retries of the volume's callback
static BOOL data_02734464;               // the volume is being set
static SndexRequest data_0273449c;
static SndexNotify data_0273447c;
static PMExitCallbackInfo sExitInfo;
static PMSleepCallbackInfo sSleepInfo;

static SNDEXResult func_02704448(int volume, SNDEXCallback callback, void *arg, BOOL byLevel);
static SNDEXResult func_027044c0(int volume, SNDEXCallback callback, void *arg);
static SNDEXResult func_027044f8(int target, const u16 *coefficients, SNDEXCallback callback, void *arg);
static void func_027049b0(int tag, u32 data, BOOL err);
static void func_02704b58(SNDEXResult result, void *arg);
static BOOL func_02704b74(u32 command, u8 value);
static BOOL func_02704be0(u32 command, u16 value);
static SNDEXResult func_02704c5c(void);
static void func_02704cc4(SNDEXResult result);
static void func_02704d48(SNDEXCallback callback, void *arg, void *result, u8 command);
static void func_02704d64(void *arg);
static void func_02704d74(void);
static void func_02704db0(SNDEXResult result, void *arg);
static void func_02704e04(void);

// TwlSDK's func_02704364, by its code: sets the library up, once
// pokebw2's SNDEX_Init
void func_02704364(void) {
    u32 irq = OS_DisableInterrupts();

    if (data_0273445c != SNDEX_STATE_UNINITIALIZED) {
        OS_RestoreInterrupts(irq);
        return;
    }
    data_0273445c = SNDEX_STATE_INITIALIZING;
    OS_RestoreInterrupts(irq);

    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_NONE);
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_SNDEX, PXI_PROC_ARM7)) {
        SVC_WaitByLoop(10);
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_SNDEX, func_027049b0);

    PM_SetExitCallbackInfo(&sExitInfo, func_02704d64, NULL);
    PM_InsertPreExitCallback(&sExitInfo, SNDEX_PM_PRIORITY);
    PM_SetSleepCallbackInfo(&sSleepInfo, func_02704d64, NULL);
    PM_InsertPreSleepCallback(&sSleepInfo, SNDEX_PM_PRIORITY);
    data_0273445c = SNDEX_STATE_IDLE;
}

// Sends the command 0x84, with a value of at most 31, or at most 7 when byLevel is set
static SNDEXResult func_02704448(int volume, SNDEXCallback callback, void *arg, BOOL byLevel) {
    SNDEXResult result;

    if (volume > (byLevel ? 7 : 31)) {
        return SNDEX_RESULT_ILLEGAL_PARAMETER;
    }
    result = func_02704c5c();
    if (result != SNDEX_RESULT_SUCCESS) {
        return result;
    }
    func_02704d48(callback, arg, NULL, SNDEX_COMMAND_84);
    if (!func_02704b74(data_0273449c.command, (u8)((volume & 0x1f) | (byLevel << 6)))) {
        return SNDEX_RESULT_SEND_ERROR;
    }
    return SNDEX_RESULT_SUCCESS;
}

static SNDEXResult func_027044c0(int volume, SNDEXCallback callback, void *arg) {
    if (!OS_IsRunOnTwl()) {
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    return func_02704448(volume, callback, arg, FALSE);
}

// Sends the words of the filter request, one after the other, and gives the last the callback
static SNDEXResult func_027044f8(int target, const u16 *coefficients, SNDEXCallback callback, void *arg) {
    SNDEXResult result;

    if (data_02734454) {
        return SNDEX_RESULT_BUSY;
    }
    data_02734454 = TRUE;
    if (!OSi_IsCodecTwlMode()) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    result = func_02704c5c();
    if (result != SNDEX_RESULT_SUCCESS) {
        data_02734454 = FALSE;
        return result;
    }

    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_TARGET);
    if (!func_02704be0(data_0273449c.command, (u16)target)) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    // Each word waits until the answer to the last one is in and the library is idle again
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_COEFFICIENT_0);
    if (!func_02704be0(data_0273449c.command, coefficients[0])) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_COEFFICIENT_1);
    if (!func_02704be0(data_0273449c.command, coefficients[1])) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_COEFFICIENT_2);
    if (!func_02704be0(data_0273449c.command, coefficients[2])) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_COEFFICIENT_3);
    if (!func_02704be0(data_0273449c.command, coefficients[3])) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_FILTER_COEFFICIENT_4);
    if (!func_02704be0(data_0273449c.command, coefficients[4])) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    while (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
    }
    func_02704d48(callback, arg, NULL, SNDEX_COMMAND_FILTER_END);
    if (!func_02704be0(data_0273449c.command, 0)) {
        data_02734454 = FALSE;
        return SNDEX_RESULT_SEND_ERROR;
    }
    return SNDEX_RESULT_SUCCESS;
}

// Sets the microphone's filter and waits for the answer
// pokebw2's SNDEX_SetIirFilter
SNDEXResult func_027047c0(int target, const u16 *coefficients) {
    OSMessage message;
    OSMessage messages[1];
    OSMessageQueue queue;

    if (OS_GetCurrentThread() == NULL) {
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    OS_InitMessageQueue(&queue, messages, 1);
    message = (OSMessage)(uintptr_t)func_027044f8(target, coefficients, func_02704b58, &queue);
    if (message == NULL) {
        OS_ReceiveMessage(&queue, &message, OS_MESSAGE_BLOCK);
    }
    data_02734468 = FALSE;
    data_02734454 = FALSE;
    return (SNDEXResult)(uintptr_t)message;
}

// The request 0xfb: waits for the answer, and leaves the library in the exclusive state
SNDEXResult SNDEXi_PreProcessForShutterSound(void) {
    SNDEXResult result;
    OSMessage messages[1];
    OSMessageQueue queue;

    if (!OSi_IsCodecTwlMode()) {
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    if (OS_GetCurrentThread() == NULL) {
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    // In the exclusive state the request goes on without starting; the second test of the state needs it to change
    // between the two reads
    if (data_0273445c != SNDEX_STATE_EXCLUSIVE) {
        result = func_02704c5c();
        if (result != SNDEX_RESULT_SUCCESS) {
            return result;
        }
    } else if (data_0273445c == SNDEX_STATE_EXCLUSIVE_ENDING) {
        return SNDEX_RESULT_BUSY;
    }

    OS_InitMessageQueue(&queue, messages, 1);
    func_02704d48(func_02704b58, &queue, NULL, SNDEX_COMMAND_EXCLUSIVE_BEGIN);
    if (!func_02704b74(data_0273449c.command, 0)) {
        return SNDEX_RESULT_SEND_ERROR;
    }
    OS_ReceiveMessage(&queue, (OSMessage *)&result, OS_MESSAGE_BLOCK);
    return result;
}

// The request 0xfc, which ends the exclusive state: the callback gets the answer
SNDEXResult SNDEXi_PostProcessForShutterSound(SNDEXCallback callback, void *arg) {
    SNDEXResult result;

    if (!OSi_IsCodecTwlMode() || data_0273445c != SNDEX_STATE_EXCLUSIVE) {
        return SNDEX_RESULT_ILLEGAL_STATE;
    }
    result = SNDEX_RESULT_SEND_ERROR;
    data_0273445c = SNDEX_STATE_EXCLUSIVE_ENDING;
    func_02704d48(callback, arg, NULL, SNDEX_COMMAND_EXCLUSIVE_END);
    if (func_02704b74(data_0273449c.command, 0)) {
        result = SNDEX_RESULT_SUCCESS;
    }
    return result;
}

// The PXI callback, which gets the ARM7's words: the answers to the request in progress, and the notifications that it
// sends of its own
static void func_027049b0(int tag, u32 data, BOOL err) {
    u8 command = (u8)((data & 0xff0000) >> 16);
    u8 answer = (u8)((data & 0xff00) >> 8);
    u8 value = (u8)(data & 0xff);

    if (command == SNDEX_COMMAND_NOTIFY) {
        if (func_02704c5c() != SNDEX_RESULT_SUCCESS) {
            return;
        }
        func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_NOTIFY);
    }
    if (tag != PXI_FIFO_TAG_SNDEX || err == TRUE ||
        (data_0273445c != SNDEX_STATE_REQUEST && data_0273445c != SNDEX_STATE_EXCLUSIVE &&
         data_0273445c != SNDEX_STATE_EXCLUSIVE_ENDING) ||
        data_0273449c.command != command) {
        func_02704cc4(SNDEX_RESULT_FATAL_ERROR);
        return;
    }

    switch (answer) {
    case SNDEX_ANSWER_OK:
        switch (command) {
        case 1:
            if (data_0273449c.result != NULL) {
                *(u32 *)data_0273449c.result = value;
            }
            break;
        case 2:
            if (data_0273449c.result != NULL) {
                *(u32 *)data_0273449c.result = value;
            }
            break;
        case 3:
        case 4:
            if (data_0273449c.result != NULL) {
                *(u8 *)data_0273449c.result = value;
            }
            break;
        case 5:
            if (data_0273449c.result != NULL) {
                *(u32 *)data_0273449c.result = value;
            }
            break;
        case SNDEX_COMMAND_NOTIFY:
            if (data_0273447c.callback != NULL) {
                data_0273447c.callback(answer, data_0273447c.arg);
            }
            break;
        case 0xfe:
            if (data_0273449c.result != NULL) {
                *(u32 *)data_0273449c.result = value;
            }
            break;
        }
        func_02704cc4(SNDEX_RESULT_SUCCESS);
        break;
    case SNDEX_ANSWER_ERROR_2:
        func_02704cc4(SNDEX_RESULT_ILLEGAL_PARAMETER);
        break;
    case SNDEX_ANSWER_ERROR_3:
        func_02704cc4(SNDEX_RESULT_BUSY);
        break;
    case SNDEX_ANSWER_ERROR_4:
        func_02704cc4(SNDEX_RESULT_ILLEGAL_STATE);
        break;
    case SNDEX_ANSWER_ERROR_5:
        if (command == 4) {
            if (data_0273449c.result != NULL) {
                *(u8 *)data_0273449c.result = 0;
            }
        }
        func_02704cc4(SNDEX_RESULT_ARM7_ERROR);
        break;
    default:
        func_02704cc4(SNDEX_RESULT_FATAL_ERROR);
        break;
    }
}

// The callback of the synchronous requests: sends the result to the thread that waits
static void func_02704b58(SNDEXResult result, void *arg) {
    OS_SendMessage(arg, (OSMessage)(uintptr_t)result, OS_MESSAGE_NOBLOCK);
}

// Sends a word with a command and a value of one byte. FALSE, and the state back to idle, when the FIFO refuses it
static BOOL func_02704b74(u32 command, u8 value) {
    u32 irq = OS_DisableInterrupts();

    if (PXI_SendWordByFifo(PXI_FIFO_TAG_SNDEX, ((command << 16) & 0xff0000) | (value & 0xff), FALSE) < 0) {
        data_0273445c = SNDEX_STATE_IDLE;
        OS_RestoreInterrupts(irq);
        return FALSE;
    }
    OS_RestoreInterrupts(irq);
    return TRUE;
}

// Sends a word with a command and a value of two bytes
static BOOL func_02704be0(u32 command, u16 value) {
    u32 irq = OS_DisableInterrupts();

    if (PXI_SendWordByFifo(PXI_FIFO_TAG_SNDEX, ((command << 16) & 0xff0000) | (value & 0xffff), FALSE) < 0) {
        data_0273445c = SNDEX_STATE_IDLE;
        OS_RestoreInterrupts(irq);
        return FALSE;
    }
    if (command == SNDEX_COMMAND_FILTER_END) {
        data_02734468 = TRUE;
    }
    OS_RestoreInterrupts(irq);
    return TRUE;
}

// Starts a request if the library is idle, and puts it in the request state. It is not set up in the first two states,
// and busy in the request and exclusive ones
static SNDEXResult func_02704c5c(void) {
    u32 irq = OS_DisableInterrupts();

    switch (data_0273445c) {
    case SNDEX_STATE_UNINITIALIZED:
    case SNDEX_STATE_INITIALIZING:
        OS_RestoreInterrupts(irq);
        return SNDEX_RESULT_NOT_INITIALIZED;
    case SNDEX_STATE_REQUEST:
    case SNDEX_STATE_EXCLUSIVE:
    case SNDEX_STATE_EXCLUSIVE_ENDING:
        OS_RestoreInterrupts(irq);
        return SNDEX_RESULT_BUSY;
    default:
        data_0273445c = SNDEX_STATE_REQUEST;
        OS_RestoreInterrupts(irq);
        return SNDEX_RESULT_SUCCESS;
    }
}

// Ends the request in progress: clears it, goes back to idle (or to the exclusive state, after the request 0xfb) and
// calls its callback with the result
static void func_02704cc4(SNDEXResult result) {
    u32 irq = OS_DisableInterrupts();
    u8 command = data_0273449c.command;
    SNDEXCallback callback = data_0273449c.callback;
    void *arg = data_0273449c.arg;

    if (command == SNDEX_COMMAND_FILTER_END) {
        data_02734468 = FALSE;
        data_02734454 = FALSE;
    }
    func_02704d48(NULL, NULL, NULL, SNDEX_COMMAND_NONE);
    if (command == SNDEX_COMMAND_EXCLUSIVE_BEGIN) {
        data_0273445c = SNDEX_STATE_EXCLUSIVE;
    } else {
        data_0273445c = SNDEX_STATE_IDLE;
    }
    OS_RestoreInterrupts(irq);
    if (callback != NULL) {
        callback(result, arg);
    }
}

// Sets up the request in progress
static void func_02704d48(SNDEXCallback callback, void *arg, void *result, u8 command) {
    data_0273449c.callback = callback;
    data_0273449c.arg = arg;
    data_0273449c.result = result;
    data_0273449c.command = command;
}

// The callback that runs before the console sleeps or exits
static void func_02704d64(void *arg) {
    func_02704d74();
    func_02704e04();
}

// Waits until the last word of the filter request has been answered
static void func_02704d74(void) {
    while (data_02734468) {
        OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
        PXIi_HandlerRecvFifoNotEmpty();
    }
}

// The callback of the volume request: sends it again if it failed, up to five times, and ends the wait otherwise
static void func_02704db0(SNDEXResult result, void *arg) {
    if (result != SNDEX_RESULT_SUCCESS && data_02734460 < 5) {
        func_027044c0(data_02734450, func_02704db0, NULL);
        data_02734460++;
    } else {
        data_02734464 = FALSE;
    }
}

// Sends the volume request, if one is pending, and waits until its callback is done with it
static void func_02704e04(void) {
    if (data_02734464) {
        while (func_027044c0(data_02734450, func_02704db0, NULL) != SNDEX_RESULT_SUCCESS && data_02734458 < 5) {
            data_02734458++;
        }
        while (data_02734464) {
            OS_SpinWaitSysCycles(SNDEX_SPIN_CYCLES);
            PXIi_HandlerRecvFifoNotEmpty();
        }
    }
}
