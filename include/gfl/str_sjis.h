// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/str_sjis.h.
#ifndef BWPORT_GFL_STR_SJIS_H
#define BWPORT_GFL_STR_SJIS_H

#include "types.h"

// Text in Shift JIS (str_sjis.c). The ROM embeds no name for this file

// Converts at most length characters of a string up to GFL_StrBufGetTerminator(), and its terminator, to Shift JIS.
// Each character takes one or two bytes of dest
void func_02052274(const u16 *src, char *dest, u32 length);

#endif // BWPORT_GFL_STR_SJIS_H
