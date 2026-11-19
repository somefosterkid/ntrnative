#include "global.h"
#include "sprite_resources_header.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "signature_app.h"
#include "game_stats.h"
#include "msgdata.h"
#include "touch_pad_data_buffer.h"
#include "struct_defs/resdat_narc_structs.h"

struct DrawingState;

struct PreviousTouchDataBuffer;

struct SignatureApp;

struct SignatureTouchDataBuffer;

struct Sprite;

struct TouchPadDataBuffer;

struct YesNoPrompt;

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

u32 sub_02021280(TouchPadDataBuffer *outBuffer, enum TouchPadExternalBufferWriteMethod externalBufferWriteMethod, u32 changeThreshold);

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

extern int TouchscreenHitbox_FindRectAtTouchHeld(const TouchScreenRect rects[]);

extern int TouchscreenHitbox_FindRectAtTouchNew(const TouchScreenRect rects[]);

typedef struct YesNoPrompt YesNoPrompt;

BOOL PlaySE(u16 seqID);

enum SignatureAppSubstate {
    SIGNATURE_APP_SUBSTATE_0_UNUSED = 0,
    SIGNATURE_APP_SUBSTATE_MAIN,
    SIGNATURE_APP_SUBSTATE_DONE_PRESSED,
    SIGNATURE_APP_SUBSTATE_CONFIRM_DONE,
    SIGNATURE_APP_SUBSTATE_PROMPT_RESET,
    SIGNATURE_APP_SUBSTATE_OPEN_RESET_MENU,
    SIGNATURE_APP_SUBSTATE_CONFIRM_RESET,
};

void ov52_021E927C(SignatureApp *signatureApp, int bankEntry);

void ov52_021E8CBC(Sprite **buttonSprites, int pressedButtonIndex);

void ov52_021E8CDC(Sprite **buttonSprite, BOOL isPressed);

void ov52_021E9364(SignatureApp *signatureApp);

extern const TouchScreenRect ov52_021E94BA[4];

extern const TouchScreenRect ov52_021E94B2[2];

void ov52_021E8BDC(SignatureApp *signatureApp);

void ov52_021E8BDC(SignatureApp *signatureApp) {
    int touchedIndex = TouchscreenHitbox_FindRectAtTouchNew(ov52_021E94BA);

    if (touchedIndex != -1) {
        if (touchedIndex == 0) {
            if (signatureApp->subState == SIGNATURE_APP_SUBSTATE_MAIN) {
                ov52_021E927C(signatureApp, 10);
                signatureApp->subState = SIGNATURE_APP_SUBSTATE_DONE_PRESSED;
                ov52_021E8CDC(signatureApp->sprite, 1);
                PlaySE(1501);
            }
        } else {

            signatureApp->brushTypeUnused = touchedIndex;
            ov52_021E8CBC(signatureApp->sprite, touchedIndex);
        }
    }

    int heldIndex = TouchscreenHitbox_FindRectAtTouchHeld(ov52_021E94B2);

    if (heldIndex != -1) {
        ov52_021E9364(signatureApp);
    }

    TouchPadDataBuffer tpBuffer;

    if (sub_02021280(&tpBuffer, TOUCH_PAD_EXTERNAL_BUFFER_WRITE_METHOD_ALL_DATA_WITHOUT_WRAPPING, 1) == AUTO_SAMPLING_OPERATION_RESULT_SUCCESS) {
        for (int i = 0; i < tpBuffer.bufferSize; i++) {
            signatureApp->touchDataBuffer.x[i] = tpBuffer.buffer[i].x;
            signatureApp->touchDataBuffer.y[i] = tpBuffer.buffer[i].y;
        }

        signatureApp->touchDataBuffer.bufferSize = tpBuffer.bufferSize;
        signatureApp->touchDataBuffer.brushTypeUnused = signatureApp->brushTypeUnused;
    }
}
