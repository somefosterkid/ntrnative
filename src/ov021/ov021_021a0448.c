// Black's code at ov021 0x021a0448-0x021a0cf4. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021a0448.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "system/rtc.h"
#include "gfl/arc.h"
#include "field/ov021_021a0138.h"
#include "nitro/mi.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_vec.h"

void func_ov021_021a07b0(void);
void FieldLight_FlushCore(const FieldLightWork *work, FieldFog *fog, G3DLight *lights);
void FieldLight_LoadDataCore(FieldLightWork *work, u32 arcId, u32 fileId);
void FieldLightFlashInfo_SetOneWay(FieldLightFlash *flash, u16 frames, u16 retFrames);
void FieldLightFlashInfo_Set(FieldLightFlash *flash, u16 frames, u16 color, u16 retFrames);
u32 FieldLight_UpdateFlash(FieldLightFlash *flash, FieldLightInterp *interp, FieldLightEntry *current, FieldLightEntry *target);
u32 FieldLight_GetUsedEntryIndex(FieldLightWork *work, s32 now);
void FieldLightInterpolator_Begin(FieldLightInterp *interp, FieldLightEntry *from, FieldLightEntry *to, u16 frames);
void FieldLight_LoadData(FieldLightWork *work, u32 fileId);
void FieldLightInterpolator_BeginDefault(FieldLightInterp *interp, FieldLightEntry *from, FieldLightEntry *to);
u32 FieldLightFlashInfo_IsRunning(FieldLightFlash *flash);
s32 FieldLight_GetLightEndSecond(u32 season, FieldLightEntry *entry);
u32 FieldLightInterpolator_IsRunning(FieldLightInterp *interp);
void FieldLightInterpolator_Update(FieldLightInterp *interp);
void FieldLightInterpolator_Apply(const FieldLightInterp *interp, FieldLightEntry *out);

struct G3DLight {
    Light lights[4];
};

FieldLightWork *func_ov021_021a03d0(u32 param_1, s32 param_2, u16 param_3, FieldFog *param_4, G3DLight *param_5, HeapID param_6) {
    FieldLightWork *work;
    u32 entryIndex;

    work = GFL_HeapAllocate(param_6,sizeof(FieldLightWork),1,"field_light.c",0x167);
    work->season = param_3;
    FieldLight_LoadData(work, param_1);
    work->unk_38a = 1;
    entryIndex = FieldLight_GetUsedEntryIndex(work, param_2);
    work->index = entryIndex;
    MI_CpuCopy8(work->entries + work->index, &work->current, 0x34);
    work->needFlush = 1;
    work->fog = param_4;
    work->lights = param_5;
    return work;
}

void func_ov021_021a0448(void *param_1) {
    func_ov021_021a07b0();
    GFL_HeapFree(param_1);
}

void func_ov021_021a0458(FieldLightWork *work, s32 param_2) {
    BOOL flag;
    s32 lightEndSecond;
    s32 lightEndSecond2;
    s32 iStack;

    if (work->count != 0) {
        switch (work->mode) {
        case 0:
            lightEndSecond = FieldLight_GetLightEndSecond(work->season, &work->entries[work->index]);
            flag = FALSE;
            if ((s32)work->index - 1 < 0) {
                iStack = 0;
                lightEndSecond2 = FieldLight_GetLightEndSecond(work->season, &work->entries[work->count - 1]);
                if (lightEndSecond2 <= param_2) {
                    iStack = lightEndSecond2;
                    flag = TRUE;
                    lightEndSecond = lightEndSecond + 0x15180;
                }
            } else {
                iStack = FieldLight_GetLightEndSecond(work->season, &work->entries[work->index - 1]);
            }
            if ((iStack > param_2 || lightEndSecond <= param_2) && !flag) {
                work->index = (work->index + 1) % work->count;
                FieldLightInterpolator_BeginDefault(&work->interp, &work->current, &work->entries[work->index]);
            }
            break;
        case 1:
            if (FieldLight_UpdateFlash(&work->flash, &work->interp, &work->current, &work->entries[work->index]) != 0) {
                work->mode = 0;
            }
            break;
        }
        if (FieldLightInterpolator_IsRunning(&work->interp) != 0) {
            FieldLightInterpolator_Update(&work->interp);
            FieldLightInterpolator_Apply(&work->interp, &work->current);
            work->needFlush = 1;
        }
        work->now = param_2;
    }
}


