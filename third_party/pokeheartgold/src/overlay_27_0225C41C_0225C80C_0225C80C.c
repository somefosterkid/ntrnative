#include "global.h"

#include "constants/heap.h"

#include "overlay_27.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

// asm tables of u16 pairs {x, y} per cursor slot: ov27_0225D118 holds the x of slot i at [i][0], ov27_0225D11A its y at [i][0].
extern const u16 ov27_0225D118[][2];
extern const u16 ov27_0225D11A[][2];

void ov27_0225C80C(Ov27TaskData *data, s32 param_2) {
    Sprite *pSVar1;
    SpriteTemplate SStack_3c;
    CreateSpriteResourcesHeader(&data->spriteHeader, 10, 10, 10, 10, -1, -1, 0, 0, data->resMan[0], data->resMan[1], data->resMan[2], data->resMan[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    SStack_3c.spriteList = data->spriteList;
    SStack_3c.header = &data->spriteHeader;
    SStack_3c.position.z = 0;
    SStack_3c.position.x = ((u32)ov27_0225D118[param_2][0]) << 0xc;
    SStack_3c.position.y = (ov27_0225D11A[param_2][0] + 0x100) * 0x1000;
    SStack_3c.scale.x = 0x1000;
    SStack_3c.scale.y = 0x1000;
    SStack_3c.scale.z = 0x1000;
    SStack_3c.rotation = 0;
    SStack_3c.drawPriority = 1;
    SStack_3c.whichScreen = NNS_G2D_VRAM_TYPE_2DSUB;
    SStack_3c.heapID = HEAP_ID_8;
    pSVar1 = Sprite_CreateAffine(&SStack_3c);
    data->cursor = pSVar1;
    Sprite_SetAnimCtrlSeq(data->cursor, 1);
    Sprite_SetAnimActiveFlag(data->cursor, 1);
    Sprite_SetDrawFlag(data->cursor, 0);
    return;
}
