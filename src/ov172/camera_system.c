// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov257/camera_system.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/comm_tvt/camera_system.h"
#include "types.h"
#include "gfl/arc.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/ui.h"
#include "nitro/fs.h"
#include "nitro/os.h"
#include "twl/camera.h"
#include "twl/dsp.h"
#include "twl/mi.h"

// The DSi camera for the Xtransceiver: it captures frames from one of the two cameras into a ring of buffers by NDMA,
// and plays the shutter and video sounds that a DSi must play while it records, through the DSP. The camera's
// interrupt callbacks take no work, so the system is kept in data_ov172_021c88e0. Built on TwlSDK's camera demos

// The NDMA channel that receives the frames
#define CAMERA_NDMA_NO 1

// The sounds in the archive
#define ARC_CAMERA_SOUND 212 // Black's
// Black's: Black's archive has no shutter sound
enum {
    CAMERA_SOUND_VIDEO_START,
    CAMERA_SOUND_VIDEO_END,
};

#define CAMERA_SETTLE_FRAMES 4
// The frame counter stops here, and a frame is only passed on once the counter is past CAMERA_SETTLE_FRAMES
#define CAMERA_FRAME_COUNT_MAX 30

// HID flags of the camera, which keep the game from sleeping
#define CAMERA_HID_FLAG 0x40

// The sounds a DSi plays while its camera records, which func_ov172_021b64ac plays one step at a time
enum {
    CAMERA_SOUND_STATE_NONE,
    CAMERA_SOUND_STATE_VIDEO_START,
    CAMERA_SOUND_STATE_VIDEO_START_WAIT,
    CAMERA_SOUND_STATE_RECORDING,
    CAMERA_SOUND_STATE_VIDEO_END,
    CAMERA_SOUND_STATE_VIDEO_END_WAIT,
    CAMERA_SOUND_STATE_ENDED,
    CAMERA_SOUND_STATE_SHUTTER,
    CAMERA_SOUND_STATE_SHUTTER_WAIT,
};

struct CameraSystem {
    HeapID heapId;
    void **buffers;
    u8 numBuffers;
    BOOL buffersAllocated;
    BOOL capturing;
    BOOL unk14;
    u8 bufferIndex;
    BOOL startCapture;
    BOOL switchCamera;
    // Set after a buffer error, to drop the frame that the restart cut short
    BOOL skipFrame;
    u32 frameCount;
    CAMERASelect camera;
    CAMERASize size;
    u16 trimLeft;
    u16 trimRight;
    u16 trimTop;
    u16 trimBottom;
    CameraFrameCallback callback;
    void *callbackWork;
    u8 soundState;
    void *soundData;
    u32 soundSize;
};

void func_ov172_021b6338(void *work);
void func_ov172_021b644c(CameraSystem *sys);
static void func_ov172_021b65c4(CameraSystem *sys);
void func_ov172_021b65d8(CAMERAResult result);
void func_ov172_021b6620(CAMERAResult result);
void func_ov172_021b662c(CAMERAResult result);
void func_ov172_021b66b0(void *arg);
static void func_ov172_021b67bc(CameraSystem *sys);
static void func_ov172_021b68a0(CameraSystem *sys);
static u16 func_ov172_021b6938(CameraSystem *sys);
static u16 func_ov172_021b6944(CameraSystem *sys);
static u16 func_ov172_021b697c(CameraSystem *sys);
static u16 func_ov172_021b699c(CameraSystem *sys);

// pokebw2's sCameraSystem
static CameraSystem *data_ov172_021c88e0;

// pokebw2's CameraSystem_StartDma

// TwlSDK's camera and DSP functions are in the TWL-only area, out of reach of a branch: the linker adds a veneer
// after each calling function
void MI_StopNDma(u32 arg);


