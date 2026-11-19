#include "global.h"

#include "overlay_74_mystery_gift_statics.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
// PC: host-layout copy of the asm's .bss object (asm/overlay_74_thumb.s: 0x14 bytes on the DS). The asm label stays for the DS build.
MysteryGiftStatics ov74_0223D0A8;
#endif

struct OverlayManager *ov74_0222CD7C(void);

struct OverlayManager *ov74_0222CD7C(void) {
    return ov74_0223D0A8.man;
}

void ov74_0222CD88(struct OverlayManager *man);

void ov74_0222CD88(struct OverlayManager *man) {
    ov74_0223D0A8.man = man;
}
