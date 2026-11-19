#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

s32 sub_02094894(UnkStruct_020932E0 *arg0, s32 arg1) {
    Sprite *sprite;

    if (arg1 >= (s32)arg0->unk_D) {
        return 3;
    }
    sprite = arg0->unk_7E8[arg1].unk_0;
    if ((sprite != NULL) && (Sprite_GetDrawFlag(sprite) == 0)) {
        return 3;
    }
    return 1;
}
