#include "global.h"
#include "overlay_102_easy_chat_app.h"
struct EasyChatApp;

s32 ov102_021E7A58(void *);

s32 ov102_021E8820(void *, void *);

s32 ov102_021E8A34(void *, void *);

void ov102_021E8B18(struct EasyChatApp *arg0, void *arg1);

void ov102_021E8B18(EasyChatApp *arg0, void *arg1) {
    if (ov102_021E7A58(arg0) == 0) {
        if (*((s32 *)&arg0->unk_74) == 0) {
            ov102_021E8820(arg0, arg1);
            return;
        }
        ov102_021E8A34(arg0, arg1);
    }
}
