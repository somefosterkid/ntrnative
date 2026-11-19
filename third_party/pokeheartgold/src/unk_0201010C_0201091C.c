#include "global.h"

#include "screen_fade_fx.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "screen_fade_shape_globals.h"

s32 sub_0200FCDC(u16);

s32 sub_02012B1C(ScreenFade *, UnkStruct_0201006C *);

s32 sub_02012B80(ScreenFade *);

extern const UnkStruct_0200F898 _020F5D98[];

BOOL FadeFunc_40(void *param0);

extern const UnkStruct_0200F898 _020F5DB0[];
BOOL FadeFunc_41(void *param0);

BOOL FadeFunc_40(void *ptr) {
    ScreenFade *param0 = ptr;
    if (param0->state == FADE_IDLE) {
        _0210F64C.windowFadeOut.unk_0C.shape = _020F5D98;
        sub_0200FCDC(param0->color);
        sub_02012B1C(param0, &_0210F64C.windowFadeOut);
        param0->direction = FADE_OUT;
        param0->method = FADE_BY_WINDOW;
        return 0;
    }
    return sub_02012B80(param0);
}

BOOL FadeFunc_41(void *ptr) {
    ScreenFade *param0 = ptr;
    if (param0->state == FADE_IDLE) {
        _0210F64C.windowFadeIn.unk_0C.shape = _020F5DB0;
        sub_0200FCDC(param0->color);
        sub_02012B1C(param0, &_0210F64C.windowFadeIn);
        param0->direction = FADE_IN;
        param0->method = FADE_BY_WINDOW;
        return 0;
    }
    return sub_02012B80(param0);
}
