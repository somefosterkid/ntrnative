#include "global.h"

#include "system.h"
#include "unk_020210A0.h"
#include "touch_pad_data_buffer.h"

#define MAX_TOUCH_BUFFER_SIZE 8

enum TouchPadExternalBufferWriteMethod {
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING = 1,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING,
    TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITH_WRAPPING,
};

enum AutoSamplingOperationResult {
    AUTO_SAMPLING_OPERATION_RESULT_BUFFER_INDEX_OUT_OF_RANGE = -1,
    AUTO_SAMPLING_OPERATION_RESULT_SUCCESS = 1,
    AUTO_SAMPLING_OPERATION_RESULT_FAILURE,
    AUTO_SAMPLING_OPERATION_RESULT_NO_DATA_WRITTEN,
};

#define AUTO_SAMPLING_NUM_FRAMES_TO_STORE    2
#define AUTO_SAMPLING_BUFFER_MAX_SIZE        (SPI_TP_SAMPLING_FREQUENCY_MAX * AUTO_SAMPLING_NUM_FRAMES_TO_STORE) + 1 // + 1 for the active sample
#define AUTO_SAMPLING_FREQUENCY_LIMIT        SPI_TP_SAMPLING_FREQUENCY_MAX + 1
#define AUTO_SAMPLING_NUM_ERRORS_BEFORE_FAIL 5

enum TouchPadMode {
    TOUCH_PAD_MODE_INACTIVE = 0,
    TOUCH_PAD_MODE_ACTIVE_WITH_EXTERNAL_BUFFER,
    TOUCH_PAD_MODE_ACTIVE,
};

typedef struct {
    TPData *externalBuffer;
    u32 externalBufferSize;
    u32 autoSamplingBufferSize;
    TPData autoSamplingBuffer[AUTO_SAMPLING_BUFFER_MAX_SIZE];
    u32 currentExternalBufferIndex;
    u16 touchPadMode;
    u16 touchPadDisabled;
} TouchPadState;

BOOL InitializeTouchPadWithExternalBuffer(TPData *externalBuffer, u32 externalBufferSize, u32 autoSamplingFrequency);
void sub_020211AC(enum TouchPadMode touchPadMode, BOOL autoSamplingEnabled, void *externalBuffer, u32 externalBufferSize, u32 currentExternalBufferIndex, u32 autoSamplingBufferSize);
void sub_020211E0(TPData *touchPadDataBuffer, int touchPadDataBufferSize);
static enum AutoSamplingOperationResult GF_TouchpadStopAutoSampling(void);
u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold);
void sub_020212EC(TPData *touchPadDataBuffer, u32 touchPadDataBufferSize);
static enum AutoSamplingOperationResult GF_TouchpadStartAutoSampling(u32 frequency);
u32 sub_020213C8(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex, u32 changeThreshold);
u32 sub_020213F8(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex, u32 changeThreshold);
u32 sub_020214B0(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex);
void sub_02021528(TouchPadDataBuffer *outBuffer);
void sub_02021540(TouchPadDataBuffer *outBuffer, u32 lastAutoSamplingIndex);

static TouchPadState _021D2198;

void GF_TouchpadInit(void) {
    memset(&_021D2198, 0, sizeof(_021D2198));
}

void EnableTouchPad(void) {
    GF_ASSERT(_021D2198.touchPadMode == TOUCH_PAD_MODE_INACTIVE);
    _021D2198.touchPadMode = TOUCH_PAD_MODE_INACTIVE;
}

BOOL InitializeTouchPadWithExternalBuffer(TPData *externalBuffer, u32 externalBufferSize, u32 autoSamplingFrequency) {
    enum AutoSamplingOperationResult result;
    if (_021D2198.touchPadDisabled) {
        sub_020211AC(TOUCH_PAD_MODE_ACTIVE_WITH_EXTERNAL_BUFFER, TRUE, externalBuffer, externalBufferSize, 0, autoSamplingFrequency * AUTO_SAMPLING_NUM_FRAMES_TO_STORE);
        return TRUE;
    }
    if (autoSamplingFrequency >= AUTO_SAMPLING_FREQUENCY_LIMIT || autoSamplingFrequency == 0) {
        return FALSE;
    }
    if (_021D2198.touchPadMode != TOUCH_PAD_MODE_INACTIVE) {
        return FALSE;
    }
    result = GF_TouchpadStartAutoSampling(autoSamplingFrequency);
    if (result == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS) {
        sub_020211AC(TOUCH_PAD_MODE_ACTIVE_WITH_EXTERNAL_BUFFER, TRUE, externalBuffer, externalBufferSize, 0, autoSamplingFrequency * AUTO_SAMPLING_NUM_FRAMES_TO_STORE);
        result = AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
    }
    return result;
}

