#include "global.h"

#include "unk_02005D10.h"
#include "overlay_102_pmsinput_work.h"

s32 ov102_021E85A8(void *, s32, s32);

s32 ov102_021E85E8(void *);

s32 ov102_021E874C(void *, void *);

s32 ov102_021E940C(void *, s32);

extern s32 ov102_021E7AA4;

extern s32 ov102_021E87B4;

void ov102_021E8294(void *arg0, s32 *arg1);

void ov102_021E8294(void *arg0, s32 *arg1) {
    s32 arg2;
    u8 *new_var;
    s32 temp_r1;
    u16 temp_r0;
    temp_r1 = *((s32 *)&((PMSInputWork *)arg0)->keyRepeat);
    new_var = (u8 *)arg0;
    if ((((temp_r1 == 0) && ((*((u8 *)(new_var + 0x6B))) != 0)) || ((temp_r1 == 1) && ((*((u8 *)(new_var + 0x6B))) != 1))) || (temp_r0 = *((u16 *)(new_var + 0x30)), (4 & temp_r0) != 0)) {
        PlaySE(0x5E4U);
        *((s32 *)&((PMSInputWork *)arg0)->keyRepeat) = 4;
        ((PMSInputWork *)arg0)->categoryMode = (u8)((((PMSInputWork *)arg0)->categoryMode) ^ 1);
        ((PMSInputWork *)arg0)->categoryPosSaved = 0U;
        ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0x1B);
        *arg1 = 2;
        return;
    }
    if (2 & temp_r0) {
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0xA);
        *((s32 **)&((PMSInputWork *)arg0)->mainProc) = &ov102_021E7AA4;
        *arg1 = 1;
        return;
    }
    if (temp_r0 & 1) {
        arg2 = ((PMSInputWork *)arg0)->categoryPosSaved;
        if (arg2 == 0xFE) {
            PlaySE(0x5DCU);
            ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0xA);
            *((s32 **)&((PMSInputWork *)arg0)->mainProc) = &ov102_021E7AA4;
            *arg1 = 1;
            return;
        }
        if (ov102_021E85A8(*((void **)&((PMSInputWork *)arg0)->dwk), ((PMSInputWork *)arg0)->categoryMode, arg2) != 0) {
            PlaySE(0x5DCU);
            ov102_021E874C(arg0 + 0x54, arg0);
            *((s32 **)&((PMSInputWork *)arg0)->mainProc) = &ov102_021E87B4;
            ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0xB);
            *arg1 = 1;
            return;
        }
        PlaySE(0x5F2U);
        return;
    }
    if (ov102_021E85E8(arg0) != 0) {
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((PMSInputWork *)arg0)->vwk), 0x11);
    }
}
