// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/str.h, the lines in use so far.
#ifndef BWPORT_GFL_STR_H
#define BWPORT_GFL_STR_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// String buffers (strbuf.c): up to size characters, of which length are used, followed by the terminator. Names from
// swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

// Sets the character that ends strings, 0xffff unless changed
void GFL_StrBufSetTerminator(u16 terminator);
// A buffer for size characters, the terminator included
StrBuf *GFL_StrBufCreate(u32 size, HeapID heapId);
void GFL_StrBufFree(StrBuf *strbuf);
void GFL_StrBufClear(StrBuf *strbuf);
// Copies a string to a buffer that is large enough for it, or leaves the buffer as it was
void GFL_StrBufCopy(StrBuf *dest, const StrBuf *src);
StrBuf *GFL_StrBufClone(const StrBuf *strbuf, HeapID heapId);
// Returns TRUE if the strings are the same
BOOL GFL_StrBufCmp(const StrBuf *a, const StrBuf *b);
u32 GFL_StrBufGetCharCount(const StrBuf *strbuf);
// Cuts the string to length characters
void GFL_StrBufInsertTerminator(StrBuf *strbuf, u32 length);
// Sets a buffer to a terminated string, or as much of it as fits
void GFL_StrBufLoadString(StrBuf *strbuf, const u16 *src);
// Sets a string buffer to a string of up to length characters
void GFL_StrBufLoadFixedString(StrBuf *strbuf, const u16 *str, u32 length);
// Sets a buffer to length characters, the terminator included
void GFL_StrBufCopyString(StrBuf *strbuf, const u16 *src, u32 length);
// Copies the string out, at most size characters
void GFL_StrBufStoreString(const StrBuf *strbuf, u16 *dest, u32 size);
const u16 *GFL_StrBufGetStringPtr(const StrBuf *strbuf);
u16 GFL_StrBufGetTerminator(void);
// Appends a string, if it fits whole, or a character
void func_02045aa8(StrBuf *dest, const StrBuf *src);
void GFL_StrBufAppend(StrBuf *strbuf, u16 c);
void textCopy(const u16 *src, StrBuf *dest);

StrBuf *copyTrainerNameToNewStrbuf(const u16 *name, u32 heapId);

#endif // BWPORT_GFL_STR_H
