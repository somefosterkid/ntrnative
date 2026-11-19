#include "global.h"

#include "unk_02005D10.h"
#include "overlay_102_pmsinput_work.h"

s32 ov102_021E839C();

s32 ov102_021E83E4(void *);

s32 ov102_021E8458(void *);

s32 ov102_021E85A8(void *, s32, s32);

s32 ov102_021E874C(void *, void *);

s32 ov102_021E940C(void *, s32);

extern s32 ov102_021E7AA4;

extern s32 ov102_021E87B4;

void ov102_021E848C(void *arg0, s32 *arg1);

void ov102_021E848C(void *arg0, s32 *arg1) {
    s32 v0;
    s32 v1;
    s32 v2;

    v0 = ov102_021E839C();
    switch (v0) {
    case 1:
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0xA);
        *((s32 **)&((PMSInputWork *)arg0)->mainProc) = &ov102_021E7AA4;
        *arg1 = 1;
        return;
    case 2:
        PlaySE(0x5E4U);
        ((PMSInputWork *)arg0)->categoryMode = (u8)(((PMSInputWork *)arg0)->categoryMode ^ 1);
        *((s16 *)&((PMSInputWork *)arg0)->categoryPosSaved) = 0;
        ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0x1B);
        *arg1 = 2;
        return;
    default:
        if (((PMSInputWork *)arg0)->categoryMode == 0) {
            v1 = ov102_021E83E4(arg0);
            if (v1 >= 0) {
                if (ov102_021E85A8(*((void **)&((PMSInputWork *)arg0)->dwk), ((PMSInputWork *)arg0)->categoryMode, (u16)v1) == 0) {
                    PlaySE(0x5F2U);
                    return;
                }
                *((s16 *)&((PMSInputWork *)arg0)->categoryPosSaved) = (s16)v1;
                goto block_14;
            }
        } else {
            v2 = ov102_021E8458(arg0);
            if (v2 >= 0) {
                if (ov102_021E85A8(*((void **)&((PMSInputWork *)arg0)->dwk), ((PMSInputWork *)arg0)->categoryMode, (u16)v2) == 0) {
                    PlaySE(0x5F2U);
                    return;
                }
                *((s16 *)&((PMSInputWork *)arg0)->categoryPosSaved) = (s16)v2;
block_14:
                PlaySE(0x5DCU);
                ov102_021E874C(arg0 + 0x54, arg0);
                *((s32 **)&((PMSInputWork *)arg0)->mainProc) = &ov102_021E87B4;
                ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0xB);
                *arg1 = 1;
            } else {
                return;
            }
        }
        break;
    }
}
