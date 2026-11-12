// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/camera.h, with Black's names.
#ifndef BWPORT_TWL_CAMERA_H
#define BWPORT_TWL_CAMERA_H

#include "types.h"
#include "nitro/os.h"
#include "nitro/spi/ARM9/pm.h"
#include "twl/mi.h"

// TwlSDK's camera library, in the DSi's LTD autoload. The game calls it through the SDK's inline functions, which only
// call the library when the game runs on a DSi (OS_IsRunOnTwl, swan's hw_isDSi), so the check is inlined at every call.
// The functions have the names of the SDK functions they implement, from how the Xtransceiver uses them.

typedef enum {
    CAMERA_RESULT_SUCCESS = 0,
    CAMERA_RESULT_SUCCESS_TRUE = 0,
    CAMERA_RESULT_SUCCESS_FALSE,
    CAMERA_RESULT_BUSY,
    CAMERA_RESULT_ILLEGAL_PARAMETER,
    CAMERA_RESULT_SEND_ERROR,
    CAMERA_RESULT_INVALID_COMMAND,
    CAMERA_RESULT_ILLEGAL_STATUS,
    CAMERA_RESULT_FATAL_ERROR,
} CAMERAResult;

typedef enum {
    CAMERA_SELECT_NONE,
    CAMERA_SELECT_IN,
    CAMERA_SELECT_OUT,
    CAMERA_SELECT_BOTH,
} CAMERASelect;

typedef enum {
    CAMERA_CONTEXT_A = 1,
    CAMERA_CONTEXT_B,
    CAMERA_CONTEXT_BOTH,
} CAMERAContext;

typedef enum {
    CAMERA_SIZE_DSI_VGA,  // 640 x 480
    CAMERA_SIZE_QVGA,     // 320 x 240
    CAMERA_SIZE_QQVGA,    // 160 x 120
    CAMERA_SIZE_CIF,      // 352 x 288
    CAMERA_SIZE_QCIF,     // 176 x 144
    CAMERA_SIZE_DS_LCD,   // 256 x 192
    CAMERA_SIZE_DS_LCDX4, // 512 x 384
} CAMERASize;

typedef enum {
    CAMERA_EFFECT_NONE,
    CAMERA_EFFECT_MONO,
    CAMERA_EFFECT_SEPIA,
    CAMERA_EFFECT_NEGATIVE,
    CAMERA_EFFECT_NEGAFILM,
    CAMERA_EFFECT_SEPIA01,
} CAMERAEffect;

typedef enum {
    CAMERA_FLIP_NONE,
    CAMERA_FLIP_HORIZONTAL,
    CAMERA_FLIP_VERTICAL,
    CAMERA_FLIP_REVERSE,
} CAMERAFlip;

// The color temperatures of the white balance's presets; AUTO is what the sensor sets by itself
typedef enum {
    CAMERA_WHITE_BALANCE_AUTO,
    CAMERA_WHITE_BALANCE_3200K,
    CAMERA_WHITE_BALANCE_4150K,
    CAMERA_WHITE_BALANCE_5200K,
    CAMERA_WHITE_BALANCE_6000K,
    CAMERA_WHITE_BALANCE_7000K,
} CAMERAWhiteBalance;

typedef enum {
    CAMERA_OUTPUT_YUV,
    CAMERA_OUTPUT_RGB,
} CAMERAOutput;

// The camera's modes for a scene, by what camera_api.c sets the white balance, exposure and sharpness to for each (the
// names are from the DSi's camera settings)
typedef enum {
    CAMERA_PHOTO_MODE_NORMAL,
    CAMERA_PHOTO_MODE_PORTRAIT,
    CAMERA_PHOTO_MODE_LANDSCAPE,
    CAMERA_PHOTO_MODE_NIGHTVIEW,
    CAMERA_PHOTO_MODE_LETTER,
} CAMERAPhotoMode;

typedef void (*CAMERAIntrCallback)(CAMERAResult result);
// What the asynchronous I2C commands call when the ARM7 has answered
typedef void (*CAMERACallback)(CAMERAResult result, void *arg);

CAMERAResult func_02700938(void);
void func_02700b34(void);
CAMERAResult func_02700bd0(void); // puts both cameras to standby, called before CAMERA_End
CAMERAResult func_02700cb4(CAMERASelect camera);
CAMERAResult CAMERA_I2CActivateCore(CAMERASelect camera);
CAMERAResult func_02700fe4(CAMERASelect camera, CAMERAContext context, CAMERASize size);
CAMERAResult func_02701248(CAMERASelect camera, CAMERAContext context, CAMERAEffect effect);
// func_027017dc and func_02701908, though which is which is a guess
CAMERAResult func_027017dc(CAMERASelect camera, BOOL on);
CAMERAResult func_02701908(CAMERASelect camera, BOOL on);
void func_027000f8(CAMERAIntrCallback callback);
void func_02700108(CAMERAIntrCallback callback);
void func_02700118(CAMERAIntrCallback callback);
BOOL CAMERA_IsBusyCore(void);
void func_02702108(void);
void CAMERA_StopCaptureCore(void);
void func_02702158(BOOL enabled);
void func_0270218c(CAMERAOutput output);
void CAMERA_ClearBufferCore(void);
void func_0270229c(int lines);
int func_027022f8(u16 width, u16 height);
u32 func_0270234c(u16 width);
u16 func_02702364(CAMERASize size);
u16 func_027023cc(CAMERASize size);

