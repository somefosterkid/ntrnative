// GameSpy's XML value reader (lib/dwc/src/gs/xml.c): the functions the ov175 competition code calls on a
// SOAP response's element tree. Each takes the handle, the element's name, and where to put what it reads.
#ifndef BWPORT_GS_XML_H
#define BWPORT_GS_XML_H

#include "types.h"
#include "stddef.h"
#include "gs/largeint.h"

// The writer (the request's text: its buffer, length and capacity) and the element functions that add to it, each
// taking the namespace prefix and the element's name: an element's start and end tag, a text (escaped), a UTF-8
// string, a UTF-16 one, a number, a 64-bit one, a float, a hex or base64 data, a date and a large integer
u32 func_ov009_021829ac(u32 *writer, s8 *prefix, s8 *name);
u32 func_ov009_02182a2c(u32 *writer, s8 *prefix, s8 *name);
u32 func_ov009_02182ac0(u32 *writer, s8 *prefix, s8 *name, s8 *text);
u32 func_ov009_02182b68(u32 *writer, s8 *prefix, s8 *name, s8 *text);
u32 func_ov009_02182c10(u32 *writer, s8 *prefix, s8 *name, u16 *text);
u32 func_ov009_02182cf0(u32 *writer, s8 *prefix, s8 *name, s32 value);
u32 func_ov009_02182d78(u32 *writer, s8 *prefix, s8 *name, s64 value);
u32 func_ov009_02182dfc(u32 *writer, s8 *prefix, s8 *name, f32 value);
u32 func_ov009_02182e90(u32 *writer, s8 *prefix, s8 *name, const u8 *data, s32 size);
BOOL func_ov009_02182f40(u32 *writer, s8 *prefix, s8 *name, const u8 *data, s32 size);
u32 func_ov009_02182ff8(u32 *writer, s8 *prefix, s8 *name, s32 time);
BOOL func_ov009_021830d0(u32 *writer, s8 *prefix, s8 *name, LargeIntBuf *lint);
// The reader: back to its first element; an element's text into a buffer; a large integer
u32 func_ov009_021834d0(u32 *reader);
u32 func_ov009_0218372c(u32 *reader, u8 *name, s8 *text, size_t size);
BOOL func_ov009_02183e50(u32 *reader, char *name, LargeIntBuf *value);

u32 func_ov009_021834e8(u32 *param_1, u8 *param_2);
u32 func_ov009_02183670(u32 *param_1, u8 *param_2, s32 *param_3, s32 *param_4);
u32 func_ov009_021837a4(u32 *param_1, char *param_2, u8 *param_3, s32 param_4, u32 *param_5);
u32 func_ov009_02183918(u32 *param_1, u8 *param_2, u8 *param_3, s32 *param_4);
u32 func_ov009_02183a24(u32 *param_1, u8 *param_2, u32 *param_3);
u32 func_ov009_02183ae4(u32 *param_1, u8 *param_2, s64 *param_3);
u32 func_ov009_02183ba4(u32 *param_1, u8 *param_2, s32 *param_3);
u32 func_ov009_02183cd4(u32 *param_1, u8 *param_2, f32 *param_3);
s32 func_ov009_02183ee0(u32 *param_1, s8 *param_2);

#endif
