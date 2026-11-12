// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zknlist_bmp.h: the lines in use so far.
#ifndef BWPORT_APP_ZKNLIST_BMP_H
#define BWPORT_APP_ZKNLIST_BMP_H

#include "types.h"
#include "app/zknlist_main.h"

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's ZknListBmp_Update
void func_ov209_021bd3fc(ZknListWork *wk);
// pokebw2's ZknListBmp_Init
void func_ov209_021bd398(ZknListWork *wk);
// pokebw2's ZknListBmp_PutCounts
// How many species the Pokédex has seen and caught
void func_ov209_021bd628(ZknListWork *wk);
// pokebw2's ZknListBmp_PutSearchCount
// How many species the search found
void func_ov209_021bd704(ZknListWork *wk);
// pokebw2's ZknListBmp_Exit
void func_ov209_021bd3e0(ZknListWork *wk);

// pokebw2's ZknListBmp_PrintItem: prints a species' name into its row
void func_ov209_021bd75c(ZknListWork *wk, PrintWindow *window, StrBuf *strbuf, u32 index);
#endif // BWPORT_APP_ZKNLIST_BMP_H
