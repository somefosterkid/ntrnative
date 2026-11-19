#include "global.h"

#include "screen_fade_fx.h"

#include "screen_fade_internal.h"

typedef struct SectorWipeConfig {
    u16 startPhase;
    u16 endPhase;
    u8 insideMask;
    u8 outsideMask;
    u16 queued;
} SectorWipeConfig;

void sub_0200FCDC(u16);

void sub_020125EC(ScreenFade *, SectorWipeConfig *);

BOOL sub_0201262C(ScreenFade *);

extern const SectorWipeConfig _020F5D58[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_34(void *ptr);
#else
void FadeFunc_34(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_34(void *ptr) {
#else
void FadeFunc_34(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        SectorWipeConfig config = _020F5D58[4];
        sub_0200FCDC(fade->color);
        sub_020125EC(fade, &config);
        fade->direction = FADE_OUT;
        fade->method = FADE_BY_WINDOW;
#if defined(SDK_PORT) || defined(NONMATCHING)
        return FALSE; /* r0 on DS = 0, the value just stored to method */
#else
        return;
#endif
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return sub_0201262C(fade); /* r0 on DS = result of sub_0201262C() */
#else
    sub_0201262C(fade);
#endif
}

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_35(void *ptr);
#else
void FadeFunc_35(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_35(void *ptr) {
#else
void FadeFunc_35(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        SectorWipeConfig config = _020F5D58[3];
        sub_0200FCDC(fade->color);
        sub_020125EC(fade, &config);
        fade->direction = FADE_IN;
        fade->method = FADE_BY_WINDOW;
#if defined(SDK_PORT) || defined(NONMATCHING)
        return FALSE; /* r0 on DS = 0, the value just stored to method */
#else
        return;
#endif
    }
#if defined(SDK_PORT) || defined(NONMATCHING)
    return sub_0201262C(fade); /* r0 on DS = result of sub_0201262C() */
#else
    sub_0201262C(fade);
#endif
}
