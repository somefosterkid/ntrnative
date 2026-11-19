#include "global.h"
struct Ov27Task;

#include "overlay_27.h"
#include "player_avatar.h"
#include "sprite.h"
#include "system.h"
#include "touchscreen.h"
#include "overlay_27_ov27task.h"

s32 ov27_0225A468(Ov27Task *, s32);

void ov27_0225A530(Ov27Task *arg0, s32 arg1);

void ov27_0225A4B8(Ov27Task *arg0) {
    ov27_0225A468(arg0, PlayerAvatar_CheckRunningShoesLock(FieldSystem_GetPlayerAvatar(arg0->fieldSystem)));
}


s32 ov27_0225A4D0(Ov27Task *arg0) {
    s32 v0;

    v0 = ov27_0225BDC8(arg0);
    if ((PlayerAvatar_GetState(arg0->fieldSystem->playerAvatar) == 1) || (arg0->fieldSystem->unkD2_0 != 0)) {
        v0 = 0;
    }
    if (v0 != 0) {
        Sprite_SetDrawFlag(arg0->unk_390[11], 1);
        Sprite_SetDrawFlag(arg0->unk_390[13], 1);
    } else {
        Sprite_SetDrawFlag(arg0->unk_390[11], 0);
        Sprite_SetDrawFlag(arg0->unk_390[13], 0);
    }
    return v0;
}

void ov27_0225A530(Ov27Task *arg0, s32 arg1) {
    u16 v0;

    v0 = Sprite_GetAnimationNumber(arg0->unk_390[12]);
    if (Sprite_GetDrawFlag(arg0->unk_390[12]) != 0) {
        if ((arg1 == 1) && (System_GetTouchNew() != 0)) {
            gSystem.simulatedInputs = 1;
            Sprite_SetAnimCtrlSeq(arg0->unk_390[12], 6);
            return;
        }
        if ((arg1 == 1) && (v0 == 6)) {
            gSystem.simulatedInputs = 1;
            return;
        }
        Sprite_SetAnimCtrlSeq(arg0->unk_390[12], 5);
    }
}
