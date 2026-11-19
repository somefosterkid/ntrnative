#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_32.h"
#include "pm_string.h"
#include "text.h"
#include "heap.h"
#include "player_data.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0202C034.h"
#include "overlay_32_name_picker_data.h"

void ov32_0225D84C(NamePickerData *data) {
    String *src;
    String *string;
    String *v0;
    String *v2;
    u32 new_var;
    String *v4;
    u16 v3;
    u32 v1;
    u32 x;
    FillWindowPixelBuffer(&data->windows[1], 0U);
    string = NewString_ReadMsgData(data->msgData, 0x1C);
    v3 = (u16)FontID_String_GetWidth(FONT_SYSTEM, string, 0U);
    v1 = (GetWindowWidth(&data->windows[1]) * 8) - v3;
    x = ((u32)((v1 + (v1 >> 0x1F)) << 0xF)) >> 0x10;
    AddTextPrinterParameterizedWithColor(&data->windows[1], FONT_SYSTEM, string, x, 0U, 0xFFU, 0x50600U, 0L);
    String_Delete(string);
    v2 = String_New(4U, HEAP_ID_8);
    v0 = NewString_ReadMsgData(data->msgData, 0x1B);
    src = v0;
    BufferIntegerAsString(data->msgFormat, 0U, 4, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(data->msgFormat, v2, src);
    AddTextPrinterParameterizedWithColor(&data->windows[1], FONT_SYSTEM, v2, x + v3, 0U, 0xFFU, 0x50600U, 0L);
    String_Delete(v0);
    v4 = NewString_ReadMsgData(data->msgData, 0x1B);
    BufferIntegerAsString(data->msgFormat, 0U, (data->page) + 1, 1U, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(data->msgFormat, v2, v4);
    new_var = (u32)((u16)(x - FontID_String_GetWidth(FONT_SYSTEM, v2, 0U)));
    AddTextPrinterParameterizedWithColor(&data->windows[1], FONT_SYSTEM, v2, new_var, 0U, 0xFFU, 0x50600U, 0L);
    String_Delete(v4);
    String_Delete(v2);
    CopyWindowPixelsToVram_TextMode(&data->windows[1]);
    ScheduleWindowCopyToVram(&data->windows[1]);
}

void ov32_0225D988(NamePickerData *data) {
    PlayerProfile *playerProfile;
    String *v1;
    String *string;
    u32 v0;
    Window *window;
    u16 v4;
    u32 v3;

    if (data->bufferToggle == 0) {
        window = &data->windows[2];
    } else {
        window = &data->windows[10];
    }
    v1 = NewString_ReadMsgData(data->msgData, 0x1E);
    string = String_New(0x40U, HEAP_ID_8);
    playerProfile = PlayerProfile_New(HEAP_ID_8);
    v3 = GetWindowWidth(window) * 8;
    v0 = (u32)((v3 + (v3 >> 0x1F)) << 0xF) >> 0x10;
    v4 = 0;
    do {
        Save_Profile_PlayerName_Set(playerProfile, sub_0202C254(data->names, v4 + (data->page * 8)));
        BufferPlayersName(data->msgFormat, 0U, playerProfile);
        StringExpandPlaceholders(data->msgFormat, string, v1);
        FillWindowPixelBuffer(&window[v4], 0U);
        AddTextPrinterParameterizedWithColor(&window[v4], FONT_UNK_4, string, v0 - (FontID_String_GetWidth(FONT_SYSTEM, string, 0U) >> 1), 0U, 0xFFU, 0x50600U, NULL);
        CopyWindowPixelsToVram_TextMode(&window[v4]);
        ScheduleWindowCopyToVram(&window[v4]);
        v4 += 1;
    } while ((u32)v4 < 8U);
    Heap_Free(playerProfile);
    String_Delete(string);
    String_Delete(v1);
    data->bufferToggle = (u8)(1 ^ data->bufferToggle);
}

void ov32_0225DA88(NamePickerData *data, NARC *narc) {
    data->spriteList = G2dRenderer_Init(4, &data->renderer, HEAP_ID_8);
    G2dRenderer_SetSubSurfaceCoords(&data->renderer, 0, 0x100000);
    ov32_0225DADC(data);
    ov32_0225DC68(data, narc);
}

void ov32_0225DAC0(NamePickerData *data) {
    ov32_0225DCD4(data);
    ov32_0225DB00(data);
    SpriteList_Delete(data->spriteList);
}

void ov32_0225DADC(NamePickerData *data) {
    GfGfxResType type;
#if defined(SDK_PORT) || defined(NONMATCHING)
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        data->resMans[type] = Create2DGfxResObjMan(1, type, HEAP_ID_8);
        type += 1;
    } while ((u32)type < 4U);
#else
    u8 *v0;

    v0 = (u8 *)data;
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        *(GF_2DGfxResMan **)(v0 + offsetof(NamePickerData, resMans)) = Create2DGfxResObjMan(1, type, HEAP_ID_8);
        type += 1;
        v0 += sizeof(GF_2DGfxResMan *);
    } while ((u32)type < 4U);
#endif
}

void ov32_0225DB00(NamePickerData *data) {
    u32 v0;

    v0 = 0;
    do {
        Destroy2DGfxResObjMan(data->resMans[v0]);
        v0 += 1;
    } while (v0 < 4U);
}

void ov32_0225DB1C(SpriteResource **resObjs, GF_2DGfxResMan **resMans, NARC *narc, s32 fileId, s32 arg4, s32 arg5, s32 arg6, s32 pltt_num, s32 id, s32 arg9, s32 argA, s32 argB) {
    SpriteResource *charResObj;
    SpriteResource *plttResObj;

    charResObj = AddCharResObjFromOpenNarc(resMans[0], narc, fileId, 1, id, 2, HEAP_ID_8);
    resObjs[0] = charResObj;
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(charResObj);
    sub_0200A740(resObjs[0]);
    plttResObj = AddPlttResObjFromOpenNarc(resMans[1], narc, arg4, 0, arg9, 2, pltt_num, HEAP_ID_8);
    resObjs[1] = plttResObj;
    SpriteTransfer_CreatePlttTransferTask(plttResObj);
    sub_0200A740(resObjs[1]);
    resObjs[2] = AddCellOrAnimResObjFromOpenNarc(resMans[2], narc, arg5, 1, argA, GF_GFX_RES_TYPE_CELL, HEAP_ID_8);
    resObjs[3] = AddCellOrAnimResObjFromOpenNarc(resMans[3], narc, arg6, 1, argB, GF_GFX_RES_TYPE_ANIM, HEAP_ID_8);
}
