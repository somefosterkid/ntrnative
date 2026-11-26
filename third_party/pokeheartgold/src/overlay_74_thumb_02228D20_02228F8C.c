#include "global.h"
#include "overlay_74_ranger_link_manager.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02229084(void *);
#else
s32 ov74_02229084(void *);
#endif

s32 ov74_02235414(void *, void *, s32, s32, s32, s32);

s32 ov74_02235464(void *, s32, s32, s32);

s32 ov74_0223546C(void *, s32, s32);

s32 ov74_02235568(void *, void *, s32, s32, s32);

s32 ov74_02235634(void *);

s32 ov74_0223563C(void);

s32 ov74_02235690();

s32 ov74_02235728(s32, s32, s32, s32, s32, s32);

void *ov74_02235930(s32, void *, s32, s32, s32);

void ov74_02228F8C(RangerLinkManager *arg0);

void ov74_02228F8C(RangerLinkManager *arg0) {
    ov74_02235414(&arg0->messageBox, &arg0->messageBoxWindow, 0, 0xF7, 0xA, 2);
    ov74_02235464(&arg0->messageBox, 0x1B, 4, arg0->nextWindowBasetile);
    ov74_0223546C(&arg0->messageBox, 1, 1);
    ov74_02235568(arg0->bgConfig, &arg0->messageBox, 2, 0x13, 0x48);
    arg0->nextWindowBasetile = arg0->nextWindowBasetile + ov74_02235634(&arg0->messageBox);
    ov74_02235414(&arg0->manaphyEggTransferWindow, &arg0->stdWindow, 0, 0xF7, 1, 1);
    ov74_02235464(&arg0->manaphyEggTransferWindow, 0x1E, 4, arg0->nextWindowBasetile);
    ov74_0223546C(&arg0->manaphyEggTransferWindow, 0, 1);
    ov74_02235568(arg0->bgConfig, &arg0->manaphyEggTransferWindow, 1, 1, 0xB);
    arg0->nextWindowBasetile = (s32)(arg0->nextWindowBasetile + ov74_02235634(&arg0->manaphyEggTransferWindow));
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(0x71, 0xE, 0xB, 0xD, 0xC, 0);
    arg0->downloadArrow = ov74_02235930(0, arg0->downloadArrow, 0x80, 0x4C, 1);
    ov74_02229084(arg0);
}
