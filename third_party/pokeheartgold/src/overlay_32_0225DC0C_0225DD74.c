#include "global.h"
struct NamePickerData;

#include "overlay_32.h"
#include "unk_02019BA4.h"
#include "overlay_32_name_picker_data.h"

s32 ov32_0225DD24(void *, s32);

extern GridCallbacks ov32_0225E168;

extern TouchscreenHitbox ov32_0225E1DC;

extern DpadMenuBox ov32_0225E204;

void ov32_0225DD74(NamePickerData *callbackParam) {
    callbackParam->gridInput = GridInputHandler_Create(&ov32_0225E1DC, &ov32_0225E204, &ov32_0225E168, callbackParam, 1, 0U, HEAP_ID_8);
    ov32_0225DD24(callbackParam, 0);
}
