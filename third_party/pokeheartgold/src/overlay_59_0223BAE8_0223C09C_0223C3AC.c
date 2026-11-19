#include "global.h"
#include "unk_02014DA0.h"
#include "error_handling.h"
#include "overlay_71_trade3dmodel.h"
#include "bg_window.h"
#include "overlay_71_trade_sequence_data.h"
#include "overlay_71_palette_shimmer_effect.h"
#include "overlay_71_trade_pokemon_reveal_phase.h"
#include "particle_system.h"

typedef struct UnkStruct_ov59_0223C3DC {
    u8 unk_0[0x24];
    ParticleSystem *unk_24;
} UnkStruct_ov59_0223C3DC;

typedef struct UnkStruct_ov59_0223C3C0 {
    u8 unk_0[0x24];
    ParticleSystem *unk_24;
} UnkStruct_ov59_0223C3C0;

struct BgConfig;

struct BgScaleAnimation;

struct PaletteShimmerEffect;

struct SysTask;

struct SysTaskQueue;

struct Trade3DModel;

struct Trade3DScene;

struct TradePokemonRevealPhase;

struct TradeSequenceData;

typedef struct Trade3DScene Trade3DScene;

void Pokepic_Delete(Trade3DScene *tradeScene);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct BgScaleAnimation BgScaleAnimation;

void ov59_0223C3AC(TradePokemonRevealPhase *tprPhase);

void ov59_0223C3AC(TradePokemonRevealPhase *tprPhase)
{
    if (tprPhase->scene) {
        Pokepic_Delete(tprPhase->scene);
        tprPhase->scene = 0L ;
    }
}

void ov59_0223C4EC(SPLEmitter *);

void ov59_0223C3C0(UnkStruct_ov59_0223C3C0 *arg0, s32 arg1);

void ov59_0223C3C0(UnkStruct_ov59_0223C3C0 *arg0, s32 arg1) {
    sub_02015494(arg0->unk_24, arg1, ov59_0223C4EC, arg0);
    sub_02015528(arg0->unk_24, 1);
}

s32 ov59_0223C3DC(UnkStruct_ov59_0223C3DC *arg0);

s32 ov59_0223C3DC(UnkStruct_ov59_0223C3DC *arg0) {
    if (sub_020154B0(arg0->unk_24) != 0) {
        return 1;
    }
    return 0;
}

void ov59_0223C3F0(void);

void ov59_0223C3F0(void) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= 0xFFFFCFFD;
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT &= 0xCFDF;
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0U, 0, 0x7FFF, 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 2;
    reg_G3_VIEWPORT = 0xBFFF0000;
}

struct ParticleSystem;

ParticleSystem *sub_0207C7CC(enum HeapID heapID);

ParticleSystem *sub_0207C814(enum HeapID heapID, int param1, int param2);

void ov59_0223C474(void);

void ov59_0223C474(void)
{
    NNSGfdTexKey v0;
    NNSGfdPlttKey v1;
    u32 v2, v3;

    v0 = NNS_GfdAllocTexVram(0x2000 * 4, 0, 0);
    v1 = NNS_GfdAllocPlttVram(0x20 * (4 + 1), 0, 0);

    GF_ASSERT(v0 != NNS_GFD_ALLOC_ERROR_TEXKEY);
    GF_ASSERT(v1 != NNS_GFD_ALLOC_ERROR_PLTTKEY);

    v2 = NNS_GfdGetTexKeyAddr(v0);
    v3 = NNS_GfdGetPlttKeyAddr(v1);

    sub_02014DA0();
}

u32 ov59_0223C4B4(u32 arg0, s32 arg1);

u32 ov59_0223C4B4(u32 arg0, s32 arg1) {
    u32 temp_r0;

    temp_r0 = NNS_GfdDefaultFuncAllocTexVram(arg0, arg1, 0U);
    sub_02015354(temp_r0);
    return (u32) (temp_r0 << 0x10) >> 0xD;
}

u32 ov59_0223C4D0(u32 arg0, s32 arg1);

u32 ov59_0223C4D0(u32 arg0, s32 arg1) {
    u32 temp_r0;

    temp_r0 = NNS_GfdDefaultFuncAllocPlttVram(arg0, arg1, 0U);
    sub_02015394(temp_r0);
    return (u32) (temp_r0 << 0x10) >> 0xD;
}
