#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "hall_of_fame.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "text.h"
#include "unk_02019BA4.h"

void ov64_021E677C(HallOfFameShowcase *, s32);

void ov64_021E6C1C(HallOfFameShowcase *, s32, u32, u32, u32);

void ov64_021E652C(HallOfFameShowcase *showcase, s32 param_2);

void ov64_021E652C(HallOfFameShowcase *showcase, s32 param_2) {
    s32 iVar1;
    u32 uVar2;
    s32 monNum;
    s32 iStack_2c;
    s32 iStack_30;
    RTCDate RStack_28;
    Window *pWin;

    showcase->record = showcase->record + param_2;
    if ((s32)showcase->record < 0) {
        showcase->record = 0;
    } else if ((s32)showcase->record >= (s32)showcase->numRecords) {
        showcase->record = showcase->numRecords + -1;
    }
    if (showcase->numRecords == 1) {
        ManagedSprite_SetDrawFlag(showcase->sprites[15], 0);
        ManagedSprite_SetDrawFlag(showcase->sprites[16], 0);
    } else {
        if (showcase->record == 0) {
            ManagedSprite_SetDrawFlag(showcase->sprites[15], 0);
        } else {
            ManagedSprite_SetDrawFlag(showcase->sprites[15], 1);
        }
        if (showcase->record == showcase->numRecords + -1) {
            ManagedSprite_SetDrawFlag(showcase->sprites[16], 0);
        } else {
            ManagedSprite_SetDrawFlag(showcase->sprites[16], 1);
        }
    }
    iVar1 = Save_HOF_TranslateRecordIdx(showcase->hallOfFame, showcase->record);
    showcase->recordNum = iVar1;
    uVar2 = Save_HOF_RecordCountMons(showcase->hallOfFame, showcase->record);
    showcase->numMons = uVar2;
    if (showcase->iconSet == 0) {
        iVar1 = 2;
        iStack_2c = 0xdcc2;
        iStack_30 = 7;
    } else {
        iVar1 = 8;
        iStack_2c = 0xdcc8;
        iStack_30 = 0xf;
    }
    monNum = 0;
    if (monNum < (u32)showcase->numMons) {
#if !(defined(SDK_PORT) || defined(NONMATCHING))
        u32 *puVar3 = (u32 *)showcase + iVar1;
#endif
        do {
            Save_HOF_GetMonStatsByIndexPair(showcase->hallOfFame, showcase->record, monNum, &showcase->mon);
            ov64_021E6C1C(showcase, iVar1, monNum, iStack_2c + monNum, iStack_2c + monNum);
#if defined(SDK_PORT) || defined(NONMATCHING)
            ManagedSprite_SetAnimationFrame(showcase->sprites[iVar1], 0);
            ManagedSprite_SetAnim(showcase->sprites[iVar1], 0);
#else
            ManagedSprite_SetAnimationFrame((ManagedSprite *)puVar3[0x4e], 0);
            ManagedSprite_SetAnim((ManagedSprite *)puVar3[0x4e], 0);
#endif
            monNum = monNum + 1;
            iVar1 = iVar1 + 1;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
            puVar3 = puVar3 + 1;
#endif
        } while (monNum < (u32)showcase->numMons);
    }
    iVar1 = GridInputHandler_GetNextInput(showcase->gridInput);
    ov64_021E677C(showcase, iVar1);
    pWin = &showcase->windows[iStack_30];
    FillWindowPixelBuffer(pWin, 0);
    ReadMsgDataIntoString(showcase->msgData, 0, showcase->stringBuf);
    iVar1 = Save_HOF_TranslateRecordIdx(showcase->hallOfFame, showcase->record);
    BufferIntegerAsString(showcase->msgFormat, 0, iVar1, 4, PRINTING_MODE_LEFT_ALIGN, 1);
    Save_HOF_GetClearDate(showcase->hallOfFame, showcase->record, &RStack_28);
    BufferIntegerAsString(showcase->msgFormat, 1, RStack_28.year + 2000, 4, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferMonthNameAbbr(showcase->msgFormat, 2, RStack_28.month);
    BufferIntegerAsString(showcase->msgFormat, 3, RStack_28.day, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(showcase->msgFormat, showcase->stringBuf2, showcase->stringBuf);
    AddTextPrinterParameterizedWithColor(pWin, FONT_SYSTEM, showcase->stringBuf2, 0, 0, 0xff, 0xf0200, (PrinterCallback_t)0x0);
    CopyWindowPixelsToVram_TextMode(pWin);
    ScheduleWindowCopyToVram(pWin);
    showcase->iconSet = showcase->iconSet ^ 1;
    return;
}
