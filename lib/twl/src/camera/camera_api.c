// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/src/camera/camera_api.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// TwlSDK's camera library (libcamera), the PXI and I2C API in the DSi's LTD autoload: the commands that the ARM9 sends
// to the ARM7, which drives the cameras' I2C bus, and the interrupt, thread and PM callbacks that keep the cameras
// running. ARM code. The ROM names none of it; the file's name is a guess, after the SDK's camera_api.h, whose Core
// names twl/camera.h gives. The functions have those names where they are known, and keep their default names
// otherwise.
//
// Each command is a word of the camera's PXI tag, with a payload of three bytes per further word, and the ARM7
// answers it in the same form. camera_api.c caches what the settings were set to (CAMERASettings) so that, when the
// camera stops answering, the thread starts the camera again and sends them all anew (func_02700230).
//
// The file is built with -ipa function (file_flags in lib/twl/library.toml): every static has a literal of its own, and
// sUnused, which nothing reads, stays in .bss. The work is one object: its fields beyond 0xfff are reached through a
// second literal, the work's address + 0x1000.

#include "types.h"
#include "nitro/ioreg.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "twl/camera.h"
#include "nitro/spi/ARM9/pm.h"
#include "twl/scfg.h"

#define CAMERA_THREAD_PRIORITY 4
#define CAMERA_STACK_SIZE 0x1000
#define CAMERA_PM_CALLBACK_PRIORITY 0x3fc

// The camera's clock in SCFG_CLK (twl/scfg.h) is on while a command is on its way and, for an active camera, after it.
// It is cleared with `reg = reg & ~mask`: the compound assignment narrows the mask to 0xfeff, an AND with a literal
// where the ROM has a BIC

// The commands, in bits 8 to 14 of the word that the ARM9 sends, and the answer, whose command is the same. Those
// of settings are named by what the SDK's functions do with them
#define CAMERA_COMMAND_I2C_INIT 0x00
#define CAMERA_COMMAND_I2C_ACTIVATE 0x10
#define CAMERA_COMMAND_I2C_CONTEXT_SWITCH 0x12 // A DSi-only choice between the contexts A and B of a camera
#define CAMERA_COMMAND_I2C_SIZE 0x30
#define CAMERA_COMMAND_I2C_FRAME_RATE 0x31
#define CAMERA_COMMAND_I2C_EFFECT 0x32
#define CAMERA_COMMAND_I2C_FLIP 0x33
#define CAMERA_COMMAND_I2C_PHOTO_MODE 0x34
#define CAMERA_COMMAND_I2C_WHITE_BALANCE 0x35
#define CAMERA_COMMAND_I2C_EXPOSURE 0x36
#define CAMERA_COMMAND_I2C_SHARPNESS 0x37
#define CAMERA_COMMAND_I2C_AUTO_EXPOSURE 0x39
#define CAMERA_COMMAND_I2C_AUTO_WHITE_BALANCE 0x3a
#define CAMERA_COMMAND_UNK_40 0x40 // Sent for the outer camera, once it is active, when its flag is set

// The first word of a command or an answer has this bit set, the others do not
#define CAMERA_PXI_START 0x02000000

// What the ARM7 says of a command in the first word's low byte; the code tells only the first, second and fifth apart,
// and takes any other for a fatal error
enum {
    CAMERA_PXI_RESULT_SUCCESS,
    CAMERA_PXI_RESULT_SUCCESS_FALSE,
    CAMERA_PXI_RESULT_BUSY,
    CAMERA_PXI_RESULT_ILLEGAL_PARAMETER,
    CAMERA_PXI_RESULT_ILLEGAL_STATUS,
    CAMERA_PXI_RESULT_FATAL_ERROR,
};

// What the camera was last set to: CAMERA_I2C*Ex's contexts A and B are the two elements of size, effect and flip
// Black 2's, not checked against Black
typedef struct {
    CAMERASize size[2];              // 0x00
    CAMERAEffect effect[2];          // 0x08
    CAMERAFlip flip[2];              // 0x10
    CAMERAPhotoMode photoMode;       // 0x18
    CAMERAWhiteBalance whiteBalance; // 0x1c
    int frameRate;                   // 0x20
    int exposure;                    // 0x24
    int sharpness;                   // 0x28
    BOOL autoExposure;               // 0x2c
    BOOL autoWhiteBalance;           // 0x30
    CAMERAContext context;           // 0x34: the context in use
    int unk_38;                      // 0x38: the outer camera's is set by the answers to the commands 0x10 and 0x40
} CAMERASettings;

// Black 2's; twinned code confirms the offsets of busy in Black
typedef struct {
    BOOL busy;                              // 0x00: set while a command is on its way, until the ARM7 has answered
    CAMERACallback callback;                // 0x04
    CAMERAResult result;                    // 0x08: of the synchronous functions
    void *callbackArg;                      // 0x0c
    int command;                            // 0x10: of the answer that is being received
    u32 response;                           // 0x14: the low byte of its first word
    u8 received;                            // 0x18: bytes of the payload received so far
    u8 length;                              // 0x19: the length that the first word says
    u8 *buffer;                             // 0x1c: where the payload goes
    u32 bufferSize;                         // 0x20
    BOOL clockWasOn;                        // 0x24: whether the interface's clock was on before the command was sent
    BOOL keepClock;                         // 0x28: leave the clock on after the answer
    BOOL stopClock;                         // 0x2c: turn the clock off after the answer
    PMSleepCallbackInfo sleepInfo;          // 0x30
    PMExitCallbackInfo exitInfo;            // 0x40
    u64 vsyncTick;                          // 0x50: when the interface last had a frame
    CAMERAIntrCallback vsyncCallback;       // 0x58
    CAMERAIntrCallback bufferErrorCallback; // 0x5c
    CAMERAIntrCallback rebootCallback;      // 0x60
    OSThread thread;                        // 0x64: watches the camera and starts it again
    u32 stack[CAMERA_STACK_SIZE / sizeof(u32)]; // 0x124
    CAMERASelect activeCamera;                  // 0x1124
    CAMERASettings settings[2];                 // 0x1128: for the inner and the outer camera
    int sentCamera;        // 0x11a0: what the command that is on its way set, for the answer to store
    int sentContext;       // 0x11a4
    int sentValue;         // 0x11a8
    u32 errorCount;        // 0x11ac: the answers that said the state was wrong, and the timeouts
    int pollCount;         // 0x11b0
    OSMessageQueue queue;  // 0x11b4: the answers' way to wake the thread
    OSMessage messages[1]; // 0x11d4
} CAMERAWork;

