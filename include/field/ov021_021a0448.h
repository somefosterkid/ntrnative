#ifndef BWPORT_FIELD_OV021_021A0448_H
#define BWPORT_FIELD_OV021_021A0448_H

#include "types.h"
#include "gfl/g3d.h"
#include "struct_decls.h"

// One lighting keyframe, as stored in the light data file.
typedef struct FieldLightEntry {
    u16 season;
    s16 minute;
    u8 enabled[4];
    u16 color[4];
    VecFx16 dir[4];
    u16 extra[6];
} FieldLightEntry;

typedef struct FieldLightColorInterp {
    u16 r;
    s16 dr;
    u16 g;
    s16 dg;
    u16 b;
    s16 db;
} FieldLightColorInterp;

typedef struct FieldLightDirInterp {
    VecFx32 from;
    VecFx32 axis;
    u16 angle;
    u16 unk_1a;
} FieldLightDirInterp;

typedef struct FieldLightInterp {
    u16 unk_0;
    s16 unk_2;
    u8 enabled[4];
    FieldLightColorInterp color[4];
    FieldLightDirInterp dir[4];
    FieldLightColorInterp extra[6];
    u16 frame;
    u16 frameMax;
} FieldLightInterp;

typedef struct FieldLightFlash {
    u16 state;
    u16 frames;
    u16 retFrames;
    u16 color;
} FieldLightFlash;

typedef struct FieldLightWork {
    u16 mode;
    u16 season;
    G3DLight *lights;
    FieldFog *fog;
    u32 count;
    FieldLightEntry entries[16];
    u32 index;
    u16 fileId;
    FieldLightEntry current;
    u16 unk_38a;
    s32 needFlush;
    FieldLightInterp interp;
    FieldLightFlash flash;
    s32 now;
} FieldLightWork;

void func_ov021_021a0618(FieldLightWork *work, u16 param_2, u32 param_3, u32 param_4);
u32 func_ov021_021a065c(FieldLightWork *work);
void func_ov021_021a063c(FieldLightWork *work, u16, s16);
void FieldLight_ChangeData(FieldLightWork *work, u32 param_2, u32 param_3, u16 param_4);
void FieldLight_ChangeDataDefault(FieldLightWork *work);

FieldLightWork *func_ov021_021a03d0(u32 lightsId, s32 daySeconds, u16 season, FieldFog *fog, G3DLight *lights, HeapID heapId);

#endif
