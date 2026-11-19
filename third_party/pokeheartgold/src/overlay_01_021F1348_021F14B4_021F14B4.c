#include "global.h"

#include "filesystem.h"
#include "overlay_01_021F1348.h"

void *ov01_021F141C(FieldEffectManager *, u32, s32);

void *ov01_021F14B4(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd);

void *ov01_021F14B4(FieldEffectManager *fieldEffMan, u32 fileId, s32 allocAtEnd) {
    void *dest;

    dest = ov01_021F141C(fieldEffMan, NARC_GetMemberSize(fieldEffMan->fldEffNARC, fileId), allocAtEnd);
    NARC_ReadWholeMember(fieldEffMan->fldEffNARC, fileId, dest);
    return dest;
}

void ov01_021F15A0(FieldEffectRenderer *renderer);

void ov01_021F14DC(FieldEffectManager *fieldEffMan);

void ov01_021F14DC(FieldEffectManager *fieldEffMan) {
    s32 v0;
    FieldEffectRenderer *v1;

    v0 = fieldEffMan->rendererCount;
    v1 = fieldEffMan->renderers;
    if (v0 != 0) {
        do {
            ov01_021F15A0(v1);
            v1++;
            v0 -= 1;
        } while (v0 != 0);
    }
}

FieldEffectRenderer *ov01_021F1560(FieldEffectManager *fieldEffMan);

void ov01_021F15AC(FieldEffectRenderer *renderer, u32 id, void *context);

const FieldEffectRendererFuncs *ov01_021F15C4(u32 id);

void ov01_021F14F4(FieldEffectManager *fieldEffMan, u32 id);

void ov01_021F14F4(FieldEffectManager *fieldEffMan, u32 id) {
    void *context;

    context = ov01_021F15C4(id)->newFunc(fieldEffMan);
    ov01_021F15AC(ov01_021F1560(fieldEffMan), id, context);
}
