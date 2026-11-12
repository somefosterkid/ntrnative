// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/manual_explain.h: the lines in use so
// far.
#ifndef BWPORT_APP_MANUAL_EXPLAIN_H
#define BWPORT_APP_MANUAL_EXPLAIN_H

#include "types.h"
#include "app/manual_common.h"

typedef struct ManualExplain ManualExplain;

// Black 2's, not checked against Black
#define MANUAL_EXPLAIN_PAGE_MAX 9

typedef struct {
    u16 image;
    u16 msg;
} ManualExplainPage;

typedef struct {
    u8 pageCount;
    u16 titleMsg;
    ManualExplainPage pages[MANUAL_EXPLAIN_PAGE_MAX];
    u32 result;
} ManualExplainParam;

// pokebw2's ManualExplain_Create
ManualExplain *func_ov225_021bba74(ManualExplainParam *param, ManualCommon *common);
// pokebw2's ManualExplain_Free
void func_ov225_021bbae8(ManualExplain *explain);
// pokebw2's ManualExplain_Main
BOOL func_ov225_021bbb0c(ManualExplain *explain);

#endif // BWPORT_APP_MANUAL_EXPLAIN_H
