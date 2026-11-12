// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov294/intro_g3d.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "demo/intro.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/sound.h"
#include "nitro/ioreg.h"
#include "nitro/fx/fx_mtx.h"

// The intro's 3D model, which is only loaded in the parts of the intro that show it

#define SCENE_NONE 0xffff

struct IntroG3d {
    IntroGraphic *graphic;
    HeapID heapId;
    G3DManager *manager;
    G3DCamera *camera;
    u16 scene;
    BOOL visible;
    BOOL unk18;
    u32 mode;
    u32 unk20;
    u32 state;
    u32 frame;
    u32 unk2C;
};

// pokebw2's sPosition
static const VecFx32 data_ov204_021c0914 = { 0, 0, FX32_ONE };
// pokebw2's sUpVector
static const VecFx32 data_ov204_021c0920 = { 0, FX32_ONE, 0 };
// pokebw2's sTarget
static const VecFx32 data_ov204_021c0908 = { 0, 0, -FX32_ONE };

// The model and its animations
// pokebw2's sResources
static const G3DSceneResourceSetup data_ov204_021c0960[] = {
    { ARCID_INTRO, 25, 0 }, { ARCID_INTRO, 23, 0 }, { ARCID_INTRO, 24, 0 }, { ARCID_INTRO, 26, 0 }, { ARCID_INTRO, 27, 0 }, { ARCID_INTRO, 28, 0 }, // Black's
};

// pokebw2's sAnimations
static const G3DSceneAnimationSetup data_ov204_021c094c[] = {
    { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 },
};

// pokebw2's sActors
static const G3DSceneActorSetup data_ov204_021c092c[] = {
    { 0, 0, 0, 0, data_ov204_021c094c, NELEMS(data_ov204_021c094c) },
};

// pokebw2's sSceneSetup
static const G3DSceneSetup data_ov204_021c093c = { data_ov204_021c0960, NELEMS(data_ov204_021c0960), data_ov204_021c092c, NELEMS(data_ov204_021c092c) };

// pokebw2's IntroG3d_Create
IntroG3d *func_ov204_021bf8dc(IntroGraphic *graphic, u32 mode, HeapID heapId) {
    IntroG3d *g3d = GFL_HeapAllocate(heapId, sizeof(IntroG3d), TRUE, "intro_g3d.c", 162);
    VecFx32 position;
    VecFx32 upVector;
    VecFx32 target;
    G3DActor *actor;

    g3d->heapId = heapId;
    g3d->graphic = graphic;
    g3d->manager = GFL_G3DMgrCreate(10, 16, heapId);
    position = data_ov204_021c0914;
    upVector = data_ov204_021c0920;
    target = data_ov204_021c0908;
    g3d->camera = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, 0x6000, 0xffffa000, 0xffff8000,
                                      0x8000, 0x1000, 0x400000, 0x1000, &position, &upVector, &target,
                                      heapId);
    if (mode != 7) {
        g3d->scene = GFL_G3DMgrNewScene(g3d->manager, &data_ov204_021c093c);
        actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
        GFL_G3DActorBindAnm(actor, 3);
        GFL_G3DActorBindAnm(actor, 4);
        GFL_G3DActorUnbindAnm(actor, 0);
        GFL_G3DActorUnbindAnm(actor, 1);
        GFL_G3DActorUnbindAnm(actor, 2);
    } else {
        g3d->scene = SCENE_NONE;
    }
    g3d->unk18 = TRUE;
    return g3d;
}

// pokebw2's IntroG3d_Free
void func_ov204_021bf9d0(IntroG3d *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    if (g3d->scene != SCENE_NONE) {
        GFL_G3DMgrDeleteScene(g3d->manager, g3d->scene);
    }
    GFL_G3DMgrFree(g3d->manager);
    GFL_HeapFree(g3d);
}

