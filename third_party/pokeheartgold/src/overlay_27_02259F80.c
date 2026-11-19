#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_27.h"
#include "overlay_27_ov27task.h"
#include "sprite.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "task.h"
#include "text.h"

void ov27_0225A690(Ov27Task *task, s32 arg1);

void ov27_0225A7FC(Ov27Task *task);

void ov27_0225AC00(BgConfig *bgConfig, int idx, Window *win1, Window *win2, Window *wins);

void ov27_0225B010(Ov27Task *task);

void ov27_0225BB6C(Ov27Task *task, u32 unused);

void ov27_0225BC84(Ov27Task *task);

void ov27_0225BCE8(Ov27Task *task);

void ov27_0225C0E0(Ov27Task *task);

void ov27_0225C10C(Ov27Task *task);

s32 ov27_0225C1AC(Ov27Task *task, s32 arg1);

extern const BgTemplate ov27_0225D000;

extern const BgTemplate ov27_0225D01C;

SysTask *ov27_02259F80(BgConfig *bgConfig, u16 *arg1, FieldSystem *fieldSystem) {
    Ov27Task *data;
    SysTask *task;

    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18D00);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_32_H);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_16_I);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &ov27_0225D000, GF_BG_TYPE_TEXT);
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &ov27_0225D01C, GF_BG_TYPE_TEXT);
    task = CreateSysTaskAndEnvironment(ov27_0225A320, sizeof(Ov27Task), 10, HEAP_ID_8);
    data = SysTask_GetData(task);
    data->task = task;
    data->unk_00 = 0;
    data->bgConfig = bgConfig;
    data->unk_0C = arg1;
    data->fieldSystem = fieldSystem;
    data->unk_51C_1 = ov27_0225BD50(fieldSystem);
    data->unk_51C_5 = 0;
    ov27_0225AC00(bgConfig, data->unk_00, &data->unk_3D0, &data->unk_3E0, data->unk_3F0);
    FontID_Alloc(FONT_UNK_4, HEAP_ID_8);
    data->unk_4AC = MessageFormat_New(HEAP_ID_8);
    data->unk_4A8 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 196, HEAP_ID_8);
    ov27_0225C10C(data);
    data->unk_14 = ov27_0225C1AC(data, fieldSystem->unkD3);
    ov27_0225C1EC(data);
    ov27_0225AD0C(data);
    ov27_0225B010(data);
    ov27_0225BB6C(data, data->fieldSystem->unkD2_0);
    AddTextPrinterParameterizedWithColor(&data->unk_3D0, FONT_UNK_4, data->unk_4B0[1], 0, 0, 0, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    AddTextPrinterParameterizedWithColor(&data->unk_3E0, FONT_SYSTEM, data->unk_4B0[5], 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 1, 0), NULL);
    ov27_0225BCE8(data);
    ov27_0225BC84(data);
    ov27_0225A690(data, 1);
    ov27_0225C0E0(data);
    ov27_0225BDDC(&data->unk_520, data);
    if (MenuInputStateMgr_GetState(&fieldSystem->menuInputState) == MENU_INPUT_STATE_BUTTONS) {
        fieldSystem->unkD2_7 = FALSE;
    } else if (!FieldSystem_TaskIsRunning(fieldSystem)) {
        fieldSystem->unkD2_7 = TRUE;
    }
    if (ov27_0225A714(data) == 0) {
        return task;
    }
    ov27_0225A7FC(data);
    SpriteList_RenderAndAnimateSprites(data->unk_18);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    return task;
}
