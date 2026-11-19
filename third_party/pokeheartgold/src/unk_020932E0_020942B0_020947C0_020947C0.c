#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

s32 sub_020947C0(UnkStruct_020932E0 *arg0, s32 arg1) {
    s32 i;
    s32 found;
    u8 n;
    Sprite *sprite = arg0->unk_198[arg1].unk_0;

    if (sprite != NULL && Sprite_GetDrawFlag(sprite) == 0) {
        return 3;
    }
    if (arg0->unk_198[arg1].unk_4.unk_8 != 0) {
        return 3;
    }
    found = -1;
    for (i = 0; found == -1 && i < arg0->unk_D; i++) {
        if (arg0->unk_F == arg0->unk_8D0[i].unk_8 && arg1 == arg0->unk_8D0[i].unk_4) {
            found = i;
        }
    }
    if (found == -1) {
        n = arg0->unk_D;
        if (arg0->unk_E == n) {
            return 0;
        }
        for (i = 0; i < n; i++) {
            if (arg0->unk_8D0[i].unk_4 == -1) {
                return 1;
            }
        }
    }
    return 2;
}
