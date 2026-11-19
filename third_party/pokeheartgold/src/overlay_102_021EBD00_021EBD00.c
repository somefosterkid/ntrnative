#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_102_app_021ebc34.h"

BgConfig *ov102_021EA268(void *);

void ov102_021EBD68(void *param_1);

void ov102_021EBD00(Ov102App_021EBC34 *param_1);

void ov102_021EBD00(Ov102App_021EBC34 *param_1) {
    BgConfig *bgConfig;

    bgConfig = ov102_021EA268(param_1->unk_0);
    BG_ClearCharDataRange(2, 0x20, 0, HEAP_ID_35);
    FillBgTilemapRect(bgConfig, 2, 0, 0, 0, 0x20, 0x20, 0xc);
    FillWindowPixelBuffer(&param_1->unk_10, 0);
    PutWindowTilemap(&param_1->unk_10);
    CopyWindowPixelsToVram_TextMode(&param_1->unk_10);
    BgCommitTilemapBufferToVram(bgConfig, 2);
    ov102_021EBD68(param_1);
    ToggleBgLayer(2, 0);
    return;
}
