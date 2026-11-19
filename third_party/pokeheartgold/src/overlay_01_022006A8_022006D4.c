#include "global.h"

#include "billboard.h"
#include "overlay_01_021F1348.h"

s32 ov01_021F1758(FieldEffectManager *fieldEffMan, s32 arg1, s32 arg2, s32 arg3, s32 id, s32 arg5, void *arg6);

void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

void ov01_021F1930(FieldEffectManager *fieldEffMan, u32, u32, u32);

// The billboard anims right after the ov01_02209308 funcs table (the DS code reached them as ov01_02209308 + 0x14,
// which is mid-table on the host).
#if defined(SDK_PORT) || defined(NONMATCHING)
extern BillboardAnim ov01_0220931C[];
#define OV01_0220931C_ANIMS ov01_0220931C
#else
extern u8 ov01_02209308[];
#define OV01_0220931C_ANIMS (ov01_02209308 + 0x14) // DS: the 0x14-byte funcs table and the anims are laid out back to back
#endif

void ov01_022006D4(FieldEffectRendererContext *param0);

void ov01_022006D4(FieldEffectRendererContext *param0) {
    ov01_021F18D4(param0->fieldEffMan, 4, 0x78);
    ov01_021F1930(param0->fieldEffMan, 3, 0x13, 1);
    ov01_021F1758(param0->fieldEffMan, 4, 4, 0, 3, 0, OV01_0220931C_ANIMS);
    return;
}
