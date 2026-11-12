// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/report.h: the lines in use so far.
#ifndef BWPORT_FIELD_REPORT_H
#define BWPORT_FIELD_REPORT_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct ReportScreen ReportScreen;

// pokebw2's Report_Free
void func_ov034_021eff34(ReportScreen *wk);
// pokebw2's Report_InitSave
// Reads how much the save will write, for the bar
void func_ov034_021f0058(ReportScreen *wk);
// pokebw2's Report_IsLargeSave
// Whether at least a third of the save's blocks changed, so that the bar is worth showing
BOOL func_ov034_021f009c(ReportScreen *wk);
// pokebw2's Report_StartBar
// Replaces the last save's window with the bar, which fills as the save is written
void func_ov034_021f00bc(ReportScreen *wk);
// pokebw2's Report_IsBarFull
// Whether the bar is full, which stops it
BOOL func_ov034_021f00f0(ReportScreen *wk);
// pokebw2's Report_StopBar
void func_ov034_021f0108(ReportScreen *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's Report_Create
ReportScreen *func_ov034_021efec0(GameSystem *gsys, HeapID heapId);
// pokebw2's Report_Update
// Shows the party's icons once the text is printed
void func_ov034_021eff5c(ReportScreen *wk);
// pokebw2's Report_UpdateAltFrame
// Prints the text and shows the windows once it is printed
void func_ov034_021effb4(ReportScreen *wk);
// pokebw2's Report_IsReady
// Whether the windows and the party's icons are shown
BOOL func_ov034_021f0044(ReportScreen *wk);

#endif // BWPORT_FIELD_REPORT_H
