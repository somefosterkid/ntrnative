#include "global.h"

#include "screen_fade_fx.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "screen_fade_shape_globals.h"

s32 sub_0200FCDC(u16);

s32 sub_0201289C(ScreenFade *, UnkStruct_0200FE6C *);

s32 sub_020128E0(ScreenFade *);

extern const UnkStruct_0200F898 _020F5D80[];

BOOL FadeFunc_10(void *param0);

BOOL FadeFunc_10(void *ptr) {
    ScreenFade *param0 = ptr;
    if (param0->state == FADE_IDLE) {
        _0210F64C.shapes[9].shape = _020F5D80;
        sub_0200FCDC(param0->color);
        sub_0201289C(param0, &_0210F64C.shapes[9]);
        param0->direction = FADE_OUT;
        param0->method = FADE_BY_WINDOW;
        return 0;
    }
    return sub_020128E0(param0);
}
