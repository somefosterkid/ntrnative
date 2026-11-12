// Black's code at ov021 0x021c1a48-0x021c27f4. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "constants/sound.h"
#include "field/encounter_effect.h"
#include "field/field_event.h"
#include "gfl/bg_sys.h"
#include "gfl/g3d.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/heapsys.h"
#include "gfl/particle.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "pml/poke_party.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "app/psel_graphic.h"
#include "gfl/overlay.h"
#include "field/field_actor.h"
#include "field/field.h"
#include "field/fieldmap.h"
#include "field/ov021_021a0138.h"
#include "gfl/sound.h"
#include "field/ov021_021929e8.h"
#include "field/field_place_name.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "nnsys/gfd.h"
#include "gfl/fade.h"
#include "save/player_info.h"
#include "pml/poke_graphic.h"
#include "gfl/std.h"
#include "gfl/random.h"
#include "system/str_tool.h"
#include "system/gf_font.h"
#include "system/wordset.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "nitro/os.h"
#include "field/field_exp_obj.h"

typedef struct effectId effectId;

// A sound effect of a field effect, at a frame. A sound effect of -1 ends a list
typedef struct Fld3DCiSoundKey {
    u32 se;
    u32 frame;
} Fld3DCiSoundKey;


// fld_particle.c: the field's particle system
typedef struct FieldParticle FieldParticle;
ParticleSystem *func_ov021_021c1a48(FieldParticle *param_1);
void func_ov021_021c1a84(FieldParticle *param_1);
void *func_ov021_021c1a98(FieldParticle *param_1, u32 param_2, u32 param_3);
void func_ov021_021c1aa8(FieldParticle *param_1, void *param_2, BOOL param_3, TCBManager *param_4);
u32 func_ov021_021c1ab4(FieldParticle *param_1);

u32 func_ov021_021c1e7c(GameEvent *event, u32 *state, void *work);
GameEvent *EventFieldEffect_CreatePokeSpriteCore(GameSystem *param_1, void *param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7);
void func_ov021_021c24bc(SPLEmitter *emitter);
void Fld3DCi_ScreenCapTCB(TCB *tcb, void *work);

typedef struct Fld3dCiSetup {
    G3DSceneResourceSetup *resources;
    G3DSceneActorSetup *actors;
    G3DSceneAnimationSetup *animations1;
    G3DSceneAnimationSetup *animations2;
    G3DSceneSetup scene;
    u16 unk_20;
    u16 unk_22;
    u16 unk_24;
    u16 unk_26;
    u32 unk_28;
    u8 resourceCount;
    u8 actorCount;
    u8 unk_2e;
    u8 unk_2f;
    u32 unk_30;
} Fld3dCiSetup;

typedef struct FieldEffectParams {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u32 unk_c;
    u32 unk_10;
    u32 unk_14;
    u16 unk_18;
    u16 unk_1a;
    u32 voice;
} FieldEffectParams;

typedef struct Fld3dCi Fld3dCi;
struct Fld3dCi {
    FieldParticle *particleSystem;
    G3DCamera *camera0;
    G3DCamera *camera1;
    u32 unk_c;
    u16 heapId;
    G3DManager *g3dMan;
    s32 unk_18;
    SRTMatrix srt;
    ParticleSystem *particles;
    u16 unk_5c;
    u16 unk_5e;
    u16 unk_60;
    u16 unk_62;
    u16 unk_64;
    u16 effectId;
    u32 enabledBGs;
    u8 bgPriority[4];
    u32 screenCapDone;
    u32 dimDone;
    u32 unk_78;
    u32 unk_7c;
    u32 unk_80;
    u32 unk_84;
    NNSG2dCharacterData *charData;
    NNSG2dPaletteData *paletteData;
    GFLBitmap *bitmap;
    void *charBuf;
    void *paletteBuf;
    FieldEffectParams effect;
    u8 unk_bc[0x11c - 0xbc];
    u32 unk_11c;
    u32 unk_120;
    u32 unk_124;
    u32 (*unk_128)(GameEvent *, s32 *);
    u32 (*unk_12c)(GameEvent *, struct Fld3dCi *);
    u32 unk_130;
    u16 frame;
    u16 soundIndex;
};

typedef struct Fld3dCiEvent {
    Fld3dCi *ci;
    Fld3dCiSetup setup;
    u32 disableMovement;
    u32 unk_3c;
} Fld3dCiEvent;

void func_ov021_021c313c(Fld3dCi *ci, Fld3dCiEvent *data);

// The field's camera: a 40-degree perspective at 4:3, from 1 to 1024 units
static inline G3DCamera *Fld3dCi_CreateCamera0(VecFx32 *pos, VecFx32 *target, HeapID heapId) {
    VecFx32 up = { 0, FX32_ONE, 0 };

    return GFL_G3DCameraCreate(G3DCAM_PROJECTION_PERSPECTIVE, FX_SinIdx(DEG_TO_IDX(20)), FX_CosIdx(DEG_TO_IDX(20)), 0x1555, 0, 0x1000, 0x400000, 0, pos, &up, target, heapId);
}

