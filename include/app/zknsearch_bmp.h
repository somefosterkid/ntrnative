// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknsearch_bmp.h: the lines in use so
// far.
#ifndef BWPORT_APP_ZKNSEARCH_BMP_H
#define BWPORT_APP_ZKNSEARCH_BMP_H

#include "types.h"
#include "app/zknsearch_main.h"
#include "system/printsys.h"
#include "struct_decls.h"

// pokebw2's ZknSearchBmp_Init
void func_ov210_021be368(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_Exit
void func_ov210_021be3b4(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_Update
// Runs the print queue and sends the windows that are printed
void func_ov210_021be3d4(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutMain
// The main page: its labels and the criteria's values
void func_ov210_021be564(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutInfo
// The line at the top of the touch screen: what to do, or nothing while the search runs
void func_ov210_021be7e4(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_ClearInfo
void func_ov210_021be820(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutOrder
// The pages of the lists: the line at the top, and the criterion's name and value on the top screen
void func_ov210_021be83c(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutOrderValue
void func_ov210_021be8b8(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutInitial
void func_ov210_021be8f8(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutInitialValue
void func_ov210_021be978(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutType
void func_ov210_021be9d4(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutTypeValue
void func_ov210_021bea54(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutColor
void func_ov210_021beb0c(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutColorValue
void func_ov210_021beb8c(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutShape
void func_ov210_021bebe8(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PrintItem
// A list's item, into its row's window
void func_ov210_021bec60(ZknSearchWork *wk, PrintWindow *window, StrBuf *strbuf);
// pokebw2's ZknSearchBmp_FlushItem
void func_ov210_021becac(ZknSearchWork *wk, PrintWindow *window);
// pokebw2's ZknSearchBmp_PutSearching
// The messages of the search
void func_ov210_021becf8(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutStart
void func_ov210_021bed1c(ZknSearchWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknSearchBmp_PutFound
void func_ov210_021bed04(ZknSearchWork *wk);
// pokebw2's ZknSearchBmp_PutNotFound
void func_ov210_021bed10(ZknSearchWork *wk);

#endif // BWPORT_APP_ZKNSEARCH_BMP_H
