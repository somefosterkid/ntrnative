#include "global.h"

s32 ov74_0222FE4C(void);

s32 ov74_0222FEC8(void);

s32 ov74_0222FF28(void);

s32 ov74_0222FF80(void);

s32 ov74_02231070(s8);

u8 ov74_02231094(void);

void *ov74_0223115C(void);

BOOL ov74_022314DC(u32 param0);

s32 ov74_02231560(void);

void ov74_02230674(void);

void ov74_02230674(void) {
    s32 v0;

    v0 = ov74_02231094();
    switch (v0) {
    case 2:
        if (ov74_0222FF28() != 0) {
            if (ov74_022314DC(ov74_0222FEC8()) != 0) {
                *(s8 *)((u8 *)(ov74_0223115C()) + 0x60) = 1;
                ov74_02231070(3);
                return;
            }
        }
        ov74_0222FE4C();
        return;
    case 1:
        ov74_0222FF80();
        if (ov74_02231560() == 0) {
            ov74_0222FE4C();
            return;
        }
        ov74_02231070(3);
        return;
    }
}
