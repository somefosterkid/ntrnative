// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov307/egg_demo_view.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "demo/demo_particle.h"
#include "demo/egg_demo.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/particle.h"
#include "gfl/sound.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "pml/poke_party.h"
#include "system/mcss.h"

// The egg and the Pokémon that hatches from it: the egg's sprite shakes with sounds until it turns white and a 3D egg
// cracks open around it with particles, then the Pokémon fades in from white and cries. Manaphy's egg has its own
// sounds and timing

// The frame at which the egg hatches
#define HATCH_FRAME 380
#define HATCH_FRAME_MANAPHY 300
#define CRACK_SE_WAIT_END 309
// The egg's cracking sound repeats from this frame until it hatches, with a wait between plays until CRACK_SE_WAIT_END
#define CRACK_SE_START 150

// The steps of the view
enum {
    VIEW_WAIT_START,
    VIEW_SHAKE,
    VIEW_WAIT_WHITE,
    VIEW_WAIT_CRACK,
    VIEW_HATCHED,
    VIEW_SHOW_POKEMON,
    VIEW_WAIT_REVEAL,
    VIEW_WAIT_CRY,
    VIEW_CRY,
    VIEW_DONE,
};

// What the 3D egg's cracking animations are doing
enum {
    EGG_3D_IDLE,
    EGG_3D_CRACK,
    EGG_3D_CRACKED,
    EGG_3D_CLOSE,
};

typedef struct {
    u16 actor;
    SRTMatrix srt;
    BOOL visible;
} ViewActor;

// Black 2's; twinned code confirms the offsets of heapId, pkm, species, state, started, hatched, frame, crackSEPlaying,
// crackSEWait, manaphySEStep, vblankTask, mcssSystem, mcss, animationEnded, alphaFading, targetAlpha, particle,
// g3dManager, scene, eggActor, actorCount, actors, frame3D and egg3DState, and its size in Black
struct EggDemoView {
    HeapID heapId;
    PartyPkm *pkm;
    u16 species;
    s32 state;
    BOOL started;
    BOOL hatched;
    u32 frame;
    BOOL crackSEPlaying;
    s32 crackSEPlayer;
    u32 crackSEWait;
    u32 manaphySEStep;
    TCB *vblankTask;
    MCSSSystem *mcssSystem;
    MCSS *mcss;
    // Set when the sprite's animation stops at its last frame
    BOOL animationEnded;
    BOOL alphaFading;
    u8 targetAlpha;
    DemoParticle *particle;
    G3DManager *g3dManager;
    u16 scene;
    u16 eggActor;
    u16 actorCount;
    ViewActor *actors;
    u32 frame3D;
    u32 egg3DState;
};

// Where to put some Pokémon's sprites once they hatch
// Black 2's, not checked against Black
typedef struct {
    u32 species;
    f32 x;
    f32 y;
} SpritePosition;

static ViewActor *func_ov215_021f66c8(HeapID heapId, u16 count);
static void func_ov215_021f6718(ViewActor *actors);
static void func_ov215_021f6ab4(TCB *tcb, void *data);
static void func_ov215_021f6ab8(EggDemoView *view);
static void func_ov215_021f6ae0(EggDemoView *view);
static void func_ov215_021f6aec(EggDemoView *view);
static void func_ov215_021f6cac(EggDemoView *view);
void func_ov215_021f6cc0(EggDemoView *view);
static void func_ov215_021f6ccc(EggDemoView *view, u32 animation, BOOL stopAtEnd);
static void func_ov215_021f6d0c(uintptr_t param, fx32 frame);
void func_ov215_021f6d1c(EggDemoView *view);
void func_ov215_021f6d58(EggDemoView *view);
BOOL func_ov215_021f6d7c(EggDemoView *view);
void func_ov215_021f6d90(EggDemoView *view);
static DemoParticle *func_ov215_021f6db4(HeapID heapId, u16 count, const DemoParticleEvent *events);
static void func_ov215_021f6e4c(DemoParticle *particle);
void func_ov215_021f6e5c(DemoParticle *particle);
static void func_ov215_021f6ee4(DemoParticle *particle);
void func_ov215_021f6eec(DemoParticle *particle);
static void func_ov215_021f6ef8(EggDemoView *view);
static void func_ov215_021f6f18(EggDemoView *view);
static void func_ov215_021f6f24(EggDemoView *view);
static void func_ov215_021f6f64(EggDemoView *view);
static void func_ov215_021f7028(EggDemoView *view);
void func_ov215_021f7048(EggDemoView *view);
void func_ov215_021f70f8(EggDemoView *view);
BOOL func_ov215_021f7100(EggDemoView *view);
void func_ov215_021f7110(EggDemoView *view);