int func_02730fbc(MIWram wram, MIWramProc proc);
CameraSystem *func_ov172_021b6100(HeapID heapId) {
    FSFile file;
    CAMERAResult result;
    CameraSystem *sys = GFL_HeapAllocate(heapId, sizeof(CameraSystem), FALSE, "camera_system.c", 111);

    data_ov172_021c88e0 = sys;
    sys->heapId = heapId;
    sys->buffers = NULL;
    sys->bufferIndex = 0;
    sys->numBuffers = 0;
    sys->buffersAllocated = FALSE;
    sys->capturing = FALSE;
    sys->unk14 = FALSE;
    sys->camera = CAMERA_SELECT_IN;
    sys->size = CAMERA_SIZE_DS_LCD;
    sys->startCapture = FALSE;
    sys->switchCamera = FALSE;
    sys->skipFrame = FALSE;
    sys->callback = NULL;
    sys->callbackWork = NULL;
    func_02034f84(CAMERA_HID_FLAG);

    result = CAMERA_Init();
    if (result == CAMERA_RESULT_FATAL_ERROR) {
        OS_Terminate();
    }
    if (result == CAMERA_RESULT_ILLEGAL_STATUS) {
        return NULL;
    }
    CAMERA_I2CEffectEx(sys->camera, CAMERA_CONTEXT_BOTH, CAMERA_EFFECT_NONE);
    CAMERA_I2CAutoExposure(sys->camera, TRUE);
    CAMERA_I2CAutoWhiteBalance(sys->camera, TRUE);
    result = CAMERA_I2CActivate(sys->camera);
    if (result == CAMERA_RESULT_FATAL_ERROR) {
        OS_Terminate();
    }
    if (result == CAMERA_RESULT_ILLEGAL_STATUS) {
        return NULL;
    }

    sys->frameCount = 0;
    OS_EnableIrqMask(OS_IE_NDMA1);
    CAMERA_SetVsyncCallback(func_ov172_021b662c);
    CAMERA_SetBufferErrorCallback(func_ov172_021b65d8);
    CAMERA_SetRebootCallback(func_ov172_021b6620);
    func_ov172_021b68a0(sys);
    CAMERA_SetOutputFormat(CAMERA_OUTPUT_RGB);

    sys->soundData = NULL;
    sys->soundState = CAMERA_SOUND_STATE_NONE;
    MI_FreeWram(MI_WRAM_B, MI_WRAM_ARM9);
    func_02730fbc(MI_WRAM_B, MI_WRAM_ARM9);
    func_02730fbc(MI_WRAM_B, MI_WRAM_ARM7);
    MI_FreeWram(MI_WRAM_C, MI_WRAM_ARM9);
    func_02730fbc(MI_WRAM_C, MI_WRAM_ARM9);
    func_02730fbc(MI_WRAM_C, MI_WRAM_ARM7);
    DSP_OpenStaticComponentG711(&file);
    if (!DSP_LoadG711(&file, 0xff, 0xff)) {
        OS_Terminate();
    }
    GCTX_HIDSetSoftResetCallback(func_ov172_021b6338, sys);
    return sys;
}

void func_ov172_021b6338(void *work) {
    CameraSystem *sys = work;

    while (DSP_IsShutterSoundPlaying() == TRUE) {
        OS_Sleep(10);
    }
    if (OS_IsRunOnTwl()) {
        func_027072e0();
    }
    func_ov172_021b6868(sys);
    if (OS_IsRunOnTwl() == 1) {
        func_02700bd0();
    }
    MI_StopNDma(1);
    if (OS_IsRunOnTwl() == 1) {
        func_02700b34();
    }
}

void func_ov172_021b63b8(CameraSystem *sys) {
    GCTX_HIDSetSoftResetCallback(NULL, NULL);
    if (OS_IsRunOnTwl()) {
        func_027062c8();
    }
    if (OS_IsRunOnTwl()) {
        func_027072e0();
    }
    func_ov172_021b6868(sys);
    if (OS_IsRunOnTwl() == 1) {
        func_02700bd0();
    }
    MI_StopNDma(1);
    if (OS_IsRunOnTwl() == 1) {
        func_02700b34();
    }
    func_ov172_021b65c4(sys);
    func_ov172_021b67bc(sys);
    func_02034fb8(CAMERA_HID_FLAG);
    GFL_HeapFree(sys);
    data_ov172_021c88e0 = NULL;
}