BOOL InitializeTouchPad(int autoSamplingFrequency) {
    enum AutoSamplingOperationResult result;
    if (_021D2198.touchPadDisabled) {
        sub_020211AC(TOUCH_PAD_MODE_ACTIVE, TRUE, NULL, 0, 0, autoSamplingFrequency * AUTO_SAMPLING_NUM_FRAMES_TO_STORE);
        return TRUE;
    }
    if ((u32)autoSamplingFrequency >= AUTO_SAMPLING_FREQUENCY_LIMIT || autoSamplingFrequency == 0) {
        return FALSE;
    }
    if (_021D2198.touchPadMode != TOUCH_PAD_MODE_INACTIVE) {
        return FALSE;
    }
    result = GF_TouchpadStartAutoSampling(autoSamplingFrequency);
    if (result == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS) {
        sub_020211AC(TOUCH_PAD_MODE_ACTIVE, TRUE, NULL, 0, 0, autoSamplingFrequency * AUTO_SAMPLING_NUM_FRAMES_TO_STORE);
        result = AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
    }
    return result;
}

void sub_020211AC(enum TouchPadMode touchPadMode, BOOL autoSamplingEnabled, void *externalBuffer, u32 externalBufferSize, u32 currentExternalBufferIndex, u32 autoSamplingBufferSize) {
    _021D2198.touchPadMode = touchPadMode;
    gSystem.touchpadReadAuto = autoSamplingEnabled;
    _021D2198.externalBuffer = externalBuffer;
    _021D2198.externalBufferSize = externalBufferSize;
    _021D2198.autoSamplingBufferSize = autoSamplingBufferSize;
    _021D2198.currentExternalBufferIndex = currentExternalBufferIndex;

    sub_020211E0(_021D2198.autoSamplingBuffer, AUTO_SAMPLING_BUFFER_MAX_SIZE);
}

void sub_020211E0(TPData *touchPadDataBuffer, int touchPadDataBufferSize) {
    int i;

    for (i = 0; i < touchPadDataBufferSize; i++) {
        touchPadDataBuffer[i].touch = TP_TOUCH_OFF;
    }
}

static enum AutoSamplingOperationResult GF_TouchpadStopAutoSampling(void) {
    u32 errorCount = 0;
    BOOL hasError;

    if (_021D2198.touchPadMode == TOUCH_PAD_MODE_INACTIVE) {
        return AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
    }

    do {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_OFF);

        hasError = TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_OFF);

        if (hasError != FALSE) {
            errorCount++;
        }
    } while ((hasError != FALSE) && (errorCount <= AUTO_SAMPLING_NUM_ERRORS_BEFORE_FAIL));

    if (errorCount > AUTO_SAMPLING_NUM_ERRORS_BEFORE_FAIL) {
        return AUTO_SAMPLING_OPERATION_RESULT_FAILURE;
    }

    return AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
}

BOOL DisableTouchPad(void) {
    enum AutoSamplingOperationResult result;
    if (_021D2198.touchPadDisabled) {
        sub_020211AC(TOUCH_PAD_MODE_INACTIVE, FALSE, NULL, 0, 0, 0);
        return TRUE;
    }
    result = GF_TouchpadStopAutoSampling();
    if (result == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS) {
        sub_020211AC(TOUCH_PAD_MODE_INACTIVE, FALSE, NULL, 0, 0, 0);
    }
    return result;
}