// pokebw2's EGG_DEMO_VIEW_UNK_FX32. Not referenced. Its value, FX32_ONE, is the step of the egg's looping animation
// and the scale of its actor
const fx32 data_ov215_021f72fc = FX32_ONE;

// pokebw2's sEggResources
static const G3DSceneResourceSetup data_ov215_021f7354[] = {
    { ARCID_EGG_DEMO, 8, 0 },
    { ARCID_EGG_DEMO, 9, 0 },
    { ARCID_EGG_DEMO, 6, 0 },
    { ARCID_EGG_DEMO, 7, 0 },
};

// pokebw2's sEggAnimations
static const G3DSceneAnimationSetup data_ov215_021f7308[] = { { 1, 0 }, { 2, 0 }, { 3, 0 } };

// pokebw2's sEggActors
static const G3DSceneActorSetup data_ov215_021f7320[] = {
    { 0, 0, 0, 0, data_ov215_021f7308, NELEMS(data_ov215_021f7308) },
};

// pokebw2's sEggScene
static const G3DSceneSetup data_ov215_021f7330 = { data_ov215_021f7354, NELEMS(data_ov215_021f7354), data_ov215_021f7320, NELEMS(data_ov215_021f7320) };

// pokebw2's sParticleEvents
static const DemoParticleEvent data_ov215_021f7314[] = {
    { HATCH_FRAME, 0, 0 },
    { HATCH_FRAME, 0, 1 },
    { HATCH_FRAME, 0, 2 },
};

// pokebw2's sParticleResources
static const DemoParticleResource data_ov215_021f7300[DEMO_PARTICLE_UNIT_COUNT] = { { ARCID_EGG_DEMO, 10 } };

// pokebw2's sManaphyParticleEvents
static const DemoParticleEvent data_ov215_021f7340[] = {
    { 0, 0, 3 },
    { 0, 0, 4 },
    { HATCH_FRAME_MANAPHY, 0, 0 },
    { HATCH_FRAME_MANAPHY, 0, 1 },
    { HATCH_FRAME_MANAPHY, 0, 2 },
};

// pokebw2's sSpritePositions
static const SpritePosition data_ov215_021f7384[] = {
    { SPECIES_CYNDAQUIL, -0.5f, -190.0f },
    { SPECIES_SLUGMA, -0.5f, -189.9f },
    { SPECIES_WYNAUT, 0.0f, -189.8f },
    { SPECIES_MIENFOO, 0.0f, -190.2f },
};

// pokebw2's EggDemoView_CreateActors
static ViewActor *func_ov215_021f66c8(HeapID heapId, u16 count) {
    ViewActor *actors = GFL_HeapAllocate(heapId, sizeof(ViewActor) * count, TRUE, "egg_demo_view.c", 283);
    u16 i;

    for (i = 0; i < count; i++) {
        actors[i].srt.translation.x = 0;
        actors[i].srt.translation.y = 0;
        actors[i].srt.translation.z = 0;
        actors[i].srt.scale.x = FX32_ONE;
        actors[i].srt.scale.y = FX32_ONE;
        actors[i].srt.scale.z = FX32_ONE;
        MTX_Identity33_(&actors[i].srt.rotation);
    }
    return actors;
}

// pokebw2's EggDemoView_FreeActors
static void func_ov215_021f6718(ViewActor *actors) {
    GFL_HeapFree(actors);
}