void func_ov172_021b644c(CameraSystem *sys) {
    u16 width = func_ov172_021b6938(sys);
    u16 height = func_ov172_021b6944(sys);
    u32 unit;

    if (OS_IsRunOnTwl() == 1) {
        unit = func_0270234c(width);
    } else {
        unit = 0;
    }

    MI_Camera_NDmaRecvAsync(CAMERA_NDMA_NO, sys->buffers[sys->bufferIndex], unit / 4,
                        (width << 1) * height, FALSE, func_ov172_021b66b0, NULL);
}

BOOL func_02706174(const void *src, u32 length);
void func_ov172_021b657c(CameraSystem *sys, BOOL start);
// The DSP's calls, which only a DSi has
static inline void CameraSystem_PlayVideoStartSound(const void *src, u32 length) {
    if (OS_IsRunOnTwl()) {
        func_02706174(src, length);
    }
}

static inline void CameraSystem_PlayVideoEndSound(const void *src, u32 length) {
    if (OS_IsRunOnTwl()) {
        func_02706174(src, length);
    }
}

void func_ov172_021b64ac(CameraSystem *sys) {
    BOOL soundPlaying;

    switch (sys->soundState) {
    case CAMERA_SOUND_STATE_VIDEO_START:
        func_ov172_021b657c(sys, TRUE);
        CameraSystem_PlayVideoStartSound(sys->soundData, sys->soundSize);
        sys->soundState = CAMERA_SOUND_STATE_VIDEO_START_WAIT;
        break;
    case CAMERA_SOUND_STATE_VIDEO_START_WAIT:
        soundPlaying = FALSE;
        if (OS_IsRunOnTwl()) {
            soundPlaying = func_02706324();
        }
        if (soundPlaying == FALSE) {
            func_ov172_021b65c4(sys);
            sys->soundState = CAMERA_SOUND_STATE_RECORDING;
        }
        break;
    case CAMERA_SOUND_STATE_VIDEO_END:
        func_ov172_021b657c(sys, FALSE);
        CameraSystem_PlayVideoEndSound(sys->soundData, sys->soundSize);
        sys->soundState = CAMERA_SOUND_STATE_VIDEO_END_WAIT;
        break;
    case CAMERA_SOUND_STATE_VIDEO_END_WAIT:
        soundPlaying = FALSE;
        if (OS_IsRunOnTwl()) {
            soundPlaying = func_02706324();
        }
        if (soundPlaying == FALSE) {
            func_ov172_021b65c4(sys);
            sys->soundState = CAMERA_SOUND_STATE_ENDED;
        }
        break;
    case CAMERA_SOUND_STATE_ENDED:
        break;
    }
}

void func_ov172_021b657c(CameraSystem *sys, BOOL start) {
    u16 file = start == TRUE ? CAMERA_SOUND_VIDEO_START : CAMERA_SOUND_VIDEO_END;

    func_ov172_021b65c4(sys);
    sys->soundSize = GFL_ArcSysGetDataLength(ARC_CAMERA_SOUND, file);
    sys->soundData = allocConfigDSSoftwareFeature(sys->heapId, sys->soundSize, "camera_system.c", 315);
    GFL_ArcSysRead(sys->soundData, ARC_CAMERA_SOUND, file);
    DC_FlushRange(sys->soundData, sys->soundSize);
}

// pokebw2's CameraSystem_FreeSound
static void func_ov172_021b65c4(CameraSystem *sys) {
    if (sys->soundData != NULL) {
        func_0203d7fc(sys->soundData);
        sys->soundData = NULL;
    }
}

void CAMERA_StopCaptureCore(void);
void CAMERA_ClearBufferCore(void);
void func_ov172_021b65d8(CAMERAResult result) {
    CameraSystem *sys = data_ov172_021c88e0;

    if (OS_IsRunOnTwl() == 1)
        CAMERA_StopCaptureCore();

    MI_StopNDma(1);

    if (OS_IsRunOnTwl() == 1)
        CAMERA_ClearBufferCore();

    sys->skipFrame = TRUE;
    sys->startCapture = TRUE;
}

