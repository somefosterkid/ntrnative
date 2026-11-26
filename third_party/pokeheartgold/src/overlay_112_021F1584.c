#include "global.h"
#include "message_format.h"
#include "overlay_112_sprite_app.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov112_021F1488(s32, u16 *, u8, s32);
#else
s32 ov112_021F1488(s32, u16 *, s32, s32);
#endif

s32 ov112_021F1548(s32, s32, s32);

void ov112_021F1584(s32 arg0, u16 *arg1, s32 arg2, s32 arg3);

void ov112_021F15CC(Ov112SpriteApp *arg0, u32 arg1, s32 arg2, u32 arg3, enum PrintingMode arg4);

void ov112_021F1584(s32 arg0, u16 *arg1, s32 arg2, s32 arg3) {
    s32 v0;
    s32 v1;
    u16 *v2;

    v1 = 1;
    if (*arg1 != 0) {
        v0 = 0;
        v2 = arg1;
loop_2:
        if (*v2 == 0xFFFF) {
            v1 = 0;
        } else {
            v0 += 1;
            v2 += 1;
            if (v0 < 0xB) {
                goto loop_2;
            }
        }
    }
    if (v1 != 0) {
        ov112_021F1548(arg0, arg2, arg3);
        return;
    }
    ov112_021F1488(arg0, arg1, arg2, 0xB);
}

void ov112_021F15CC(Ov112SpriteApp *arg0, u32 idx, s32 num, u32 numDigits, enum PrintingMode strconvmode) {
    BufferIntegerAsString(*((MessageFormat * *)&arg0->msgFormat), idx, num, numDigits, strconvmode, 1);
}
