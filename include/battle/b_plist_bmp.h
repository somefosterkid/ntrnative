// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_plist_bmp.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_B_PLIST_BMP_H
#define BWPORT_BATTLE_B_PLIST_BMP_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BPlistBmp_Init
void func_ov198_0220bc24(BPlistWork *work);
// pokebw2's BPlistBmp_CreatePageWindows
void func_ov198_0220bc70(BPlistWork *work, u8 page);
// pokebw2's BPlistBmp_FreePageWindows
void func_ov198_0220bdd4(BPlistWork *work);
// pokebw2's BPlistBmp_Exit
void func_ov198_0220be04(BPlistWork *work);
// pokebw2's BPlistBmp_DrawPage
void func_ov198_0220be2c(BPlistWork *work, u8 page);
// pokebw2's BPlistBmp_PrintInfo10
void func_ov198_0220c688(BPlistWork *work);
// pokebw2's BPlistBmp_PrintMessage
void func_ov198_0220d6ac(BPlistWork *work);
// pokebw2's BPlistBmp_OpenMessage
void func_ov198_0220e13c(BPlistWork *work);
// pokebw2's BPlistBmp_SetEmbargoMessage
void func_ov198_0220e1d4(BPlistWork *work);
// pokebw2's BPlistBmp_FlushWindows
void func_ov198_0220e22c(BPlistWork *work);
// pokebw2's BPlistBmp_TransferPage
void func_ov198_0220e25c(BPlistWork *work);
// pokebw2's BPlistBmp_DrawInfoFrame
void func_ov198_0220e27c(BPlistWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BPlistBmp_PrintInfo9
void func_ov198_0220c67c(BPlistWork *work);

#endif // BWPORT_BATTLE_B_PLIST_BMP_H
