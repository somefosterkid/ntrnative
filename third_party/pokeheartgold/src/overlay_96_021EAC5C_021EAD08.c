#include "global.h"

#include "error_handling.h"
#include "sprite.h"

Sprite *ov96_021E8BAC(s32 param_1, s32);

void ov96_021EAD08(s32 *param_1, s32 param_2);

void ov96_021EAD08(s32 *param_1, s32 param_2) {
    Sprite *sprite;
    u32 seq;

    sprite = ov96_021E8BAC(*param_1, param_2);
    switch (param_2) {
    case 0:
        param_1[0xe] = 0;
        break;
    case 20:
        param_1[0xe] = 4;
        break;
    case 21:
        param_1[0xe] = 5;
        break;
    default:
        return;
    }
    if (param_2 != 0x14 && param_2 != 0x15) {
        switch (param_1[0xd]) {
        case 1:
            seq = 0;
            break;
        case 2:
            seq = 1;
            break;
        case 3:
            seq = 2;
            break;
        case 4:
            seq = 3;
            break;
        default:
            GF_AssertFail();
            return;
        }
    } else {
        seq = param_1[0xe] & 0xff;
    }
    Sprite_TryChangeAnimSeq(sprite, seq);
}