// pokebw2's EggDemoView_Create
EggDemoView *func_ov215_021f6720(HeapID heapId, PartyPkm *pkm) {
    EggDemoView *view = GFL_HeapAllocate(heapId, sizeof(EggDemoView), TRUE, "egg_demo_view.c", 433);

    view->heapId = heapId;
    view->pkm = pkm;
    view->species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
    view->state = VIEW_WAIT_START;
    view->started = FALSE;
    view->hatched = FALSE;
    view->frame = 0;
    view->crackSEPlaying = FALSE;
    view->crackSEWait = 0;
    view->manaphySEStep = 0;
    view->vblankTask = GFL_VBlankTCBAdd(func_ov215_021f6ab4, view, 1);
    func_ov215_021f6ab8(view);
    func_ov215_021f6aec(view);
    if (view->species == SPECIES_MANAPHY) {
        view->particle = func_ov215_021f6db4(view->heapId, NELEMS(data_ov215_021f7340), data_ov215_021f7340);
    } else {
        view->particle = func_ov215_021f6db4(view->heapId, NELEMS(data_ov215_021f7314), data_ov215_021f7314);
    }
    func_ov215_021f6ef8(view);
    func_ov215_021f6f64(view);
    return view;
}

// pokebw2's EggDemoView_Free
void func_ov215_021f67b0(EggDemoView *view) {
    func_ov215_021f7028(view);
    func_ov215_021f6f18(view);
    func_ov215_021f6e4c(view->particle);
    func_ov215_021f6cac(view);
    func_ov215_021f6ae0(view);
    GFL_TCBRemove(view->vblankTask);
    GFL_HeapFree(view);
}

