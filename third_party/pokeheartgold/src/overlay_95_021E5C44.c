#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "text.h"
#include "gf_gfx_planes.h"

void ov95_021E5C44(BgConfig *bgConfig, Window *window, enum GFBgLayer bgLayer, int tilemapTop, int tilemapLeft, int width, int height, int baseTile, int palette);

u8 ov95_021E5CAC(Window *arg0, s32 arg1, Pokemon *arg2, u32 arg3);

void ov95_021E5C44(BgConfig *bgConfig, Window *window, enum GFBgLayer bgLayer, int tilemapTop, int tilemapLeft, int width, int height, int baseTile, int palette) {
    InitWindow(window);
    AddWindowParameterized(bgConfig, window, bgLayer, tilemapTop, tilemapLeft, width, height, palette, baseTile);
    DrawFrameAndWindow2(window, 1, 20, 12);
    FillWindowPixelBuffer(window, 15);
    CopyWindowToVram(window);
}

u8 ov95_021E5CAC(Window *window, s32 strno, Pokemon *arg2, u32 textSpeed) {
    String *src;
    Pokemon *mon;
    Window *v0;
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;
    String *v1;
    u8 v2;

    v0 = window;
    mon = arg2;
    FillWindowPixelBuffer(window, 0xFU);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xBB, HEAP_ID_70);
    messageFormat = MessageFormat_New(HEAP_ID_70);
    string = NewString_ReadMsgData(msgData, strno);
    src = string;
    v1 = String_New(0xFFU, HEAP_ID_70);
    BufferBoxMonSpeciesName(messageFormat, 0U, Mon_GetBoxMon(mon));
    StringExpandPlaceholders(messageFormat, v1, src);
    v2 = AddTextPrinterParameterized(window, FONT_MESSAGE, v1, 0U, 0U, textSpeed, NULL);
    DestroyMsgData(msgData);
    String_Delete(v1);
    String_Delete(string);
    MessageFormat_Delete(messageFormat);
    return v2;
}
