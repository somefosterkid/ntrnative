#include "global.h"

#include "constants/heap.h"

#include "overlay_86_frontier_records_app.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

extern const GridCallbacks ov86_021E7EA8;

extern const TouchscreenHitbox ov86_021E7F94[];

extern const DpadMenuBox ov86_021E7FE4[];

void ov86_021E7860(FrontierRecordsApp *param_1) {
    u16 uVar1;
    GridInputHandler *pGVar2;

    uVar1 = param_1->cursor;
    pGVar2 = GridInputHandler_Create(ov86_021E7F94, ov86_021E7FE4, &ov86_021E7EA8, param_1, 1, (u8)uVar1, HEAP_ID_121);
    param_1->inputHandler = pGVar2;
    ov86_021E781C(param_1, uVar1);
    return;
}
