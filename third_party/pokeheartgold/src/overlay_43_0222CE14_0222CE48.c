#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_43_palpad.h"
#include "pm_string.h"
#include "save.h"
#include "text.h"
#include "unk_0202C034.h"

void ov43_0222AAA4(PalPadGfx *gfx, u64 friendKey);

void ov43_0222CE48(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);

void ov43_0222CE48(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    String *string;
    String *format;
    u32 x;
    WiFiList *wifiList;
    DWCUserData *userData;
    u64 friendKey;

    InitWindow(&friendCode->windows[0]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[0], 3, 4, 0xb, 0x18, 8, 0xb, 1);
    InitWindow(&friendCode->windows[1]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[1], 3, 9, 6, 0xe, 2, 0xb, 0xc1);
    InitWindow(&friendCode->windows[2]);
    AddWindowParameterized(gfx->bgConfig, &friendCode->windows[2], 3, 0x15, 0x15, 9, 2, 0xb, 0xdd);
    FillWindowPixelBuffer(&friendCode->windows[0], 0);
    FillWindowPixelBuffer(&friendCode->windows[1], 0);
    FillWindowPixelBuffer(&friendCode->windows[2], 0);
    string = String_New(0x80, heapId);
    format = String_New(0x80, heapId);
    wifiList = sub_0202C6F4(shared->saveData);
    userData = sub_0202C08C(wifiList);
    friendKey = DWC_CreateFriendKey(userData);
    if (friendKey != 0) {
        ReadMsgDataIntoString(gfx->msgData, 53, string);
        x = (0xc0 - FontID_String_GetWidthMultiline((enum Font)0, string, 0)) >> 1;
        AddTextPrinterParameterizedWithColor(&friendCode->windows[0], (enum Font)0, string, x, 0, 0xff, 0x10200, NULL);
        ov43_0222AAA4(gfx, friendKey);
        ReadMsgDataIntoString(gfx->msgData, 0x34, format);
        StringExpandPlaceholders(gfx->msgFormat, string, format);
        AddTextPrinterParameterizedWithColor(&friendCode->windows[1], (enum Font)0, string, 0, 0, 0xff, 0xF0200, NULL);
    } else {
        ReadMsgDataIntoString(gfx->msgData, 0x36, string);
        x = (0xc0 - FontID_String_GetWidthMultiline((enum Font)0, string, 0)) >> 1;
        AddTextPrinterParameterizedWithColor(&friendCode->windows[0], (enum Font)0, string, x, 0, 0xff, 0x10200, NULL);
    }
    ReadMsgDataIntoString(gfx->msgData, 0x33, string);
    AddTextPrinterParameterizedWithColor(&friendCode->windows[2], FONT_UNK_4, string, 0, 0, 0xff, 0x10f00, NULL);
    String_Delete(string);
    String_Delete(format);
    friendCode->screenFile = GfGfxLoader_GetScrnDataFromOpenNarc(gfx->narc, 0xe, 1, &friendCode->screen, heapId);
}