void func_ov215_021f67e0(EggDemoView *view) {
    u32 hatchFrame;

    switch (view->state) {
    case VIEW_WAIT_START:
        if (view->started) {
            view->state = VIEW_SHAKE;
            view->frame = 0;
            func_ov215_021f6ccc(view, 1, TRUE);
            func_ov215_021f6eec(view->particle);
        }
        break;
    case VIEW_SHAKE:
        if (view->species == SPECIES_MANAPHY) {
            hatchFrame = HATCH_FRAME_MANAPHY;
        } else {
            hatchFrame = HATCH_FRAME;
        }
        view->frame++;
        if (view->species == SPECIES_MANAPHY) {
            switch (view->manaphySEStep) {
            case 0:
                if (view->frame == 10) {
                    GFL_SndSEPlay(SEQ_SE_EDEMO_02);
                    view->manaphySEStep = 1;
                }
                break;
            case 1:
                if (GFL_SndPlayerIsActiveAny() == FALSE) {
                    GFL_SndSEPlay(SEQ_SE_EDEMO_03);
                    view->manaphySEStep = 2;
                }
                break;
            case 2:
                if (GFL_SndPlayerIsActiveAny() == FALSE) {
                    view->manaphySEStep = 3;
                }
                break;
            case 3:
                break;
            }
        } else {
            if (view->frame == 20) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_01);
            } else if (view->frame == 109) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_01);
            } else if (view->frame == 218) {
                GFL_SndSEPlay(SEQ_SE_W181_01);
            }
            if (view->frame == 10) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_05);
            } else if (view->frame == 15) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_05);
            } else if (view->frame == 85) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_05);
            } else if (view->frame == 90) {
                GFL_SndSEPlay(SEQ_SE_EDEMO_05);
            }
            if (view->crackSEPlaying && GFL_SndPlayerIsActive(view->crackSEPlayer) == FALSE) {
                view->crackSEPlaying = FALSE;
                if (view->frame >= CRACK_SE_START && view->frame < CRACK_SE_WAIT_END) {
                    view->crackSEWait = 20;
                } else if (view->frame > CRACK_SE_WAIT_END) {
                    view->crackSEWait = 0;
                }
            }
            if (view->frame >= CRACK_SE_START && view->frame < hatchFrame) {
                if (view->crackSEPlaying == FALSE) {
                    if (view->crackSEWait == 0) {
                        view->crackSEPlayer = GFL_SndSeqGetPlayerIndex(SEQ_SE_EDEMO_04);
                        GFL_SEPlayKeepVol(SEQ_SE_EDEMO_04, view->crackSEPlayer);
                        view->crackSEPlaying = TRUE;
                    } else {
                        view->crackSEWait--;
                    }
                }
            } else if (view->frame == hatchFrame && view->crackSEPlaying &&
                       GFL_SndPlayerIsActive(view->crackSEPlayer)) {
                GFL_SndPlayerStop(view->crackSEPlayer);
                view->crackSEPlaying = FALSE;
            }
        }
        if (view->frame == hatchFrame) {
            func_ov215_021f6d58(view);
            func_ov215_021f70f8(view);
            GFL_SndSEPlay(SEQ_SE_TDEMO_011);
            view->state = VIEW_WAIT_WHITE;
        }
        break;
    case VIEW_WAIT_WHITE:
        if (func_ov215_021f6d7c(view)) {
            view->state = VIEW_WAIT_CRACK;
        }
        break;
    case VIEW_WAIT_CRACK:
        if (func_ov215_021f7100(view)) {
            func_ov215_021f6cc0(view);
            view->hatched = TRUE;
            view->state = VIEW_HATCHED;
        }
        break;
    case VIEW_HATCHED:
        view->hatched = FALSE;
        break;
    case VIEW_WAIT_REVEAL:
        view->frame++;
        if (view->frame >= 120) {
            func_ov215_021f6d90(view);
            func_ov215_021f7110(view);
            view->state = VIEW_WAIT_CRY;
            view->frame = 0;
        }
        break;
    case VIEW_WAIT_CRY:
        view->frame++;
        if (view->frame >= 65) {
            view->state = VIEW_CRY;
            PokeVoice_Play(PokeParty_GetParam(view->pkm, PKM_PARAM_SPECIES, NULL),
                           PokeParty_GetParam(view->pkm, PKM_PARAM_FORM, NULL), 64, 0, 0, 0, 0, 0);
        }
        break;
    case VIEW_CRY:
        if (PokeVoice_IsPlayingAny() == FALSE) {
            view->state = VIEW_DONE;
        }
        break;
    case VIEW_SHOW_POKEMON:
    case VIEW_DONE:
        break;
    }
    func_ov215_021f6d1c(view);
    func_02014c08(view->mcssSystem);
    func_ov215_021f6e5c(view->particle);
    func_ov215_021f7048(view);
}


// pokebw2's EggDemoView_Draw
void func_ov215_021f6a24(EggDemoView *view) {
    func_02014d2c(view->mcssSystem);
    func_ov215_021f6f24(view);
    func_ov215_021f6ee4(view->particle);
}

// pokebw2's EggDemoView_Start
void func_ov215_021f6a3c(EggDemoView *view) {
    view->started = TRUE;
}

// pokebw2's EggDemoView_IsHatched
BOOL func_ov215_021f6a44(EggDemoView *view) {
    return view->hatched;
}

// pokebw2's EggDemoView_ShowPokemon
void func_ov215_021f6a48(EggDemoView *view, PartyPkm *pkm) {
    func_ov215_021f6cac(view);
    view->pkm = pkm;
    func_ov215_021f6aec(view);
    func_ov215_021f6ccc(view, 0, FALSE);
    func_02015ea0(view->mcss);
    func_02015f50(view->mcss, 16, 16, 0, GX_RGB(31, 31, 31));
    view->state = VIEW_SHOW_POKEMON;
}

// pokebw2's EggDemoView_IsWhite
BOOL func_ov215_021f6a84(EggDemoView *view) {
    return func_0201600c(view->mcss) == FALSE;
}

// pokebw2's EggDemoView_Reveal
void func_ov215_021f6a98(EggDemoView *view) {
    view->state = VIEW_WAIT_REVEAL;
    view->frame = 0;
}

// pokebw2's EggDemoView_IsDone
BOOL func_ov215_021f6aa4(EggDemoView *view) {
    return view->state >= VIEW_DONE;
}

