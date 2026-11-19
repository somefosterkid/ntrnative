#include "global.h"

#include "unk_020932E0.h"
#include "yes_no_prompt.h"

void sub_02093908(UnkStruct_020932E0 *arg0) {
    enum YesNoResponse v0;

    v0 = YesNoPrompt_HandleInput(arg0->unk_8);
    switch (v0) {
    case YESNORESPONSE_YES:
        sub_020950F8(&arg0->unk_4660, 1);
        YesNoPrompt_Reset(arg0->unk_8);
        arg0->unk_14 = 6;
        return;
    case YESNORESPONSE_NO:
        sub_020950F8(&arg0->unk_4660, 1);
        YesNoPrompt_Reset(arg0->unk_8);
        sub_02094F5C(arg0, arg0->unk_46B0);
        sub_020950C0(arg0);
        sub_02095794();
        sub_020956B8(arg0);
        arg0->unk_14 = 0;
        return;
    }
}
