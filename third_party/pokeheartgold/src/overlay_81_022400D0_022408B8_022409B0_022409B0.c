#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_81_battle_factory_app.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "text.h"


void ov81_022409B0(BattleFactoryApp *app, Window *param_2, s32 param_3, u32 param_4, u32 param_5, u8 param_6, u8 param_7, u8 param_8, u8 param_9, Party *param_10);

void ov81_022409B0(BattleFactoryApp *app, Window *param_2, s32 param_3, u32 param_4, u32 param_5, u8 param_6, u8 param_7, u8 param_8, u8 param_9, Party *param_10) {
    u8 uVar1;
    unsigned short new_var;
    u8 bVar2;
    Pokemon *mon;
    String *pSVar3;
    String *src;
    BoxPokemon *boxmon;
    u32 uVar4;
    unsigned short x;
    enum Font fontId;
    FillWindowPixelBuffer(param_2, param_8);
    mon = Party_GetMonByIndex(param_10, param_3);
    pSVar3 = String_New(0xb, HEAP_ID_100);
    new_var = ((u32)param_7) << 8;
    src = NewString_ReadMsgData(app->msgLoader, 0x1c);
    boxmon = Mon_GetBoxMon(mon);
    BufferBoxMonSpeciesName(app->strTemplate, 0, boxmon);
    StringExpandPlaceholders(app->strTemplate, pSVar3, src);
    fontId = (enum Font)param_9;
    AddTextPrinterParameterizedWithColor(param_2, fontId, pSVar3, param_4, param_5, 0xff, ((((u32)param_6 & 0xff) << 0x10) | new_var) | ((u32)param_8), (PrinterCallback_t)0x0);
    String_Delete(src);
    String_Delete(pSVar3);
    uVar4 = GetMonData(mon, 0xb0, (void *)0x0);
    if (uVar4 == 1) {
        uVar1 = GetMonGender(mon);
        bVar2 = GetWindowWidth(param_2);
        x = (((bVar2 - 1) * 8) - 4) & 0xffff;
        if (uVar1 == 0) {
            pSVar3 = NewString_ReadMsgData(app->msgLoader, 0x1a);
            AddTextPrinterParameterizedWithColor(param_2, fontId, pSVar3, x, param_5, 0xff, 0x70800, (PrinterCallback_t)0x0);
            String_Delete(pSVar3);
        } else if (uVar1 == 1) {
            pSVar3 = NewString_ReadMsgData(app->msgLoader, 0x1b);
            AddTextPrinterParameterizedWithColor(param_2, fontId, pSVar3, x, param_5, 0xff, 0x30400, (PrinterCallback_t)0x0);
            String_Delete(pSVar3);
        }
    }
    ScheduleWindowCopyToVram(param_2);
    return;
}
