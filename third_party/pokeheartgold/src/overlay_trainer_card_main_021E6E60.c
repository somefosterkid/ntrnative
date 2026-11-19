#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "render_window.h"
#include "trainer_card_main.h"
#include "yes_no_prompt.h"

extern u32 ov51_021E7F48;

void ov51_021E6E60(TrainerCardMain *cardMain) {
    YesNoPrompt *pYVar1;
    u32 uVar2;

    uVar2 = 0;
    do {
        AddWindow(cardMain->bgConfig, &cardMain->windows[uVar2], (WindowTemplate *)(&ov51_021E7F48 + uVar2 * 2));
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 0xe);
    BG_FillCharDataRange(cardMain->bgConfig, GF_BG_LYR_SUB_3, 0, 1, 0);
    BG_FillCharDataRange(cardMain->bgConfig, GF_BG_LYR_MAIN_CNT, 0, 1, 0);
    LoadUserFrameGfx2(cardMain->bgConfig, GF_BG_LYR_MAIN_CNT, 0x3e1, 0xd, cardMain->frame, HEAP_ID_25);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_25);
    pYVar1 = YesNoPrompt_Create(HEAP_ID_25);
    cardMain->yesNoPrompt = pYVar1;
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, 7);
    ScheduleBgTilemapBufferTransfer(cardMain->bgConfig, 4);
    return;
}

void ov51_021E6EF0(TrainerCardMain *cardMain) {
    u16 v0;

    YesNoPrompt_Destroy(cardMain->yesNoPrompt);
    v0 = 0;
    do {
        RemoveWindow(&cardMain->windows[v0]);
        v0 += 1;
    } while ((u32)v0 < 0xEU);
}
