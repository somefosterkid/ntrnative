#include "global.h"
#include "overlay_00_nintendo_wfcmanager.h"

s32 ov00_021E7234(s32);

void ov00_021E7300(void);

void ov00_021E7300(void) {
    s32 v0;

    v0 = 0;
    do {
        ov00_021E7234(v0);
        v0 += 1;
    } while (v0 < 4);
}

extern NintendoWFCManager *_0221A680;

s32 ov00_021E7314(void);

s32 ov00_021E7314(void) {
    return (s32) _0221A680->needsGameSave;
}

void ov00_021E7328(void);

void ov00_021E7328(void) {
    *((s8 *)&_0221A680->needsGameSave) = 0;
}
