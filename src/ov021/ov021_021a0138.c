// Black's code at ov021 0x021a0138-0x021a02c4. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021a0138.h"
#include "field/field_fog.h"
#include "struct_decls.h"
#include "gfl/heap.h"
#include "nitro/gx.h"
#include "gfl/graphics.h"

u32 FieldFogAnimation_IsRunning(FieldFogAnimation *animation);
void FieldFogAnimation_Step(FieldFogAnimation *animation);
s32 FieldFogAnimation_CalcFogOffset(FieldFogAnimation *animation);
u32 FieldFogAnimation_CalcFogDepthShift(FieldFogAnimation *animation);

void FieldFogAnimation_Set(FieldFogAnimation *animation, s32 offsetStart, s32 offsetEnd, s32 depthShiftStart,
        u16 depthShiftEnd, u16 frameCount);


void func_ov021_021a0138(void *param_1) {
    GFL_HeapFree(param_1);
}

void func_ov021_021a0140(FieldFog *fog) {
    u32 result;

    result = FieldFogAnimation_IsRunning(&fog->animation);
    if (result != 0) {
        FieldFogAnimation_Step(&fog->animation);
        fog->offset = FieldFogAnimation_CalcFogOffset(&fog->animation);
        result = FieldFogAnimation_CalcFogDepthShift(&fog->animation);
        fog->depthShift = (s8)result;
        fog->dirty = 1;
    }
}

void func_ov021_021a0170(FieldFog *fog) {
    if (fog->dirty != 0) {
        G3X_SetFog(fog->enabled, fog->alphaMode, fog->depthShift, fog->offset);
        G3X_SetFogColor(fog->color, fog->alpha);
        G3X_SetFogTable(fog->table.words);
        fog->dirty = 0;
    }
}


void FieldFog_SetEnabled(FieldFog *fog, u8 param_2) {
    fog->enabled = param_2;
    fog->dirty = 1;
}

void FieldFog_SetAlphaMode(FieldFog *fog, u8 param_2) {
    fog->alphaMode = param_2;
    fog->dirty = 1;
}

void FieldFog_SetDepthShift(FieldFog *fog, u32 param_2) {
    fog->depthShift = param_2;
    fog->dirty = 1;
}

void FieldFog_SetOffset(FieldFog *fog, u32 param_2) {
    fog->offset = param_2;
    fog->dirty = 1;
}

void FieldFog_SetColor(FieldFog *fog, u16 param_2) {
    fog->color = param_2;
    fog->dirty = 1;
}

void FieldFog_SetAlpha(FieldFog *fog, u8 param_2) {
    fog->alpha = param_2;
    fog->dirty = 1;
}

u32 FieldFog_GetDepthShift(FieldFog *fog) {
    return fog->depthShift;
}

void FieldFog_SetDefaultTable(FieldFog *fog) {
    s32 i;
    s32 v;

    for (i = 0; i < 0x20; i++) {
        v = i * 4;
        fog->table.bytes[i] = v;
        fog->table.bytes[i] += v / 32;
    }
}


void FieldFog_Animate(FieldFog *fog, u32 color, u32 depthShift, u32 duration) {
    FieldFogAnimation_Set(&fog->animation, fog->offset, color, fog->depthShift, (u16)depthShift, (u16)duration);
}

void FieldFog_AnimateEx(FieldFog *fog, s32 param_2, s32 param_3, u32 param_4, u32 param_5, u16 param_6) {
    FieldFogAnimation_Set(&fog->animation, param_2, param_3, (u16)param_4, (u16)param_5, param_6);
}


BOOL FieldFog_IsAnimationRunning(FieldFog *fog) {
    return FieldFogAnimation_IsRunning(&fog->animation);
}

u32 FieldFogAnimation_IsRunning(FieldFogAnimation *animation) {
    if (animation->frame < animation->frameCount) {
        return 1;
    }
    return 0;
}

void FieldFogAnimation_Set(FieldFogAnimation *animation, s32 offsetStart, s32 offsetEnd, s32 depthShiftStart,
        u16 depthShiftEnd, u16 frameCount) {
    animation->frame = 0;
    animation->frameCount = frameCount;
    animation->offsetStart = offsetStart;
    animation->depthShiftStart = depthShiftStart;
    animation->offsetDelta = offsetEnd - offsetStart;
    animation->depthShiftDelta = depthShiftEnd - depthShiftStart;
}


void FieldFogAnimation_Step(FieldFogAnimation *animation) {
    if (animation->frame < animation->frameCount) {
        animation->frame = animation->frame + 1;
    }
}

s32 FieldFogAnimation_CalcFogOffset(FieldFogAnimation *animation) {
    s32 v = animation->offsetDelta * animation->frame / animation->frameCount;
    return v + animation->offsetStart;
}

u32 FieldFogAnimation_CalcFogDepthShift(FieldFogAnimation *animation) {
    s32 v = animation->depthShiftDelta * animation->frame / animation->frameCount;
    return v + animation->depthShiftStart & 0xffff;
}
