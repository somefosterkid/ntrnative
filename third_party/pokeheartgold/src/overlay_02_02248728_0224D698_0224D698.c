#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "player_avatar.h"
#include "unk_02005D10.h"
#include "overlay_02_field3d_effects.h"

void ov02_0224D698(Ov02Field3dEffectSlot *slot, PlayerAvatar *avatar, fx32 offsetY, fx32 offsetZ) {
    int i;
    VecFx32 pos;

    if (slot->playing != 0) {
        GF_AssertFail();
    }
    PlayerAvatar_CopyPositionVector(avatar, &pos);
    Field3dObject_SetPosEx(&slot->object, pos.x, pos.y + offsetY, pos.z + offsetZ);
    slot->playing = 1;
    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        Field3dModelAnimation_FrameSet(&slot->anims[i], 0);
    }
    Field3dObject_SetActiveFlag(&slot->object, TRUE);
    PlaySE(0x626);
}
