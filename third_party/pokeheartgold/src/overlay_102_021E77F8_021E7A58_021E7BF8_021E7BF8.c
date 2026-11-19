#include "global.h"
#include "overlay_102_easy_chat_app.h"
struct EasyChatApp;

u32 ov102_021E7AB8(void *, u32);

u32 ov102_021E7BD8(void *);

u32 ov102_021E7BE8(void *);

void ov102_021E940C(void *, u32);

u32 ov102_021E7BF8(struct EasyChatApp *param_1);

u32 ov102_021E7BF8(EasyChatApp *param_1) {
    s32 uVar1;

    if (*((s32 *)&param_1->type) == 0) {
        uVar1 = ov102_021E7BD8(param_1);
    } else {
        uVar1 = ov102_021E7BE8(param_1);
    }
    switch (uVar1) {
    case 0:
        param_1->unk_4C = 1;
        ov102_021E940C(*((void * *)&param_1->unk_14), 0xf);
        uVar1 = ov102_021E7AB8(param_1, 1);
        return uVar1;
    case 1:
        param_1->unk_4C = 2;
        ov102_021E940C(*((void * *)&param_1->unk_14), 0xf);
        uVar1 = ov102_021E7AB8(param_1, 2);
        return uVar1;
    case 2:
    case 3:
        param_1->categoryCursorPos = 0;
        *((s16 *)&param_1->currentWordSlot) = uVar1 + -2;
        param_1->unk_4C = 0;
        ov102_021E940C(*((void * *)&param_1->unk_14), 0xf);
        uVar1 = ov102_021E7AB8(param_1, 0);
        return uVar1;
    default:
        return 0;
    }
}
