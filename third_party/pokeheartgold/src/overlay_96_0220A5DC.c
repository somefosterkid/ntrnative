#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "overlay_96_ov96_0220_data.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E8318(void *param_1, s16 param_2);
#else
void ov96_021E8318(void *param_1, u16 param_2);
#endif

void ov96_0220A5DC(PokeathlonCourseData *param_1, u8 param_2);

void ov96_0220A660(SysTask *arg0, void *arg1);
void ov96_0220A704(Ov96_0220Data *data, s8 arg1, s8 arg2);

void ov96_0220A5DC(PokeathlonCourseData *param_1, u8 param_2) {
    Ov96_0220Data *pHVar1;
    String *string;
    MessageFormat *messageFormat;
    MsgData *msgData;

    pHVar1 = PokeathlonCourse_GetHeapAllocPtr4(param_1);
    messageFormat = pHVar1->msgFormat;
    msgData = pHVar1->msgData;
    ov96_021E8318((u8 *)param_1, (s16)param_2);
    FillWindowPixelBuffer(&pHVar1->windows[2], 0);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0x9f, pHVar1->heapId);
    AddTextPrinterParameterizedWithColor(&pHVar1->windows[2], FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&pHVar1->windows[2]);
    return;
}

void ov96_0220A660(SysTask *task, void *param) {
    Ov96_0220Data *arg1 = param;
    s32 value;
    s32 v0;

    if (arg1->unk_26E != 0) {
        v0 = 1;
    } else {
        v0 = -1;
    }
    value = arg1->unk_26C * v0;
    ScheduleSetBgPosText(arg1->bgConfig, 0U, BG_POS_OP_SET_XSCALE, value);
    ScheduleSetBgPosText(arg1->bgConfig, 1U, BG_POS_OP_SET_XSCALE, value);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(arg1->spriteSystem), 0, (0 - value) << 0xC);
    arg1->unk_26C = (s8)(arg1->unk_26C - arg1->unk_26D);
    arg1->unk_26E = (u8)(arg1->unk_26E ^ 1);
    if ((s32)arg1->unk_26C <= 0) {
        ScheduleSetBgPosText(arg1->bgConfig, 0U, BG_POS_OP_SET_XSCALE, 0x10);
        ScheduleSetBgPosText(arg1->bgConfig, 1U, BG_POS_OP_SET_XSCALE, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(arg1->spriteSystem), 0, 0);
        arg1->unk_268 = NULL;
        SysTask_Destroy(task);
    }
}

void ov96_0220A704(Ov96_0220Data *data, s8 arg1, s8 arg2) {
    SysTask *task;

    task = data->unk_268;
    if (task != NULL) {
        SysTask_Destroy(task);
    }
    data->unk_26E = 0;
    data->unk_26C = arg1;
    data->unk_26D = arg2;
    data->unk_268 = SysTask_CreateOnMainQueue(ov96_0220A660, data, 2U);
}