void func_ov021_021a0568(FieldLightWork *work, s32 param_2) {
    if ((work->needFlush != 0) || (param_2 != 0)) {
        FieldLight_FlushCore(work, work->fog, work->lights);
        work->needFlush = 0;
    }
}

void FieldLight_ChangeData(FieldLightWork *work, u32 param_2, u32 param_3, u16 param_4) {
    u32 index;

    func_ov021_021a07b0();
    FieldLight_LoadDataCore(work, param_2, param_3);
    index = FieldLight_GetUsedEntryIndex(work, work->now);
    work->index = index;
    FieldLightInterpolator_Begin(&work->interp, &work->current, &work->entries[index], param_4);
}

void FieldLight_ChangeDataDefault(FieldLightWork *work) {
    u32 index;

    func_ov021_021a07b0();
    FieldLight_LoadData(work, (u32)work->fileId);
    index = FieldLight_GetUsedEntryIndex(work, work->now);
    work->index = index;
    FieldLightInterpolator_BeginDefault(&work->interp, &work->current, &work->entries[index]);
}

void func_ov021_021a0618(FieldLightWork *work, u16 param_2, u32 param_3, u32 param_4) {
    FieldLightFlashInfo_Set(&work->flash, param_3, param_4, param_2);
    work->mode = 1;
}


void func_ov021_021a063c(FieldLightWork *work, u16 param_2, s16 param_3) {
    FieldLightFlashInfo_SetOneWay(&work->flash, param_3, param_2);
    work->mode = 1;
}


u32 func_ov021_021a065c(FieldLightWork *work) {
    return FieldLightFlashInfo_IsRunning(&work->flash);
}

void FieldLight_FlushCore(const FieldLightWork *work, FieldFog *fog, G3DLight *lights) {
    VecFx16 zeroDir = { 0, 0, 0 };
    GXRgb zeroColor = 0;
    s32 i;

    if (work->unk_38a != 0) {
        for (i = 0; i < 4; i++) {
            if (work->current.enabled[i] != 0) {
                GFL_G3DLightSetDirVector(lights, i, (VecFx16 *)&work->current.dir[i]);
                GFL_G3DLightSetColor(lights, i, &work->current.color[i]);
            } else {
                GFL_G3DLightSetDirVector(lights, i, &zeroDir);
                GFL_G3DLightSetColor(lights, i, &zeroColor);
            }
        }
        NNS_G3dGlbMaterialColorDiffAmb(work->current.extra[0], work->current.extra[1], TRUE);
        NNS_G3dGlbMaterialColorSpecEmi(work->current.extra[2], work->current.extra[3], FALSE);
        FieldFog_SetColor(fog, work->current.extra[4]);
        G3X_SetClearColor(work->current.extra[5], 0x1f, 0x7fff, 0, FALSE);
    }
}


void FieldLight_LoadData(FieldLightWork *work, u32 fileId) {
    work->fileId = fileId;
    FieldLight_LoadDataCore(work, 0x3d, fileId);
}

void FieldLight_LoadDataCore(FieldLightWork *work, u32 arcId, u32 fileId) {
    u32 i;
    s32 j;

    work->count = GFL_ArcSysGetDataLength(arcId, fileId) / sizeof(FieldLightEntry);
    GFL_ArcSysRead(work->entries, arcId, fileId);
    for (i = 0; i < work->count; i++) {
        VecFx16 *dirs = work->entries[i].dir;
        for (j = 0; j < 4; j++) {
            VEC_Fx16Normalize(&dirs[j], &dirs[j]);
        }
    }
}


void func_ov021_021a07b0(void) {
}

u32 FieldLight_GetUsedEntryIndex(FieldLightWork *work, s32 now) {
    u32 i;
    s32 prev;
    s32 ret;

    prev = 0;
    for (i = 0; i < work->count; i++) {
        ret = FieldLight_GetLightEndSecond(work->season, &work->entries[i]);
        if (prev <= now && ret > now) {
            return i;
        }
        prev = ret;
    }
    return 0;
}


