#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"
#include "unk_020932E0.h"

void sub_0209501C(UnkStruct_020932E0 *arg0, s32 strno, enum Font fontId) {
    MsgData *msgData;
    String *string;

    LoadUserFrameGfx2(arg0->unk_0, GF_BG_LYR_MAIN_FIRST, 0x3D2U, 0xBU, 0U, arg0->unk_4);
    AddWindowParameterized(arg0->unk_0, &arg0->unk_4660, 3U, 2U, 0x15U, 0x15U, 2U, 0xDU, 0xC6U);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x12A, arg0->unk_4);
    string = NewString_ReadMsgData(msgData, strno);
    FillWindowPixelBuffer(&arg0->unk_4660, 0xFU);
    AddTextPrinterParameterizedWithColor(&arg0->unk_4660, fontId, string, 0U, 0U, 0xFFU, 0x1020FU, NULL);
    String_Delete(string);
    DestroyMsgData(msgData);
    CopyWindowToVram(&arg0->unk_4660);
    GfGfx_EngineATogglePlanes(8U, 1U);
}
