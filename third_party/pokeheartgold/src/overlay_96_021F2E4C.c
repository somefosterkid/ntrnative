#include "global.h"

#include "sys_task_api.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "string_util.h"
#include "text.h"
#include "overlay_96_ov96_arena_work.h"
#include "pokeathlon/pokeathlon.h"

extern s32 ov96_0221DB5C;

void ov96_021F2E4C(SysTask *arg0, void *arg1);

s32 ov96_021EAED4(s32 *, s32, s32, s32, s32);
void ov96_021F2E80(PokeathlonCourseData *arg0, Ov96ArenaFx *arg1, void *arg2, s32 arg3, s32 arg4);
extern WindowTemplate ov96_0221BC5C;
void ov96_021F2EC8(Ov96ArenaWork *arg0);
void ov96_021F2EFC(Ov96ArenaWork *param_1, s32 param_2);

void ov96_021F2E4C(SysTask *task, void *arg1) {
    Ov96ArenaFx *fx = arg1;

    fx->unk_2C = (s16)(fx->unk_2C + *((s32 *)((&ov96_0221DB5C) + fx->unk_2E)));
    fx->unk_2E = (u16)(fx->unk_2E + 1);
    if ((u32)fx->unk_2E >= 5U) {
        fx->unk_2C = 0;
        fx->unk_14 = NULL;
        fx->unk_2E = 0U;
        fx->unk_28 = 0xC;
        SysTask_Destroy(task);
    }
}

void ov96_021F2E80(PokeathlonCourseData *arg0, Ov96ArenaFx *arg1, void *arg2, s32 arg3, s32 arg4) {
    s32 v0;

    if (arg1->unk_28 == arg3) {
        v0 = arg3 / 3;
        if (ov96_021E60D8(arg0, v0, arg3 % 3)->statIdx[2] != 0) {
            ov96_021EAED4(arg2, 0, arg1->unk_2C, arg4, 0);
        }
    }
}

void ov96_021F2EC8(Ov96ArenaWork *work) {
    AddWindow(work->bgConfig, &work->window, &ov96_0221BC5C);
    BG_FillCharDataRange(work->bgConfig, GF_BG_LYR_MAIN_3, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
}

void ov96_021F2EFC(Ov96ArenaWork *work, s32 param_2) {
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *string;

    FillWindowPixelBuffer(&work->window, 0);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, work->heapId);
    messageFormat = MessageFormat_New(work->heapId);
    BufferIntegerAsString(messageFormat, 0, param_2, 3, PRINTING_MODE_LEFT_ALIGN, 1);
    string = ReadMsgData_ExpandPlaceholders(messageFormat, msgData, 0x9e, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->window, FONT_UNK_4, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    DestroyMsgData(msgData);
    CopyWindowToVram(&work->window);
    return;
}
