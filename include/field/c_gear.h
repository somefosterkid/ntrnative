#ifndef BWPORT_FIELD_C_GEAR_H
#define BWPORT_FIELD_C_GEAR_H

#include "types.h"
#include "struct_decls.h"
#include "field/c_gear_sw.h"

void func_ov029_021f2980(void *param_1);
GameEvent *func_ov029_021f29d8(CGearSw *work, BOOL enabled, FieldSubscreen *subscreen);
void func_ov029_021f2984(void *param_1);
BOOL func_ov029_021f2994(CGearSw *work);
// The panel at a cell, and whether it may be changed or set
int func_ov029_021f2b9c(CGearSw *work, int col, int row);
BOOL func_ov029_021f2bb8(CGearSw *work, int col, int row);
BOOL func_ov029_021f2bd4(CGearSw *work, int col, int row, int value);
BOOL func_ov029_021f2da8(CGearSw *work);
// Whether the C-Gear left for touch action 5 (func_ov010_0215fbf4's event); stopping the call alert
u32 func_ov029_021f2ab8(CGearSw *work);
void func_ov029_021f2ad4(CGearSw *work);
void func_ov029_021f2848(CGearSw *work, u32 param_2);
// The clock and the lamps' positions
void func_ov029_021f1f80(CGearSw *work);
void func_ov029_021f20ac(CGearSw *work);
// Makes (col, row) the set cell of panel type in the settings' grid, unsetting the others, and redraws them
void func_ov029_021f2aec(CGearSw *work, CGearSettings *settings, s32 col, s32 row, u32 type);
// pokebw2's CGearSw_CreateWithCallback: calls callback with callbackWork once the C-Gear shows
CGearSw *func_ov029_021f27d8(CGearSettings *param_1, FieldSubscreen *param_2, GameSystem *param_3, CGearSwCallback callback, void *callbackWork, s32 param_6);
CGearSw *func_ov029_021f279c(CGearSettings *param_1, FieldSubscreen *param_2, GameSystem *param_3, s32 param_4);

#endif // BWPORT_FIELD_C_GEAR_H
