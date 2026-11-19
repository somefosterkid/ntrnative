#include "global.h"

#include "pokemon_summary_app.h"
#include "touchscreen.h"

extern TouchscreenHitbox _021038D4;

void sub_0208ADCC(void);

extern TouchscreenHitbox _021038AC;
s32 sub_0208ADDC(void);
extern TouchscreenHitbox _021038B0;
s32 sub_0208AE08(PokemonSummary *summary);
extern TouchscreenHitbox _021038B4;
s32 sub_0208AE48(void);
s32 sub_0208AE88(void);

void sub_0208ADCC(void) {
    TouchscreenHitbox_FindRectAtTouchNew(&_021038D4);
}

s32 sub_0208ADDC(void) {
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&_021038D4);
    if (v0 == -1) {
        if (TouchscreenHitbox_TouchNewIsIn(&_021038AC) == 1) {
            return 4;
        }
        v0 = -1;
        return v0;
    }
    return v0;
}

s32 sub_0208AE08(PokemonSummary *summary) {
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&_021038D4);
    if (v0 == -1) {
        if (TouchscreenHitbox_TouchNewIsIn(&_021038B0) == 1) {
            return 4;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&_021038AC) == 1) {
            return 5;
        }
        v0 = -1;
        return v0;
    }
    return v0;
}

s32 sub_0208AE48(void) {
    s32 v0;

    v0 = TouchscreenHitbox_FindRectAtTouchNew(&_021038D4);
    if (v0 == -1) {
        if (TouchscreenHitbox_TouchNewIsIn(&_021038B4) == 1) {
            return 4;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&_021038AC) == 1) {
            return 5;
        }
        v0 = -1;
        return v0;
    }
    return v0;
}

s32 sub_0208AE88(void) {
    if (TouchscreenHitbox_TouchNewIsIn(&_021038B4) == 1) {
        return 0;
    }
    if (TouchscreenHitbox_TouchNewIsIn(&_021038AC) == 1) {
        return 1;
    }
    return -1;
}