// The sound effects of the field effects, by the frame each one starts at. A sound effect of -1 ends a list
static const Fld3DCiSoundKey data_ov021_021da1d8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1c0[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da388[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da178[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da3c0[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da580[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da430[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da468[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da4a0[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da510[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da548[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da200[] = { { SEQ_SE_FLD_01, 24 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da210[] = { { SEQ_SE_FLD_172, 1 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da290[] = {
    { SEQ_SE_ROTATION_B, 10 },
    { SEQ_SE_SHDEMO_04, 38 },
    { SEQ_SE_TDEMO_003, 91 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da2d0[] = {
    { SEQ_SE_W009_01, 1 },
    { SEQ_SE_FLD_164, 22 },
    { SEQ_SE_SHDEMO_04, 46 },
    { SEQ_SE_FLD_127, 117 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da2f8[] = {
    { SEQ_SE_FLD_164, 9 },
    { SEQ_SE_FLD_164, 15 },
    { SEQ_SE_TDEMO_006, 30 },
    { SEQ_SE_W009_01, 49 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da260[] = { { SEQ_SE_FLD_166, 1 }, { SEQ_SE_SYS_89, 38 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da220[] = { { SEQ_SE_FLD_01, 24 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da230[] = { { SEQ_SE_FLD_01, 24 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da240[] = { { SEQ_SE_FLD_172, 1 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da250[] = { { SEQ_SE_FLD_157, 1 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1d0[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da170[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da2b0[] = {
    { SEQ_SE_FLD_164, 2 },
    { SEQ_SE_NDEMO_01, 12 },
    { SEQ_SE_SDEMO_03, 21 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da320[] = {
    { SEQ_SE_W010_01, 10 },
    { SEQ_SE_W010_01, 27 },
    { SEQ_SE_W010_01, 41 },
    { SEQ_SE_W043_01, 52 },
    { SEQ_SE_SYS_89, 75 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da278[] = { { SEQ_SE_FLD_152, 16 }, { SEQ_SE_SDEMO_03, 32 }, { -1 } };
static const Fld3DCiSoundKey data_ov021_021da188[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da150[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da190[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1a8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da198[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da160[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da158[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da148[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da168[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1f0[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da140[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1a0[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1f8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1e8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1b8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1c8[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da628[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da660[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da698[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da350[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da3f8[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da4d8[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da5b8[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da5f0[] = {
    { SEQ_SE_TDEMO_006, 1 },
    { SEQ_SE_BGDEMO_01, 3 },
    { SEQ_SE_W319_01, 159 },
    { SEQ_SE_BGDEMO_02, 208 },
    { SEQ_SE_SYS_89, 272 },
    { SEQ_SE_ROTATION_B, 316 },
    { -1 },
};
static const Fld3DCiSoundKey data_ov021_021da1e0[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da180[] = { { -1 } };
static const Fld3DCiSoundKey data_ov021_021da1b0[] = { { -1 } };

// The sound effects of each field effect, by its effect ID
static const Fld3DCiSoundKey *const data_ov021_021da720[] = {
    data_ov021_021da1d8, data_ov021_021da1c0, data_ov021_021da388, data_ov021_021da178, data_ov021_021da3c0,
    data_ov021_021da580, data_ov021_021da430, data_ov021_021da468, data_ov021_021da4a0, data_ov021_021da510,
    data_ov021_021da548, data_ov021_021da200, data_ov021_021da210, data_ov021_021da290, data_ov021_021da2d0,
    data_ov021_021da2f8, data_ov021_021da260, data_ov021_021da220, data_ov021_021da230, data_ov021_021da240,
    data_ov021_021da250, data_ov021_021da1d0, data_ov021_021da170, data_ov021_021da2b0, data_ov021_021da320,
    data_ov021_021da278, data_ov021_021da188, data_ov021_021da150, data_ov021_021da190, data_ov021_021da1a8,
    data_ov021_021da198, data_ov021_021da160, data_ov021_021da158, data_ov021_021da148, data_ov021_021da168,
    data_ov021_021da1f0, data_ov021_021da140, data_ov021_021da1a0, data_ov021_021da1f8, data_ov021_021da1e8,
    data_ov021_021da1b8, data_ov021_021da1c8, data_ov021_021da628, data_ov021_021da660, data_ov021_021da698,
    data_ov021_021da350, data_ov021_021da3f8, data_ov021_021da4d8, data_ov021_021da5b8, data_ov021_021da5f0,
    data_ov021_021da1e0, data_ov021_021da180, data_ov021_021da1b0,
};

// The free heap each field effect needs, by its effect ID
static const u32 data_ov021_021da7f4[] = {
#include "asset/fld3d_ci_data_ov021_021da7f4.inc"
};

u32 func_ov021_021c27f4(GameEvent *event, s32 *state);
u32 func_ov021_021c2854(GameEvent *event, Fld3dCi *ci);
s32 *func_ov074_021f54e0(s32);
u32 func_ov021_021c2d68(GameEvent *event, s32 *state);
void func_ov021_021c2f84(PlayerInfo *info, Fld3dCi *ci, const char *texName, const char *plName, u32 msgId, BOOL calcWidth);
u32 func_ov021_021c2cf4(GameEvent *event, Fld3dCi *ci);
void func_ov021_021c24a8(Fld3dCi *ci, s32 emitterId);
void func_ov021_021c24f0(Fld3dCiSetup *setup, u32 fileId, HeapID heapId);
void func_ov021_021c27c0(Fld3dCi *ci);
u32 func_ov021_021c2bd4(s32);
GameEventReturnCode func_ov021_021c2b1c(GameEvent *event, u32 *state, void *work);
void func_ov021_021c2764(Fld3dCi *ci);
void func_ov021_021c2320(GameEvent *param_1, Fld3dCi *param_2, Fld3dCiSetup *param_3, u8 param_4);
GameEventReturnCode func_ov021_021c2b98(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021c2b5c(GameEvent *event, u32 *state, void *work);
u32 func_ov021_021c21b4(Fld3dCi *work);
u32 func_ov021_021c223c(Fld3dCi *work);
u32 func_ov021_021c22ac(Fld3dCi *work);
void func_ov021_021c3158(Fld3dCi *ci);
void func_ov021_021c2484(Fld3dCi *ci, Fld3dCiSetup *setup);
void func_ov021_021c2734(Fld3dCiSetup *setup);
void func_ov021_021a0568(void *arg, u32 flag);
void func_ov021_021c3198(TCB *param_1, void *param_2);
void func_ov021_021c27cc(Fld3dCi *ci);
void func_ov021_021c27e8(Fld3dCi *ci);
void func_ov021_021c2918(Fld3dCi *ci);
void func_ov021_021c27a4(Fld3dCi *ci);
void func_ov021_021c30d4(void);


Fld3dCi *func_ov021_021c1af4(HeapID heapId, FieldParticle *particleSystem) {
    Fld3dCi *work;

    work = GFL_HeapAllocate(heapId, sizeof(Fld3dCi), TRUE, "fld3d_ci.c", 0x158);
    work->g3dMan = GFL_G3DMgrCreate(8, 2, heapId);
    work->heapId = heapId;
    work->particleSystem = particleSystem;
    work->unk_18 = -1;
    {
        VecFx32 pos = { 0, 0, FX32_CONST(128) };
        VecFx32 target = { 0, 0, 0 };

        work->camera0 = Fld3dCi_CreateCamera0(&pos, &target, heapId);
    }
    {
        VecFx32 pos = { 0, 0, FX32_CONST(128) };
        VecFx32 target = { 0, 0, 0 };
        VecFx32 up = { 0, FX32_ONE, 0 };

        work->camera1 = GFL_G3DCameraCreate(G3DCAM_PROJECTION_ORTHO, 0x30000, -0x30000, -0x40000, 0x40000, 0x1000, 0x400000, 0, &pos, &up, &target, heapId);
    }
    work->unk_c = 0;
    return work;
}



void func_ov021_021c1c04(Fld3dCi *ci) {
    GFL_G3DCameraFree(ci->camera0);
    GFL_G3DCameraFree(ci->camera1);
    GFL_G3DMgrFree(ci->g3dMan);
    GFL_HeapFree(ci);
}

void func_ov021_021c1c24(Fld3dCi *ci, G3DSceneSetup *setup) {
    ci->unk_18 = GFL_G3DMgrNewScene(ci->g3dMan, setup);
    ci->srt.scale.x = FX32_ONE;
    ci->srt.scale.y = FX32_ONE;
    ci->srt.scale.z = FX32_ONE;
    MTX_Identity33_(&ci->srt.rotation);
    ci->srt.translation.x = 0;
    ci->srt.translation.y = 0;
    ci->srt.translation.z = 0;
}

void func_ov021_021c1c4c(Fld3dCi *work) {
    G3DCamera *cam;
    G3DActor *actor;
    u16 firstActor;
    u16 actorCount;
    u8 i;

    if (work->unk_c == 0) {
        cam = work->camera0;
    } else {
        cam = work->camera1;
    }
    GFL_G3DCameraFlush(cam);
    if (work->unk_18 != -1) {
        firstActor = GFL_G3DMgrGetSceneFirstActorIdx(work->g3dMan, (u16)work->unk_18);
        actorCount = GFL_G3DSceneGetNodeActorCount(work->g3dMan, (u16)work->unk_18);
        for (i = 0; i < actorCount; i++) {
            actor = GFL_G3DMgrGetActor(work->g3dMan, firstActor + i);
            GFL_G3DSysDrawObj(actor, &work->srt);
        }
    }
}


GameEvent *EventFieldEffect_Create(GameSystem *gsys, void *g3dCi, u8 effectId) {
    GameEvent *event;
    Fld3dCi *ci = g3dCi;

    if (GFL_HeapGetHighestAllocatableSize(0x15) < data_ov021_021da7f4[effectId]) {
        return (GameEvent *)0x0;
    }
    ci->unk_64 = 0;
    ci->unk_130 = 0;
    ci->frame = 0;
    ci->soundIndex = 0;
    ci->effectId = effectId;
    ci->unk_128 = NULL;
    ci->unk_12c = NULL;
    ci->unk_c = 0;
    event = GameEvent_Create(gsys, (GameEvent *)0x0, (void *)func_ov021_021c1e7c, sizeof(Fld3dCiEvent));
    func_ov021_021c313c(ci, GameEvent_GetData(event));
    return event;
}

GameEvent *EventFieldEffect_CreatePokeSpriteCore(GameSystem *param_1, void *param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6, u32 param_7) {
    GameEvent *event;
    Fld3dCi *work;
    FieldEffectParams *params;

    event = EventFieldEffect_Create(param_1, param_2, 3);
    if (event == NULL) {
        return NULL;
    }
    work = param_2;
    work->unk_128 = func_ov021_021c27f4;
    work->unk_12c = func_ov021_021c2854;
    params = &work->effect;
    params->unk_0 = param_3;
    params->unk_4 = param_4;
    params->unk_8 = param_5;
    params->unk_c = param_6;
    params->unk_10 = param_7;
    params->unk_18 = 5;
    params->unk_14 = 1;
    params->unk_1a = 0x1e;
    return event;
}


GameEvent *EventFieldEffect_CreatePokeSprite(GameSystem *gsys, void *g3DCi, u8 partySlot) {
    GameData *gameData;
    PokeParty *party;
    PartyPkm *pkm;
    u32 result;
    u32 result2;
    u32 result3;
    BOOL BVar4;
    GameEvent *pGVar6;

    gameData = GSYS_GetGameData(gsys);
    party = GameData_GetParty(gameData);
    pkm = PokeParty_GetPkm(party, (u32)partySlot);
    result = PokeParty_GetParam(pkm, 5, (void *)0x0);
    result2 = PokeParty_GetParam(pkm, 0x6f, (void *)0x0);
    result3 = PokeParty_GetParam(pkm, 0x6e, (void *)0x0);
    BVar4 = PokeParty_IsRare(pkm);
    pGVar6 = EventFieldEffect_CreatePokeSpriteCore(gsys, g3DCi, result, result2, result3, BVar4, PokeParty_GetParam(pkm, 0x4c, (void *)0x0));
    return pGVar6;
}

GameEvent *func_ov021_021c1dcc(GameSystem *param_1, void *param_2, u32 param_3, u32 param_4) {
    s32 *ptr;
    GameEvent *event;
    Fld3dCiEvent *data;
    Fld3dCi *work;
    FieldEffectParams *params;

    GFL_OvlLoad(OVERLAY_ID(74));
    ptr = func_ov074_021f54e0(param_3);
    event = EventFieldEffect_Create(param_1, param_2, (u8)ptr[0]);
    if (event == NULL) {
        GFL_OvlUnload(OVERLAY_ID(74));
        return NULL;
    }
    data = GameEvent_GetData(event);
    func_ov021_021c313c(param_2, data);
    data->unk_3c = param_4;
    work = param_2;
    if (ptr[4] != 0) {
        work->unk_128 = func_ov021_021c2d68;
    } else {
        work->unk_128 = NULL;
    }
    work->unk_12c = func_ov021_021c2cf4;
    work->unk_c = 2;
    params = &work->effect;
    params->unk_0 = ptr[3];
    params->unk_4 = ptr[1];
    params->unk_8 = ptr[2];
    if (ptr[4] == 1) {
        params->unk_c = 1;
    } else {
        params->unk_c = 0;
    }
    params->unk_10 = ptr[0] == 0xd && params->unk_c != 0;
    GFL_OvlUnload(OVERLAY_ID(74));
    return event;
}




u32 func_ov021_021c1e7c(GameEvent *event, u32 *state, void *work) {
    Fld3dCiEvent *data = work;
    Fld3dCi *ci = data->ci;
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    Field *field = GSYS_GetField(gsys);

    switch (*state) {
    case 0:
        if (data->disableMovement) {
            DisableAllActorsMovement(Field_GetActorSystem(field));
        }
        func_ov021_021c27a4(ci);
        func_ov021_021c27c0(ci);
        if (!func_ov021_021c2bd4(ci->effectId)) {
            GameEvent_ChainNext(event, GameEvent_Create(gsys, event, func_ov021_021c2b1c, 0));
        }
        (*state)++;
        break;
    case 1:
        (*state)++;
        if (ci->effectId == 0xb || ci->effectId == 0x12) {
            SetActorHidden(FieldPlayer_GetActor(Field_GetPlayer(field)), TRUE);
            break;
        }
        if (ci->effectId == 0x11) {
            FieldActor *actor = FindFieldActor(Field_GetActorSystem(field), ci->effect.unk_0);

            if (actor != NULL) {
                DeleteActor(actor);
            }
            break;
        }
    case 2:
        func_ov021_02189a58(field, TRUE);
        func_ov021_021c2764(ci);
        (*state)++;
        break;
    case 3:
        if (ci->screenCapDone == 0) {
            break;
        }
        GFL_BGSysSetBGPriority(0, 0);
        GFL_BGSysSetBGPriority(2, 1);
        {
            BGSysLCDConfig lcdConfig = { GX_DISPMODE_GRAPHICS, GX_BGMODE_5, GX_BGMODE_0, GX_BG0_AS_3D };

            GFL_BGSysSetLCDConfig(&lcdConfig);
        }
        reg_G2_BG2CNT = (reg_G2_BG2CNT & 0x43) | 0x4084;
        GX_SetBankForBG(8);
        G2_SetBlendAlpha(0, 4, 0, 0);
        GFL_BGSysSetBGEnabled(2, 1);
        FieldFog_SetEnabled(Field_GetFog(field), 0);
        (*state)++;
        break;
    case 4:
        func_ov021_021c2320(event, ci, &data->setup, ci->effectId);
        GFL_BGSysSetBGEnabled(2, 1);
        GFL_BGSysSetBGEnabled(0, 1);
        GFL_HeapGetFreeSize(ci->heapId);
        Field_SetRenderMode(field, 1);
        if (data->setup.unk_30 != 0) {
            func_ov021_021c30d4();
        }
        (*state)++;
        break;
    case 5:
        if (func_ov021_021c2bd4(ci->effectId)) {
            GameEvent_ChainNext(event, GameEvent_Create(gsys, event, func_ov021_021c2b98, 0));
        } else {
            GameEvent_ChainNext(event, GameEvent_Create(gsys, event, func_ov021_021c2b5c, 0));
        }
        (*state)++;
        break;
    case 6:
        if (func_02005c64() != 0) {
            break;
        }
        (*state)++;
    case 7:
        ci->frame++;
        {
            u32 a = func_ov021_021c21b4(ci);
            u32 b = func_ov021_021c223c(ci);
            u32 c = func_ov021_021c22ac(ci);

            ci->unk_11c = a;
            ci->unk_120 = b;
            ci->unk_124 = c;
        }
        func_ov021_021c3158(ci);
        {
            u32 r = 0;

            if (ci->unk_12c != NULL) {
                r = ci->unk_12c(event, ci);
            } else if (ci->unk_11c != 0 && ci->unk_120 != 0 && ci->unk_124 != 0) {
                r = 1;
            }
            if (r == 0) {
                break;
            }
        }
        (*state)++;
        break;
    case 8:
        Field_SetRenderMode(field, 0);
        func_ov021_021c2484(ci, &data->setup);
        if (func_ov021_021c2bd4(ci->effectId)) {
            FieldActor *actor = FieldPlayer_GetActor(Field_GetPlayer(field));

            SetActorHidden(actor, FALSE);
            CheckSetActorFaceDir(actor, 1);
            func_ov021_02189a58(field, FALSE);
        } else {
            GFL_BGSysSetBGEnabled(0, 0);
            G3X_SetClearColor(0, 0x1f, 0x7fff, 0, 0);
        }
        FieldFog_SetEnabled(Field_GetFog(field), 1);
        (*state)++;
        break;
    case 9:
        func_ov021_021a0568(func_ov021_02188c60(field), TRUE);
        ci->dimDone = 0;
        GFL_VBlankTCBAdd(func_ov021_021c3198, &ci->dimDone, 0);
        (*state)++;
        break;
    case 10: {
        void *msgBGSys;
        PlaceName *placeName;

        if (ci->dimDone == 0) {
            break;
        }
        GFL_HeapGetFreeSize(ci->heapId);
        msgBGSys = Field_GetMsgBGSys(field);
        placeName = Field_GetPlaceName(field);
        func_ov021_02193068(msgBGSys);
        func_ov021_021bc5d4(placeName);
        func_ov021_021904d0(msgBGSys);
        reg_G2_BG2CNT = (reg_G2_BG2CNT & 0x43) | 0x1f18;
        GFL_BGSysClearBG(2);
        func_ov021_021c27cc(ci);
        func_ov021_021c27e8(ci);
        func_ov021_02189a58(field, FALSE);
        if (data->disableMovement) {
            EnableAllActorsMovement(Field_GetActorSystem(field));
        }
        (*state)++;
        break;
    }
    case 11:
        return 1;
    }
    return 0;
}


u32 func_ov021_021c21b4(Fld3dCi *work) {
    if (work->unk_64 == 0) {
        if (work->unk_78 == 0) {
            if (work->unk_5c != 0) {
                work->unk_5c--;
            } else {
                func_ov021_021c24a8(work, 0);
                work->unk_78 = 1;
            }
        }
        if (work->unk_7c == 0) {
            if (work->unk_5e != 0) {
                work->unk_5e--;
            } else {
                func_ov021_021c24a8(work, 1);
                work->unk_7c = 1;
            }
        }
        if (work->unk_78 != 0 && work->unk_7c != 0) {
            work->unk_64 = 1;
        }
    } else {
        if (func_ov021_021c1ab4(work->particleSystem) != 0) {
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_021c223c(Fld3dCi *work) {
    G3DActor *actor;
    u16 actorIdx;
    u16 anmCount;
    u8 i;
    BOOL stepped;

    if (work->unk_80 == 0) {
        return 1;
    }
    if (work->unk_60 != 0) {
        work->unk_60--;
    } else {
        actorIdx = GFL_G3DMgrGetSceneFirstActorIdx(work->g3dMan, (u16)work->unk_18);
        actor = GFL_G3DMgrGetActor(work->g3dMan, actorIdx);
        anmCount = GFL_G3DActorGetAnmCount(actor);
        for (i = 0; i < anmCount; i++) {
            stepped = GFL_G3DActorStepAnmFrame(actor, i, 0x1000);
        }
        if (stepped == 0) {
            return 1;
        }
    }
    return 0;
}

u32 func_ov021_021c22ac(Fld3dCi *work) {
    G3DActor *actor;
    u16 actorIdx;
    u16 anmCount;
    u8 i;
    BOOL stepped;

    if (work->unk_84 == 0) {
        return 1;
    }
    if (work->unk_62 != 0) {
        work->unk_62--;
    } else {
        actorIdx = GFL_G3DMgrGetSceneFirstActorIdx(work->g3dMan, (u16)work->unk_18);
        actor = GFL_G3DMgrGetActor(work->g3dMan, actorIdx + 1);
        anmCount = GFL_G3DActorGetAnmCount(actor);
        for (i = 0; i < anmCount; i++) {
            stepped = GFL_G3DActorStepAnmFrame(actor, i, 0x1000);
        }
        if (stepped == 0) {
            return 1;
        }
    }
    return 0;
}

void func_ov021_021c2320(GameEvent *param_1, Fld3dCi *param_2, Fld3dCiSetup *param_3, u8 param_4) {
    G3DActor *actor;
    GameSystem *gsys;
    GameEvent *next;
    void *tcb;
    u8 i;
    int firstActor;
    int actorCount;
    u8 j;
    int anmCount;
    int r;

    param_2->particles = func_ov021_021c1a48(param_2->particleSystem);
    func_ov021_021c24f0(param_3, param_4, param_2->heapId);
    func_ov021_021c1c24(param_2, &param_3->scene);
    if (param_3->unk_28 != 0xffff) {
        tcb = func_ov021_021c1a98(param_2->particleSystem, 0x73, param_3->unk_28);
        func_ov021_021c1aa8(param_2->particleSystem, tcb, 1, NULL);
    } else {
        tcb = NULL;
    }
    firstActor = GFL_G3DMgrGetSceneFirstActorIdx(param_2->g3dMan, (u16)param_2->unk_18);
    actorCount = GFL_G3DSceneGetNodeActorCount(param_2->g3dMan, (u16)param_2->unk_18);
    for (i = 0; i < actorCount; i++) {
        actor = GFL_G3DMgrGetActor(param_2->g3dMan, firstActor + i);
        anmCount = GFL_G3DActorGetAnmCount(actor);
        for (j = 0; j < anmCount; j++) {
            GFL_G3DActorBindAnm(actor, j);
        }
    }
    param_2->unk_5c = param_3->unk_20;
    param_2->unk_5e = param_3->unk_22;
    param_2->unk_60 = param_3->unk_24;
    param_2->unk_62 = param_3->unk_26;
    if (tcb != NULL) {
        r = func_02051b04(tcb);
    } else {
        r = 0;
    }
    if (r == 2) {
        param_2->unk_7c = param_2->unk_78 = 0;
    } else if (r == 1) {
        param_2->unk_78 = 0;
        param_2->unk_7c = 1;
    } else {
        param_2->unk_7c = param_2->unk_78 = 1;
    }
    param_2->unk_80 = 0;
    param_2->unk_84 = 0;
    if (param_3->unk_2e != 0) {
        param_2->unk_80 = 1;
    }
    if (param_3->unk_2f != 0) {
        param_2->unk_84 = 1;
    }
    if (param_2->unk_128 != NULL) {
        gsys = GameEvent_GetGameSystem(param_1);
        next = GameEvent_Create(gsys, param_1, (GameEventCallback)param_2->unk_128, 0);
        GameEvent_ChainNext(param_1, next);
    }
}






void func_ov021_021c2484(Fld3dCi *ci, Fld3dCiSetup *setup) {
    func_ov021_021c2734(setup);
    GFL_G3DMgrDeleteScene(ci->g3dMan, (u16)ci->unk_18);
    ci->unk_18 = -1;
    func_ov021_021c1a84(ci->particleSystem);
}

void func_ov021_021c24a8(Fld3dCi *ci, s32 emitterId) {
    func_020515cc(ci->particles, emitterId, func_ov021_021c24bc, NULL);
}

void func_ov021_021c24bc(SPLEmitter *emitter) {
    VecFx32 offset = { 0, 0, 0 };

    offset.z = 64;
    emitter->position.x = offset.x + emitter->resource->header->emitterBasePos.x;
    emitter->position.y = offset.y + emitter->resource->header->emitterBasePos.y;
    emitter->position.z = offset.z + emitter->resource->header->emitterBasePos.z;
}

typedef struct Fld3dCiFile {
    u32 unk_0;
    u16 particleDelay[2];
    u16 models[2];
    u16 actorDelay[2];
    u16 animations1[4];
    u16 animations2[4];
    u32 unk_20;
} Fld3dCiFile;
void func_ov021_021c24f0(Fld3dCiSetup *setup, u32 fileId, HeapID heapId) {
    Fld3dCiFile file;
    u16 ids[8];
    u8 actorCount;
    u8 anim1Count;
    u8 anim2Count;
    u8 modelStart;
    u8 anim1Start;
    u8 anim2Start;
    u8 total;
    u8 i;
    u8 j;

    GFL_ArcSysRead(&file, 0x75, fileId);
    actorCount = 0;
    for (i = 0; i < 2; i++) {
        if (file.models[i] != 0xffff) {
            actorCount++;
        }
    }
    anim2Count = 0;
    anim1Count = 0;
    for (i = 0; i < 4; i++) {
        if (file.animations1[i] != 0xffff) {
            anim1Count++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (file.animations2[i] != 0xffff) {
            anim2Count++;
        }
    }
    modelStart = 0;
    anim1Start = modelStart + actorCount;
    anim2Start = anim1Start + anim1Count;
    total = actorCount + anim1Count + anim2Count;
    if (total != 0) {
        setup->resources = GFL_HeapAllocate(heapId, total * sizeof(G3DSceneResourceSetup), TRUE, "fld3d_ci.c", 0x566);
    }
    if (actorCount != 0) {
        setup->actors = GFL_HeapAllocate(heapId, actorCount * sizeof(G3DSceneActorSetup), TRUE, "fld3d_ci.c", 0x56a);
    }
    if (anim1Count != 0) {
        setup->animations1 = GFL_HeapAllocate(heapId, anim1Count * sizeof(G3DSceneAnimationSetup), TRUE, "fld3d_ci.c", 0x56e);
    }
    if (anim2Count != 0) {
        setup->animations2 = GFL_HeapAllocate(heapId, anim2Count * sizeof(G3DSceneAnimationSetup), TRUE, "fld3d_ci.c", 0x572);
    }
    setup->resourceCount = total;
    setup->actorCount = actorCount;
    setup->unk_2e = anim1Count;
    setup->unk_2f = anim2Count;
    setup->unk_20 = file.particleDelay[0];
    setup->unk_22 = file.particleDelay[1];
    setup->unk_24 = file.actorDelay[0];
    setup->unk_26 = file.actorDelay[1];
    ids[0] = file.models[0];
    ids[1] = file.models[1];
    ids[2] = file.animations1[0];
    ids[3] = file.animations1[1];
    ids[4] = file.animations1[2];
    ids[5] = file.animations2[0];
    ids[6] = file.animations2[1];
    ids[7] = file.animations2[2];
    i = 0;
    for (j = 0; j < 8; j++) {
        if (ids[j] != 0xffff) {
            setup->resources[i].arcId = 0x73;
            setup->resources[i].fileId = ids[j];
            setup->resources[i].type = 0;
            i++;
        }
    }
    for (i = 0; i < anim1Count; i++) {
        setup->animations1[i].resource = anim1Start + i;
        setup->animations1[i].index = 0;
    }
    for (i = 0; i < anim2Count; i++) {
        setup->animations2[i].resource = anim2Start + i;
        setup->animations2[i].index = 0;
    }
    for (i = 0; i < actorCount; i++) {
        setup->actors[i].modelResource = modelStart + i;
        setup->actors[i].modelIndex = 0;
        setup->actors[i].texResource = modelStart + i;
    }
    if (actorCount >= 1) {
        setup->actors[0].animations = setup->animations1;
        setup->actors[0].animationCount = anim1Count;
    }
    if (actorCount >= 2) {
        setup->actors[1].animations = setup->animations2;
        setup->actors[1].animationCount = anim2Count;
    }
    setup->scene.resources = setup->resources;
    setup->scene.resourceCount = total;
    setup->scene.actorCount = actorCount;
    setup->scene.actors = setup->actors;
    setup->unk_28 = file.unk_0;
    setup->unk_30 = file.unk_20;
}



void func_ov021_021c2734(Fld3dCiSetup *setup) {
    if (setup->animations1 != NULL) {
        GFL_HeapFree(setup->animations1);
    }
    if (setup->animations2 != NULL) {
        GFL_HeapFree(setup->animations2);
    }
    if (setup->actors != NULL) {
        GFL_HeapFree(setup->actors);
    }
    if (setup->resources != NULL) {
        GFL_HeapFree(setup->resources);
    }
}

void func_ov021_021c2764(Fld3dCi *ci) {
    ci->screenCapDone = 0;
    GX_SetBankForLCDC(8);
    GFL_VBlankTCBAdd(Fld3DCi_ScreenCapTCB, &ci->screenCapDone, 0);
}

void Fld3DCi_ScreenCapTCB(TCB *tcb, void *work) {
    u32 *done = work;

    reg_GX_DISPCAPCNT = 0x81330010;
    GFL_TCBRemove(tcb);
    if (done != NULL) {
        *done = 1;
    }
}

void func_ov021_021c27a4(Fld3dCi *ci) {
    s32 bg;

    bg = 0;
    do {
        ci->bgPriority[bg] = GFL_BGSysGetBGPriority((u8)bg);
        bg = bg + 1;
    } while (bg < 4);
}

void func_ov021_021c27c0(Fld3dCi *ci) {
    ci->enabledBGs = GFL_BGSysGetEnabledBGsA();
}

void func_ov021_021c27cc(Fld3dCi *ci) {
    u32 i;

    i = 0;
    do {
        GFL_BGSysSetBGPriority(i & 0xff, ci->bgPriority[i]);
        i = i + 1;
    } while ((s32)i < 4);
}

void func_ov021_021c27e8(Fld3dCi *ci) {
    GFL_BGSysSetEnabledBGsA(ci->enabledBGs);
}

u32 func_ov021_021c27f4(GameEvent *event, s32 *state) {
    Fld3dCiEvent *data = GameEvent_GetData(GameEvent_GetReturnEvent(event));
    Fld3dCi *ci = data->ci;

    GSYS_GetField(GameEvent_GetGameSystem(event));
    switch (*state) {
    case 0:
        func_ov021_021c2918(ci);
        (*state)++;
        break;
    case 1:
        GFL_BitmapFree(ci->bitmap);
        GFL_HeapFree(ci->charBuf);
        GFL_HeapFree(ci->paletteBuf);
        (*state)++;
        break;
    case 2:
        return TRUE;
    }
    return FALSE;
}

u32 func_ov021_021c2854(GameEvent *event, Fld3dCi *ci) {
    BOOL done;
    FieldEffectParams *param = &ci->effect;

    if (param->unk_14 != 0 && param->unk_18 != 0) {
        param->unk_18--;
        if (param->unk_18 == 0) {
            if (param->unk_0 == 441) {
                PokeVoiceChatterInfo info;

                PokeVoice_CreateChatterInfo(&info);
                param->voice = PokeVoice_Play(param->unk_0, param->unk_4, 0x40, 0, 0, 0, 0, &info);
            } else {
                param->voice = PokeVoice_Play(param->unk_0, param->unk_4, 0x40, 0, 0, 0, 0, NULL);
            }
        }
        param->unk_1a = 30;
        return FALSE;
    }
    done = FALSE;
    if (param->unk_18 == 0) {
        if (!PokeVoice_IsPlayingAny()) {
            done = TRUE;
        } else if (param->unk_1a != 0) {
            param->unk_1a--;
        } else if (PokeVoice_GetVolume(param->voice) > 0) {
            PokeVoice_AdjustVolume(param->voice, -16);
        } else {
            PokeVoice_ReleaseAll();
            done = TRUE;
        }
    }
    if (ci->unk_11c != 0 && ci->unk_120 != 0 && ci->unk_124 != 0 && done) {
        return TRUE;
    }
    return FALSE;
}

void func_ov021_021c2918(Fld3dCi *ci) {
    FieldEffectParams *param;
    HeapID heapId = ci->heapId;
    void *res;
    u8 *charRaw;
    u8 *pixels;
    int i;
    int j;
    u32 texAddr;
    u32 palAddr;
    u32 charFile;
    u32 palFile;
    int tile;
    NNSGfdTexKey texKey;
    NNSGfdPlttKey palKey;

    ci->bitmap = GFL_BitmapCreate(16, 16, 32, heapId);
    res = GFL_G3DMgrGetResource(ci->g3dMan, 0);
    texKey = GFL_G3DResGetTexVRAMHandle(res);
    palKey = GFL_G3DResGetPltVRAMHandle(res);
    texAddr = NNS_GfdGetTexKeyAddr(texKey);
    palAddr = NNS_GfdGetPlttKeyAddr(palKey);
    param = &ci->effect;
    charFile = GetPokemonSingleCellCharacterDataNo(GetPokemonGraphicsARCID(), param->unk_0, param->unk_4, param->unk_8,
                                                   param->unk_c, 0, param->unk_10);
    palFile = GetPokemonPaletteDataNo(GetPokemonGraphicsARCID(), param->unk_0, param->unk_4, param->unk_8, param->unk_c,
                                      0, param->unk_10);
    tile = 0;
    ci->charData = NULL;
    ci->paletteData = NULL;
    ci->charBuf = GFL_G2DIOReadOBJNCGR(GetPokemonGraphicsARCID(), charFile, TRUE, &ci->charData, heapId);
    ci->paletteBuf = GFL_G2DIOReadNCLR(GetPokemonGraphicsARCID(), palFile, &ci->paletteData, heapId);
    charRaw = ci->charData->pRawData;
    pixels = GFL_BitmapGetPixelData(ci->bitmap);
    MIi_CpuClear32(0, pixels, 0x2000);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            MIi_CpuCopy32(charRaw + tile * 32, pixels + 0x440 + (j + i * 16) * 32, 32);
            tile++;
        }
    }
    for (i = 0; i < 8; i++) {
        for (j = 8; j < 12; j++) {
            MIi_CpuCopy32(charRaw + tile * 32, pixels + 0x440 + (j + i * 16) * 32, 32);
            tile++;
        }
    }
    for (i = 8; i < 12; i++) {
        for (j = 0; j < 8; j++) {
            MIi_CpuCopy32(charRaw + tile * 32, pixels + 0x440 + (j + i * 16) * 32, 32);
            tile++;
        }
    }
    for (i = 8; i < 12; i++) {
        for (j = 8; j < 12; j++) {
            MIi_CpuCopy32(charRaw + tile * 32, pixels + 0x440 + (j + i * 16) * 32, 32);
            tile++;
        }
    }
    GFL_BitmapMakeLinear(ci->bitmap, FALSE, heapId);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, texAddr, GFL_BitmapGetPixelData(ci->bitmap), 0x2000);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, palAddr, ci->paletteData->pRawData, 32);
    GFL_HeapGetFreeSize(ci->heapId);
}




GameEventReturnCode func_ov021_021c2b1c(GameEvent *event, u32 *state, void *work) {
    GSYS_GetField(GameEvent_GetGameSystem(event));
    switch (*state) {
    case 0:
        GFL_FadeSet(4, 0, 16, -1);
        (*state)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021c2b5c(GameEvent *event, u32 *state, void *work) {
    GSYS_GetField(GameEvent_GetGameSystem(event));
    switch (*state) {
    case 0:
        GFL_FadeSet(4, 16, 0, -1);
        (*state)++;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021c2b98(GameEvent *event, u32 *state, void *data) {
    GameSystem *gsys;

    gsys = GameEvent_GetGameSystem(event);
    GSYS_GetField(gsys);
    switch (*state) {
    case 0:
        GFL_FadeSet(3, 16, 0, 0);
        ++*state;
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}


typedef struct {
    u16 firstResource;
    u16 resourceCount;
    u16 firstActor;
    u16 actorCount;
} G3DScene;
struct G3DManager {
    void **resources;
    u8 *resourceUsed;
    u16 resourceLimit;
    G3DActor **actors;
    u8 *actorFlags;
    u16 actorLimit;
    G3DScene scenes[64];
    HeapID heapId;
};
typedef struct {
    u16 magic;
    u16 type;
    NNSG3dResFileHeader *data;
} G3DResource;
struct G3DModel {
    u16 magic;
    NNSG3dRenderObj *renderObj;
    G3DResource *mdlResource;
    G3DResource *texResource;
};
typedef struct {
    u16 magic;
    NNSG3dAnmObj *anmObj;
    G3DResource *resource;
} G3DAnimation;
struct G3DActor {
    u16 magic;
    G3DModel *model;
    G3DAnimation **animations;
    u16 animationCount;
};
u32 func_ov021_021c2edc(const NNSG3dResTex *tex, const char *name);
u32 func_ov021_021c2f14(const NNSG3dResTex *tex, const char *name);

u32 func_ov021_021c2bd4(s32 param_1) {
    if ((param_1 == 0xC) || (param_1 == 0x13)) {
        return 1U;
    }
    return 0U;
}

void func_ov021_021c2be4(Fld3dCi *ci, const char *param_2, const char *param_3, u32 param_4, u32 param_5) {
    void *res;
    NNSG3dResFileHeader *header;
    NNSG3dResTex *tex;
    u32 dstAddr;
    u32 pltDstAddr;
    HeapID heapId;
    u8 *pixels;
    s32 col;
    s32 tileIdx;
    u8 *charRaw;
    s32 row;

    heapId = ci->heapId;
    tileIdx = 0;
    res = GFL_G3DMgrGetResource(ci->g3dMan, 0);
    GFL_G3DResGetTexVRAMHandle(res);
    GFL_G3DResGetPltVRAMHandle(res);
    header = GFL_G3DResGetResData(res);
    tex = NNS_G3dGetTex(header);
    dstAddr = func_ov021_021c2edc(tex, param_2);
    pltDstAddr = func_ov021_021c2f14(tex, param_3);
    ci->charData = NULL;
    ci->paletteData = NULL;
    res = GFL_G2DIOReadBGNCGR(0xb4, param_4, 1, &ci->charData, heapId);
    ci->charBuf = res;
    res = GFL_G2DIOReadNCLR(0xb4, param_5, &ci->paletteData, heapId);
    ci->paletteBuf = res;
    charRaw = ci->charData->pRawData;
    pixels = GFL_BitmapGetPixelData(ci->bitmap);
    MIi_CpuClear32(0, pixels, 0x2000);
    row = 0;
    do {
        col = 0;
        do {
            MIi_CpuCopy32(charRaw + tileIdx * 0x20, pixels + (col + row * 0x10) * 0x20, 0x20);
            col = col + 1;
            tileIdx = tileIdx + 1;
        } while (col < 0x10);
        row = row + 1;
    } while (row < 0x10);
    GFL_BitmapMakeLinear(ci->bitmap, 0, heapId);
    pixels = GFL_BitmapGetPixelData(ci->bitmap);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, dstAddr, pixels, 0x2000);
    NNS_GfdRegisterNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, pltDstAddr, ci->paletteData->pRawData, 0x20);
    GFL_HeapGetFreeSize(ci->heapId);
}

u32 func_ov021_021c2cf4(GameEvent *event, Fld3dCi *ci) {
    switch (ci->unk_130) {
    case 0:
        if (ci->unk_11c != 0 && ci->unk_120 != 0 && ci->unk_124 != 0) {
            ci->unk_130++;
        }
        break;
    case 1:
        GFL_FadeSet(((Fld3dCiEvent *)GameEvent_GetData(event))->unk_3c != 0 ? 12 : 3, 0, 16, -1);
        ci->unk_130++;
    case 2:
        if (!GFL_FadeIsRunning()) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

u32 func_ov021_021c2d68(GameEvent *event, s32 *state) {
    Fld3dCiEvent *data = GameEvent_GetData(GameEvent_GetReturnEvent(event));
    GameSystem *gsys = GameEvent_GetGameSystem(event);
    PlayerInfo *info = GetGameDataPlayerInfo(GSYS_GetGameData(gsys));
    Fld3dCi *ci = data->ci;
    FieldEffectParams *param;
    u32 charFile;
    u32 palFile;

    GSYS_GetField(gsys);
    param = &ci->effect;
    switch (*state) {
    case 0:
        ci->bitmap = GFL_BitmapCreate(16, 16, 32, ci->heapId);
        func_ov021_021c2be4(ci, "trwb_face001", "trwb_face001_pl", param->unk_4, param->unk_8);
        (*state)++;
        break;
    case 1:
        GFL_BitmapFree(ci->bitmap);
        GFL_HeapFree(ci->charBuf);
        GFL_HeapFree(ci->paletteBuf);
        func_ov021_021c2f84(info, ci, "name_up", "name_up_pl", param->unk_0, FALSE);
        (*state)++;
        break;
    case 2:
        if (param->unk_c == 0) {
            *state = 4;
            break;
        }
        ci->bitmap = GFL_BitmapCreate(16, 16, 32, ci->heapId);
        if (getTrainerGender(info) == 0) {
            charFile = 0;
            palFile = 0x16;
        } else {
            charFile = 1;
            palFile = 0x17;
        }
        func_ov021_021c2be4(ci, "trwb_hero_ine", "trwb_hero_ine_pl", charFile, palFile);
        (*state)++;
        break;
    case 3:
        GFL_BitmapFree(ci->bitmap);
        GFL_HeapFree(ci->charBuf);
        GFL_HeapFree(ci->paletteBuf);
        func_ov021_021c2f84(info, ci, "name_down", "name_down_pl", 0, param->unk_10);
        (*state)++;
        break;
    case 4:
        ci->srt.translation.x = 0;
        ci->srt.translation.y = 0;
        ci->srt.translation.z = FX32_CONST(-700);
        GFL_G3DSysSetSwapBufferParams(0, 0);
        return TRUE;
    }
    return FALSE;
}

void func_ov021_021c2f50(NNSG3dResName *resName, const char *name);

u32 func_ov021_021c2ebc(const NNSG3dResTex *tex, const NNSG3dResDictTexData *texData)
{
    return ((texData->texImageParam << 16) >> 13) + ((tex->texInfo.vramKey << 16) >> 13);
}

u32 func_ov021_021c2ecc(const NNSG3dResTex *tex, const NNSG3dResDictPlttData *plttData)
{
    return (plttData->offset << 3) + ((tex->plttInfo.vramKey << 16) >> 13);
}

u32 func_ov021_021c2edc(const NNSG3dResTex *tex, const char *name)
{
    NNSG3dResName resName;
    const NNSG3dResDictTexData *texData;

    func_ov021_021c2f50(&resName, name);
    if (tex != NULL) {
        texData = NNS_G3dGetResDataByName(&tex->dict, &resName);
    } else {
        texData = NULL;
    }
    if (texData == NULL) {
        return 0;
    }
    return func_ov021_021c2ebc(tex, texData);
}


u32 func_ov021_021c2f14(const NNSG3dResTex *tex, const char *name)
{
    NNSG3dResName resName;
    const NNSG3dResDictPlttData *result;

    func_ov021_021c2f50(&resName, name);
    if (tex != NULL && tex->plttInfo.ofsDict != 0) {
        result = NNS_G3dGetResDataByName((const NNSG3dResDict *)((const u8 *)tex + tex->plttInfo.ofsDict), &resName);
    } else {
        result = NULL;
    }
    if (result == NULL) {
        return 0;
    }
    return func_ov021_021c2ecc(tex, result);
}

void func_ov021_021c2f50(NNSG3dResName *resName, const char *name)
{
    int len = STD_GetStringLength(name);
    u8 i;

    for (i = 0; i < 4; i++) {
        resName->val[i] = 0;
    }
    for (i = 0; i < len; i++) {
        resName->name[i] = name[i];
    }
}


void func_ov021_021c2f84(PlayerInfo *playerInfo, Fld3dCi *ci, const char *texName, const char *plName, u32 msgId,
                         BOOL calcWidth) {
    WordSet *wordSet;
    Font *font;
    StrBuf *msg;
    MsgData *msgData;
    void *texture;
    HeapID heapId;
    u32 strHeap;
    StrBuf *buf;
    s32 width;
    u16 *raw;
    G3DTextDrawResource res;

    texture = GFL_G3DMgrGetResource(ci->g3dMan, 0);
    heapId = ci->heapId;
    msgData = GFL_MsgSysLoadData(FALSE, 2, 0xb0, heapId);
    if (msgId == 0) {
        strHeap = (heapId & 0x7fff) | 0x8000;
        wordSet = GFL_WordSetSystemCreateDefault(strHeap);
        buf = GFL_StrBufCreate(8, strHeap);
        msg = GFL_MsgDataLoadStrbufNew(msgData, msgId);
        copyVarForText(wordSet, 0, playerInfo);
        GFL_WordSetFormatStrbuf(wordSet, buf, msg);
        GFL_StrBufFree(msg);
        GFL_WordSetSystemFree(wordSet);
    } else {
        msg = GFL_MsgDataLoadStrbufNew(msgData, msgId);
        buf = GFL_StrBufCreate(0x10, (heapId & 0x7fff) | 0x8000);
        GFL_StrBufUncompress(buf, msg);
        GFL_StrBufFree(msg);
    }
    width = 0;
    if (calcWidth) {
        font = GFL_FontCreate(0x17, 0, 0, FALSE, heapId);
        width = GFL_FontGetBlockWidth(buf, font, 0);
        GFL_FontFree(font);
        width = 0x3c - width;
        if (width < 0) {
            width = 0;
        }
    }
    ci->paletteBuf = GFL_G2DIOReadNCLR(0x17, 5, &ci->paletteData, heapId);
    raw = ci->paletteData->pRawData;
    raw[1] = 0x7fff;
    raw[2] = 0;
    G3DTextDraw_CreateResource(texture, texName, raw, plName, buf, width, 0, 0x440, heapId, &res);
    GFL_HeapFree(ci->paletteBuf);
    GFL_HeapFree(buf);
    GFL_MsgDataFree(msgData);
}

GameEvent *EventFieldEffect_CreateAlderFly(GameSystem *gsys, Fld3dCi *ci, u32 actorId) {
    GameEvent *event = EventFieldEffect_Create(gsys, ci, 17);

    if (event == NULL) {
        return NULL;
    }
    ci->effect.unk_0 = actorId;
    return event;
}

void func_ov021_021c30d4(void) {
    u32 banks;
    u16 *pltt;
    s32 i;

    banks = GX_ResetBankForBG();
    pltt = (u16 *)HW_LCDC_VRAM_D;
    for (i = 0; i < 0xc000; i++) {
        u16 c = *pltt;
        u8 b = (c >> 10) & 0x1f;
        u8 g = (c >> 5) & 0x1f;
        u8 r = c & 0x1f;
        u8 a = (c >> 15) & 1;
        b = b >> 1;
        g = g >> 1;
        r = r >> 1;
        *pltt = (a << 15) | (b << 10) | (g << 5) | r;
        pltt++;
    }
    GX_SetBankForBG(banks);
}


void func_ov021_021c313c(Fld3dCi *ci, Fld3dCiEvent *data) {
    MI_CpuFill8(data, 0, sizeof(Fld3dCiEvent));
    data->ci = ci;
    data->disableMovement = 0;
    data->unk_3c = 0;
}

void func_ov021_021c3158(Fld3dCi *ci) {
    const Fld3DCiSoundKey *keys = data_ov021_021da720[ci->effectId];

    if (keys[ci->soundIndex].se != -1 && ci->frame == keys[ci->soundIndex].frame) {
        GFL_SndSEPlay(keys[ci->soundIndex].se);
        ci->soundIndex++;
    }
}

void func_ov021_021c3198(TCB *tcb, void *work) {
    u32 *done = work;

    GFL_BGSysSetBGEnabled(0, 1);
    {
        BGSysLCDConfig config = { 1, 0, 0, TRUE };

        GFL_BGSysSetLCDConfig(&config);
    }
    GFL_TCBRemove(tcb);
    if (done != NULL) {
        *done = TRUE;
    }
}
