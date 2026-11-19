#include "global.h"

#include "bg_window.h"
#include "render_window.h"
#include "text.h"
#include "trainer_card_main.h"
#include "yes_no_prompt.h"

void ov51_021E77A0(TrainerCardMain *cardMain, int arg1) {
    if (arg1 == 0) {
        DrawFrameAndWindow2(&cardMain->windows[11], 1, 0x3E1U, 0xDU);
    }
    FillWindowPixelBuffer(&cardMain->windows[11], 0xFU);
    cardMain->textPrinterId = AddTextPrinterParameterizedWithColor(&cardMain->windows[11], FONT_MESSAGE, cardMain->promptStrings[arg1], 0U, 0U, (u32)cardMain->textSpeed, 0x1020FU, NULL);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, 4U);
}

void ov51_021E7804(TrainerCardMain *cardMain, int unused) {
    YesNoPromptTemplate YStack_20;

    MI_CpuFill8(&YStack_20, 0, sizeof(YesNoPromptTemplate));
    YStack_20.bgConfig = cardMain->bgConfig;
    YStack_20.bgId = 4;
    YStack_20.tileStart = 0x3a5;
    YStack_20.plttSlot = 0xb;
    YStack_20.x = 0x1a;
    YStack_20.y = 6;
    YStack_20.ignoreTouchFlag = cardMain->inTouchMode;
    YStack_20.initialCursorPos = 0;
    YesNoPrompt_InitFromTemplate(cardMain->yesNoPrompt, &YStack_20);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, 4);
    return;
}
