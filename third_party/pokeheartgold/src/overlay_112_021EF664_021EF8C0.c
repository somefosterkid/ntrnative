#include "global.h"

#include "message_format.h"
#include "overlay_112_pokewalker_app.h"
#include "pm_string.h"
#include "pokewalker.h"
#include "screen_fade.h"

s32 ov112_021EF8C0(void);

s32 ov112_021EF8C0(void) {
    if (IsPaletteFadeFinished() == 1) {
        return 3;
    }
    return 2;
}

u8 ov112_021EA08C(Ov112PokewalkerApp *, s32, s32);

s32 ov112_021EF8D4(Ov112PokewalkerApp *arg0);

s32 ov112_021EF8D4(Ov112PokewalkerApp *arg0) {
    String *string;

    BufferPokewalkerCourseName(arg0->msgFormat, 1U, (u32)(u8)sub_02032718(arg0->pokewalker));
    string = String_New(0xCU, HEAP_ID_154);
    CopyU16ArrayToStringN(string, arg0->unk_9D54, 0xBU);
    BufferString(arg0->msgFormat, 1U, string, (s32)((u32)(arg0->unk_9D51 << 0x19) >> 0x1E), 1, arg0->unk_1090);
    String_Delete(string);
    arg0->unk_1E524 = ov112_021EA08C(arg0, 0, 0x2C);
    arg0->unk_C = 4;
    return 0x23;
}
