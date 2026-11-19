#include "global.h"
#include "overlay_74_ranger_link_manager.h"

s32 ov74_02229084(void *);

s32 ov74_02235414(void *, void *, s32, s32, s32, s32);

s32 ov74_02235464(void *, s32, s32, s32);

s32 ov74_0223546C(void *, s32, s32);

s32 ov74_02235568(void *, void *, s32, s32, s32);

s32 ov74_02235634(void *);

s32 ov74_0223563C(void);

s32 ov74_02235690();

s32 ov74_02235728(s32, s32, s32, s32, s32, s32);

void *ov74_02235930(s32, void *, s32, s32, s32);

void ov74_02228F8C(void *arg0);

void ov74_02228F8C(void *arg0) {
    ov74_02235414(arg0 + 0x48, arg0 + 0x28, 0, 0xF7, 0xA, 2);
    ov74_02235464(arg0 + 0x48, 0x1B, 4, *((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile));
    ov74_0223546C(arg0 + 0x48, 1, 1);
    ov74_02235568(*(void **)((u8 *)arg0 + 4), arg0 + 0x48, 2, 0x13, 0x48);
    *((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile) = (s32)(*((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile) + ov74_02235634(arg0 + 0x48));
    ov74_02235414(arg0 + 0x9C, arg0 + 0x18, 0, 0xF7, 1, 1);
    ov74_02235464(arg0 + 0x9C, 0x1E, 4, *((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile));
    ov74_0223546C(arg0 + 0x9C, 0, 1);
    ov74_02235568(*(void **)((u8 *)arg0 + 4), arg0 + 0x9C, 1, 1, 0xB);
    *((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile) = (s32)(*((s32 *)&((RangerLinkManager *)arg0)->nextWindowBasetile) + ov74_02235634(arg0 + 0x9C));
    ov74_0223563C();
    ov74_02235690();
    ov74_02235728(0x71, 0xE, 0xB, 0xD, 0xC, 0);
    *((void **)&((RangerLinkManager *)arg0)->downloadArrow) = ov74_02235930(0, *((void **)&((RangerLinkManager *)arg0)->downloadArrow), 0x80, 0x4C, 1);
    ov74_02229084(arg0);
}
