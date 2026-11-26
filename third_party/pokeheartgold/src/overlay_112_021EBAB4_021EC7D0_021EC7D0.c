#include "global.h"

#include "constants/heap.h"

#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_112_pokewalker_app.h"
#include "string_util.h"

void ov112_021E9FA4(void *, s32, String *, u32, u32);

void ov112_021EC7D0(Ov112PokewalkerApp *param_1) {
    Ov112PokewalkerApp *p = param_1;
    String *str;
    s32 num;
    s32 i;
    s32 v;

    for (i = 0; i < 5; i++) {
        v = p->unk_1EC58[p->unk_1EC54 * 5 + i];
        if (v >= 0) {
            str = NewString_ReadMsgData(p->msgData, v + 0x5B);
        } else {
            str = NewString_ReadMsgData(p->msgData, 0x76);
        }
        ov112_021E9FA4(param_1, i, str, 4, 0xF0100);
        String_Delete(str);
    }
    BufferIntegerAsString(p->msgFormat, 10, p->unk_1EC54 + 1, 1, PRINTING_MODE_LEFT_ALIGN, 1);
    num = ov112_021EC7B8(param_1);
    BufferIntegerAsString(p->msgFormat, 11, num, 1, PRINTING_MODE_LEFT_ALIGN, 1);
    str = ReadMsgData_ExpandPlaceholders(p->msgFormat, p->msgData, 0x51, HEAP_ID_154);
    ov112_021E9FA4(param_1, 7, str, 0, 0x10E00);
    String_Delete(str);
    ov112_021EC8A4(param_1, p->unk_1EC54, p->unk_1EC50);
}
