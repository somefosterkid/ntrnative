#include "global.h"

#include "message_format.h"
#include "pm_string.h"
#include "overlay_112_sprite_app.h"

extern u16 ov112_021FF2EC;

void ov112_021F1488(Ov112SpriteApp *arg0, u16 *arg1, u8 arg2, u32 arg3);

void ov112_021F1488(Ov112SpriteApp *arg0, u16 *src, u8 arg2, u32 maxsize) {
    s32 v0;
    u32 fieldno;
    String *string;
    s32 v1;
    u16 *v2;

    fieldno = arg2;
    v0 = 1;
    string = String_New(maxsize, arg0->heapID);
    if (*src != 0) {
        v1 = 0;
        if ((s32)maxsize > 0) {
            v2 = src;
loop_3:
            if (*v2 == 0xFFFF) {
                v0 = 0;
            } else {
                v1 += 1;
                v2 += 1;
                if (v1 < (s32)maxsize) {
                    goto loop_3;
                }
            }
        }
    }
    if (v0 != 0) {
        CopyU16ArrayToString(string, &ov112_021FF2EC);
    } else {
        CopyU16ArrayToString(string, src);
    }
    BufferString(arg0->msgFormat, fieldno, string, 2, 1, 2);
    String_Delete(string);
}
