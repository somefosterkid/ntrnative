// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_category.h: the lines in use so
// far.
#ifndef BWPORT_APP_MANUAL_CATEGORY_H
#define BWPORT_APP_MANUAL_CATEGORY_H

#include "types.h"
#include "app/manual_common.h"

typedef struct ManualCategory ManualCategory;

typedef struct {
    u16 scroll;
    u16 cursor;
    // A MANUAL_LIST_RESULT_*, and the category that was picked
    u32 result;
    u16 selected;
} ManualCategoryParam;

// pokebw2's ManualCategory_Create
ManualCategory *func_ov225_021bb6e0(ManualCategoryParam *param, ManualCommon *common);
// pokebw2's ManualCategory_Free
void func_ov225_021bb7f4(ManualCategory *category);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualCategory_Main
BOOL func_ov225_021bb844(ManualCategory *category);

#endif // BWPORT_APP_MANUAL_CATEGORY_H
