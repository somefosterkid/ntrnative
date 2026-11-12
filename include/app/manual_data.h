// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_data.h: the lines in use so far.
#ifndef BWPORT_APP_MANUAL_DATA_H
#define BWPORT_APP_MANUAL_DATA_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct ManualData ManualData;

// Black 2's, not checked against Black
// What a title needs before the manual lists it
enum {
    MANUAL_CONDITION_NONE,
    MANUAL_CONDITION_C_GEAR,
    MANUAL_CONDITION_FLAG,
    MANUAL_CONDITION_MEDAL_BOX,
    MANUAL_CONDITION_HABITAT_LIST,
};

// The picture of a page that has none
#define MANUAL_IMAGE_NONE 0xffff

// pokebw2's ManualData_Free
void func_ov225_021b9ba4(ManualData *data);
// pokebw2's ManualData_GetTitleMsg
u16 func_ov225_021b9bc8(ManualData *data, u16 title);
// pokebw2's ManualData_GetTitleCondition
u16 func_ov225_021b9bd4(ManualData *data, u16 title);
// pokebw2's ManualData_GetPageMsg
u16 func_ov225_021b9bf8(ManualData *data, u16 title, u16 page);
// pokebw2's ManualData_GetPageImage
u16 func_ov225_021b9c0c(ManualData *data, u16 title, u16 page);
// pokebw2's ManualData_GetTitleCount
u16 func_ov225_021b9c20(ManualData *data);
// pokebw2's ManualData_GetCategoryMsg
u16 func_ov225_021b9c28(ManualData *data, u16 category);
// pokebw2's ManualData_GetCategoryTitle
u16 func_ov225_021b9c40(ManualData *data, u16 category, u16 index);
// pokebw2's ManualData_GetCategoryCount
u16 func_ov225_021b9c50(ManualData *data);
// pokebw2's ManualData_IsRead
BOOL func_ov225_021b9c98(ManualData *data, u16 flag, GameData *gameData);
// pokebw2's ManualData_IsImageValid
BOOL func_ov225_021b9cc8(ManualData *data, u16 image);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualData_GetDefaultImage
// The picture the upper screen shows outside of a title's pages
u16 func_ov225_021b9cdc(ManualData *data);
// pokebw2's ManualData_Create
ManualData *func_ov225_021b9b3c(ArcTool *arc, HeapID heapId);
// pokebw2's ManualData_GetTitleReadFlag
// The flag of the miscellaneous save data that says the title was read
u16 func_ov225_021b9be0(ManualData *data, u16 title);
// pokebw2's ManualData_GetTitlePageCount
u16 func_ov225_021b9bec(ManualData *data, u16 title);
// pokebw2's ManualData_GetCategoryTitleCount
u16 func_ov225_021b9c34(ManualData *data, u16 category);
// pokebw2's ManualData_IsOpen
BOOL func_ov225_021b9c58(ManualData *data, u16 condition, GameData *gameData);
// pokebw2's ManualData_SetRead
void func_ov225_021b9cb0(ManualData *data, u16 flag, GameData *gameData);

#endif // BWPORT_APP_MANUAL_DATA_H
