#include "global.h"

#include "msgdata.h"
#include "overlay_81.h"
#include "pm_string.h"
#include "bg_window.h"
#include "gf_gfx_loader.h"

s32 ov81_02243068(Window *, String *, s32, s32, s32, s32, s32);

void ov81_022430E8(Window *param_1);

void ov81_02243100(BgConfig *arg0, NARC *arg1);

void ov81_022430B4(void *arg0, MsgData *msgData, s32 strno, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    ov81_02243068(arg0, string, arg3, arg4, arg5, arg6, arg7);
    String_Delete(string);
}

void ov81_022430E8(Window *param_1) {
    SetWindowX(param_1, 4);
    SetWindowX(param_1 + 1, 0x12);
    return;
}

void ov81_02243100(BgConfig *bgConfig, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x86, bgConfig, GF_BG_LYR_SUB_1, 0x3D9U, 0U, 1, HEAP_ID_100);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0xC2, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_9_OFFSET, 0x40U, HEAP_ID_100);
}
