#include "global.h"
#include "overlay_102_easy_chat_app.h"
struct EasyChatApp;

s32 ov102_021E7A4C(void *, s32 *);

u32 ov102_021E7B18(void *);

u32 ov102_021E7BF8(void *);

s32 ov102_021E8D50(void *, s32 *);

s32 ov102_021E9464(void *);

extern s32 ov102_021E8240;

extern s32 ov102_021E8D94;

extern s32 ov102_021E8E84;

s32 ov102_021E7C84(struct EasyChatApp *arg0, u32 *arg1);

s32 ov102_021E7C84(EasyChatApp *arg0, u32 *arg1) {
    u32 v0;

    v0 = *arg1;
    switch (v0) {
    case 0:
        if (ov102_021E9464(*((void **)&arg0->unk_14)) != 0) {
            if (*((s32 *)&arg0->unk_74) == 0) {
                *arg1 = ov102_021E7B18(arg0);
            } else {
                *arg1 = ov102_021E7BF8(arg0);
            }
        }
        break;
    case 1:
        *arg1 = 3;
        ov102_021E8D50(arg0, &ov102_021E8D94);
        break;
    case 2:
        *arg1 = 3;
        ov102_021E8D50(arg0, &ov102_021E8E84);
        break;
    case 3:
        *arg1 = 0;
        break;
    case 4:
        if (ov102_021E9464(*((void **)&arg0->unk_14)) != 0) {
            ov102_021E7A4C(arg0, &ov102_021E8240);
        }
        break;
    }
    return 0;
}
