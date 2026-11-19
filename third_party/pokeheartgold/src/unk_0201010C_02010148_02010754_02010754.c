#include "global.h"

#include "screen_fade_fx.h"

#include "screen_fade_internal.h"

void sub_0200FCDC(u16);


void sub_020122B8(ScreenFade *, UnkStruct_0200FEA4 *);

BOOL sub_020122F8(ScreenFade *);

extern const UnkStruct_0200FEA4 _020F5D58[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_32(void *ptr);
#else
void FadeFunc_32(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_32(void *ptr) {
#else
void FadeFunc_32(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        UnkStruct_0200FEA4 buf = _020F5D58[6];
        sub_0200FCDC(fade->color);
        sub_020122B8(fade, &buf);
        fade->direction = FADE_OUT;
        fade->method = FADE_BY_WINDOW;
#if defined(SDK_PORT) || defined(NONMATCHING)
        return FALSE; /* r0 on DS = 0, the value just stored to method */
#else
        return;
#endif
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return sub_020122F8(fade); /* r0 on DS = result of sub_020122F8() */
#else
    sub_020122F8(fade);
#endif
}
