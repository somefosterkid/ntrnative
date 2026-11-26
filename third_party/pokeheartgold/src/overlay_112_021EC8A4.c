#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokewalker.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021EA010(void *, s32, String *, s32, s32);

void ov112_021EC8A4(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2) {
    String *string;
    String *v0;
    String *v1;

    if ((arg2 == 5) || (ov112_021ECA88(arg0) == -1)) {
        string = NewString_ReadMsgData(arg0->msgData, 0x76);
        ov112_021EA010(arg0, 0, string, 0, 0x10200);
        String_Delete(string);
        FillWindowPixelBuffer(&arg0->windows2[1], 0U);
        CopyWindowToVram(&arg0->windows2[1]);
        return;
    }
    v0 = NewString_ReadMsgData(arg0->msgData, ov112_021ECA88(arg0) + 0x5B);
    ov112_021EA010(arg0, 0, v0, 0, 0x10200);
    String_Delete(v0);
    v1 = NewString_ReadMsgData(arg0->msgData, ov112_021ECA88(arg0) + 0x77);
    ov112_021EA010(arg0, 1, v1, 0, 0x10200);
    String_Delete(v1);
}

s32 ov112_021EC950(Ov112PokewalkerApp *arg0) {
    s32 courseNo;
    s32 v0;

    arg0->unk_1EC54 = 0;
    arg0->unk_1EC50 = 0;
    MI_CpuFill8(arg0->unk_1EC58, 0xFFU, 0x1EU);
    v0 = 0;
    courseNo = 0;
    do {
        if (Pokewalker_CourseIsUnlocked(arg0->pokewalker, courseNo) != 0) {
            arg0->unk_1EC58[v0] = (s8)courseNo;
            v0 += 1;
        }
        courseNo += 1;
    } while (courseNo < 0x1B);
    return v0;
}
