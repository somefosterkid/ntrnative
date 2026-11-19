#include "global.h"

#include "pokeathlon/pokeathlon_save.h"

#include "bg_window.h"
#include "field_system.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_03.h"
#include "overlay_31.h"
#include "player_data.h"
#include "pm_string.h"
#include "render_text.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "text.h"
#include "overlay_manager.h"

static void ov31_0225D60C(MartSubScreen *arg0);

extern TouchscreenHitbox _0225EE40;

struct OverlayManager;
BOOL ov31_0225D758(OverlayManager *appMan, int *state);

SysTask *ov31_0225D520(BgConfig *arg0, u16 *arg1, FieldSystem *arg2, MartData *arg3) {
    MartData *martData;
    FieldSystem *v0;
    SysTask *sysTask;
    MartSubScreen *v1;

    v0 = arg2;
    martData = arg3;
    Heap_Create(HEAP_ID_3, HEAP_ID_8, 0x18000U);
    sysTask = CreateSysTaskAndEnvironment(ov31_0225D7A0, sizeof(MartSubScreen), 0xAU, HEAP_ID_8);
    v1 = SysTask_GetData(sysTask);
    v1->sysTask = sysTask;
    v1->unk_0 = 0;
    v1->bgConfig = arg0;
    v1->lastTouchMenuInput = arg1;
    v1->fieldSystem = v0;
    v1->unk_30 = 0;
    v1->options = Save_PlayerData_GetOptionsAddr(v1->fieldSystem->saveData);
    v1->playerProfile = Save_PlayerData_GetProfile(v1->fieldSystem->saveData);
    v1->pokeathlonSave = Save_Pokeathlon_Get(v1->fieldSystem->saveData);
    v1->state = 0;
    v1->martData = martData;
    ov31_0225DAC4(v1);
    ov31_0225DB38(v1);
    ov31_0225D60C(v1);
    ov31_0225D684(v1, 0);
    ov31_0225DE84(v1);
    ov31_0225DF98(v1);
    ov31_0225DD14(v1);
    ov03_022581BC(v1->martData);
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    GfGfx_EngineBTogglePlanes(1U, 1U);
    GfGfx_EngineBTogglePlanes(2U, 1U);
    GfGfx_EngineBTogglePlanes(4U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    TextFlags_SetFastForwardTouchButtonHitbox(&_0225EE40);
    return sysTask;
}

static void ov31_0225D60C(MartSubScreen *arg0) {
    arg0->messageFormat = MessageFormat_New_Custom(8U, 0x40U, HEAP_ID_8);
    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x1B3, HEAP_ID_8);
    arg0->itemMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xDE, HEAP_ID_8);
    arg0->string = String_New(0x90U, HEAP_ID_8);
}

static void ov31_0225D654(MartSubScreen *arg0) {
    String_Delete(arg0->string);
    DestroyMsgData(arg0->itemMsgData);
    DestroyMsgData(arg0->msgData);
    MessageFormat_Delete(arg0->messageFormat);
}

void ov31_0225D684(MartSubScreen *arg0, s32 arg1) {
    String *string;

    string = NewString_ReadMsgData(arg0->msgData, 0x11);
    ClearWindowTilemapAndCopyToVram(&arg0->unk_64);
    FillWindowPixelBuffer(&arg0->unk_64, 0U);
    if (arg1 == 0) {
        AddTextPrinterParameterizedWithColor(&arg0->unk_64, FONT_SYSTEM, string, ((u32)(0x30 - FontID_String_GetWidth(FONT_SYSTEM, string, 0U)) >> 1) + 8, 0U, 0xFFU, 0xF0E00U, NULL);
    } else {
        AddTextPrinterParameterizedWithColor(&arg0->unk_64, FONT_SYSTEM, string, 5U, 0U, 0xFFU, 0xF0E00U, NULL);
    }
    ScheduleWindowCopyToVram(&arg0->unk_64);
    String_Delete(string);
}



void ov31_0225D710(BgConfig *bgConfig, SysTask *arg1) {
    MartSubScreen *v0;

    v0 = SysTask_GetData(arg1);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov31_0225DBA0(v0);
    ov31_0225D654(v0);
    DestroySysTaskAndEnvironment(arg1);
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
    Heap_Destroy(HEAP_ID_8);
}

BOOL ov31_0225D758(OverlayManager *appMan, int *state) {
    return 1;
}

u32 ov31_0225D75C(MartSubScreen *param_1, s32 param_2) {
    switch (param_2) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (param_2 + (u32)param_1->martData->pageOffset < (u32)param_1->martData->count) {
            return 1;
        }
        break;
    case 6:
    case 7:
    case 8:
        return 1;
    }
    return 0;
}
