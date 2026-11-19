#include "global.h"
#include "overlay_102_easy_chat_app.h"
struct EasyChatApp;

void ov102_021E8D58(struct EasyChatApp *arg0);

void ov102_021E8D58(EasyChatApp *arg0) {
    *((s32 *)&arg0->taskFunc) = 0;
}

s32 ov102_021E940C(void *, s32);

s32 ov102_021E9464(void *);

void ov102_021E8D60(struct EasyChatApp *arg0, s32 *arg1);

void ov102_021E8D60(EasyChatApp *arg0, s32 *arg1) {
    s32 v0;

    v0 = *arg1;
    switch (v0) {
    case 0:
        ov102_021E940C(*((void **)&arg0->unk_14), 2);
        *arg1 += 1;
        return;
    case 1:
        if (ov102_021E9464(*((void **)&arg0->unk_14)) != 0) {
            ov102_021E8D58(arg0);
        }
        return;
    }
}
