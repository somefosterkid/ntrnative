// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknsearch_list.h: the lines in use so
// far.
#ifndef BWPORT_APP_ZKNSEARCH_LIST_H
#define BWPORT_APP_ZKNSEARCH_LIST_H

#include "types.h"
#include "app/zknsearch_main.h"

// pokebw2's ZknSearchList_Exit
void func_ov210_021c0550(ZknSearchWork *wk);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknSearchList_InitOrder
void func_ov210_021c0234(ZknSearchWork *wk);
// pokebw2's ZknSearchList_InitInitial
void func_ov210_021c02dc(ZknSearchWork *wk);
// pokebw2's ZknSearchList_InitType
void func_ov210_021c0384(ZknSearchWork *wk);
// pokebw2's ZknSearchList_InitColor
void func_ov210_021c0434(ZknSearchWork *wk);
// pokebw2's ZknSearchList_InitShape
void func_ov210_021c04cc(ZknSearchWork *wk);

#endif // BWPORT_APP_ZKNSEARCH_LIST_H