// MWCC lays these out in reverse order of declaration: the flag first, then the word, then the work
// pokebw2's sCameraWork
static CAMERAWork data_02732fc8;
static u32 sUnused; // Nothing reads it; the library's other code that used it was stripped
// pokebw2's sCameraInitialized
static BOOL data_02732fc0;

static void func_02701aa0(int tag, u32 data, BOOL err);
static void func_02701fd8(CAMERAResult result, void *arg);
static void func_02702028(void);
static void func_02702070(void *arg);
static CAMERAResult func_02700c18(CAMERASelect camera, CAMERACallback callback, void *arg);
static CAMERAResult func_02700cec(CAMERASelect camera, CAMERACallback callback, void *arg);
static CAMERAResult func_02700df0(CAMERASelect camera, CAMERAContext context, CAMERACallback callback, void *arg);
static CAMERAResult func_02700eb8(CAMERASelect camera, CAMERAContext context, CAMERASize size, CAMERACallback callback,
                                  void *arg);
static CAMERAResult func_02701020(CAMERASelect camera, int rate, CAMERACallback callback, void *arg);
static CAMERAResult func_02701120(CAMERASelect camera, CAMERAContext context, CAMERAEffect effect,
                                  CAMERACallback callback, void *arg);
static CAMERAResult func_02701284(CAMERASelect camera, CAMERAContext context, CAMERAFlip flip, CAMERACallback callback,
                                  void *arg);
static CAMERAResult func_027013a4(CAMERASelect camera, CAMERAPhotoMode mode, CAMERACallback callback, void *arg);
static CAMERAResult func_02701488(CAMERASelect camera, CAMERAWhiteBalance whiteBalance, CAMERACallback callback,
                                  void *arg);
static CAMERAResult func_02701570(CAMERASelect camera, int exposure, CAMERACallback callback, void *arg);
static CAMERAResult func_02701644(CAMERASelect camera, int sharpness, CAMERACallback callback, void *arg);
static CAMERAResult func_02701718(CAMERASelect camera, BOOL on, CAMERACallback callback, void *arg);
static CAMERAResult func_02701814(CAMERASelect camera, BOOL on, CAMERACallback callback, void *arg);
static CAMERAResult func_02701940(int value, CAMERACallback callback, void *arg);
static BOOL func_027019b4(u32 command, u32 length, u8 data);
static void func_02701a60(const u8 *data);
// The bytes of a command after the first go out three at a time, so a command's buffer has room for the two bytes
// that the last three can reach past its size
#define CAMERA_COMMAND_BUFFER_SIZE(size) ((size) + 2)
static void func_02701fe8(CAMERAResult result);
static void func_027001f0(void);

// The interrupt of the camera interface: a frame is ready, or the buffer overflowed
static void func_02700078(void) {
    OS_SetIrqCheckFlag(OS_IE_CAMERA);
    if (CAMERA_IsBufferError()) {
        if (data_02732fc8.bufferErrorCallback != NULL) {
            data_02732fc8.bufferErrorCallback(CAMERA_RESULT_SUCCESS);
        }
    } else {
        data_02732fc8.vsyncTick = OS_GetTick();
        if (data_02732fc8.vsyncCallback != NULL) {
            data_02732fc8.vsyncCallback(CAMERA_RESULT_SUCCESS);
        }
    }
}

// pokebw2's CAMERA_SetVsyncCallbackCore
void func_027000f8(CAMERAIntrCallback callback) {
    data_02732fc8.vsyncCallback = callback;
}

// pokebw2's CAMERA_SetBufferErrorCallbackCore
void func_02700108(CAMERAIntrCallback callback) {
    data_02732fc8.bufferErrorCallback = callback;
}

// pokebw2's CAMERA_SetRebootCallbackCore
void func_02700118(CAMERAIntrCallback callback) {
    data_02732fc8.rebootCallback = callback;
}

// What a camera's white balance, sharpness and exposure are for a photo mode (func_02700128, func_02700168 and
// func_027001b8)
static CAMERAWhiteBalance func_02700128(CAMERAPhotoMode mode) {
    switch (mode) {
    case CAMERA_PHOTO_MODE_NORMAL:
    case CAMERA_PHOTO_MODE_PORTRAIT:
        return CAMERA_WHITE_BALANCE_AUTO;
    case CAMERA_PHOTO_MODE_LANDSCAPE:
        return CAMERA_WHITE_BALANCE_5200K;
    case CAMERA_PHOTO_MODE_NIGHTVIEW:
    case CAMERA_PHOTO_MODE_LETTER:
        return CAMERA_WHITE_BALANCE_AUTO;
    default:
        return CAMERA_WHITE_BALANCE_AUTO;
    }
}

static int func_02700168(CAMERAPhotoMode mode) {
    switch (mode) {
    case CAMERA_PHOTO_MODE_NORMAL:
        return 0;
    case CAMERA_PHOTO_MODE_PORTRAIT:
        return -2;
    case CAMERA_PHOTO_MODE_LANDSCAPE:
        return 1;
    case CAMERA_PHOTO_MODE_NIGHTVIEW:
        return -1;
    case CAMERA_PHOTO_MODE_LETTER:
        return 2;
    default:
        return 0;
    }
}

static int func_027001b8(CAMERAPhotoMode mode) {
    switch (mode) {
    case CAMERA_PHOTO_MODE_NORMAL:
    case CAMERA_PHOTO_MODE_PORTRAIT:
    case CAMERA_PHOTO_MODE_LANDSCAPE:
        return 0;
    case CAMERA_PHOTO_MODE_NIGHTVIEW:
    case CAMERA_PHOTO_MODE_LETTER:
        return 2;
    default:
        return 0;
    }
}

// The camera answered with a wrong state: count it and wake the thread
static void func_027001f0(void) {
    data_02732fc8.busy = TRUE;
    data_02732fc8.errorCount++;
    OS_SendMessage(&data_02732fc8.queue, 0, OS_MESSAGE_NOBLOCK);
}

