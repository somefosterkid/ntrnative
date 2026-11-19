#include "global.h"

#include "sprite.h"
#include "unk_020932E0.h"

void sub_02094150(SpriteTemplate *arg0, UnkStruct_020932E0 *arg1, NNS_G2D_VRAM_TYPE arg2) {
    arg0->spriteList = arg1->unk_18;
    arg0->header = &arg1->unk_174;
    arg0->position.z = 0;
    arg0->scale.x = 0x1000;
    arg0->scale.y = 0x1000;
    arg0->scale.z = 0x1000;
    arg0->rotation = 0;
    arg0->drawPriority = 0x14;
    arg0->whichScreen = arg2;
    arg0->heapID = arg1->unk_4;
}
