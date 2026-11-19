#include "global.h"
#include "camera.h"
#include "overlay_68_move_relearner.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "unk_0208805C.h"

void ov68_021E7388(MoveRelearnerMonPic *monPic);

void ov68_021E7388(MoveRelearnerMonPic *monPic) {
    Camera_Delete(monPic->camera);
    PokepicManager_Delete(monPic->pokepicManager);
    MI_CpuFill8(monPic, 0U, sizeof(MoveRelearnerMonPic));
}

void ov68_021E73A4(MoveRelearner *relearner, u32 pos, s32 palette);

void ov68_021E73A4(MoveRelearner *relearner, u32 pos, s32 palette) {
    if (pos < 4U) {
        ManagedSprite_SetPositionXY(relearner->sprites[2], 0, (s16)((pos << 5) + 0x20));
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 0U);
    } else if (pos < 6U) {
        ManagedSprite_SetPositionXY(relearner->sprites[2], (s16)(0x28 * (pos - 4)), 0xA8);
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 1U);
    } else {
        ManagedSprite_SetPositionXY(relearner->sprites[2], 0xC0, 0xA0);
        ManagedSprite_SetAnimationFrame(relearner->sprites[2], 2U);
    }
    ManagedSprite_SetPaletteOverride(relearner->sprites[2], palette);
}

void ov68_021E7424(MoveRelearner *relearner, s32 which, s32 animate);

void ov68_021E7424(MoveRelearner *relearner, s32 which, s32 animate) {
    s32 v0;
    s32 flag;

    if ((which == 0) && (animate != 0)) {
        v0 = 1;
        ManagedSprite_ResetSpriteAnimCtrlState(relearner->sprites[0]);
    } else {
        v0 = 0;
    }
    if ((s32)(relearner->args->scrollOffset + 4) < (s32)relearner->numMoves) {
        ManagedSprite_SetAnim(relearner->sprites[0], v0);
    } else {
        ManagedSprite_SetAnim(relearner->sprites[0], v0 + 2);
    }
    ManagedSprite_SetAnimateFlag(relearner->sprites[0], v0);
    if ((which == 1) && (animate != 0)) {
        flag = 1;
        ManagedSprite_ResetSpriteAnimCtrlState(relearner->sprites[1]);
    } else {
        flag = 0;
    }
    if (relearner->args->scrollOffset != 0) {
        ManagedSprite_SetAnim(relearner->sprites[1], flag + 4);
    } else {
        ManagedSprite_SetAnim(relearner->sprites[1], flag + 6);
    }
    ManagedSprite_SetAnimateFlag(relearner->sprites[1], flag);
}

s32 ov68_021E74C0(MoveRelearner *relearner);

s32 ov68_021E74C0(MoveRelearner *relearner) {
    App_StartScreenFade(1U, HEAP_ID_66);
    relearner->nextState = 0xB;
    return 0;
}
