#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system_def.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "overlay_02_02245B80.h"
#include "pm_string.h"
#include "text.h"

extern const WindowTemplate ov02_0225324C;

void ov02_0224686C(AlphUnownTextTask *env) {
    u8 bVar1;
    NARC *narc;
    MsgData *msgData;
    String *string;
    narc = NARC_New(NARC_demo_legend, env->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xf, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, env->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x10, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, env->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x11, env->fieldSystem->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, env->heapID);
    bVar1 = GetBgPriority(env->fieldSystem->bgConfig, 1);
    env->bgPriority = bVar1;
    SetBgPriority(1, 0);
    GfGfx_EngineATogglePlanes(2, 1);
    NARC_Delete(narc);
    AddWindow(env->fieldSystem->bgConfig, &env->window, &ov02_0225324C);
    FillWindowPixelBuffer(&env->window, 9);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xc6, env->heapID);
    string = NewString_ReadMsgData(msgData, env->msgNo);
    bVar1 = FontID_String_GetWidth(FONT_UNOWN, string, 0);
    AddTextPrinterParameterizedWithColor(&env->window, FONT_UNOWN, string, ((s32)(0x70 - ((u8)bVar1))) / 2, 0, 0xff, 0xf0e09, (PrinterCallback_t)0x0);
    ScheduleWindowCopyToVram(&env->window);
    String_Delete(string);
    DestroyMsgData(msgData);
    return;
}