// The callback of the commands that start the camera again: sends the next setting that is not the camera's default,
// one command per step, until all are sent. A step is the argument that the command before gave; a case whose
// setting is the default falls through to the next step
static void func_02700230(CAMERAResult result, void *arg) {
    if (result != CAMERA_RESULT_SUCCESS) {
        return;
    }
    switch ((int)arg) {
    case 0:
        func_02700c18(CAMERA_SELECT_BOTH, func_02700230, (void *)1);
        return;
    case 1:
        if (data_02732fc8.settings[0].photoMode != CAMERA_PHOTO_MODE_PORTRAIT) {
            func_027013a4(CAMERA_SELECT_IN, data_02732fc8.settings[0].photoMode, func_02700230, (void *)2);
            return;
        }
    case 2:
        if (data_02732fc8.settings[1].photoMode != CAMERA_PHOTO_MODE_NORMAL) {
            func_027013a4(CAMERA_SELECT_OUT, data_02732fc8.settings[1].photoMode, func_02700230, (void *)3);
            return;
        }
    case 3:
        if (data_02732fc8.settings[0].size[0] != CAMERA_SIZE_DS_LCD) {
            func_02700eb8(CAMERA_SELECT_IN, CAMERA_CONTEXT_A, data_02732fc8.settings[0].size[0], func_02700230,
                          (void *)4);
            return;
        }
    case 4:
        if (data_02732fc8.settings[0].size[1] != CAMERA_SIZE_DSI_VGA) {
            func_02700eb8(CAMERA_SELECT_IN, CAMERA_CONTEXT_B, data_02732fc8.settings[0].size[1], func_02700230,
                          (void *)5);
            return;
        }
    case 5:
        if (data_02732fc8.settings[1].size[0] != CAMERA_SIZE_DS_LCD) {
            func_02700eb8(CAMERA_SELECT_OUT, CAMERA_CONTEXT_A, data_02732fc8.settings[1].size[0], func_02700230,
                          (void *)6);
            return;
        }
    case 6:
        if (data_02732fc8.settings[1].size[1] != CAMERA_SIZE_DSI_VGA) {
            func_02700eb8(CAMERA_SELECT_OUT, CAMERA_CONTEXT_B, data_02732fc8.settings[1].size[1], func_02700230,
                          (void *)7);
            return;
        }
    case 7:
        if (data_02732fc8.settings[0].frameRate != 0) {
            func_02701020(CAMERA_SELECT_IN, data_02732fc8.settings[0].frameRate, func_02700230, (void *)8);
            return;
        }
    case 8:
        if (data_02732fc8.settings[1].frameRate != 0) {
            func_02701020(CAMERA_SELECT_OUT, data_02732fc8.settings[1].frameRate, func_02700230, (void *)9);
            return;
        }
    case 9:
        if (data_02732fc8.settings[0].effect[0] != CAMERA_EFFECT_NONE) {
            func_02701120(CAMERA_SELECT_IN, CAMERA_CONTEXT_A, data_02732fc8.settings[0].effect[0], func_02700230,
                          (void *)10);
            return;
        }
    case 10:
        if (data_02732fc8.settings[0].effect[1] != CAMERA_EFFECT_NONE) {
            func_02701120(CAMERA_SELECT_IN, CAMERA_CONTEXT_B, data_02732fc8.settings[0].effect[1], func_02700230,
                          (void *)11);
            return;
        }
    case 11:
        if (data_02732fc8.settings[1].effect[0] != CAMERA_EFFECT_NONE) {
            func_02701120(CAMERA_SELECT_OUT, CAMERA_CONTEXT_A, data_02732fc8.settings[1].effect[0], func_02700230,
                          (void *)12);
            return;
        }
    case 12:
        if (data_02732fc8.settings[1].effect[1] != CAMERA_EFFECT_NONE) {
            func_02701120(CAMERA_SELECT_OUT, CAMERA_CONTEXT_B, data_02732fc8.settings[1].effect[1], func_02700230,
                          (void *)13);
            return;
        }
    case 13:
        if (data_02732fc8.settings[0].flip[0] != CAMERA_FLIP_VERTICAL) {
            func_02701284(CAMERA_SELECT_IN, CAMERA_CONTEXT_A, data_02732fc8.settings[0].flip[0], func_02700230,
                          (void *)14);
            return;
        }
    case 14:
        if (data_02732fc8.settings[0].flip[1] != CAMERA_FLIP_VERTICAL) {
            func_02701284(CAMERA_SELECT_IN, CAMERA_CONTEXT_B, data_02732fc8.settings[0].flip[1], func_02700230,
                          (void *)15);
            return;
        }
    case 15:
        if (data_02732fc8.settings[1].flip[0] != CAMERA_FLIP_NONE) {
            func_02701284(CAMERA_SELECT_OUT, CAMERA_CONTEXT_A, data_02732fc8.settings[1].flip[0], func_02700230,
                          (void *)16);
            return;
        }
    case 16:
        if (data_02732fc8.settings[1].flip[1] != CAMERA_FLIP_NONE) {
            func_02701284(CAMERA_SELECT_OUT, CAMERA_CONTEXT_B, data_02732fc8.settings[1].flip[1], func_02700230,
                          (void *)17);
            return;
        }
    case 17:
        if (data_02732fc8.settings[0].whiteBalance != func_02700128(data_02732fc8.settings[0].photoMode)) {
            func_02701488(CAMERA_SELECT_IN, data_02732fc8.settings[0].whiteBalance, func_02700230, (void *)18);
            return;
        }
    case 18:
        if (data_02732fc8.settings[1].whiteBalance != func_02700128(data_02732fc8.settings[1].photoMode)) {
            func_02701488(CAMERA_SELECT_OUT, data_02732fc8.settings[1].whiteBalance, func_02700230, (void *)19);
            return;
        }
    case 19: {
        int exposure = data_02732fc8.settings[0].exposure;
        if (exposure != func_027001b8(data_02732fc8.settings[0].photoMode)) {
            func_02701570(CAMERA_SELECT_IN, exposure, func_02700230, (void *)20);
            return;
        }
    }
    case 20: {
        int exposure = data_02732fc8.settings[1].exposure;
        if (exposure != func_027001b8(data_02732fc8.settings[1].photoMode)) {
            func_02701570(CAMERA_SELECT_OUT, exposure, func_02700230, (void *)21);
            return;
        }
    }
    case 21: {
        int sharpness = data_02732fc8.settings[0].sharpness;
        if (sharpness != func_02700168(data_02732fc8.settings[0].photoMode)) {
            func_02701644(CAMERA_SELECT_IN, sharpness, func_02700230, (void *)22);
            return;
        }
    }
    case 22: {
        int sharpness = data_02732fc8.settings[1].sharpness;
        if (sharpness != func_02700168(data_02732fc8.settings[1].photoMode)) {
            func_02701644(CAMERA_SELECT_OUT, sharpness, func_02700230, (void *)23);
            return;
        }
    }
    case 23:
        if (data_02732fc8.settings[0].autoExposure != TRUE) {
            func_02701718(CAMERA_SELECT_IN, data_02732fc8.settings[0].autoExposure, func_02700230, (void *)24);
            return;
        }
    case 24:
        if (data_02732fc8.settings[1].autoExposure != TRUE) {
            func_02701718(CAMERA_SELECT_OUT, data_02732fc8.settings[1].autoExposure, func_02700230, (void *)25);
            return;
        }
    case 25:
        if (data_02732fc8.settings[0].autoWhiteBalance != TRUE) {
            func_02701814(CAMERA_SELECT_IN, data_02732fc8.settings[0].autoWhiteBalance, func_02700230, (void *)26);
            return;
        }
    case 26:
        if (data_02732fc8.settings[1].autoWhiteBalance != TRUE) {
            func_02701814(CAMERA_SELECT_OUT, data_02732fc8.settings[1].autoWhiteBalance, func_02700230, (void *)27);
            return;
        }
    case 27: {
        CAMERAContext context = data_02732fc8.settings[0].context;
        if (context != CAMERA_CONTEXT_A) {
            if (OS_IsRunOnTwl() == TRUE) {
                func_02700df0(CAMERA_SELECT_IN, context, func_02700230, (void *)28);
            }
            return;
        }
    }
    case 28: {
        CAMERAContext context = data_02732fc8.settings[1].context;
        if (context != CAMERA_CONTEXT_A) {
            if (OS_IsRunOnTwl() == TRUE) {
                func_02700df0(CAMERA_SELECT_OUT, context, func_02700230, (void *)29);
            }
            return;
        }
    }
    case 29:
        if (data_02732fc8.activeCamera != CAMERA_SELECT_NONE) {
            func_02700cec(data_02732fc8.activeCamera, func_02700230, (void *)30);
            return;
        }
    case 30:
        if (data_02732fc8.activeCamera == CAMERA_SELECT_OUT && data_02732fc8.settings[1].unk_38 == 1) {
            if (OS_IsRunOnTwl() == TRUE) {
                func_02701940(1, func_02700230, (void *)31);
            }
            return;
        }
    default:
        break;
    }
    data_02732fc8.errorCount = 0;
    if (data_02732fc8.rebootCallback != NULL) {
        data_02732fc8.rebootCallback(CAMERA_RESULT_SUCCESS);
    }
}

