#include "global.h"
struct TrainerCardMain;

#include "bg_window.h"
#include "trainer_card_main.h"

void ov51_021E71D0(struct TrainerCardMain *param_1, Window *param_2);

void ov51_021E71D0(TrainerCardMain *param_1, Window *param_2) {
    Window *window;
    s32 iVar1;

    if (*((s32 *)&param_1->showingBack) != 0) {
        iVar1 = 7;
        window = param_2 + 7;
        do {
            CopyWindowToVram(window);
            iVar1 = iVar1 + 1;
            window = window + 1;
        } while (iVar1 < 0xb);
        return;
    }
    iVar1 = 0;
    do {
        CopyWindowToVram(param_2);
        iVar1 = iVar1 + 1;
        param_2 = param_2 + 1;
    } while (iVar1 < 7);
    return;
}
