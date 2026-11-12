// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/subscreen.h, the lines in use so far.
#ifndef BWPORT_FIELD_SUBSCREEN_H
#define BWPORT_FIELD_SUBSCREEN_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black's layout is only partly known: the word at 8 is not 0 while the sub screen is not ready
// The fields the code reads so far, named by offset
struct FieldSubscreen {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
    u16 unk_c;
    u8 unk_e[2];
    // The screen to go back to once the field menu closes (FieldSubscreen_SaveReturnSubscreen)
    u32 unk_10;
    Field *field;
    // The callback func_ov021_021a0f7c sets and its work (pokebw2's callback, callbackWork): the C-Gear calls it
    void (*unk_18)(void *work);
    void *unk_1c;
    void *unk_20;
    // Allocated, never used (pokebw2's unused)
    u32 unused[2];
};

void FieldSubscreen_ReqChange(FieldSubscreen *subscreen, u32 mode);
BOOL FieldSubscreen_IsReady(FieldSubscreen *subscreen);
// pokebw2's func_ov036_021984f0
void func_ov021_021a0fb4(FieldSubscreen *subscreen, HeapID heapId);
u32 FieldSubscreen_GetIDForChange(FieldSubscreen *subscreen, u32 param);
// Changes to a screen at once, without the wipe
void func_ov021_021a0f40(FieldSubscreen *param_1, u32 param_2);
// pokebw2's FieldSubscreen_ReqChangeEx: changes the subscreen, calling the callback with work once it has changed
void func_ov021_021a0f7c(FieldSubscreen *subscreen, u32 mode, void (*callback)(void *work), void *work);
u32 FieldSubscreen_GetScreenID(FieldSubscreen *subscreen);
u32 FieldSubscreen_GetReturnSubscreen(FieldSubscreen *subscreen);
void func_ov021_021a0fa8(FieldSubscreen *subscreen);
// A screen of the table: its ID and the functions that run it. The field names are pokebw2's FieldSubscreenDef's
typedef struct FieldSubscreenState {
    u32 screenId;
    void (*init)(FieldSubscreen *subscreen, u32 previousScreenId);
    // active is whether no event is running
    void (*update)(FieldSubscreen *subscreen, BOOL active);
    void (*updateAltFrame)(FieldSubscreen *subscreen, BOOL active);
    GameEvent *(*checkEvent)(FieldSubscreen *subscreen, BOOL enabled);
    void (*terminate)(FieldSubscreen *subscreen);
    void (*draw)(FieldSubscreen *subscreen);
} FieldSubscreenState;
extern const FieldSubscreenState data_ov021_021d8728[];

u32 func_ov021_021a12a4(FieldSubscreen *subscreen);

// pokebw2's func_ov036_02198b04, func_ov036_02198b10, func_ov036_02198b1c, func_ov036_02198b28,
// func_ov036_02198b34 and func_ov036_02198b40
BOOL func_ov021_021a1518(FieldSubscreen *subscreen);
void func_ov021_021a1524(FieldSubscreen *subscreen);
void func_ov021_021a1530(FieldSubscreen *subscreen);
BOOL func_ov021_021a153c(FieldSubscreen *subscreen);
void func_ov021_021a1548(FieldSubscreen *subscreen);
BOOL func_ov021_021a1554(FieldSubscreen *subscreen);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Sets the subscreen to change to once the field menu closes
void FieldSubscreen_SaveReturnSubscreen(FieldSubscreen *subscreen, u32 screenId);
void func_ov021_021a12d4(FieldSubscreen *subscreen, HeapID heapId);

void func_ov021_021a12bc(FieldSubscreen *subscreen, u32 param);

FieldSubscreen *func_ov021_021a0cf4(u32 heapId, Field *field, u32 mode);

#endif // BWPORT_FIELD_SUBSCREEN_H
