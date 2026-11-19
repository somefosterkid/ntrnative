#include "global.h"

#include "player_data.h"
#include "unk_02034354.h"
#include "unk_02035900.h"
#include "unk_02034B0C.h"

extern u8 *ov74_0223D0A4;

void ov74_02229C0C(void);

s32 ov74_02229BC0(s32 *, s16);
extern s32 ov74_02229C5C;
void ov74_02229C44(void);

void ov74_02229C0C(void) {
    BOOL BVar1;
    PlayerProfile *pPVar2;
    u32 a0;
    s32 iVar3;

    a0 = 0;
    iVar3 = 0;
    do {
        if ((*(s32 *)(ov74_0223D0A4 + iVar3 + 0x28) == 0) && (BVar1 = sub_020373B4((u16)a0), BVar1 == 1)) {
            pPVar2 = sub_02034818(a0);
            *(PlayerProfile **)(ov74_0223D0A4 + iVar3 + 0x28) = pPVar2;
        }
        a0 = a0 + 1;
        iVar3 = iVar3 + 4;
    } while ((s32)a0 < 0x10);
    return;
}

void ov74_02229C44(void) {
    if (sub_02034DB8() != 0) {
        ov74_02229BC0(&ov74_02229C5C, 0);
    }
}
