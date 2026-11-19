#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "text.h"
#include "unk_02034354.h"
#include "unk_02035900.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov73_021E6830(void *param_1);
#else
s32 ov73_021E6830();
#endif

s32 ov73_021E71E4(void *, s32, s8);

s32 ov73_021E756C(void *, s32, s32);

s32 ov73_021E75FC(s32);

s32 ov73_021E761C(u8 *param_1);

void ov73_021E705C(void *arg0, s32 arg1, u32 arg2);

void ov73_021E705C(void *arg0, s32 arg1, u32 arg2) {
    switch (arg1) {
    case 2:
        ov73_021E761C(arg0);
        break;
    case 7:
    case 8:
        break;
    case 13:
#if defined(SDK_PORT) || defined(NONMATCHING)
        ov73_021E6830(arg0);
#else
        ov73_021E6830();
#endif
        break;
    case 25:
        if (ov73_021E75FC(*(s32 *)((u8 *)arg0 + 0x4C)) == 0) {
            RemoveTextPrinter((u8) * (s32 *)((u8 *)arg0 + 0x4C));
        }
        ov73_021E71E4(arg0, 1, 2);
        ScheduleBgTilemapBufferTransfer(*(BgConfig **)arg0, 1U);
        ov73_021E756C(arg0, 0xC, 0);
        ov73_021E6830(arg0);
        break;
    case 19:
        if (*(u8 *)((u8 *)arg0 + 0x4A28) != 1) {
            BufferPlayersName(*(MessageFormat **)((u8 *)arg0 + 0x24), 0U, sub_02034818(arg2));
            if (arg2 != sub_0203769C()) {
                ov73_021E6830(arg0);
                if (sub_0203769C() == 0) {
                    *(s32 *)((u8 *)arg0 + 0x4A24) = (s32)(*(s32 *)((u8 *)arg0 + 0x4A24) & (0xFFFF ^ arg2));
                }
                goto block_22;
            }
        }
        return;
    default:
        return;
    }
block_22:
    *(s32 *)((u8 *)arg0 + 0x318) = arg1;
}