// pokebw2's EggDemoView_VBlank
static void func_ov215_021f6ab4(TCB *tcb, void *data) {
}

// pokebw2's EggDemoView_InitMcss
static void func_ov215_021f6ab8(EggDemoView *view) {
    view->mcssSystem = func_02014b08(1, view->heapId);
    func_02016020(view->mcssSystem, 0x30000);
    func_02015bf0(view->mcssSystem);
    view->alphaFading = FALSE;
    view->targetAlpha = 0;
}

// pokebw2's EggDemoView_FreeMcss
static void func_ov215_021f6ae0(EggDemoView *view) {
    func_02014ba8(view->mcssSystem);
}

// pokebw2's EggDemoView_AddMcss
static void func_ov215_021f6aec(EggDemoView *view) {
    u8 i = 0;
    u32 species;
    VecFx32 pos;
    VecFx32 scale;
    VecFx32 offset;
    f32 height;
    f32 top;
    f32 left;

    view->mcss = func_02017270(view->mcssSystem, view->pkm, 0, 0, FX32_CONST(-190), FX32_CONST(-800));
    func_02015ff0(view->mcss, 1);
    species = PokeParty_GetParam(view->pkm, PKM_PARAM_LEGAL_SPECIES, NULL);
    if (species != SPECIES_EGG) {
        for (i = 0; i < NELEMS(data_ov215_021f7384); i++) {
            if (species == data_ov215_021f7384[i].species) {
                pos.x = FX32_CONST(data_ov215_021f7384[i].x);
                pos.y = FX32_CONST(data_ov215_021f7384[i].y);
                pos.z = FX32_CONST(-800);
                func_02015c48(view->mcss, &pos);
                break;
            }
        }
    }
    scale.x = FX32_CONST(16);
    scale.y = FX32_CONST(16);
    scale.z = FX32_ONE;
    func_02015cac(view->mcss, &scale);
    height = func_02015f0c(view->mcss);
    top = func_02015f1c(view->mcss);
    left = func_02015f14(view->mcss);
    if (height > 96.0f) {
        height = 96.0f;
    }
    height = (96.0f - height) / 2.0f + top;
    left = 0.0f - left;
    offset.x = FX32_CONST(left);
    offset.y = FX32_CONST(height);
    offset.z = 0;
    func_02015c78(view->mcss, &offset);
    func_02015e04(view->mcss);
}

// pokebw2's EggDemoView_RemoveMcss
static void func_ov215_021f6cac(EggDemoView *view) {
    func_02015ea0(view->mcss);
    func_02015ba4(view->mcssSystem, view->mcss);
}

// pokebw2's EggDemoView_HideMcss
void func_ov215_021f6cc0(EggDemoView *view) {
    func_02015ea0(view->mcss);
}

// pokebw2's EggDemoView_SetAnimation
static void func_ov215_021f6ccc(EggDemoView *view, u32 animation, BOOL stopAtEnd) {
    NNSG2dMultiCellAnimation *mcAnim;

    func_02016028(view->mcss, animation);
    mcAnim = func_02015ee8(view->mcss);
    NNS_G2dRestartMCAnimation(mcAnim);
    func_02015e4c(view->mcss);
    if (stopAtEnd) {
        NNS_G2dSetAnimCtrlCallBackFunctor(&mcAnim->animCtrl, NNS_G2D_ANMCALLBACKTYPE_LAST_FRM, (uintptr_t)view,
                                          (NNSG2dAnmCallBackPtr)func_ov215_021f6d0c);
        view->animationEnded = FALSE;
    } else {
        func_020173b4(view->mcss);
    }
}

// pokebw2's EggDemoView_OnAnimationEnd
static void func_ov215_021f6d0c(uintptr_t param, fx32 frame) {
    EggDemoView *view = (EggDemoView *)param;

    func_02015e04(view->mcss);
    view->animationEnded = TRUE;
}

