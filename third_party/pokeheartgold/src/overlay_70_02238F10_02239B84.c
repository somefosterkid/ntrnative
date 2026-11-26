#include "global.h"
struct GTSApplicationState;

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02239B84(GTSApplicationState *arg0, s32 arg1, u32 arg2);

void ov70_02239B84(GTSApplicationState *arg0, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(arg0->gtsMessageLoader, strno, arg0->genericMessageBuffer);
    FillWindowPixelBuffer(&arg0->bottomInstructionWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->bottomInstructionWindow, 0, 1U, 0xEU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->bottomInstructionWindow, FONT_MESSAGE, arg0->genericMessageBuffer, 0U, 0U, textSpeed, NULL);
}

void ov70_02239BDC(GTSApplicationState *arg0, s32 arg1, u32 arg2);

void ov70_02239BDC(GTSApplicationState *arg0, s32 strno, u32 textSpeed) {
    ReadMsgDataIntoString(arg0->gtsMessageLoader, strno, arg0->genericMessageBuffer);
    FillWindowPixelBuffer(&arg0->confirmationWindow, 0xFU);
    DrawFrameAndWindow2(&arg0->confirmationWindow, 0, 1U, 0xEU);
    arg0->textPrinter = (s32)AddTextPrinterParameterized(&arg0->confirmationWindow, FONT_MESSAGE, arg0->genericMessageBuffer, 0U, 0U, textSpeed, NULL);
}

void ov70_02239C34(Window *arg0, MsgData *arg1, enum Font arg2, s32 arg3);

void ov70_02239C34(Window *window, MsgData *msgData, enum Font fontId, s32 strno) {
    String *string;

    string = NewString_ReadMsgData(msgData, strno);
    AddTextPrinterParameterizedWithColor(window, fontId, string, 0U, 0U, 0U, 0xA0900U, NULL);
    String_Delete(string);
}

void ov70_02239C6C(struct GTSApplicationState *arg0);

void ov70_02239C6C(GTSApplicationState *arg0) {
    BgConfig *bgConfig;

    bgConfig = arg0->bgConfig;
    GfGfxLoader_GXLoadPal(NARC_a_1_0_0, 5, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200U, HEAP_ID_61);
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 0x11, bgConfig, GF_BG_LYR_SUB_1, 0U, 0xA800U, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 0x28, bgConfig, GF_BG_LYR_SUB_1, 0U, 0x600U, 1, HEAP_ID_61);
    FillBgTilemapRect(bgConfig, 5U, 0U, 0U, 0x18U, 0x20U, 8U, 0x11U);
    BgCommitTilemapBufferToVram(bgConfig, 5U);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_1_OFFSET, HEAP_ID_61);
}

void ov70_02239CF8(GTSApplicationState *arg0);

void ov70_02239CF8(GTSApplicationState *arg0) {
    GfGfxLoader_LoadCharData(NARC_a_1_0_0, 0x12, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_61);
    GfGfxLoader_LoadScrnData(NARC_a_1_0_0, 0x29, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_61);
    Sprite_SetDrawFlag(arg0->unk_F10, 1);
}

s32 ov70_022450EC(void *, MsgData *, s32);

void ov70_02239D44(GTSApplicationState *arg0, s32 arg1);

void ov70_02239D44(GTSApplicationState *arg0, s32 arg1) {
    AddWindowParameterized(arg0->bgConfig, &arg0->unk_1198, 4U, 0xDU, 0x13U, 0x10U, 4U, 1U, 1U);
    FillWindowPixelBuffer(&arg0->unk_1198, 0U);
    ov70_022450EC(&arg0->unk_1198, arg0->gtsMessageLoader, arg1);
}

s32 ov70_02239EAC(BgConfig *);

s32 ov70_02239FA4(void *);

s32 ov70_0223A06C(void *);

s32 ov70_0223A0D4(GTSApplicationState *);

s32 ov70_0223A1E4(void *arg0);

void ov70_0223A578(MsgData *, MsgData *, MessageFormat *, Window *, BoxPokemon *, s16 *);

void ov70_0223A72C(MsgData *, Window *, u16 *, Pokemon *, Window *);

s32 ov70_0223A7E4(Pokemon *);

s32 ov70_0223F470(void *, void *, void *, void *, s32, s32, s32);

s32 ov70_0223F864(s32, s32, s32);

s32 ov70_02239D8C(GTSApplicationState *arg0);

s32 ov70_02239D8C(GTSApplicationState *arg0) {
    ov70_0223A1E4(arg0);
    ov70_02239EAC(arg0->bgConfig);
    ov70_02239FA4(arg0);
    ov70_0223A0D4(arg0);
    ov70_0223A06C(arg0);
    ov70_0223A578(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, &arg0->infoWindows[13], Mon_GetBoxMon(&arg0->receivedListing.pokemon), &arg0->receivedListing.criteria.species);
    ov70_0223A72C(arg0->gtsMessageLoader, &arg0->infoWindows[20], arg0->receivedListing.trainerNames, &arg0->receivedListing.pokemon, &arg0->infoWindows[25]);
    ov70_0223F470(arg0->gtsMessageLoader, arg0->speciesMessageLoader, arg0->stringTemplate, &arg0->infoWindows[22], (s32) arg0->receivedListing.requirements.species, (s32) arg0->receivedListing.requirements.gender, ov70_0223F864(arg0->receivedListing.requirements.level, arg0->receivedListing.requirements.level2, 0));
    ov70_0223A7E4(&arg0->receivedListing.pokemon);
    BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_61);
    arg0->currentScreenInstruction = 0;
    return 2;
}
