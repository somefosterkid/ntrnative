#include "global.h"

#include "constants/heap.h"

#include "overlay_43_palpad.h"
#include "unk_02005D10.h"

void ov43_0222A318(PalPadShared *shared, s8 nextScreen, s8 nextSubState);
BOOL ov43_0222CE14(PalPadFriendCode *friendCode, PalPadShared *shared);
void ov43_0222CE48(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
void ov43_0222D008(PalPadFriendCode *friendCode);
void ov43_0222D028(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222D090(PalPadFriendCode *friendCode, PalPadGfx *gfx);
void ov43_0222D0F8(PalPadFriendCode *friendCode, PalPadGfx *gfx, BOOL pressed);

void ov43_0222CD90(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId);
BOOL ov43_0222CD98(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx);
void ov43_0222CE0C(PalPadFriendCode *friendCode);

void ov43_0222CD90(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapId) {
    ov43_0222CE48(friendCode, shared, gfx, heapId);
}

BOOL ov43_0222CD98(PalPadFriendCode *friendCode, PalPadShared *shared, PalPadGfx *gfx) {
    switch (shared->subState) {
    case 0:
        ov43_0222D028(friendCode, shared, gfx);
        shared->subState = 1;
    default:
        break;
    case 1:
        if (ov43_0222CE14(friendCode, shared) != 0) {
            PlaySE(0x5DD);
            ov43_0222D0F8(friendCode, gfx, TRUE);
            friendCode->timer = 4;
            shared->subState = 2;
        }
        break;
    case 2:
        if (--friendCode->timer <= 0) {
            ov43_0222D0F8(friendCode, gfx, FALSE);
            ov43_0222A318(shared, 0, 2);
            ov43_0222D090(friendCode, gfx);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void ov43_0222CE0C(PalPadFriendCode *friendCode) {
    ov43_0222D008(friendCode);
}
