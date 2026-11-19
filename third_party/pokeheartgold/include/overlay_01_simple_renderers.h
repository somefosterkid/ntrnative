#ifndef POKEHEARTGOLD_OVERLAY_01_SIMPLE_RENDERERS_H
#define POKEHEARTGOLD_OVERLAY_01_SIMPLE_RENDERERS_H

#include "global.h"
#include "overlay_01_021F1348.h"
#include "simple3danimation.h"
#include "simple3dmodel.h"
#include "sys_task.h"

// Contexts of the small field effect renderers that load one model resource (ov01_021F19F4) and draw or animate it.

// Renderer 2 (ov01_021FE200): ten models and ten render objects for each of two sets. 0x824 bytes at 32 bits.
typedef struct {
    FieldEffectManager *unk_00;
    Simple3DModel unk_04[10];
    Simple3DModel unk_CC[10];
    Simple3DRenderObj unk_194[10];
    Simple3DRenderObj unk_4DC[10];
} UnkStruct_021F1CF8;

// Renderers 3 (ov01_021FE780) and 4 (ov01_02200858): a model and its render object. 0x6C bytes at 32 bits.
typedef struct UnkStruct_ov5_021F4DD8 {
    FieldEffectManager *unk_00;
    Simple3DModel unk_04;
    Simple3DRenderObj unk_18;
} UnkStruct_ov5_021F4DD8;

// Renderer 5 (ov01_021FEA0C): the same, loaded on demand (unk_04) and counted (unk_00).
typedef struct UnkStruct_ov5_021F2874 {
    int unk_00;
    int unk_04;
    int unk_08;
    SysTask *unk_0C;
    FieldEffectManager *unk_10;
    Simple3DModel unk_14;
    Simple3DRenderObj unk_28;
} UnkStruct_ov5_021F2874;

// Renderer 6 (ov01_021FEC38): a model and its animation.
typedef struct UnkStruct_ov5_021F23FC {
    FieldEffectManager *unk_00;
    Simple3DModel unk_04;
    Simple3DAnimation unk_18;
} UnkStruct_ov5_021F23FC;

// Renderers 6 (ov01_021FEC38) and 7 (ov01_021FED9C): the renderer's context holds the model and the animation to copy
// (UnkStruct_ov5_021F2BFC); each anim manager it makes (ov01_021FED14 / ov01_021FEE04) has its own animation copy, the render
// object and the userData below (UnkStruct_ov5_021F2C98).
typedef struct UnkStruct_ov5_021F2BFC {
    FieldEffectManager *unk_00;
    Simple3DModel unk_04;
    Simple3DAnimation unk_18;
} UnkStruct_ov5_021F2BFC;

typedef struct UnkStruct_ov5_021F2C38 {
    FieldEffectManager *unk_00;
    UnkStruct_ov5_021F2BFC *unk_04;
} UnkStruct_ov5_021F2C38;

typedef struct UnkStruct_ov5_021F2C98 {
    Simple3DAnimation unk_00;
    Simple3DRenderObj unk_24;
    UnkStruct_ov5_021F2C38 unk_78;
} UnkStruct_ov5_021F2C98;

void ov01_021F1A18(FieldEffectManager *fieldEffMan, Simple3DAnimation *anim, u32 unused, u32 memberIndex, BOOL allocAtEnd);

void ov01_021FE2B8(UnkStruct_021F1CF8 *ctx);
void ov01_021FE7AC(UnkStruct_ov5_021F4DD8 *ctx);
void ov01_021FE7D0(UnkStruct_ov5_021F4DD8 *ctx);
void ov01_02200884(UnkStruct_ov5_021F4DD8 *ctx);
void ov01_022008A8(UnkStruct_ov5_021F4DD8 *ctx);
void ov01_021FEA48(UnkStruct_ov5_021F2874 *ctx);
void ov01_021FEA7C(UnkStruct_ov5_021F2874 *ctx);
void ov01_021FEC64(UnkStruct_ov5_021F23FC *ctx);
void ov01_021FEC8C(UnkStruct_ov5_021F23FC *ctx);
void ov01_021FEDF0(UnkStruct_ov5_021F23FC *ctx);

#endif // POKEHEARTGOLD_OVERLAY_01_SIMPLE_RENDERERS_H
