#include "global.h"

#include "field/overlay_01_021FD1B8.h"

#include "overlay_01_021F1348.h"

void ov01_021FD258(FieldEffectManager *, u32);

void ov01_021FD290(FieldEffectManager *, u32, u32);

extern u32 ov01_02208E30;

void FieldEffect_InitRenderObject(FieldEffectManager *fieldEffectManager) {
    u32 *puVar1;
    u32 iVar2;

    iVar2 = 0xc;
    puVar1 = (u32 *)&ov01_02208E30;
    ov01_021FD258(fieldEffectManager, 0xc);
    do {
        ov01_021FD290(fieldEffectManager, *puVar1, puVar1[1]);
        puVar1 = puVar1 + 2;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    return;
}
