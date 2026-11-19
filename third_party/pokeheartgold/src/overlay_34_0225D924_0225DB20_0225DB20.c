#include "global.h"

#include "constants/heap.h"

#include "overlay_34.h"
#include "sprite.h"
#include "unk_02009D48.h"

extern const int ov34_0225E6A0[3];

void ov34_0225DB20(Ov34Work *work) {
    int i;

    CreateSpriteResourcesHeader(&work->header, 999, 999, 999, 999, 0xffffffff, 0xffffffff, 0, 0, work->resMans[0], work->resMans[1], work->resMans[2], work->resMans[3], NULL, NULL);

    {
        SpriteTemplate spriteTemplate;

        spriteTemplate.spriteList = work->spriteList;
        spriteTemplate.header = &work->header;
        spriteTemplate.position.z = 0;
        spriteTemplate.scale.x = FX32_ONE;
        spriteTemplate.scale.y = FX32_ONE;
        spriteTemplate.scale.z = FX32_ONE;
        spriteTemplate.rotation = 0;
        spriteTemplate.drawPriority = 0;
        spriteTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
        spriteTemplate.heapID = HEAP_ID_FIELD1;

        for (i = 0; i < 3; i++) {
            spriteTemplate.position.x = FX32_CONST(256 - 24);
            spriteTemplate.position.y = FX32_CONST(ov34_0225E6A0[i]) + (192 << FX32_SHIFT);

            work->sprites[i] = Sprite_CreateAffine(&spriteTemplate);

            Sprite_SetAnimActiveFlag(work->sprites[i], 1);
            Sprite_SetAnimCtrlSeq(work->sprites[i], i);
        }
    }
}

int ov34_0225DC00(Ov34ChatLog *chatLog, int index) {
    index++;

    if (index == 30) {
        index = 0;
    }

    return index;
}

s32 ov34_0225DC0C(s32 arg0, s32 arg1) {
    s32 v0;

    v0 = arg0 + arg1;
    if (v0 >= 0x1E) {
        v0 -= 0x1E;
    }
    return v0;
}