s32 FieldLight_GetLightEndSecond(u32 season, FieldLightEntry *entry) {
    u8 lightChangeHoursForSeasons;

    lightChangeHoursForSeasons = GetLightChangeHoursForSeasons(season, (u32)entry->season);
    return (u32)lightChangeHoursForSeasons * 0xe10 + entry->minute * 0x3c;
}

void FieldLightColorInterpolator_Set(FieldLightColorInterp *param_1, s32 param_2, s32 param_3) {
    param_1->r = param_2 & 0x1f;
    param_1->dr = (s16)(param_3 & 0x1f) - param_1->r;
    param_1->g = (param_2 & 0x3e0) >> 5;
    param_1->dg = ((param_3 & 0x3e0) << 11 >> 16) - param_1->g;
    param_1->b = (param_2 & 0x7c00) >> 10;
    param_1->db = ((param_3 & 0x7c00) << 6 >> 16) - param_1->b;
}

u16 FieldLightColorInterpolator_Calc(const FieldLightColorInterp *param_1, s32 param_2, s32 param_3) {
    s16 a;
    s16 b;
    s16 c;

    a = (s16)((param_1->dr * param_2) / param_3);
    a += param_1->r;
    b = (s16)((param_1->dg * param_2) / param_3);
    b += param_1->g;
    c = (s16)((param_1->db * param_2) / param_3);
    c += param_1->b;
    return (u16)((b << 5) | a | (c << 10));
}

void FieldLightDirInterpolator_Set(FieldLightDirInterp *param_1, const VecFx16 *param_2, VecFx16 *param_3) {
    VecFx16 axb;
    fx16 x, y, z;

    z = param_2->z;
    y = param_2->y;
    x = param_2->x;
    param_1->from.y = y;
    param_1->from.z = z;
    param_1->from.x = x;
    VEC_Fx16CrossProduct(param_2, param_3, &axb);
    z = axb.z;
    y = axb.y;
    x = axb.x;
    param_1->axis.x = x;
    param_1->axis.y = y;
    param_1->axis.z = z;
    param_1->angle = FX_AcosIdx(VEC_Fx16DotProduct(param_2, param_3));
}

void FieldLightDirInterpolator_Calc(const FieldLightDirInterp *param_1, s32 param_2, s32 param_3, VecFx16 *param_4) {
    MtxFx43 mtx;
    VecFx32 vec;
    s32 index;
    fx32 x, y, z;

    index = (u16)((param_1->angle * param_2) / param_3) >> 4;
    MTX_RotAxis43(&mtx, &param_1->axis, FX_SinCosTable_[index * 2], FX_SinCosTable_[index * 2 + 1]);
    MTX_MultVec43(&param_1->from, &mtx, &vec);
    VEC_Normalize(&vec, &vec);
    z = vec.z;
    y = vec.y;
    x = vec.x;
    param_4->x = x;
    param_4->y = y;
    param_4->z = z;
}

void FieldLightInterpolator_BeginDefault(FieldLightInterp *interp, FieldLightEntry *from, FieldLightEntry *to) {
    FieldLightInterpolator_Begin(interp, from, to, 0x3c);
}

void FieldLightInterpolator_Begin(FieldLightInterp *interp, FieldLightEntry *from, FieldLightEntry *to, u16 frames) {
    s32 i;

    interp->frame = 0;
    interp->frameMax = frames;
    interp->unk_0 = to->season;
    interp->unk_2 = to->minute;
    for (i = 0; i < 4; i++) {
        FieldLightColorInterpolator_Set(&interp->color[i], from->color[i], to->color[i]);
        FieldLightDirInterpolator_Set(&interp->dir[i], &from->dir[i], &to->dir[i]);
        interp->enabled[i] = to->enabled[i];
    }
    FieldLightColorInterpolator_Set(&interp->extra[0], from->extra[0], to->extra[0]);
    FieldLightColorInterpolator_Set(&interp->extra[1], from->extra[1], to->extra[1]);
    FieldLightColorInterpolator_Set(&interp->extra[2], from->extra[2], to->extra[2]);
    FieldLightColorInterpolator_Set(&interp->extra[3], from->extra[3], to->extra[3]);
    FieldLightColorInterpolator_Set(&interp->extra[4], from->extra[4], to->extra[4]);
    FieldLightColorInterpolator_Set(&interp->extra[5], from->extra[5], to->extra[5]);
}


