// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/app_keycursor.h, the lines in use so
// far, with Black's names: pokebw2's KeyCursor_Create, KeyCursor_Free and KeyCursor_Update are its own guesses.
#ifndef BWPORT_SYSTEM_APP_KEYCURSOR_H
#define BWPORT_SYSTEM_APP_KEYCURSOR_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// The cursor that shows a message waits for a button

KeyCursor *func_02028590(u16 bgColor, BOOL keys, BOOL touch, HeapID heapId);
void func_02028604(KeyCursor *cursor);
// Shows the cursor while the stream is paused, until a button is pressed
void func_020286c4(KeyCursor *cursor, PrintStream *stream, BmpWin *window);

// pokebw2's KeyCursor_Erase
// Erases the cursor from a window's bitmap, and draws its next frame
void func_02028618(KeyCursor *cursor, GFLBitmap *bitmap, u16 bgColor);
// pokebw2's KeyCursor_Draw
void func_0202865c(KeyCursor *cursor, GFLBitmap *bitmap, u16 bgColor);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's KeyCursor_CreateEx
// KeyCursor_Create with a cursor of 2 by 2 tiles of its own
KeyCursor *func_020285a4(u16 bgColor, BOOL keys, BOOL touch, HeapID heapId, const u8 *bitmap);

#endif // BWPORT_SYSTEM_APP_KEYCURSOR_H
