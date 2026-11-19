#include "global.h"

#include "overlay_85.h"
#include "unk_02035900.h"

s32 ov85_021E9324();

s32 ov85_021E9EA4(void);

s32 ov85_021E9FCC(s32);

s32 ov85_021E92F8(s32 arg0, s32 arg1) {
    ov85_021E9324();
    if (sub_0203769C() == 0) {
        if (ov85_021E9EA4() != 1) {
            ov85_021E9FCC(arg0);
        }
    } else {
        ov85_021E9FCC(arg0);
    }
    return arg1;
}