// Checks that the camera may take a command: FATAL_ERROR, after it has stopped the active camera, once it failed more
// than twice, ILLEGAL_STATUS while it is being started again
static CAMERAResult func_027007c8(void) {
    if (data_02732fc8.errorCount > 2) {
        if (data_02732fc8.activeCamera != CAMERA_SELECT_NONE) {
            data_02732fc8.stopClock = TRUE;
            data_02732fc8.activeCamera = CAMERA_SELECT_NONE;
            func_027019b4(CAMERA_COMMAND_I2C_ACTIVATE, 1, 0);
        }
        return CAMERA_RESULT_FATAL_ERROR;
    }
    if (data_02732fc8.errorCount != 0) {
        return CAMERA_RESULT_ILLEGAL_STATUS;
    }
    return CAMERA_RESULT_SUCCESS;
}

// The thread: every 100 ms it looks for an answer that said the camera was in a wrong state and, every second, for a
// frame; if there is none for a second, it counts that as a failure as well. On a failure it resets the interface
// and sends the settings again
static void func_02700824(void *arg) {
    OSMessage message;
    int elapsed;

    OS_DisableInterrupts();
    for (;;) {
        OS_Sleep(100);
        if (!OS_ReceiveMessage(&data_02732fc8.queue, &message, OS_MESSAGE_NOBLOCK)) {
            if (data_02732fc8.errorCount != 0) {
                continue;
            }
            if (data_02732fc8.activeCamera == CAMERA_SELECT_NONE) {
                continue;
            }
            if (data_02732fc8.busy) {
                continue;
            }
            if (++data_02732fc8.pollCount < 10) {
                continue;
            }
            data_02732fc8.pollCount = 0;
            // The ticks since the last frame as an int, which the comparison widens to an unsigned 64 bits
            elapsed = OS_GetTick() - data_02732fc8.vsyncTick;
            if (elapsed < OS_MilliSecondsToTicks(1000)) {
                continue;
            }
            data_02732fc8.busy = TRUE;
            data_02732fc8.errorCount++;
        }
        if (func_027007c8() == CAMERA_RESULT_FATAL_ERROR) {
            data_02732fc8.busy = FALSE;
            if (data_02732fc8.rebootCallback != NULL) {
                data_02732fc8.rebootCallback(CAMERA_RESULT_FATAL_ERROR);
            }
            return;
        }
        OS_EnableInterrupts();
        CAMERA_ResetCore();
        OS_DisableInterrupts();
        data_02732fc8.busy = FALSE;
        func_02700230(CAMERA_RESULT_SUCCESS, NULL);
    }
}

