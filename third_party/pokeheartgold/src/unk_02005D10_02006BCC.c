#include "global.h"

#include "sound.h"
#include "sound_02004A44.h"
#include "sound_radio.h"

s32 sub_02006B84(void);

s32 sub_02006BB0(s32);

s32 IsFanfarePlaying(void);

void sub_02006C04(void);

s32 IsFanfarePlaying(void) {
    u8 a0;

    GF_SdatGetAttrPtr(0xEU);
    if (sub_02006B84() == 1) {
        return 1;
    }
    sub_02006BB0(0);
    a0 = GF_GetPlayerNoBySeq(GF_GetCurrentPlayingBGM());
    if (a0 != 0xFF) {
        sub_020053A8(a0, 0U);
    }
    SndRadio_PausePlayer(0);
    return 0;
}

void sub_02006C04(void) {
    u16 *puVar1;

    puVar1 = GF_SdatGetAttrPtr(0xe);
    *puVar1 = 0xf;
    return;
}
