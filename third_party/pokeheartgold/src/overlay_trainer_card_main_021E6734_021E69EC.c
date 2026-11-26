#include "global.h"

#include "trainer_card_main.h"

void ov51_021E69EC(TrainerCardMain *cardMain);

void ov51_021E69EC(TrainerCardMain *cardMain) {
    MtxFx22 MStack_18;
    MStack_18._00 = FX_Inv(cardMain->scaleX);
    MStack_18._01 = 0;
    MStack_18._10 = 0;
    MStack_18._11 = FX_Inv(cardMain->scaleY);
    OS_WaitVBlankIntr();
    G2S_SetBG2Affine(&MStack_18, 0x80, 0x60, 0, 0);
    G2S_SetBG3Affine(&MStack_18, 0x80, 0x60, 0, 0);
    cardMain->scaleChanged = FALSE;
    return;
}