// pokebw2's CAMERA_InitCore
CAMERAResult func_02700938(void) {
    if (data_02732fc0) {
        return CAMERA_RESULT_BUSY;
    }
    data_02732fc0 = TRUE;
    data_02732fc8.vsyncCallback = NULL;
    data_02732fc8.bufferErrorCallback = NULL;
    data_02732fc8.rebootCallback = NULL;
    data_02732fc8.busy = FALSE;
    data_02732fc8.callback = NULL;
    data_02732fc8.keepClock = FALSE;
    data_02732fc8.stopClock = FALSE;
    data_02732fc8.activeCamera = CAMERA_SELECT_NONE;
    data_02732fc8.errorCount = 0;
    data_02732fc8.pollCount = 0;
    PXI_Init();
    if (!PXI_IsCallbackReady(PXI_FIFO_TAG_CAMERA, PXI_PROC_ARM7)) {
        return CAMERA_RESULT_FATAL_ERROR;
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CAMERA, func_02701aa0);
    CAMERA_ResetCore();
    CAMERA_StopCaptureCore();
    CAMERA_ClearBufferCore();
    PM_SetSleepCallbackInfo(&data_02732fc8.sleepInfo, func_02702070, NULL);
    PM_InsertPreSleepCallback(&data_02732fc8.sleepInfo, CAMERA_PM_CALLBACK_PRIORITY);
    PM_SetExitCallbackInfo(&data_02732fc8.exitInfo, func_02702070, NULL);
    PM_InsertPreExitCallback(&data_02732fc8.exitInfo, CAMERA_PM_CALLBACK_PRIORITY);
    func_0270223c(0x200);
    func_02702268(TRUE);
    CAMERA_SetInterruptEnableCore(TRUE);
    OS_SetIrqFunction(OS_IE_CAMERA, func_02700078);
    OS_EnableIrqMask(OS_IE_CAMERA);
    data_02732fc8.settings[0].size[0] = CAMERA_SIZE_DS_LCD;
    data_02732fc8.settings[0].size[1] = CAMERA_SIZE_DSI_VGA;
    data_02732fc8.settings[0].effect[0] = CAMERA_EFFECT_NONE;
    data_02732fc8.settings[0].effect[1] = CAMERA_EFFECT_NONE;
    data_02732fc8.settings[0].flip[0] = CAMERA_FLIP_VERTICAL;
    data_02732fc8.settings[0].flip[1] = CAMERA_FLIP_VERTICAL;
    data_02732fc8.settings[0].photoMode = CAMERA_PHOTO_MODE_PORTRAIT;
    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
    data_02732fc8.settings[0].frameRate = 0;
    data_02732fc8.settings[0].exposure = 0;
    data_02732fc8.settings[0].sharpness = -2;
    data_02732fc8.settings[0].autoExposure = TRUE;
    data_02732fc8.settings[0].autoWhiteBalance = TRUE;
    data_02732fc8.settings[0].context = CAMERA_CONTEXT_A;
    data_02732fc8.settings[0].unk_38 = 0;
    data_02732fc8.settings[1].size[0] = CAMERA_SIZE_DS_LCD;
    data_02732fc8.settings[1].size[1] = CAMERA_SIZE_DSI_VGA;
    data_02732fc8.settings[1].effect[0] = CAMERA_EFFECT_NONE;
    data_02732fc8.settings[1].effect[1] = CAMERA_EFFECT_NONE;
    data_02732fc8.settings[1].flip[0] = CAMERA_FLIP_NONE;
    data_02732fc8.settings[1].flip[1] = CAMERA_FLIP_NONE;
    data_02732fc8.settings[1].photoMode = CAMERA_PHOTO_MODE_NORMAL;
    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
    data_02732fc8.settings[1].frameRate = 0;
    data_02732fc8.settings[1].exposure = 0;
    data_02732fc8.settings[1].sharpness = 0;
    data_02732fc8.settings[1].autoExposure = TRUE;
    data_02732fc8.settings[1].autoWhiteBalance = TRUE;
    data_02732fc8.settings[1].context = CAMERA_CONTEXT_A;
    data_02732fc8.settings[1].unk_38 = 0;
    OS_InitMessageQueue(&data_02732fc8.queue, data_02732fc8.messages, 1);
    OS_CreateThread(&data_02732fc8.thread, func_02700824, NULL, data_02732fc8.stack + CAMERA_STACK_SIZE / sizeof(u32),
                    CAMERA_STACK_SIZE, CAMERA_THREAD_PRIORITY);
    OS_WakeupThreadDirect(&data_02732fc8.thread);
    return CAMERA_RESULT_SUCCESS;
}

// pokebw2's CAMERA_EndCore
void func_02700b34(void) {
    if (!data_02732fc0) {
        return;
    }
    data_02732fc0 = FALSE;
    PM_DeletePreSleepCallback(&data_02732fc8.sleepInfo);
    PM_DeletePreExitCallback(&data_02732fc8.exitInfo);
    CAMERA_I2CActivateCore(CAMERA_SELECT_NONE);
    CAMERA_ResetCore();
    CAMERA_StopCaptureCore();
    CAMERA_ClearBufferCore();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CAMERA, NULL);
    func_0270223c(0);
    func_02702268(FALSE);
    CAMERA_SetInterruptEnableCore(FALSE);
    OS_SetIrqFunction(OS_IE_CAMERA, NULL);
    OS_DisableIrqMask(OS_IE_CAMERA);
    OS_KillThread(&data_02732fc8.thread, NULL);
}

// Stops the capture, waits for the interface to finish, clears its buffer and deactivates the cameras
CAMERAResult func_02700bd0(void) {
    int timeout = 0x1991b0;

    CAMERA_StopCaptureCore();
    while (timeout-- > 0 && CAMERA_IsBusyCore()) {
        OS_SpinWaitSysCycles(4);
    }
    CAMERA_ClearBufferCore();
    return CAMERA_I2CActivateCore(CAMERA_SELECT_NONE);
}

// Starts the I2C of the cameras
static CAMERAResult func_02700c18(CAMERASelect camera, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data_02732fc8.stopClock = TRUE;
    return func_027019b4(CAMERA_COMMAND_I2C_INIT, 1, camera) ? CAMERA_RESULT_SUCCESS : CAMERA_RESULT_SEND_ERROR;
}

