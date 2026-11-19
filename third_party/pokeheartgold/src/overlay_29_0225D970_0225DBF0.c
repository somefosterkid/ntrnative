#include "global.h"

#include "unk_02019BA4.h"
#include "overlay_29_number_entry_data.h"

s32 ov29_0225DB9C(void *, s32);

s32 ov29_0225DC50(void *);

extern GridCallbacks ov29_0225E13C;

extern TouchscreenHitbox ov29_0225E248;

extern DpadMenuBox ov29_0225E280;

void ov29_0225DBF0(struct NumberEntryData *callbackParam);

void ov29_0225DBF0(NumberEntryData *callbackParam) {
    callbackParam->gridInput = GridInputHandler_Create(&ov29_0225E248, &ov29_0225E280, &ov29_0225E13C, callbackParam, 1, 0U, HEAP_ID_8);
    ov29_0225DB9C(callbackParam, 0);
    ov29_0225DC50(callbackParam);
}
