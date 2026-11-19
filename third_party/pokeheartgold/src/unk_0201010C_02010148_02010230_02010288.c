#include "global.h"

#include "screen_fade_fx.h"
#include "screen_fade_internal.h"
#include "screen_fade_shape_globals.h"

void sub_0200FCDC(u16);

void sub_0201289C(ScreenFade *, UnkStruct_0200FE6C *);

BOOL sub_020128E0(ScreenFade *);

extern const UnkStruct_0200F898 _020F5D90[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_08(void *ptr);
#else
void FadeFunc_08(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_08(void *ptr) {
#else
void FadeFunc_08(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        _0210F64C.shapes[1].shape = _020F5D90;
        sub_0200FCDC(fade->color);
        sub_0201289C(fade, &_0210F64C.shapes[1]);
        fade->direction = FADE_OUT;
        fade->method = FADE_BY_WINDOW;
#if defined(SDK_PORT) || defined(NONMATCHING)
        return FALSE; /* r0 on DS = 0, the value just stored to method */
#else
        return;
#endif
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return sub_020128E0(fade); /* r0 on DS = result of sub_020128E0() */
#else
    sub_020128E0(fade);
#endif
}
