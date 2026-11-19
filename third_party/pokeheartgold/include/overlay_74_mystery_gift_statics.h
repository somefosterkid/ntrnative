#ifndef POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_STATICS_H
#define POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_STATICS_H

#include "global.h"

struct OverlayManager;

// How far the Mystery Gift app's (overlay 74) wireless distribution search has got; stored in MysteryGiftStatics.state.
enum WirelessDistributionSearchState {
    WIRELESS_DISTRIBUTION_STATE_INIT = 37,
    WIRELESS_DISTRIBUTION_STATE_38,
    WIRELESS_DISTRIBUTION_STATE_39,
    WIRELESS_DISTRIBUTION_STATE_40,
    WIRELESS_DISTRIBUTION_STATE_41,
    WIRELESS_DISTRIBUTION_STATE_42,
    WIRELESS_DISTRIBUTION_STATE_43,
    WIRELESS_DISTRIBUTION_STATE_44,
    WIRELESS_DISTRIBUTION_STATE_45,
    WIRELESS_DISTRIBUTION_STATE_46,
    WIRELESS_DISTRIBUTION_STATE_47,
    WIRELESS_DISTRIBUTION_STATE_48,
};

// The app's file-scope variables: 0x14 bytes of asm .bss on the DS (ov74_0223D0A8, listed in tools/hg/data/LABELS.txt
// because the PC build defines its own host-sized copy in ov74_0222CD7C's file). The offsets in the comments are the
// DS ones; asm/overlay_74_thumb_02229F90_0222A2A4_0222AB0C.s still stores the chosen menu entry at +0x8.
typedef struct MysteryGiftStatics {
    struct OverlayManager *man; // 0x00: set by ov74_0222AE6C (the app's init), read by ov74_0222CD7C
    void *workBuf; // 0x04: Heap_Alloc'd ov74_02236980() bytes, handed to ov74_02236680; freed by ov74_0222CEC0
    int (*selected)(struct OverlayManager *); // 0x08: the menu entry ov74_0222AB0C last called
    u32 frameCounter; // 0x0C: counts frames while the distribution search runs
    u32 state; // 0x10: enum WirelessDistributionSearchState
} MysteryGiftStatics; // size: 0x14

extern MysteryGiftStatics ov74_0223D0A8;

#endif // POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_STATICS_H
