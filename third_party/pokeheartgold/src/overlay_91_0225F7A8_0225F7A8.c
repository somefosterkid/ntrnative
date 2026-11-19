#include "global.h"
#include "sprite.h"
#include "unk_02026E30.h"
#include "unk_02009D48.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_0225D6A0(u8 *);

void ov91_0225FDD8(u8 *);

void ov91_0226023C(u8 *, void *);

void ov91_0226078C(void *, u32);

void ov91_02260D98(u8 *);

void ov91_022610A8(u8 *);

void ov91_0225F7A8(u8 *param_1, u8 *param_2);

void ov91_0225F7A8(u8 *param_1, u8 *param_2) {
  u32 auStack_18 [3];
  
  ov91_02260D98(param_2 + 0x6a98);
  ov91_0225D6A0(param_2 + 900);
  Thunk_G3X_Reset();
  ov91_0225FDD8(param_2);
  NNS_G3dGlbLightVector(GX_LIGHTID_0,0,-0x1000,0);
  NNS_G3dGlbLightColor(GX_LIGHTID_0,0x7fff);
  NNS_G3dGlbMaterialColorDiffAmb(0x7fff,0x7fff,0);
  NNS_G3dGlbMaterialColorSpecEmi(0x7fff,0x7fff,0);
  NNS_G3dGeBufferOP_N(0x11,(u32 *)0x0,0);
  ov91_022610A8(param_2 + 0x1a0);
  auStack_18[2] = 1;
  NNS_G3dGeBufferOP_N(0x12,auStack_18 + 2,1);
  NNS_G3dGeBufferOP_N(0x11,(u32 *)0x0,0);
  ov91_0226023C(param_2 + 0x4c0,param_1 + 0x19cc);
  auStack_18[1] = 1;
  NNS_G3dGeBufferOP_N(0x12,auStack_18 + 1,1);
  NNS_G3dGeBufferOP_N(0x11,(u32 *)0x0,0);
  ov91_0226078C(param_2,*(u32 *)(param_1 + 0x10));
  auStack_18[0] = 1;
  NNS_G3dGeBufferOP_N(0x12,auStack_18,1);
  RequestSwap3DBuffers(GX_SORTMODE_AUTO,GX_BUFFERMODE_Z);
  SpriteList_RenderAndAnimateSprites(((UnkStruct_ov115_02261ADC *)param_2)->unk_1C);
  return;
}