u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold) {
    u32 result = AUTO_SAMPLING_OPERATION_RESULT_NO_DATA_WRITTEN;
    u16 latestIndex;
    if (_021D2198.touchPadDisabled) {
        if (outBuffer != NULL) {
            sub_02021528(outBuffer);
        }
        if (externalBufferWriteMethod == TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_NO_WRITE) {
            return AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
        }
        return 0;
    }
    if (_021D2198.touchPadMode != TOUCH_PAD_MODE_INACTIVE) {
        latestIndex = TP_GetLatestIndexInAuto();
        sub_020212EC(_021D2198.autoSamplingBuffer, AUTO_SAMPLING_BUFFER_MAX_SIZE);
        if (outBuffer != NULL) {
            sub_02021540(outBuffer, latestIndex);
        }
        result = (_021D2198.touchPadMode == TOUCH_PAD_MODE_ACTIVE_WITH_EXTERNAL_BUFFER) ? sub_020213C8(externalBufferWriteMethod, latestIndex, changeThreshold) : AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
    }
    return result;
}

void sub_020212EC(TPData *touchPadDataBuffer, u32 touchPadDataBufferSize) {
    int i;
    TPData convertedData;

    for (i = 0; i < touchPadDataBufferSize; i++) {
        TP_GetCalibratedPoint(&convertedData, &(touchPadDataBuffer[i]));
        touchPadDataBuffer[i] = convertedData;
    }
}

void GF_TouchpadResumeOnLidOpen(void) {
    enum AutoSamplingOperationResult autoSamplingResult;

    if (_021D2198.touchPadDisabled == FALSE || _021D2198.touchPadMode == TOUCH_PAD_MODE_INACTIVE) {
        return;
    }

    autoSamplingResult = GF_TouchpadStartAutoSampling(_021D2198.autoSamplingBufferSize / AUTO_SAMPLING_NUM_FRAMES_TO_STORE);
    GF_ASSERT(autoSamplingResult == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS);

    _021D2198.touchPadDisabled = FALSE;
}

void GF_TouchpadPauseOnLidClose(void) {
    u32 autoSamplingResult;

    if (_021D2198.touchPadDisabled == TRUE || _021D2198.touchPadMode == TOUCH_PAD_MODE_INACTIVE) {
        return;
    }

    autoSamplingResult = GF_TouchpadStopAutoSampling();
    GF_ASSERT(autoSamplingResult == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS);

    _021D2198.touchPadDisabled = TRUE;
}

static enum AutoSamplingOperationResult GF_TouchpadStartAutoSampling(u32 frequency) {
    int errorCount = 0;
    BOOL hasError;

    do {
        TP_RequestAutoSamplingStartAsync(0, frequency, _021D2198.autoSamplingBuffer, AUTO_SAMPLING_BUFFER_MAX_SIZE);
        TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_ON);

        hasError = TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_ON);

        if (hasError != FALSE) {
            errorCount++;
        }
    } while ((hasError != FALSE) && (errorCount <= AUTO_SAMPLING_NUM_ERRORS_BEFORE_FAIL));

    if (errorCount > AUTO_SAMPLING_NUM_ERRORS_BEFORE_FAIL) {
        return AUTO_SAMPLING_OPERATION_RESULT_FAILURE;
    }

    return AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
}

u32 sub_020213C8(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex, u32 changeThreshold) {
    u32 externalBufferEndIndex;

    switch (externalBufferWriteMethod) {
    case TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING:
    case TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITHOUT_WRAPPING:
        externalBufferEndIndex = sub_020213F8(externalBufferWriteMethod, latestAutoSamplingIndex, changeThreshold);
        break;
    case TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING:
    case TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITH_WRAPPING:
        externalBufferEndIndex = sub_020214B0(externalBufferWriteMethod, latestAutoSamplingIndex);
        break;
    default:
        externalBufferEndIndex = AUTO_SAMPLING_OPERATION_RESULT_SUCCESS;
        break;
    }

    return externalBufferEndIndex;
}

static inline int CalcAbsDifference(int int1, int int2) {
    return (int1 >= int2) ? (int1 - int2) : (int2 - int1);
}

