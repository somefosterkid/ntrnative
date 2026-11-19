#include "global.h"

#include "unk_02005D10.h"
#include "overlay_102_easy_chat_app.h"

s32 ov102_021E8B40(void *, u16);

s32 ov102_021E8C3C(void *);

s32 ov102_021E8C64(void *);

s32 ov102_021E8CA4(void *);

s32 ov102_021E940C(void *, s32);

extern s32 ov102_021E7AA4;

extern s32 ov102_021E8240;

void ov102_021E8820(void *arg0, s32 *arg1);

void ov102_021E8820(void *arg0, s32 *arg1) {
    s32 temp_r0;
    s32 var_r0;
    u16 temp_r1;

    temp_r0 = *((s32 *)&((EasyChatApp *)arg0)->touchedButtonIndex);
    if (((temp_r0 == 0) && (((EasyChatApp *)arg0)->mode != 0)) || ((temp_r0 == 1) && (((EasyChatApp *)arg0)->mode != 1)) || (4 & ((EasyChatApp *)arg0)->pressedKeys)) {
        PlaySE(0x5E4U);
        *((s32 *)&((EasyChatApp *)arg0)->touchedButtonIndex) = 4;
        *((s16 *)&((EasyChatApp *)arg0)->categoryCursorPos) = 0;
        ((EasyChatApp *)arg0)->mode = (u8)(((EasyChatApp *)arg0)->mode ^ 1);
        ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0xC);
        *((s32 **)&((EasyChatApp *)arg0)->nextMainFunc) = &ov102_021E8240;
        *arg1 = 3;
        return;
    }
    if (temp_r0 == 2) {
        var_r0 = ov102_021E8C3C(arg0 + 0x54);
    } else if (temp_r0 == 3) {
        var_r0 = ov102_021E8C64(arg0 + 0x54);
    } else {
        var_r0 = ov102_021E8B40(arg0 + 0x54, ((EasyChatApp *)arg0)->pressedKeysRepeatable);
    }
    switch (var_r0) {
    case 1:
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0x12);
        return;
    case 2:
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0x13);
        *arg1 = 1;
        return;
    case 3:
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0x13);
        *arg1 = 2;
        return;
    default:
        temp_r1 = ((EasyChatApp *)arg0)->pressedKeys;
        if (2 & temp_r1) {
            PlaySE(0x5DCU);
            ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0xC);
            *((s32 **)&((EasyChatApp *)arg0)->nextMainFunc) = &ov102_021E8240;
            *arg1 = 3;
            return;
        }
        if (1 & temp_r1) {
            if (*((u16 *)&((EasyChatApp *)arg0)->unk_60.unk_00) != 0) {
                PlaySE(0x5DCU);
                ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0xC);
                *((s32 **)&((EasyChatApp *)arg0)->nextMainFunc) = &ov102_021E8240;
                *arg1 = 3;
                return;
            }
            PlaySE(0x5DCU);
            *((u16 *)&((EasyChatApp *)arg0)->unk_60.unk_02) = 0xFFFF;
            ov102_021E8CA4(arg0);
            *((s32 **)&((EasyChatApp *)arg0)->nextMainFunc) = &ov102_021E7AA4;
            ov102_021E940C(*((void **)&((EasyChatApp *)arg0)->unk_14), 0xD);
            *arg1 = 3;
        } else {
            return;
        }
        break;
    }
}
