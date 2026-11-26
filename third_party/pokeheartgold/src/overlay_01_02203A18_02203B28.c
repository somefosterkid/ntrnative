#include "global.h"
#include "overlay_01_021F1348.h"

#include "overlay_01_02203A18.h"
#include "unk_02023694.h"


void ov01_021F18D4(FieldEffectManager *fieldEffMan, u32 id, u32 memberIndex);

s32 ov01_021F1908(FieldEffectManager *fieldEffMan, s32, s32);

s32 ov01_021F1930(FieldEffectManager *fieldEffMan, s32, s32, s32);

extern s32 ov01_0220952C;

void *ov01_021F1740(u8 *, s32, u32 *);

void ov01_02203B28(UnkOv01_02203A18 *arg0) {
    ov01_021F18D4(arg0->fieldEffMan, 0xC, 0x82);
    ov01_021F1908(arg0->fieldEffMan, 0xC, 0x8C);
    ov01_021F1930(arg0->fieldEffMan, 0xD, 0x1C, 1);
    ov01_021F1758(arg0->fieldEffMan, 0xE, 0xC, 0xC, 0xD, 0, &ov01_0220952C);
}

void ov01_021F18C8(FieldEffectManager *fieldEffMan, u32 id);

s32 ov01_021F1924(FieldEffectManager *fieldEffMan, s32);

s32 ov01_021F1970(FieldEffectManager *fieldEffMan, s32);

void ov01_02203B70(UnkOv01_02203A18 *arg0) {
    ov01_021F18FC(arg0->fieldEffMan, 0xC);
    ov01_021F1924(arg0->fieldEffMan, 0xC);
    ov01_021F1970(arg0->fieldEffMan, 0xD);
    ov01_021F18C8(arg0->fieldEffMan, 0xE);
}

Sprite *ov01_02203B98(void *arg0, void *arg1) {
    Sprite *sprite;

    sprite = ov01_021F1740(arg0, 0xE, arg1);
    NNS_G3dMdlSetMdlFogEnableFlagAll(sub_02023F90(sprite), 0);
    return sprite;
}
