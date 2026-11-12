#ifndef POKEBW2_GFL_STR_SJIS_H
#define POKEBW2_GFL_STR_SJIS_H

#include "types.h"

// Text in Shift JIS (str_sjis.c). The ROM embeds no name for this file

// Converts at most length characters of a string up to GFL_StrBufGetTerminator(), and its terminator, to Shift JIS.
// Each character takes one or two bytes of dest
void StrUnicodeToSjis(const u16 *src, char *dest, u32 length);

#endif // POKEBW2_GFL_STR_SJIS_H
