#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"
#include "overlay_96_ov96_course_hud.h"

void ov96_022147FC(Ov96CourseHud *hud) {
    u8 playerIdx;
    String *v0;
    FillWindowPixelBuffer(&hud->nameWindow, 0U);
    playerIdx = (u8)ov96_021E5F24(hud->course);
    v0 = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(hud->course, (s32)playerIdx), hud->heapID);
    AddTextPrinterParameterizedWithColor(&hud->nameWindow, FONT_SYSTEM, v0, 0U, 0U, 0xFFU, 0xF0E00U, 0L);
    String_Delete(v0);
    CopyWindowToVram(&hud->nameWindow);
}

static void ov96_02214854(Ov96CourseHud *hud);

static void ov96_02214854(Ov96CourseHud *hud) {
    String *string;

    FillWindowPixelBuffer(&hud->msgWindow, 0);
    string = ReadMsgData_ExpandPlaceholders(hud->msgFormat, hud->msgData, 0x137, hud->heapID);
    AddTextPrinterParameterizedWithColor(&hud->msgWindow, FONT_SYSTEM, string, 0, 0, 0xff, 0xf0e00, (PrinterCallback_t)0x0);
    String_Delete(string);
    CopyWindowToVram(&hud->msgWindow);
    return;
}

extern WindowTemplate ov96_0221D650;

extern WindowTemplate ov96_0221D658;

void ov96_022148A4(Ov96CourseHud *hud) {
    AddWindow(hud->bgConfig, &hud->nameWindow, &ov96_0221D650);
    AddWindow(hud->bgConfig, &hud->msgWindow, &ov96_0221D658);
    BG_FillCharDataRange(hud->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, hud->heapID);
}

void ov96_022148E8(Ov96CourseHud *hud, s32 arg1, s32 arg2) {
    if ((arg2 == ov96_021E5F24(hud->course)) && (hud->unk_44 != arg1)) {
        hud->unk_44 = arg1;
    }
}

u32 ov96_02214904(Ov96CourseHud *hud) {
    return (u32)hud->unk_44 & 0xff;
}

s32 ov96_021EB52C(void *, s32, s32);

void ov96_0221490C(Ov96CourseHud *hud, s32 arg1) {
    u8 v0;

    if (arg1 <= 0x384) {
        Sprite_SetDrawFlag(hud->digits[0], 0);
        Sprite_SetDrawFlag(hud->digits[1], 0);
        ov96_021EB52C(hud->unk_4C, 1, 0);
        if (hud->timeUpShown == 0) {
            ov96_02214854(hud);
            hud->timeUpShown = 1;
        }
    } else {
        v0 = (u8)(arg1 / 30);
        Sprite_SetAnimCtrlSeq(hud->digits[0], (u8)((s32)v0 / 10) + 1);
        Sprite_SetAnimCtrlSeq(hud->digits[1], (u8)((s32)v0 % 10) + 1);
    }
}
