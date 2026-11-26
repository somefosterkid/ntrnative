#include "global.h"
struct EasyChatApp;

#include "unk_02005D10.h"
#include "overlay_102_easy_chat_app.h"

s32 ov102_021E839C(void *param_1);

u32 ov102_021E896C(void *);

s32 ov102_021E8CA4(void *);

s32 ov102_021E940C(void *, s32);

void ov102_021E8A34(struct EasyChatApp *arg0, s32 *arg1);

void ov102_021E8A34(EasyChatApp *arg0, s32 *arg1) {
    s32 v0;
    u32 v1;

    v0 = ov102_021E839C(arg0);
    switch (v0) {
    case 1:
        PlaySE(0x5DCU);
        ov102_021E940C(*((void **)&arg0->unk_14), 0xC);
        arg0->nextMainFunc = ov102_021E8240;
        *arg1 = 3;
        return;
    case 2:
        PlaySE(0x5E4U);
        *((s16 *)&arg0->categoryCursorPos) = 0;
        arg0->mode = (u8)(arg0->mode ^ 1);
        ov102_021E940C(*((void **)&arg0->unk_14), 0xC);
        arg0->nextMainFunc = ov102_021E8240;
        *arg1 = 3;
        return;
    default:
        v1 = ov102_021E896C(arg0);
        switch (v1) {
        case 5:
            PlaySE(0x5DCU);
            ov102_021E8CA4(arg0);
            arg0->nextMainFunc = ov102_021E7AA4;
            ov102_021E940C(*((void **)&arg0->unk_14), 0xD);
            *arg1 = 3;
            return;
        case 1:
            PlaySE(0x5DCU);
            ov102_021E940C(*((void **)&arg0->unk_14), 0x12);
            return;
        case 2:
            PlaySE(0x5DCU);
            ov102_021E940C(*((void **)&arg0->unk_14), 0x13);
            *arg1 = 1;
            return;
        case 3:
            PlaySE(0x5DCU);
            ov102_021E940C(*((void **)&arg0->unk_14), 0x13);
            *arg1 = 2;
        default:
            return;
        }
        break;
    }
}
