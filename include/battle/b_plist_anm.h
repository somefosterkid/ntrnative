// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/b_plist_anm.h.
#ifndef BWPORT_BATTLE_B_PLIST_ANM_H
#define BWPORT_BATTLE_B_PLIST_ANM_H

#include "types.h"
#include "struct_decls.h"

// pokebw2's BPlistAnm_CutButtonScrn, BPlistAnm_CutPageScrn, BPlistAnm_StartButtonAnm, BPlistAnm_MainButtonAnm,
// BPlistAnm_PutPageButtons, BPlistAnm_RestorePalette and BPlistAnm_PutReturnButton
void func_ov198_0220b120(BPlistWork *work, const void *scrn);
void func_ov198_0220b3b4(BPlistWork *work, const void *scrn);
void func_ov198_0220b760(BPlistWork *work, u8 button);
void func_ov198_0220b7ac(BPlistWork *work);
void func_ov198_0220b848(BPlistWork *work, u8 page);
void func_ov198_0220bb94(BPlistWork *work, u8 page);
void func_ov198_0220bbf0(BPlistWork *work);

#endif // BWPORT_BATTLE_B_PLIST_ANM_H
