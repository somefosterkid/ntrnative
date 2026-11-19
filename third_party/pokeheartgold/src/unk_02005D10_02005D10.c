#include "global.h"

#include "sound.h"

s32 sub_02005DA0(s32, enum SoundHandleNo);

s32 sub_02006C14(enum SoundHandleNo, s32, s32, s32, u16, u8);

s32 sub_02005D10(u8 arg0);

s32 sub_02005D10(u8 unk) {
    s32 v0;
    enum SoundHandleNo v1;

    v1 = GF_GetSndHandleByPlayerNo(GF_GetPlayerNoBySeq(unk));
    v0 = sub_02006C14(v1, -1, -1, -1, unk, 1);
    if (v0 == 0) {
        sub_02004920((u16)unk);
    }
    sub_02005DA0(unk, v1);
    return v0;
}
