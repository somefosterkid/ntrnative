#include "global.h"

#include "constants/heap.h"
#include "gf_gfx_planes.h"

#include "bg_window.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_27.h"
#include "render_text.h"
#include "sys_task.h"
#include "systask_environment.h"

extern const BgTemplate ov27_0225D370;

extern const BgTemplate ov27_0225D38C;

extern const BgTemplate ov27_0225D3A8;



SysTask *ov27_0225C250(BgConfig *bgConfig, u16 *touchInput, FieldSystem *fieldSystem, u16 *result) {
    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000);

    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);

    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &ov27_0225D370, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &ov27_0225D38C, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &ov27_0225D3A8, GF_BG_TYPE_TEXT);

    BG_ClearCharDataRange(GF_BG_LYR_SUB_0, 0x20, 0, HEAP_ID_FIELD1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_1, 0x20, 0, HEAP_ID_FIELD1);
    BG_ClearCharDataRange(GF_BG_LYR_SUB_2, 0x20, 0, HEAP_ID_FIELD1);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    SysTask *task = CreateSysTaskAndEnvironment(ov27_0225C434, sizeof(Ov27TaskData), 10, HEAP_ID_8);
    Ov27TaskData *data = SysTask_GetData(task);

    data->task = task;
    data->unk_14 = 0;
    data->bgConfig = bgConfig;
    data->touchInput = touchInput;
    data->fieldSystem = fieldSystem;
    data->unk_48 = 0;
    data->state = 0;
    data->result = result;
    data->callback = NULL;
    data->callbackMenu = NULL;
    data->windowA.pixelBuffer = NULL;
    data->windowB.pixelBuffer = NULL;

    FontID_Alloc(FONT_UNK_4, HEAP_ID_8);
    ov27_0225C914(data, data->fieldSystem->unkD2_0);
    ov27_0225C4AC(data);
    ov27_0225C72C(data);
    ov27_0225C80C(data, 0);

    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
    TextFlags_SetCanTouchSpeedUpPrint(1);

    return task;
}