// Steps the sprite's alpha toward targetAlpha
// pokebw2's EggDemoView_UpdateAlpha
void func_ov215_021f6d1c(EggDemoView *view) {
    u8 alpha;

    if (view->alphaFading) {
        alpha = func_02015fac(view->mcss);
        if (alpha > view->targetAlpha) {
            alpha--;
        } else {
            alpha++;
        }
        func_02015fbc(view->mcss, alpha);
        if (alpha == view->targetAlpha) {
            view->alphaFading = FALSE;
        }
    }
}

// pokebw2's EggDemoView_FadeToWhite
void func_ov215_021f6d58(EggDemoView *view) {
    func_02015e04(view->mcss);
    func_02015f50(view->mcss, 0, 16, 0, GX_RGB(31, 31, 31));
}

// pokebw2's EggDemoView_IsFadeDone
BOOL func_ov215_021f6d7c(EggDemoView *view) {
    return func_0201600c(view->mcss) == FALSE;
}

// pokebw2's EggDemoView_FadeFromWhite
void func_ov215_021f6d90(EggDemoView *view) {
    func_02015eb0(view->mcss);
    func_02015f50(view->mcss, 16, 0, 2, GX_RGB(31, 31, 31));
}

// pokebw2's EggDemoParticle_Create
static DemoParticle *func_ov215_021f6db4(HeapID heapId, u16 count, const DemoParticleEvent *events) {
    // Not used, like the camera target of the evolution demo copy of this function: pokebw2's EggDemoParticle_Create
    // has it too, and the code zeroes it all the same
    VecFx32 pos = { 0, 0, 0 };
    DemoParticle *particle = GFL_HeapAllocate(heapId, sizeof(DemoParticle), TRUE, "egg_demo_view.c", 1282);
    void *resource;
    u32 i;

    particle->frame = 0;
    particle->index = 0;
    particle->count = count;
    particle->events = events;
    particle->active = FALSE;
    particle->stopTimer = -1;
    func_02050b80(heapId);
    for (i = 0; i < DEMO_PARTICLE_UNIT_COUNT; i++) {
        particle->units[i].system = func_02050c04(particle->units[i].buffer, DEMO_PARTICLE_BUFFER_SIZE, TRUE, heapId);
        resource = func_0205121c(data_ov215_021f7300[i].arcId, data_ov215_021f7300[i].fileId, heapId);
        particle->units[i].resourceCount = func_02051b04(resource);
        func_02051230(particle->units[i].system, resource, TRUE, FALSE);
    }
    return particle;
}

// pokebw2's EggDemoParticle_Free
static void func_ov215_021f6e4c(DemoParticle *particle) {
    func_02050ed4();
    GFL_HeapFree(particle);
}

// Emits the particles of the events at the current frame
// pokebw2's EggDemoParticle_Update
void func_ov215_021f6e5c(DemoParticle *particle) {
    if (particle->active) {
        VecFx32 pos = { 0, FX32_CONST(-0.5), 0 };

        while (particle->index < particle->count) {
            if (particle->frame != particle->events[particle->index].frame) {
                break;
            }
            func_020515b4(particle->units[particle->events[particle->index].unit].system,
                          particle->events[particle->index].emitter, &pos);
            particle->index++;
        }
        particle->frame++;
    }
    if (particle->stopTimer >= 0) {
        if (particle->stopTimer == 0) {
            func_02051620(particle->units[0].system);
        }
        particle->stopTimer--;
    }
}

// pokebw2's EggDemoParticle_Draw
static void func_ov215_021f6ee4(DemoParticle *particle) {
    func_02050be8();
}

// pokebw2's EggDemoParticle_Start
void func_ov215_021f6eec(DemoParticle *particle) {
    particle->active = TRUE;
}

