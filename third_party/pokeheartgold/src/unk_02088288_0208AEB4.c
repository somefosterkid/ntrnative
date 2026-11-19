#include "global.h"
struct PokemonSummary;

#include "touchscreen.h"
#include "sprite.h"
#include "pokemon_summary_app.h"

extern TouchscreenHitbox _021038B8;

void sub_0208AEB4(void);

s32 sub_0208AF08(void *, s32);
s32 sub_0208BCB4(PokemonSummary *);
s32 sub_0208AED4(struct PokemonSummary *arg0);

void sub_0208AEB4(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&_021038B8);
}

extern TouchscreenHitbox _021039E8;

void sub_0208AEC4(void);

void sub_0208AEC4(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&_021039E8);
}

s32 sub_0208AED4(PokemonSummary *arg0) {
    Sprite_SetAnimCtrlSeq(arg0->sprites[9], 1);
    Sprite_SetDrawFlag(arg0->sprites[10], 1);
    sub_0208BCB4(arg0);
    sub_0208AF08(arg0, 1);
    return 9;
}