void FieldLightInterpolator_BeginToConstColor(FieldLightInterp *interp, FieldLightEntry *from, u32 color, u16 frames) {
    long i;

    interp->frame = 0;
    interp->frameMax = frames;
    interp->unk_0 = from->season;
    interp->unk_2 = from->minute;
    for (i = 0; i < 4; i++) {
        FieldLightColorInterpolator_Set(&interp->color[i], from->color[i], color);
        VecFx16 *d = &from->dir[i];
        FieldLightDirInterpolator_Set(&interp->dir[i], d, d);
        interp->enabled[i] = from->enabled[i];
    }
    FieldLightColorInterpolator_Set(&interp->extra[0], from->extra[0], color);
    FieldLightColorInterpolator_Set(&interp->extra[1], from->extra[1], color);
    FieldLightColorInterpolator_Set(&interp->extra[2], from->extra[2], color);
    FieldLightColorInterpolator_Set(&interp->extra[3], from->extra[3], color);
    FieldLightColorInterpolator_Set(&interp->extra[4], from->extra[4], color);
    FieldLightColorInterpolator_Set(&interp->extra[5], from->extra[5], color);
}


void FieldLightInterpolator_Update(FieldLightInterp *interp) {
    if (interp->frame < interp->frameMax) {
        interp->frame++;
    }
}


u32 FieldLightInterpolator_IsRunning(FieldLightInterp *interp) {
    if (interp->frame < interp->frameMax) {
        return 1;
    }
    return 0;
}

void FieldLightInterpolator_Apply(const FieldLightInterp *interp, FieldLightEntry *out) {
    s32 i;

    out->season = interp->unk_0;
    out->minute = interp->unk_2;
    for (i = 0; i < 4; i++) {
        out->color[i] = FieldLightColorInterpolator_Calc(&interp->color[i], interp->frame, interp->frameMax);
        FieldLightDirInterpolator_Calc(&interp->dir[i], interp->frame, interp->frameMax, &out->dir[i]);
        out->enabled[i] = interp->enabled[i];
    }
    out->extra[0] = FieldLightColorInterpolator_Calc(&interp->extra[0], interp->frame, interp->frameMax);
    out->extra[1] = FieldLightColorInterpolator_Calc(&interp->extra[1], interp->frame, interp->frameMax);
    out->extra[2] = FieldLightColorInterpolator_Calc(&interp->extra[2], interp->frame, interp->frameMax);
    out->extra[3] = FieldLightColorInterpolator_Calc(&interp->extra[3], interp->frame, interp->frameMax);
    out->extra[4] = FieldLightColorInterpolator_Calc(&interp->extra[4], interp->frame, interp->frameMax);
    out->extra[5] = FieldLightColorInterpolator_Calc(&interp->extra[5], interp->frame, interp->frameMax);
}


void FieldLightFlashInfo_Set(FieldLightFlash *flash, u16 frames, u16 color, u16 retFrames) {
    flash->state = 1;
    flash->frames = frames;
    flash->retFrames = color;
    flash->color = retFrames;
}

void FieldLightFlashInfo_SetOneWay(FieldLightFlash *flash, u16 frames, u16 retFrames) {
    flash->state = 1;
    flash->frames = frames;
    flash->color = retFrames;
    flash->retFrames = 0xffff;
}

u32 FieldLight_UpdateFlash(FieldLightFlash *flash, FieldLightInterp *interp, FieldLightEntry *current, FieldLightEntry *target) {
    switch (flash->state) {
    case 1:
        FieldLightInterpolator_BeginToConstColor(interp, current, flash->color, flash->frames);
        flash->state = 2;
        break;
    case 2:
        if (FieldLightInterpolator_IsRunning(interp) == 0) {
            if (flash->retFrames != 0xffff) {
                FieldLightInterpolator_Begin(interp, current, target, flash->retFrames);
            }
            flash->state = 3;
        }
        break;
    case 3:
        if (FieldLightInterpolator_IsRunning(interp) == 0) {
            flash->state = 0;
        }
        break;
    case 0:
        return 1;
    }
    return 0;
}

u32 FieldLightFlashInfo_IsRunning(FieldLightFlash *flash) {
    if (flash->state != 0) {
        return 1;
    }
    return 0;
}
