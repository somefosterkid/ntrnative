#include "global.h"

#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "trainer_card_main.h"

void ov51_021E6038(TrainerCardMain *cardMain) {
    s32 v0;

    v0 = 0;
    do {
        String_Delete(cardMain->labelStrings[v0]);
        v0 += 1;
    } while (v0 < 0xE);
    String_Delete(cardMain->promptStrings[1]);
    String_Delete(cardMain->promptStrings[0]);
    String_Delete(cardMain->buttonStrings[2]);
    String_Delete(cardMain->buttonStrings[1]);
    String_Delete(cardMain->buttonStrings[0]);
    String_Delete(cardMain->colonString);
    String_Delete(cardMain->numberString);
    String_Delete(cardMain->formattedString);
    String_Delete(cardMain->unk_33C8);
    DestroyMsgData(cardMain->msgData);
    FontID_Release(FONT_UNK_4);
}