// pokebw2's EggDemoView_Init3D
static void func_ov215_021f6ef8(EggDemoView *view) {
    GFL_G3DSysSetSwapBufferParams(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    view->g3dManager = GFL_G3DMgrCreate(4, 1, view->heapId);
    view->actorCount = 0;
}

// pokebw2's EggDemoView_Free3D
static void func_ov215_021f6f18(EggDemoView *view) {
    GFL_G3DMgrFree(view->g3dManager);
}

// pokebw2's EggDemoView_Draw3D
static void func_ov215_021f6f24(EggDemoView *view) {
    u16 i;
    ViewActor *actor;

    for (i = 0; i < view->actorCount; i++) {
        actor = &view->actors[i];
        if (actor->visible) {
            GFL_G3DSysDrawObj(GFL_G3DMgrGetActor(view->g3dManager, actor->actor), &actor->srt);
        }
    }
}

// pokebw2's EggDemoView_AddScene
static void func_ov215_021f6f64(EggDemoView *view) {
    u16 i;
    u16 first;
    ViewActor *actor;
    G3DActor *egg;
    fx32 frame;

    view->scene = GFL_G3DMgrNewScene(view->g3dManager, &data_ov215_021f7330);
    view->actorCount = 1;
    view->actors = func_ov215_021f66c8(view->heapId, view->actorCount);
    first = GFL_G3DMgrGetSceneFirstActorIdx(view->g3dManager, view->scene);
    view->eggActor = 0;
    actor = &view->actors[0];
    actor->actor = first;
    actor->srt.translation.x = 0;
    actor->srt.translation.y = 0;
    actor->srt.translation.z = 0;
    actor->visible = TRUE;
    for (i = 0; i < view->actorCount; i++) {
        GFL_G3DActorBindAnm(GFL_G3DMgrGetActor(view->g3dManager, view->actors[i].actor), 0);
    }
    view->frame3D = 0;
    egg = GFL_G3DMgrGetActor(view->g3dManager, view->actors[view->eggActor].actor);
    frame = 0;
    view->egg3DState = EGG_3D_IDLE;
    GFL_G3DActorBindAnm(egg, 1);
    GFL_G3DActorBindAnm(egg, 2);
    GFL_G3DActorSetAnmFrame(egg, 1, &frame);
    GFL_G3DActorSetAnmFrame(egg, 2, &frame);
}

// pokebw2's EggDemoView_DeleteScene
static void func_ov215_021f7028(EggDemoView *view) {
    GFL_G3DMgrDeleteScene(view->g3dManager, view->scene);
    func_ov215_021f6718(view->actors);
    view->actorCount = 0;
}

// pokebw2's EggDemoView_Update3D
void func_ov215_021f7048(EggDemoView *view) {
    u16 i;
    G3DActor *egg;
    BOOL running1;
    BOOL running2;

    for (i = 0; i < view->actorCount; i++) {
        GFL_G3DActorStepAnmFrameLoop(GFL_G3DMgrGetActor(view->g3dManager, view->actors[i].actor), 0, FX32_ONE);
    }
    view->frame3D++;
    egg = GFL_G3DMgrGetActor(view->g3dManager, view->actors[view->eggActor].actor);
    if (view->egg3DState == EGG_3D_CRACK) {
        running1 = GFL_G3DActorStepAnmFrame(egg, 1, FX32_CONST(5));
        running2 = GFL_G3DActorStepAnmFrame(egg, 2, FX32_CONST(5));
        if (running1 == FALSE && running2 == FALSE) {
            view->egg3DState = EGG_3D_CRACKED;
        }
    } else if (view->egg3DState == EGG_3D_CLOSE) {
        running1 = GFL_G3DActorStepAnmFrame(egg, 1, -FX32_CONST(2));
        running2 = GFL_G3DActorStepAnmFrame(egg, 2, -FX32_CONST(2));
        if (running1 == FALSE && running2 == FALSE) {
            view->egg3DState = EGG_3D_IDLE;
        }
    }
}

// pokebw2's EggDemoView_CrackEgg
void func_ov215_021f70f8(EggDemoView *view) {
    view->egg3DState = EGG_3D_CRACK;
}

// pokebw2's EggDemoView_IsEggCracked
BOOL func_ov215_021f7100(EggDemoView *view) {
    return view->egg3DState == EGG_3D_CRACKED;
}

// pokebw2's EggDemoView_CloseEgg
void func_ov215_021f7110(EggDemoView *view) {
    view->egg3DState = EGG_3D_CLOSE;
}
