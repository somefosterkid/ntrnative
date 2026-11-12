// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_title.h: the lines in use so far.
#ifndef BWPORT_APP_MANUAL_TITLE_H
#define BWPORT_APP_MANUAL_TITLE_H

#include "types.h"
#include "app/manual_common.h"

typedef struct ManualTitle ManualTitle;

// Which titles the list has: those of a category, or all
enum {
    MANUAL_TITLE_MODE_CATEGORY,
    MANUAL_TITLE_MODE_ALL,
};

typedef struct {
    u32 mode;
    u16 category;
    u16 scroll;
    u16 cursor;
    // A MANUAL_LIST_RESULT_*, and the title that was picked
    u32 result;
    u16 selected;
} ManualTitleParam;

// pokebw2's ManualTitle_Create
ManualTitle *func_ov225_021bb854(ManualTitleParam *param, ManualCommon *common);
// pokebw2's ManualTitle_Free
void func_ov225_021bba14(ManualTitle *title);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualTitle_Main
BOOL func_ov225_021bba64(ManualTitle *title);

#endif // BWPORT_APP_MANUAL_TITLE_H
