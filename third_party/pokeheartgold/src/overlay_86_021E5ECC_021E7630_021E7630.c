#include "global.h"

#include "constants/heap.h"

#include "overlay_86_frontier_records_app.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern const GridCallbacks ov86_021E7EB8;

extern const TouchscreenHitbox ov86_021E8094[];

extern const DpadMenuBox ov86_021E8104[];

void ov86_021E7630(FrontierRecordsApp *param_1) {
    GridInputHandler *pGVar1;

    pGVar1 = GridInputHandler_Create(ov86_021E8094, ov86_021E8104, &ov86_021E7EB8, param_1, 1, (u8)param_1->category, HEAP_ID_121);
    param_1->inputHandler = pGVar1;
    ov86_021E75EC(param_1, param_1->category);
    return;
}
