// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/str_tool.h: the prototypes of Black's
// named functions and the types they need.
#ifndef BWPORT_SYSTEM_STR_TOOL_H
#define BWPORT_SYSTEM_STR_TOOL_H

#include "types.h"
#include "struct_decls.h"

// Writes a number in digits characters, the last of which is always printed; ascii picks ASCII digits over full-width
// ones. A negative number gets a minus sign first
void GFL_WordSetFormatNumber(StrBuf *strbuf, s32 number, u32 digits, u32 pad, BOOL ascii);
// Copies at most n characters, the last of which is always the terminator
void wcharsncpy(const u16 *src, u16 *dest, u32 n);
BOOL GFL_StrBufIsCompressed(const StrBuf *strbuf);

#define NUM_PAD_SPACE 1
#define NUM_PAD_ZERO 2

// Returns TRUE if the strings are the same
BOOL wcharscmp(const u16 *a, const u16 *b);
// Copies src, expanding it if it is compressed, as Trainer names in message file 409 are
void GFL_StrBufUncompress(StrBuf *dest, const StrBuf *src);

// How GFL_WordSetFormatNumber pads a number to its digits
#define NUM_PAD_NONE 0

#endif // BWPORT_SYSTEM_STR_TOOL_H
