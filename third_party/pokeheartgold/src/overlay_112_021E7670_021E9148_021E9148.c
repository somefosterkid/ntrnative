#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

void ov112_021E9148(u8 *param_1, Ov112PokewalkerApp *param_2, u8 *param_3, u8 *param_4) {
    MsgData *msgData;
    String *pSVar1;
    u8 *sVar2;
    Window *window;
    s32 iVar3;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xed, HEAP_ID_154);
    FontID_Alloc(FONT_UNK_5, HEAP_ID_154);
    iVar3 = 0;
    sVar2 = param_3;
    window = (Window *)param_1;
    do {
        pSVar1 = NewString_ReadMsgData(param_2->msgData2, (u32) * (u16 *)(sVar2 + 0x80));
        AddTextPrinterParameterizedWithColor(window, FONT_UNK_5, pSVar1, 2, 1, 0xff, 0x30100, (PrinterCallback_t)0x0);
        String_Delete(pSVar1);
        sVar2 = sVar2 + 6;
        iVar3 = iVar3 + 1;
        window = window + 1;
    } while (iVar3 < 10);
    iVar3 = 0;
    do {
        pSVar1 = NewString_ReadMsgData(msgData, (u32) * (u16 *)(param_3 + (u32) * (u8 *)(param_4 + iVar3) * 0x14 + 8));
        AddTextPrinterParameterizedWithColor((Window *)(param_1 + (iVar3 + 10) * sizeof(Window)), FONT_UNK_5, pSVar1, 2, 1, 0xff, 0x30100, (PrinterCallback_t)0x0);
        String_Delete(pSVar1);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 3);
    pSVar1 = NewString_ReadMsgData(param_2->msgData, param_2->unk_1D798 + 0x5b);
    AddTextPrinterParameterizedWithColor((Window *)(param_1 + 0xd0), FONT_UNK_5, pSVar1, 2, 0, 0xff, 0x30100, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    pSVar1 = String_New(0xc, HEAP_ID_154);
    GetBoxMonData(param_2->unk_1E430, 0x77, pSVar1);
    AddTextPrinterParameterizedWithColor((Window *)(param_1 + 0xe0), FONT_UNK_5, pSVar1, 2, 1, 0xff, 0x30100, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    FontID_Release(FONT_UNK_5);
    DestroyMsgData(msgData);
    return;
}
