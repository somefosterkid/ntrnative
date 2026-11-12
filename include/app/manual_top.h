// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_top.h: the lines in use so far.
#ifndef BWPORT_APP_MANUAL_TOP_H
#define BWPORT_APP_MANUAL_TOP_H

#include "types.h"
#include "app/manual_common.h"
#include "struct_decls.h"

typedef struct ManualTop ManualTop;

enum {
    MANUAL_TOP_RESULT_CLOSE,
    MANUAL_TOP_RESULT_RETURN,
    MANUAL_TOP_RESULT_CATEGORY,
    MANUAL_TOP_RESULT_TITLE,
};

struct ManualTopParam {
    u8 cursor;
    u32 result;
};

// pokebw2's ManualTop_Create
ManualTop *func_ov225_021bb2b4(ManualTopParam *param, ManualCommon *common);
// pokebw2's ManualTop_Main
BOOL func_ov225_021bb31c(ManualTop *top);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ManualTop_Free
void func_ov225_021bb308(ManualTop *top);

#endif // BWPORT_APP_MANUAL_TOP_H
