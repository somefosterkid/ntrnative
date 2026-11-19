#include "global.h"

#include "overlay_75.h"
#include "screen_fade.h"
#include "unk_02037C94.h"
#include "unk_0203A3B0.h"

struct UnkStruct_ov75_02249258 {
    u8 unk_0[0x4];
    void *unk_4;
    s32 unk_8;
    s32 unk_C;
    u8 unk_10[0x78];
    void *unk_88;
};

s32 ov75_02247878(void *);

s32 ov75_0224921C(void *arg0);

s32 ov75_0224921C(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 8) != 0x1E) {
        sub_0203957C();
    }
    sub_0203A914();
    ov75_02247878(arg0);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_116);
    *(s32 *)((u8 *)arg0 + 8) = 0;
    return 1;
}

YesNoPrompt *ov75_02247890(BgConfig *, s32, s32);

s32 ov75_02249258(UnkStruct_ov75_02249258 *arg0) {
    arg0->unk_88 = ov75_02247890(arg0->unk_4, 0x234, 0);
    arg0->unk_8 = (s32) arg0->unk_C;
    return 0;
}
