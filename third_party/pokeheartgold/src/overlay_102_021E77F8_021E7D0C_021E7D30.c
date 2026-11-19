#include "global.h"
struct EasyChatApp;

#include "unk_02005D10.h"
#include "overlay_102_easy_chat_app.h"

void ov102_021E940C(void *, u32);

s32 ov102_021EA238(void *);

u32 ov102_021E7D30(struct EasyChatApp *param_1, u32 param_2);

u32 ov102_021E7D30(EasyChatApp *param_1, u32 param_2) {
    s32 iVar1;

    switch (param_2) {
    case 2:
        PlaySE(0x5dc);
        return 4;
    case 1:
        PlaySE(0x5dc);
        return 3;
    case 0:
        iVar1 = ov102_021EA238(*((void * *)&param_1->unk_14));
        if (iVar1 == 0) {
            return 1;
        }
        PlaySE(0x5dc);
        param_1->categoryCursorPos = 0;
        ov102_021E940C(*((void * *)&param_1->unk_14), 9);
        return 6;
    case 3:
        PlaySE(0x5dc);
        ov102_021E940C(*((void * *)&param_1->unk_14), 0xf);
        break;
    case 4:
        PlaySE(0x5dc);
        param_1->currentWordSlot = 0;
        ov102_021E940C(*((void * *)&param_1->unk_14), 3);
        return 2;
    default:
        break;
    }
    return 1;
}
