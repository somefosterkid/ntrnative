#include "global.h"

#include "bg_window.h"
#include "trainer_card_main.h"

void ov51_021E66C0(void *arg0);

void ov51_021E6C6C(TrainerCardMain *);

void ov51_021E6D44(BgConfig *, enum GFBgLayer, u8 *);

void ov51_021E6F18(u8 *, Window *, u8 *);

void ov51_021E71D0(TrainerCardMain *, Window *);

void ov51_021E7208(u8 *, Window *, u8 *);

void ov51_021E6734(TrainerCardMain *param_1);

void ov51_021E6734(TrainerCardMain *param_1) {
    ov51_021E66C0(param_1);
    ov51_021E7D44(&param_1->sprites, 0, 0, 1);
    if (param_1->showingBack == 0) {
        ov51_021E6C6C(param_1);
        ov51_021E76EC(param_1, 1);
    } else {
        ov51_021E6D44(param_1->bgConfig, GF_BG_LYR_SUB_3, param_1->signaturePixels);
        ov51_021E76EC(param_1, 0);
    }
    ov51_021E6F18((u8 *)param_1, param_1->windows, (u8 *)param_1->card);
    ov51_021E7208((u8 *)param_1, param_1->windows, (u8 *)param_1->card);
    ov51_021E71D0(param_1, param_1->windows);
    return;
}
