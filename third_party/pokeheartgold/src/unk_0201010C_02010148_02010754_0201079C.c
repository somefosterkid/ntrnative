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

void sub_020122B8(ScreenFade *, SectorWipeConfig *);

BOOL sub_020122F8(ScreenFade *);

extern const SectorWipeConfig _020F5D98[];

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_33(void *ptr);
#else
void FadeFunc_33(void *ptr);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL FadeFunc_33(void *ptr) {
#else
void FadeFunc_33(void *ptr) {
#endif
    ScreenFade *fade = ptr;
    if (fade->state == FADE_IDLE) {
        SectorWipeConfig config = _020F5D98[5];
        sub_0200FCDC(fade->color);
        sub_020122B8(fade, &config);
        fade->direction = FADE_IN;
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