// pokebw2's CameraSystem_OnReboot
void func_ov172_021b6620(CAMERAResult result) {
    if (result != CAMERA_RESULT_FATAL_ERROR) {
        func_ov172_021b65d8(result);
    }
}

// pokebw2's CameraSystem_OnVsync
void func_02702108(void);
void func_ov172_021b662c(CAMERAResult result) {
    CameraSystem *sys = data_ov172_021c88e0;

    if (sys->capturing) {
        if (sys->frameCount <= CAMERA_FRAME_COUNT_MAX) {
            sys->frameCount++;
        }
        if (sys->switchCamera) {
            CAMERASelect cam = sys->camera;
            CAMERAResult res;
            if (OS_IsRunOnTwl() == 1) {
                res = CAMERA_I2CActivateCore(cam);
            } else {
                res = CAMERA_RESULT_FATAL_ERROR;
            }
            if (res == CAMERA_RESULT_FATAL_ERROR) {
                OS_Terminate();
            }
            sys->frameCount = 0;
            sys->switchCamera = FALSE;
        }
        if (sys->startCapture) {
            func_ov172_021b644c(sys);
            if (OS_IsRunOnTwl() == 1) {
                CAMERA_ClearBufferCore();
            }
            if (OS_IsRunOnTwl() == 1) {
                func_02702108();
            }
            sys->startCapture = FALSE;
        }
    }
}

BOOL CAMERA_IsBusyCore(void);
BOOL MI_IsNDmaBusy(u32 ndmaNo);
void func_ov172_021b66b0(void *arg) {
    CameraSystem *sys = data_ov172_021c88e0;
    BOOL ok;

    MI_StopNDma(CAMERA_NDMA_NO);
    if (OS_IsRunOnTwl() == 1) {
        ok = CAMERA_IsBusyCore();
    } else {
        ok = FALSE;
    }
    if (ok != TRUE) {
        return;
    }
    if (MI_IsNDmaBusy(CAMERA_NDMA_NO)) {
        MI_StopNDma(CAMERA_NDMA_NO);
    }
    if (sys->skipFrame) {
        sys->skipFrame = FALSE;
    } else if (sys->frameCount > CAMERA_SETTLE_FRAMES) {
        if (sys->callback != NULL) {
            sys->callback(sys->buffers[sys->bufferIndex], sys->callbackWork);
        }
        sys->bufferIndex = (sys->bufferIndex + 1) % sys->numBuffers;
    }
    if (sys->capturing == TRUE) {
        func_ov172_021b644c(sys);
    }
}

// pokebw2's CameraSystem_SetFrameCallback
void func_ov172_021b6744(CameraSystem *sys, CameraFrameCallback callback, void *work) {
    sys->callback = callback;
    sys->callbackWork = work;
}

// pokebw2's CameraSystem_AllocBuffers
void func_ov172_021b674c(CameraSystem *sys, int count, HeapID heapId) {
    u16 width = func_ov172_021b6938(sys);
    u16 height = func_ov172_021b6944(sys);
    u8 i;

    if (sys->buffers == NULL) {
        sys->buffers = GFL_HeapAllocate(sys->heapId, count * sizeof(void *), FALSE, "camera_system.c", 471);
        sys->numBuffers = count;
        for (i = 0; i < count; i++) {
            sys->buffers[i] =
                allocConfigDSSoftwareFeature(heapId, CAMERA_GET_FRAME_BYTES(width, height), "camera_system.c", 476);
        }
        sys->buffersAllocated = TRUE;
    }
}

// pokebw2's CameraSystem_FreeBuffers
static void func_ov172_021b67bc(CameraSystem *sys) {
    u8 i;
    BOOL result;

    if (sys->capturing == FALSE) {
        if (OS_IsRunOnTwl() == TRUE)
            result = CAMERA_IsBusyCore();
        else
            result = FALSE;
        if (result)
            return;
        if (sys->buffersAllocated == TRUE) {
            for (i = 0; i < sys->numBuffers; i++) {
                func_0203d7fc(sys->buffers[i]);
            }
        }
        GFL_HeapFree(sys->buffers);
        sys->numBuffers = 0;
        sys->buffers = NULL;
    }
}

