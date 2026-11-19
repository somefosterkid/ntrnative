#include "global.h"

#include "screen_fade_fx.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "screen_fade_internal.h"
#include "screen_fade_shape_globals.h"

s32 sub_0200FCDC(u16);

s32 sub_0201289C(void *, UnkStruct_0200FE6C *);

s32 sub_020128E0(void *);

extern const UnkStruct_0200F898 _020F5D5C[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_03(void *ptr);
#else
void FadeFunc_03(void *ptr);
#endif

extern const UnkStruct_0200F898 _020F5D58[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_03(void *ptr) {
#else
void FadeFunc_03(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        _0210F64C.shapes[2].shape = _020F5D5C;
        sub_0200FCDC(fade->color);
        sub_0201289C(fade, &_0210F64C.shapes[2]);
        fade->direction = FADE_IN;
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

extern const UnkStruct_0200F898 _020F5D64[];

BOOL FadeFunc_04(void *ptr);

BOOL FadeFunc_04(void *ptr) {
    ScreenFade *param0 = ptr;
    if (param0->state == FADE_IDLE) {
        _0210F64C.shapes[4].shape = _020F5D64;
        sub_0200FCDC(param0->color);
        sub_0201289C(param0, &_0210F64C.shapes[4]);
        param0->direction = FADE_OUT;
        param0->method = FADE_BY_WINDOW;
        return 0;
    }
    return sub_020128E0(param0);
}

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_05(void *ptr);
#else
void FadeFunc_05(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_05(void *ptr) {
#else
void FadeFunc_05(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        _0210F64C.shapes[5].shape = _020F5D58;
        sub_0200FCDC(fade->color);
        sub_0201289C(fade, &_0210F64C.shapes[5]);
        fade->direction = FADE_IN;
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