u32 sub_020213F8(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex, u32 changeThreshold) {
    int i;
    s32 xDiff;
    s32 yDiff;
    s16 bufferIndex;

    for (i = 0; i < _021D2198.autoSamplingBufferSize; i++) {
        bufferIndex = latestAutoSamplingIndex - _021D2198.autoSamplingBufferSize + i + 1;

        if (bufferIndex < 0) {
            bufferIndex += AUTO_SAMPLING_BUFFER_MAX_SIZE;
        }

        if ((_021D2198.autoSamplingBuffer[bufferIndex].touch == TP_TOUCH_ON) && (_021D2198.autoSamplingBuffer[bufferIndex].validity == TP_VALIDITY_VALID)) {
            xDiff = CalcAbsDifference(_021D2198.externalBuffer[_021D2198.currentExternalBufferIndex - 1].x, _021D2198.autoSamplingBuffer[bufferIndex].x);
            yDiff = CalcAbsDifference(_021D2198.externalBuffer[_021D2198.currentExternalBufferIndex - 1].y, _021D2198.autoSamplingBuffer[bufferIndex].y);

            if ((xDiff >= changeThreshold) || (yDiff >= changeThreshold)) {
                _021D2198.externalBuffer[_021D2198.currentExternalBufferIndex] = _021D2198.autoSamplingBuffer[bufferIndex];
                _021D2198.currentExternalBufferIndex++;

                if (_021D2198.currentExternalBufferIndex >= _021D2198.externalBufferSize) {
                    if (externalBufferWriteMethod == TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ONLY_TOUCHES_WITH_WRAPPING) {
                        _021D2198.currentExternalBufferIndex %= _021D2198.externalBufferSize;
                    } else {
                        return AUTO_SAMPLING_OPERATION_RESULT_BUFFER_INDEX_OUT_OF_RANGE;
                    }
                }
            }
        }
    }

    return _021D2198.currentExternalBufferIndex;
}

u32 sub_020214B0(enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 latestAutoSamplingIndex) {
    int i;
    s16 bufferIndex;

    for (i = 0; i < _021D2198.autoSamplingBufferSize; i++) {
        bufferIndex = latestAutoSamplingIndex - _021D2198.autoSamplingBufferSize + i + 1;

        if (bufferIndex < 0) {
            bufferIndex += AUTO_SAMPLING_BUFFER_MAX_SIZE;
        }

        _021D2198.externalBuffer[_021D2198.currentExternalBufferIndex] = _021D2198.autoSamplingBuffer[bufferIndex];
        _021D2198.currentExternalBufferIndex++;

        if (_021D2198.currentExternalBufferIndex >= _021D2198.externalBufferSize) {
            if (externalBufferWriteMethod == TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING) {
                _021D2198.currentExternalBufferIndex %= _021D2198.externalBufferSize;
            } else {
                return AUTO_SAMPLING_OPERATION_RESULT_BUFFER_INDEX_OUT_OF_RANGE;
            }
        }
    }

    return _021D2198.currentExternalBufferIndex;
}

void sub_02021528(TouchPadDataBuffer *outBuffer) {
    int i;

    outBuffer->bufferSize = 0;

    for (i = 0; i < MAX_TOUCH_BUFFER_SIZE; i++) {
        outBuffer->buffer[i].validity = TP_VALIDITY_VALID;
        outBuffer->buffer[i].touch = TP_TOUCH_OFF;
        outBuffer->buffer[i].x = 0;
        outBuffer->buffer[i].y = 0;
    }
}

void sub_02021540(TouchPadDataBuffer *outBuffer, u32 lastAutoSamplingIndex) {
    int i;
    s16 bufferIndex;

    sub_02021528(outBuffer);

    for (i = 0; i < _021D2198.autoSamplingBufferSize; i++) {
        bufferIndex = lastAutoSamplingIndex - _021D2198.autoSamplingBufferSize + i + 1;

        if (bufferIndex < 0) {
            bufferIndex += AUTO_SAMPLING_BUFFER_MAX_SIZE;
        }

        if (_021D2198.autoSamplingBuffer[bufferIndex].validity == TP_VALIDITY_VALID) {
            outBuffer->buffer[outBuffer->bufferSize] = _021D2198.autoSamplingBuffer[bufferIndex];
            outBuffer->bufferSize++;
        }
    }
}