// pokebw2's CAMERA_I2CInitCore
CAMERAResult func_02700cb4(CAMERASelect camera) {
    CAMERAResult result = func_02700c18(camera, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

// Activates a camera, or none
static CAMERAResult func_02700cec(CAMERASelect camera, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_NONE && camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    if (camera == CAMERA_SELECT_NONE) {
        data_02732fc8.stopClock = TRUE;
    } else {
        data_02732fc8.keepClock = TRUE;
    }
    data_02732fc8.activeCamera = camera;
    if (data_02732fc8.errorCount == 0) {
        data_02732fc8.sentValue = 0;
    }
    data_02732fc8.vsyncTick = OS_GetTick();
    return func_027019b4(CAMERA_COMMAND_I2C_ACTIVATE, 1, camera) ? CAMERA_RESULT_SUCCESS : CAMERA_RESULT_SEND_ERROR;
}

CAMERAResult CAMERA_I2CActivateCore(CAMERASelect camera) {
    CAMERAResult result = func_02700cec(camera, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

// Chooses the context of a camera that it uses
static CAMERAResult func_02700df0(CAMERASelect camera, CAMERAContext context, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (context != CAMERA_CONTEXT_A && context != CAMERA_CONTEXT_B) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = context;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = context;
    if (!func_027019b4(CAMERA_COMMAND_I2C_CONTEXT_SWITCH, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02700eb8(CAMERASelect camera, CAMERAContext context, CAMERASize size, CAMERACallback callback,
                                  void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(3)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (context != CAMERA_CONTEXT_A && context != CAMERA_CONTEXT_B && context != CAMERA_CONTEXT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (size) {
    case CAMERA_SIZE_DSI_VGA:
    case CAMERA_SIZE_QVGA:
    case CAMERA_SIZE_QQVGA:
    case CAMERA_SIZE_CIF:
    case CAMERA_SIZE_QCIF:
    case CAMERA_SIZE_DS_LCD:
    case CAMERA_SIZE_DS_LCDX4:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = context;
    data[2] = size;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentContext = context;
    data_02732fc8.sentValue = size;
    if (!func_027019b4(CAMERA_COMMAND_I2C_SIZE, 3, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

// pokebw2's CAMERA_I2CSizeExCore
CAMERAResult func_02700fe4(CAMERASelect camera, CAMERAContext context, CAMERASize size) {
    CAMERAResult result = func_02700eb8(camera, context, size, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

static CAMERAResult func_02701020(CAMERASelect camera, int rate, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (rate) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = rate;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = rate;
    if (!func_027019b4(CAMERA_COMMAND_I2C_FRAME_RATE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02701120(CAMERASelect camera, CAMERAContext context, CAMERAEffect effect,
                                  CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(3)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (context != CAMERA_CONTEXT_A && context != CAMERA_CONTEXT_B && context != CAMERA_CONTEXT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (effect) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = context;
    data[2] = effect;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentContext = context;
    data_02732fc8.sentValue = effect;
    if (!func_027019b4(CAMERA_COMMAND_I2C_EFFECT, 3, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

// pokebw2's CAMERA_I2CEffectExCore
CAMERAResult func_02701248(CAMERASelect camera, CAMERAContext context, CAMERAEffect effect) {
    CAMERAResult result = func_02701120(camera, context, effect, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

static CAMERAResult func_02701284(CAMERASelect camera, CAMERAContext context, CAMERAFlip flip, CAMERACallback callback,
                                  void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(3)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (context != CAMERA_CONTEXT_A && context != CAMERA_CONTEXT_B && context != CAMERA_CONTEXT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (flip) {
    case CAMERA_FLIP_NONE:
    case CAMERA_FLIP_HORIZONTAL:
    case CAMERA_FLIP_VERTICAL:
    case CAMERA_FLIP_REVERSE:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = context;
    data[2] = flip;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentContext = context;
    data_02732fc8.sentValue = flip;
    if (!func_027019b4(CAMERA_COMMAND_I2C_FLIP, 3, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_027013a4(CAMERASelect camera, CAMERAPhotoMode mode, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (mode) {
    case CAMERA_PHOTO_MODE_NORMAL:
    case CAMERA_PHOTO_MODE_PORTRAIT:
    case CAMERA_PHOTO_MODE_LANDSCAPE:
    case CAMERA_PHOTO_MODE_NIGHTVIEW:
    case CAMERA_PHOTO_MODE_LETTER:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = mode;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = mode;
    if (!func_027019b4(CAMERA_COMMAND_I2C_PHOTO_MODE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02701488(CAMERASelect camera, CAMERAWhiteBalance whiteBalance, CAMERACallback callback,
                                  void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    switch (whiteBalance) {
    case CAMERA_WHITE_BALANCE_AUTO:
    case CAMERA_WHITE_BALANCE_3200K:
    case CAMERA_WHITE_BALANCE_4150K:
    case CAMERA_WHITE_BALANCE_5200K:
    case CAMERA_WHITE_BALANCE_6000K:
    case CAMERA_WHITE_BALANCE_7000K:
        break;
    default:
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = whiteBalance;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = whiteBalance;
    if (!func_027019b4(CAMERA_COMMAND_I2C_WHITE_BALANCE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02701570(CAMERASelect camera, int exposure, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (exposure < -5 || exposure > 5) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = exposure;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = exposure;
    if (!func_027019b4(CAMERA_COMMAND_I2C_EXPOSURE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02701644(CAMERASelect camera, int sharpness, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    if (sharpness < -3 || sharpness > 5) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = sharpness;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = sharpness;
    if (!func_027019b4(CAMERA_COMMAND_I2C_SHARPNESS, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

static CAMERAResult func_02701718(CAMERASelect camera, BOOL on, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = on != FALSE;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = on;
    if (!func_027019b4(CAMERA_COMMAND_I2C_AUTO_EXPOSURE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

// pokebw2's CAMERA_I2CAutoExposureCore
CAMERAResult func_027017dc(CAMERASelect camera, BOOL on) {
    CAMERAResult result = func_02701718(camera, on, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

static CAMERAResult func_02701814(CAMERASelect camera, BOOL on, CAMERACallback callback, void *arg) {
    CAMERAResult state = func_027007c8();
    u8 data[CAMERA_COMMAND_BUFFER_SIZE(2)];
    u32 enabled;

    if (state == CAMERA_RESULT_FATAL_ERROR) {
        return state;
    }
    if (camera != CAMERA_SELECT_IN && camera != CAMERA_SELECT_OUT && camera != CAMERA_SELECT_BOTH) {
        return CAMERA_RESULT_ILLEGAL_PARAMETER;
    }
    // The white balance is automatic only while its setting is the first
    if ((camera & CAMERA_SELECT_IN) && data_02732fc8.settings[0].whiteBalance != CAMERA_WHITE_BALANCE_AUTO) {
        return CAMERA_RESULT_INVALID_COMMAND;
    }
    if ((camera & CAMERA_SELECT_OUT) && data_02732fc8.settings[1].whiteBalance != CAMERA_WHITE_BALANCE_AUTO) {
        return CAMERA_RESULT_INVALID_COMMAND;
    }
    enabled = OS_DisableInterrupts();
    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return state == CAMERA_RESULT_ILLEGAL_STATUS ? CAMERA_RESULT_ILLEGAL_STATUS : CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data[0] = camera;
    data[1] = on != FALSE;
    data_02732fc8.sentCamera = camera;
    data_02732fc8.sentValue = on;
    if (!func_027019b4(CAMERA_COMMAND_I2C_AUTO_WHITE_BALANCE, 2, data[0])) {
        return CAMERA_RESULT_SEND_ERROR;
    }
    func_02701a60(&data[1]);
    return CAMERA_RESULT_SUCCESS;
}

// pokebw2's CAMERA_I2CAutoWhiteBalanceCore
CAMERAResult func_02701908(CAMERASelect camera, BOOL on) {
    CAMERAResult result = func_02701814(camera, on, func_02701fd8, NULL);

    data_02732fc8.result = result;
    if (result == CAMERA_RESULT_SUCCESS) {
        func_02702028();
    }
    return data_02732fc8.result;
}

static CAMERAResult func_02701940(int value, CAMERACallback callback, void *arg) {
    u32 enabled = OS_DisableInterrupts();

    if (data_02732fc8.busy) {
        OS_RestoreInterrupts(enabled);
        return CAMERA_RESULT_BUSY;
    }
    data_02732fc8.busy = TRUE;
    OS_RestoreInterrupts(enabled);
    data_02732fc8.callback = callback;
    data_02732fc8.callbackArg = arg;
    data_02732fc8.sentValue = value;
    return func_027019b4(CAMERA_COMMAND_UNK_40, 1, value) ? CAMERA_RESULT_SUCCESS : CAMERA_RESULT_SEND_ERROR;
}

// Sends the first word of a command, with its first byte of data, after turning the interface's clock on
static BOOL func_027019b4(u32 command, u32 length, u8 data) {
    u32 word = ((command << 8) & 0x7f00) | CAMERA_PXI_START | ((length << 16) & 0xff0000) | (data & 0xff);
    u32 enabled;
    u16 clk;
    int wasOn;

    enabled = OS_DisableInterrupts();
    clk = reg_SCFG_CLK;
    reg_SCFG_CLK = clk | SCFG_CLK_CAMERA;
    OS_RestoreInterrupts(enabled);
    wasOn = clk & SCFG_CLK_CAMERA;
    if (!wasOn) {
        OS_SpinWaitSysCycles(0x14);
    }
    data_02732fc8.clockWasOn = wasOn >> 8;
    if (PXI_SendWordByFifo(PXI_FIFO_TAG_CAMERA, word, FALSE) < 0) {
        if (!data_02732fc8.clockWasOn) {
            enabled = OS_DisableInterrupts();
            reg_SCFG_CLK = reg_SCFG_CLK & ~SCFG_CLK_CAMERA;
            OS_RestoreInterrupts(enabled);
        }
        return FALSE;
    }
    return TRUE;
}

// Sends the next three bytes of a command
static void func_02701a60(const u8 *data) {
    u32 word = (data[0] << 16) | (data[1] << 8) | data[2];

    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CAMERA, word, FALSE) < 0) {
    }
}

// BUG: the stores for context B of a command with a context are outside the test of the camera, so a command for the
// outer camera also sets context B of the inner camera and the other way round. The cached settings of the camera
// that was not addressed are then wrong, and the thread sends them when it starts the cameras again.
#ifdef BUGFIX
#define SENT_CONTEXT_B(camera) ((data_02732fc8.sentCamera & (camera)) && (data_02732fc8.sentContext & CAMERA_CONTEXT_B))
#else
#define SENT_CONTEXT_B(camera) (data_02732fc8.sentContext & CAMERA_CONTEXT_B)
#endif

// The camera interface's PXI callback: puts together an answer from its words and, with the whole answer, updates
// what the settings are
static void func_02701aa0(int tag, u32 data, BOOL err) {
    CAMERAResult result;

    if (err) {
        func_02701fe8(CAMERA_RESULT_FATAL_ERROR);
        return;
    }
    if (data & CAMERA_PXI_START) {
        data_02732fc8.length = (data & 0xff0000) >> 16;
        data_02732fc8.received = 0;
        data_02732fc8.command = (data & 0x7f00) >> 8;
        data_02732fc8.response = data & 0xff;
    } else {
        if (data_02732fc8.buffer == NULL) {
            func_02701fe8(CAMERA_RESULT_FATAL_ERROR);
            return;
        }
        if (data_02732fc8.received < data_02732fc8.bufferSize) {
            data_02732fc8.buffer[data_02732fc8.received++] = (data & 0xff0000) >> 16;
        }
        if (data_02732fc8.received < data_02732fc8.bufferSize) {
            data_02732fc8.buffer[data_02732fc8.received++] = (data & 0xff00) >> 8;
        }
        if (data_02732fc8.received < data_02732fc8.bufferSize) {
            data_02732fc8.buffer[data_02732fc8.received++] = data;
        }
    }
    if (data_02732fc8.received < data_02732fc8.length - 1) {
        return;
    }
    if (data_02732fc8.stopClock || (!data_02732fc8.keepClock && !data_02732fc8.clockWasOn)) {
        u32 enabled = OS_DisableInterrupts();

        reg_SCFG_CLK = reg_SCFG_CLK & ~SCFG_CLK_CAMERA;
        OS_RestoreInterrupts(enabled);
    }
    data_02732fc8.keepClock = FALSE;
    data_02732fc8.stopClock = FALSE;
    result = CAMERA_RESULT_SUCCESS;
    switch (data_02732fc8.response) {
    case CAMERA_PXI_RESULT_SUCCESS:
        switch (data_02732fc8.command) {
        case CAMERA_COMMAND_I2C_ACTIVATE:
            data_02732fc8.settings[1].unk_38 = data_02732fc8.sentValue;
            break;
        case CAMERA_COMMAND_I2C_CONTEXT_SWITCH:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].context = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].context = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_SIZE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[0].size[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_IN)) {
                data_02732fc8.settings[0].size[1] = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[1].size[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_OUT)) {
                data_02732fc8.settings[1].size[1] = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_FRAME_RATE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].frameRate = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].frameRate = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_EFFECT:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[0].effect[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_IN)) {
                data_02732fc8.settings[0].effect[1] = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[1].effect[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_OUT)) {
                data_02732fc8.settings[1].effect[1] = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_FLIP:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[0].flip[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_IN)) {
                data_02732fc8.settings[0].flip[1] = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                if (data_02732fc8.sentContext & CAMERA_CONTEXT_A) {
                    data_02732fc8.settings[1].flip[0] = data_02732fc8.sentValue;
                }
            }
            if (SENT_CONTEXT_B(CAMERA_SELECT_OUT)) {
                data_02732fc8.settings[1].flip[1] = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_PHOTO_MODE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].photoMode = data_02732fc8.sentValue;
                switch (data_02732fc8.settings[0].photoMode) {
                case CAMERA_PHOTO_MODE_NORMAL:
                    data_02732fc8.settings[0].sharpness = 0;
                    data_02732fc8.settings[0].exposure = 0;
                    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_PORTRAIT:
                    data_02732fc8.settings[0].sharpness = -2;
                    data_02732fc8.settings[0].exposure = 0;
                    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_LANDSCAPE:
                    data_02732fc8.settings[0].sharpness = 1;
                    data_02732fc8.settings[0].exposure = 0;
                    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_5200K;
                    break;
                case CAMERA_PHOTO_MODE_NIGHTVIEW:
                    data_02732fc8.settings[0].sharpness = -1;
                    data_02732fc8.settings[0].exposure = 2;
                    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_LETTER:
                    data_02732fc8.settings[0].sharpness = 2;
                    data_02732fc8.settings[0].exposure = 2;
                    data_02732fc8.settings[0].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                }
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].photoMode = data_02732fc8.sentValue;
                switch (data_02732fc8.settings[1].photoMode) {
                case CAMERA_PHOTO_MODE_NORMAL:
                    data_02732fc8.settings[1].sharpness = 0;
                    data_02732fc8.settings[1].exposure = 0;
                    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_PORTRAIT:
                    data_02732fc8.settings[1].sharpness = -2;
                    data_02732fc8.settings[1].exposure = 0;
                    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_LANDSCAPE:
                    data_02732fc8.settings[1].sharpness = 1;
                    data_02732fc8.settings[1].exposure = 0;
                    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_5200K;
                    break;
                case CAMERA_PHOTO_MODE_NIGHTVIEW:
                    data_02732fc8.settings[1].sharpness = -1;
                    data_02732fc8.settings[1].exposure = 2;
                    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                case CAMERA_PHOTO_MODE_LETTER:
                    data_02732fc8.settings[1].sharpness = 2;
                    data_02732fc8.settings[1].exposure = 2;
                    data_02732fc8.settings[1].whiteBalance = CAMERA_WHITE_BALANCE_AUTO;
                    break;
                }
            }
            break;
        case CAMERA_COMMAND_I2C_WHITE_BALANCE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].whiteBalance = data_02732fc8.sentValue;
                data_02732fc8.settings[0].autoWhiteBalance = TRUE;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].whiteBalance = data_02732fc8.sentValue;
                data_02732fc8.settings[1].autoWhiteBalance = TRUE;
            }
            break;
        case CAMERA_COMMAND_I2C_EXPOSURE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].exposure = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].exposure = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_SHARPNESS:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].sharpness = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].sharpness = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_AUTO_EXPOSURE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].autoExposure = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].autoExposure = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_I2C_AUTO_WHITE_BALANCE:
            if (data_02732fc8.sentCamera & CAMERA_SELECT_IN) {
                data_02732fc8.settings[0].autoWhiteBalance = data_02732fc8.sentValue;
            }
            if (data_02732fc8.sentCamera & CAMERA_SELECT_OUT) {
                data_02732fc8.settings[1].autoWhiteBalance = data_02732fc8.sentValue;
            }
            break;
        case CAMERA_COMMAND_UNK_40:
            data_02732fc8.settings[1].unk_38 = data_02732fc8.sentValue;
            break;
        }
        break;
    case CAMERA_PXI_RESULT_SUCCESS_FALSE:
        result = CAMERA_RESULT_SUCCESS_FALSE;
        break;
    case CAMERA_PXI_RESULT_ILLEGAL_STATUS:
        result = CAMERA_RESULT_ILLEGAL_STATUS;
        break;
    case CAMERA_PXI_RESULT_BUSY:
    case CAMERA_PXI_RESULT_ILLEGAL_PARAMETER:
    case CAMERA_PXI_RESULT_FATAL_ERROR:
    default:
        result = CAMERA_RESULT_FATAL_ERROR;
        break;
    }
    func_02701fe8(result);
    if (result == CAMERA_RESULT_ILLEGAL_STATUS) {
        func_027001f0();
    }
}

// The synchronous functions' callback
static void func_02701fd8(CAMERAResult result, void *arg) {
    data_02732fc8.result = result;
}

// Ends the command that is on its way: calls its callback with the result
static void func_02701fe8(CAMERAResult result) {
    CAMERACallback callback;

    if (data_02732fc8.busy) {
        data_02732fc8.busy = FALSE;
    }
    callback = data_02732fc8.callback;
    if (callback == NULL) {
        return;
    }
    data_02732fc8.callback = NULL;
    callback(result, data_02732fc8.callbackArg);
}

// Waits for the command to end, by reading the PXI FIFO itself when the interrupts are off
static void func_02702028(void) {
    CAMERAWork *work = &data_02732fc8;

    while (work->busy) {
        if (OS_GetCpsrIrq() == 0x80 || reg_OS_IME == 0) {
            PXIi_HandlerRecvFifoNotEmpty();
        }
    }
}

// The PM callback, for the system sleeping and exiting: deactivates the cameras
static void func_02702070(void *arg) {
    CAMERA_I2CActivateCore(CAMERA_SELECT_NONE);
}
