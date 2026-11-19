#include "global.h"

#include "map_object.h"
#include "sprite.h"
#include "overlay_02_hmcut_in.h"

s32 ov01_021FCD8C(SysTask *, s32, s32, s32);

s32 ov02_0224B2C0(void *);

s32 ov02_0224B6B0(void *, s32);

s32 ov02_0224B0E0(FlyTaskEnv *arg0);

s32 ov02_0224B0E0(FlyTaskEnv *arg0) {
    Sprite *sprite;
    s32 v0;

    sprite = arg0->cutIn->playerSprite;
    Sprite_SetAnimCtrlSeq(sprite, 3);
    Sprite_SetDrawFlag(sprite, 1);
    ov02_0224B6B0(arg0->cutIn, 1);
    sub_0205F484(arg0->cutIn->playerObject);
    v0 = Sprite_GetMatrixPtr(sprite)->y;
    arg0->playerOffsetY = (s32)(v0 - Sprite_GetMatrixPtr(arg0->birdSprite)->y);
    ov02_0224B2C0(arg0->unk_60);
    ov01_021FCD8C(arg0->task, 2, 0, 0xC);
    arg0->scaleDelta = 0x100;
    arg0->angleParam = 0x80000;
    arg0->pathParam = 0;
    arg0->pathDelta = 0x800;
    arg0->subState = 3;
    arg0->state = (u8)(arg0->state + 1);
    return 1;
}
