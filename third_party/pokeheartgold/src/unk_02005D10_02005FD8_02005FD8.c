#include "global.h"

#include "sound.h"
#include "unk_02005D10.h"

s32 sub_02005728(s32);

s32 sub_02005EEC(void);

void sub_02005FD8(void);

void sub_02005FD8(void) {
    s32 v2;
    u8 *v1;
    u8 *v0;

    v1 = GF_SdatGetAttrPtr(0x10U);
    v0 = GF_SdatGetAttrPtr(0x11U);
    NNS_SndPlayerStopSeq(GF_GetSoundHandle(7), 0);
    sub_02005EEC();
    v2 = 0;
    do {
        sub_0200615C(v2 + 3, 0);
        v2 += 1;
    } while (v2 < 4);
    sub_02006300(0);
    if (*v1 == 1) {
        sub_02005728(0xE);
    }
    if (*v0 == 1) {
        sub_02005728(0xF);
    }
}
