// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_bag_bmp.h: the lines in use so far.
#ifndef BWPORT_BATTLE_B_BAG_BMP_H
#define BWPORT_BATTLE_B_BAG_BMP_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BBagBmp_FlushWindows
void func_ov197_0220b980(BBagWork *work);
// pokebw2's BBagBmp_Init
void func_ov197_0220b120(BBagWork *work);
// pokebw2's BBagBmp_DrawPage
void func_ov197_0220b1b0(BBagWork *work, u8 page);
// pokebw2's BBagBmp_TransferPage
void func_ov197_0220b990(BBagWork *work);
// pokebw2's BBagBmp_DrawSlots
void func_ov197_0220b520(BBagWork *work);
// pokebw2's BBagBmp_PrintPageNumber
void func_ov197_0220b584(BBagWork *work);
// pokebw2's BBagBmp_OpenMessage
void func_ov197_0220b904(BBagWork *work);
// pokebw2's BBagBmp_Exit
void func_ov197_0220b1a0(BBagWork *work);

#endif // BWPORT_BATTLE_B_BAG_BMP_H