void func_ov172_021b6814(CameraSystem *sys) {
    if (sys->capturing == FALSE) {
        BOOL result;
        if (OS_IsRunOnTwl() == TRUE) {
            result = CAMERA_IsBusyCore();
        } else {
            result = FALSE;
        }
        if (result == FALSE) {
            if (MI_IsNDmaBusy(TRUE) == TRUE) {
                MI_StopNDma(TRUE);
            }
            sys->capturing = TRUE;
            sys->startCapture = TRUE;
            sys->frameCount = 0;
        }
    }
}

// pokebw2's CameraSystem_Stop
void func_ov172_021b6868(CameraSystem *sys) {
    if (sys->capturing == TRUE) {
        sys->capturing = FALSE;
        sys->startCapture = FALSE;
        if (OS_IsRunOnTwl() == TRUE) {
            CAMERA_StopCaptureCore();
        }
    }
}

// pokebw2's CameraSystem_IsSoundDone
BOOL func_ov172_021b688c(CameraSystem *sys) {
    if (sys->soundState == CAMERA_SOUND_STATE_ENDED || sys->soundState == CAMERA_SOUND_STATE_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's CameraSystem_SetupCapture
static void func_ov172_021b68a0(CameraSystem *sys) {
    int lines;
    u16 width, height;

    if (sys->capturing == FALSE) {
        u32 result;
        if (OS_IsRunOnTwl() == 1)
            result = CAMERA_IsBusyCore();
        else
            result = 0;
        if (result != 0)
            return;
        if (OS_IsRunOnTwl() == 1)
            func_02702158(0);
        lines = 0;
        sys->trimLeft = 0;
        sys->trimRight = func_ov172_021b697c(sys);
        sys->trimTop = 0;
        sys->trimBottom = func_ov172_021b699c(sys);
        width = func_ov172_021b6938(sys);
        height = func_ov172_021b6944(sys);
        if (OS_IsRunOnTwl() == 1)
            lines = func_027022f8(width, height);
        if (OS_IsRunOnTwl() == 1)
            func_0270229c(lines);
        sys->unk14 = FALSE;
    }
}

// pokebw2's CameraSystem_GetWidth
static u16 func_ov172_021b6938(CameraSystem *sys) {
    return sys->trimRight - sys->trimLeft;
}

// pokebw2's CameraSystem_GetHeight
static u16 func_ov172_021b6944(CameraSystem *sys) {
    return sys->trimBottom - sys->trimTop;
}

// pokebw2's CameraSystem_SetSize
void func_ov172_021b6950(CameraSystem *sys, CAMERASize size) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02700fe4(CAMERA_SELECT_BOTH, CAMERA_CONTEXT_BOTH, size);
    }
    sys->size = size;
    func_ov172_021b68a0(sys);
}

// pokebw2's CameraSystem_SizeToWidth
static u16 func_ov172_021b697c(CameraSystem *sys) {
    CAMERASize size = sys->size;
    if (OS_IsRunOnTwl() == 1) {
        return func_02702364(size);
    }
    return 0;
}

// pokebw2's CameraSystem_SizeToHeight
static u16 func_ov172_021b699c(CameraSystem *sys) {
    CAMERASize size = sys->size;
    if (OS_IsRunOnTwl() == 1) {
        return func_027023cc(size);
    }
    return 0;
}

// pokebw2's CameraSystem_SwitchCamera
void func_ov172_021b69bc(CameraSystem *sys, CAMERASelect camera) {
    if (sys->camera != camera && sys->switchCamera == FALSE) {
        sys->camera = camera;
        sys->switchCamera = TRUE;
    }
}

// pokebw2's CameraSystem_PlayVideoStartSound
void func_ov172_021b69d0(CameraSystem *sys) {
    sys->soundState = CAMERA_SOUND_STATE_VIDEO_START;
}

// pokebw2's CameraSystem_PlayVideoEndSound
void func_ov172_021b69d8(CameraSystem *sys) {
    sys->soundState = CAMERA_SOUND_STATE_VIDEO_END;
}

