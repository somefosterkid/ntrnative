#include "global.h"

#include "billboard.h"
#include "overlay_01_021F1348.h"

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);
void ov01_021F1908(FieldEffectManager *fieldEffMan, u32, u32);
void ov01_021F1930(FieldEffectManager *fieldEffMan, u32, u32, u32);

// The billboard anims right after the ov01_022091C0 funcs table (the DS code reached them as ov01_022091C0 + 0x14,
// which is mid-table on the host).
#if defined(SDK_PORT) || defined(NONMATCHING)
extern BillboardAnim ov01_022091D4[];
#define OV01_022091D4_ANIMS ov01_022091D4
#else
extern u8 ov01_022091C0[];
#define OV01_022091D4_ANIMS (ov01_022091C0 + 0x14) // DS: the 0x14-byte funcs table and the anims are laid out back to back
#endif

void ov01_021FF490(FieldEffectRendererContext *ctx);

void ov01_021FF490(FieldEffectRendererContext *ctx) {
    ov01_021F18D4(ctx->fieldEffMan, 3, 0x77);
    ov01_021F1908(ctx->fieldEffMan, 2, 0x90);
    ov01_021F1930(ctx->fieldEffMan, 2, 0x12, 1);
    ov01_021F1758(ctx->fieldEffMan, 3, 3, 2, 2, 0, OV01_022091D4_ANIMS);
}
