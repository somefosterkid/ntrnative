#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_43_palpad.h"

void ov43_0222D008(PalPadFriendCode *friendCode);

void ov43_0222D008(PalPadFriendCode *friendCode) {
    RemoveWindow(&friendCode->windows[0]);
    RemoveWindow(&friendCode->windows[1]);
    RemoveWindow(&friendCode->windows[2]);
    Heap_Free(friendCode->screenFile);
}