// camera.c's other functions on the camera interface's registers (SCFG_CLK and the control word at 0x04004202), which
// camera_api.c calls while it starts the camera, stops it and handles its interrupt. The three named ones have guesses
// in the SDK's style, not its names
void CAMERA_ResetCore(void);                 // resets the interface and its camera clock
BOOL CAMERA_IsBufferErrorCore(void);         // whether the interface reports a buffer error (bit 4 of its control)
void CAMERA_SetInterruptEnableCore(BOOL on); // sets or clears the interface's interrupt enable (bit 11 of its control)
void func_0270223c(u32 bits);                // replaces bits 8 and 9 of the interface's control
void func_02702268(BOOL on);                 // sets or clears bit 10 of the interface's control

static inline CAMERAResult CAMERA_Init(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        CAMERAResult result = func_02700938();
        if (result == CAMERA_RESULT_SUCCESS) {
            result = func_02700cb4(CAMERA_SELECT_BOTH);
        }
        return result;
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline void CAMERA_End(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02700b34();
    }
}

// Not an SDK name: what the SDK calls the function is unknown
static inline CAMERAResult CAMERA_I2CStop(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_02700bd0();
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline CAMERAResult CAMERA_I2CActivate(CAMERASelect camera) {
    if (OS_IsRunOnTwl() == TRUE) {
        return CAMERA_I2CActivateCore(camera);
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline CAMERAResult CAMERA_I2CSizeEx(CAMERASelect camera, CAMERAContext context, CAMERASize size) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_02700fe4(camera, context, size);
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline CAMERAResult CAMERA_I2CEffectEx(CAMERASelect camera, CAMERAContext context, CAMERAEffect effect) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_02701248(camera, context, effect);
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline CAMERAResult CAMERA_I2CAutoExposure(CAMERASelect camera, BOOL on) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_027017dc(camera, on);
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline CAMERAResult CAMERA_I2CAutoWhiteBalance(CAMERASelect camera, BOOL on) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_02701908(camera, on);
    }
    return CAMERA_RESULT_FATAL_ERROR;
}

static inline void CAMERA_SetVsyncCallback(CAMERAIntrCallback callback) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_027000f8(callback);
    }
}

static inline void CAMERA_SetBufferErrorCallback(CAMERAIntrCallback callback) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02700108(callback);
    }
}

static inline void CAMERA_SetRebootCallback(CAMERAIntrCallback callback) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02700118(callback);
    }
}

static inline BOOL CAMERA_IsBusy(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        return CAMERA_IsBusyCore();
    }
    return FALSE;
}

static inline void CAMERA_StartCapture(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02702108();
    }
}

static inline void CAMERA_StopCapture(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        CAMERA_StopCaptureCore();
    }
}

static inline void CAMERA_SetTrimming(BOOL enabled) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_02702158(enabled);
    }
}

static inline void CAMERA_SetOutputFormat(CAMERAOutput output) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_0270218c(output);
    }
}

static inline void CAMERA_ClearBuffer(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        CAMERA_ClearBufferCore();
    }
}

static inline BOOL CAMERA_IsBufferError(void) {
    if (OS_IsRunOnTwl() == TRUE) {
        return CAMERA_IsBufferErrorCore();
    }
    return FALSE;
}

static inline void CAMERA_SetTransferLines(int lines) {
    if (OS_IsRunOnTwl() == TRUE) {
        func_0270229c(lines);
    }
}

static inline int CAMERA_GetMaxLinesRound(u16 width, u16 height) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_027022f8(width, height);
    }
    return 0;
}

static inline u32 CAMERA_GetBytesAtOnce(u16 width) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_0270234c(width);
    }
    return 0;
}

static inline u16 CAMERA_SizeToWidth(CAMERASize size) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_02702364(size);
    }
    return 0;
}

static inline u16 CAMERA_SizeToHeight(CAMERASize size) {
    if (OS_IsRunOnTwl() == TRUE) {
        return func_027023cc(size);
    }
    return 0;
}

#define CAMERA_GET_FRAME_BYTES(width, height) ((width) * 2 * (height))


#endif // BWPORT_TWL_CAMERA_H
