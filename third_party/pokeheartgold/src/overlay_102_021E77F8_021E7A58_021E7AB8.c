#include "global.h"
struct EasyChatApp;

#include "unk_02005D10.h"
#include "overlay_102_easy_chat_app.h"

void ov102_021E940C(void *, u32);

u32 ov102_021E7AB8(struct EasyChatApp *param_1, u32 param_2);

u32 ov102_021E7AB8(EasyChatApp *param_1, u32 param_2) {
    switch (param_2) {
    case 2:
        PlaySE(0x5dc);
        return 2;
    case 1:
        PlaySE(0x5dc);
        return 1;
    case 0:
        PlaySE(0x5dc);
        param_1->categoryCursorPos = 0;
        ov102_021E940C(*((void * *)&param_1->unk_14), 9);
        return 4;
    case 3:
        PlaySE(0x5dc);
        ov102_021E940C(*((void * *)&param_1->unk_14), 0xf);
    }
    return 0;
}
