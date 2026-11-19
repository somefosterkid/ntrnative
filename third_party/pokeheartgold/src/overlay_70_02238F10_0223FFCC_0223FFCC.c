#include "global.h"

#include "pokemon.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_022380EC(void);

s32 ov70_0223FFCC(GTSApplicationState *arg0);

s32 ov70_0223FFCC(GTSApplicationState *arg0) {
    ov70_022380EC();
    arg0->currentScreenInstruction = 0xA;
    arg0->networkTimeoutCounter = 0;
    return 3;
}

s32 ov70_02237F38(void);

s32 ov70_02237F58(void);

s32 ov70_0223FFE8(GTSApplicationState *arg0);

s32 ov70_0223FFE8(GTSApplicationState *arg0) {
    u32 v0;

    if (ov70_02237F38() != 0) {
        v0 = ov70_02237F58() + 0xF;
        arg0->networkTimeoutCounter = 0;
        switch (v0) {
        case 15:
            arg0->currentScreenInstruction = 0x21;
            break;
        case 10:
        case 11:
        case 12:
            sub_02039AD8(3);
            break;
        case 0:
        case 1:
        case 3:
        case 13:
            sub_02039AD8(4);
            break;
        case 2:
            sub_020399EC();
            break;
        }
    } else {
        arg0->networkTimeoutCounter = (s32)(arg0->networkTimeoutCounter + 1);
        if (arg0->networkTimeoutCounter == 0xE10) {
            sub_020399EC();
        }
    }
    return 3;
}

s32 ov70_02238208(s32, void *, void *);

void ov70_02240D44(GTSApplicationState *, s16, s16);

s32 ov70_0224006C(GTSApplicationState *arg0);

s32 ov70_0224006C(GTSApplicationState *arg0) {
    Pokemon_RemoveCapsule((Pokemon *)&arg0->receivedListing);
    ov70_02238208(arg0->searchResults[arg0->selectedSearchResult].listingId, &arg0->receivedListing, &arg0->selectedListing);
    ov70_02240D44(arg0, 0xE, 0x10);
    arg0->currentScreenInstruction = 0xD;
    arg0->networkTimeoutCounter = 0;
    return 3;
}
