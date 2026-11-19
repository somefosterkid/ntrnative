#include "global.h"
#include "sprite.h"

struct Sprite;

void Sprite_TryChangeAnimSeq(Sprite *sprite, int seq);

Sprite *ov96_021E8BAC(u32 param0);

void ov96_021EAC5C(u32 *param0, int animId);

void ov96_021EAC5C(u32 *param0, int animId) {
    Sprite *sprite = ov96_021E8BAC(param0[0]);
    u32 seq;

    switch (animId) {
    case 0:
    case 4:
    case 8:
    case 0xc:
    case 0x10:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1a:
        param0[0xe] = animId;
        break;
    default:
        return;
    }
    if (!(animId == 0x14 || animId == 0x15)) {
        switch (param0[0xd]) {
        case 1:
            seq = (u8)animId;
            break;
        case 2:
            seq = (u8)(animId + 1);
            break;
        case 3:
            seq = (u8)(animId + 2);
            break;
        case 4:
            seq = (u8)(animId + 3);
            break;
        default:
            GF_AssertFail();
            return;
        }
    } else {
        seq = (u8)animId;
    }
    Sprite_TryChangeAnimSeq(sprite, seq);
}
