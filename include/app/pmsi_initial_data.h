// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pmsi_initial_data.h.
#ifndef BWPORT_APP_PMSI_INITIAL_DATA_H
#define BWPORT_APP_PMSI_INITIAL_DATA_H

#include "types.h"
#include "struct_decls.h"

// The initials that the phrase input sorts words by, A to Z and the others, and where their buttons are, of overlay
// 110. The ROM doesn't name this file. The names are Black's dsd names; pokebw2's guesses are in the comments

u32 func_ov110_021bb1f4(void); // PMSIInitial_GetCount
// The initial's letter (PMSIInitial_GetString)
void func_ov110_021bb1f8(u32 initial, StrBuf *buf);
// The initial's letter (PMSIInitial_GetCode)
u16 func_ov110_021bb214(u32 initial);
// Where the initial's button is (PMSIInitial_GetPos)
void func_ov110_021bb224(u32 initial, u32 *x, u32 *y);
// The initials next to it, for the cursor, or a button above or below the grid
#define PMSI_INITIAL_POS_BUTTON_0 0xfc
#define PMSI_INITIAL_POS_BUTTON_1 0xfd
#define PMSI_INITIAL_POS_BUTTON_2 0xfe
u32 func_ov110_021bb240(u32 initial); // PMSIInitial_GetUp
u32 func_ov110_021bb250(u32 initial); // PMSIInitial_GetDown
u32 func_ov110_021bb260(u32 initial); // PMSIInitial_GetLeft
u32 func_ov110_021bb270(u32 initial); // PMSIInitial_GetRight
u32 func_ov110_021bb280(u32 initial); // PMSIInitial_GetBottom

#endif // BWPORT_APP_PMSI_INITIAL_DATA_H
