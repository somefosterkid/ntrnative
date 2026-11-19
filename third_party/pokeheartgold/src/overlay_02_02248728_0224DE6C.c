#include "global.h"

#include "overlay_02_field3d_effects.h"

void ov02_0224DE6C(Ov02Field3dEffectSlot *slot) {
    int i;
    BOOL finished;

    if (slot->playing == 0) {
        return;
    }

    finished = TRUE;

    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        finished &= Field3dModelAnimation_FrameAdvanceAndCheck(&slot->anims[i], FX32_ONE);
    }

    if (finished == TRUE) {
        slot->playing = 0;
        Field3dObject_SetActiveFlag(&slot->object, FALSE);
    }
}

void ov02_0224DEA8(Ov02Field3dEffectSlot *slot, Field3dModel *model, NNSFndAllocator *allocator, void **animFiles) {
    int i;

    memset(slot, 0, sizeof(Ov02Field3dEffectSlot));
    Field3dObject_InitFromModel(&slot->object, model);

    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        ov01_021FBE70(&slot->anims[i], model, animFiles[i], allocator);
        Field3dObject_AddAnimation(&slot->object, &slot->anims[i]);
    }

    Field3dObject_SetActiveFlag(&slot->object, FALSE);
}

void ov02_0224DEF4(Ov02Field3dEffectSlot *slot, NNSFndAllocator *allocator) {
    int i;

    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        Field3dModelAnimation_Unload(&slot->anims[i], allocator);
    }

    memset(slot, 0, sizeof(Ov02Field3dEffectSlot));
}
