#ifndef BWPORT_FIELD_OV021_021A0138_H
#define BWPORT_FIELD_OV021_021A0138_H

#include "types.h"
#include "struct_decls.h"

BOOL FieldFog_IsAnimationRunning(FieldFog *fog);
void FieldFog_SetAlphaMode(FieldFog *fog, u8 param_2);
void FieldFog_SetAlpha(FieldFog *fog, u8 param_2);
void FieldFog_SetDepthShift(FieldFog *fog, u32 param_2);
void FieldFog_SetOffset(FieldFog *fog, u32 param_2);
void FieldFog_SetDefaultTable(FieldFog *fog);
void FieldFog_SetEnabled(FieldFog *fog, u8 param_2);
void FieldFog_SetColor(FieldFog *fog, u16 param_2);

#endif
