#ifndef FIELD_FLDMMDL_DRAWDATA_H
#define FIELD_FLDMMDL_DRAWDATA_H

#include "types.h"
#include "gfl/blact.h"

// ov021's fldmmdl_drawdata.c (pokebw2's name): the billboard animation sets and the face size and scale of each
// billboard size, which fldmmdl_blact.c reads

// An animation set: the animations, by animation number, and how many there are
typedef struct BlActDefaultAnim {
    const BlActAnimStep *const *steps;
    u32 count;
} BlActDefaultAnim;

extern const u16 MMDL_BILLBOARD_SIZES_INT[3][2];
// The scale of a billboard actor, by its billboard size
extern const u16 data_ov021_021d75d0[3][2];
// The animation sets, by the set number of an object code's record
extern const BlActDefaultAnim data_ov021_021d7f68[30];

#endif // FIELD_FLDMMDL_DRAWDATA_H