// pokebw2's IntroG3d_Draw
void func_ov204_021bf9fc(IntroG3d *g3d) {
    SRTMatrix transform;

    GFL_G3DCameraFlush(g3d->camera);
    if (g3d->visible && g3d->unk18) {
        transform.translation.x = 0;
        transform.translation.y = 0;
        transform.translation.z = 0;
        transform.scale.x = FX32_ONE;
        transform.scale.y = FX32_ONE;
        transform.scale.z = FX32_ONE;
        MTX_Identity33_(&transform.rotation);
        GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(g3d->manager, g3d->scene), &transform);
    }
}

// pokebw2's IntroG3d_Open
BOOL func_ov204_021bfa40(IntroG3d *g3d) {
    G3DActor *actor;
    BOOL playing3;
    BOOL playing4;

    switch (g3d->state) {
    case 0:
        G2_SetBlendAlpha(1, 9, 0, 0);
        GFL_SndSEPlay(SEQ_SE_OPEN2);
        g3d->state++;
    case 1:
        actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
        playing3 = GFL_G3DActorStepAnmFrame(actor, 3, FX32_ONE);
        playing4 = GFL_G3DActorStepAnmFrame(actor, 4, FX32_ONE);
        if (!playing3 && !playing4) {
            GFL_G3DActorUnbindAnm(actor, 3);
            GFL_G3DActorUnbindAnm(actor, 4);
            GFL_G3DActorBindAnm(actor, 0);
            GFL_G3DActorBindAnm(actor, 1);
            GFL_G3DActorBindAnm(actor, 2);
            func_ov204_021bfbc8(g3d, 60);
            g3d->state++;
        }
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IntroG3d_SetVisible
void func_ov204_021bfae4(IntroG3d *g3d, BOOL visible) {
    g3d->visible = visible;
}

// pokebw2's IntroG3d_SetMode
void func_ov204_021bfae8(IntroG3d *g3d, u32 mode) {
    g3d->mode = mode;
    g3d->unk20 = 0;
    switch (mode) {
    case 0:
        g3d->frame = 60;
        g3d->mode = 1;
        break;
    case 1:
        g3d->frame = 70;
        break;
    case 2:
        g3d->frame = 50;
        break;
    case 3:
        g3d->frame = 50;
        break;
    case 4:
        g3d->frame = 70;
        break;
    }
}

// pokebw2's IntroG3d_Animate
BOOL func_ov204_021bfb2c(IntroG3d *g3d) {
    switch (g3d->mode) {
    case 0:
        break;
    case 1:
        g3d->frame--;
        func_ov204_021bfbc8(g3d, g3d->frame);
        if (g3d->frame == 50) {
            return TRUE;
        }
        break;
    case 2:
        g3d->frame++;
        func_ov204_021bfbc8(g3d, g3d->frame);
        if (g3d->frame == 70) {
            return TRUE;
        }
        break;
    case 3:
        g3d->frame--;
        func_ov204_021bfbc8(g3d, g3d->frame);
        if (g3d->frame == 0) {
            return TRUE;
        }
        break;
    case 4:
        g3d->frame++;
        func_ov204_021bfbc8(g3d, g3d->frame);
        if (g3d->frame == 120) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's IntroG3d_AnimateBack
BOOL func_ov204_021bfba0(IntroG3d *g3d) {
    if (g3d->mode == 3) {
        func_ov204_021bfbc8(g3d, ++g3d->frame);
    } else {
        func_ov204_021bfbc8(g3d, --g3d->frame);
    }
    if (g3d->frame == 60) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's IntroG3d_SetFrame
void func_ov204_021bfbc8(IntroG3d *g3d, u32 frame) {
    G3DActor *actor = GFL_G3DMgrGetActor(g3d->manager, g3d->scene);
    fx32 value = FX32_CONST(frame);

    GFL_G3DActorSetAnmFrame(actor, 0, &value);
    GFL_G3DActorSetAnmFrame(actor, 1, &value);
    GFL_G3DActorSetAnmFrame(actor, 2, &value);
}
