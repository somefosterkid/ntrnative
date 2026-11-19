#include "global.h"

#include "overlay_59_juice_feed_app.h"
#include "text.h"

extern s32 ov59_0223C99C[];

void ov59_0223AD84(Ov59JuiceFeedApp *arg0) {
    s32 i;

    ov59_0223AEB0(arg0, 2);
    ReadMsgDataIntoString(arg0->msgData, 0, arg0->unk_68);
    AddTextPrinterParameterizedWithColor(&arg0->windows[3], FONT_SYSTEM, arg0->unk_68, 0U, 0U, 0xFFU, 0x10200U, 0L);
    ScheduleWindowCopyToVram(&arg0->windows[3]);

    for (i = 0; i < 5; i++) {
        ReadMsgDataIntoString(arg0->msgData, ov59_0223C99C[i] + 5, arg0->unk_68);
        AddTextPrinterParameterizedWithColor(&arg0->windows[i + 5], FONT_SYSTEM, arg0->unk_68, 4U, 0U, 0xFFU, 0x10200U, 0L);
        ScheduleWindowCopyToVram(&arg0->windows[i + 5]);
    }
    ov59_0223BC88(arg0, arg0->selected);
}

void ov59_0223AE20(Ov59JuiceFeedApp *param_1) {
    Party *party;
    s32 count;
    Pokemon *mon;
    s32 found;
    s32 i;
    party = SaveArray_Party_Get(param_1->saveData);
    param_1->party = party;
    count = Party_GetCount(party);
    found = 0;
    i = 0;
    if (count > 0) {

        do {
            mon = Party_GetMonByIndex(param_1->party, i);
            if (GetMonData(mon, 0x4c, NULL) == 0) {
                ov59_0223BA64(&param_1->mons[found++], param_1->party, mon, i, param_1->heapId);
            }
            i++;
        } while (i < count);
    }
    param_1->monCount = found;
}
