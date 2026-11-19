#include "global.h"

#include "constants/heap.h"

#include "overlay_86_frontier_records_app.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

void ov86_021E75EC(void *, s32);

extern u32 ov86_021E7EB8;

extern u32 ov86_021E8094;

extern u32 ov86_021E8104;

void ov86_021E7630(FrontierRecordsApp *param_1);

void ov86_021E7630(FrontierRecordsApp *param_1) {
    GridInputHandler *pGVar1;

    pGVar1 = GridInputHandler_Create((TouchscreenHitbox *)&ov86_021E8094, (DpadMenuBox *)&ov86_021E8104, (GridCallbacks *)&ov86_021E7EB8, param_1, 1, (u8) * &param_1->category, HEAP_ID_121);
    param_1->inputHandler = pGVar1;
    ov86_021E75EC(param_1, param_1->category);
    return;
}
