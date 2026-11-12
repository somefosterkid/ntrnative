// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/print_msg.h: the lines in use so far.
#ifndef BWPORT_APP_UI_PRINT_MSG_H
#define BWPORT_APP_UI_PRINT_MSG_H

#include "types.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

// How a string is aligned at its x
#define PRINT_ALIGN_LEFT 0

#define PRINT_ALIGN_RIGHT 1
#define PRINT_ALIGN_CENTER 2

// pokebw2's PrintStrAligned
// Prints a string into a window through the queue, aligned at x by a PRINT_ALIGN_*
void func_ov067_021b6ae4(PrintWindow *window, PrintQueue *queue, u16 x, u16 y, const StrBuf *strbuf, Font *font,
                         u16 color, u32 align);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PrintFraction
// Prints "number1/number2" with the slash centered at x
void func_ov067_021b6b2c(PrintWindow *window, PrintQueue *queue, Font *font, u16 x, u16 y, u16 color, s32 number1,
                         s32 number2, HeapID heapId);

#endif // BWPORT_APP_UI_PRINT_MSG_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/print_msg.h: the lines in use so far.
#ifndef BWPORT_APP_UI_PRINT_MSG_H

#include "types.h"
#include "system/printsys.h"
#include "struct_decls.h"

// How a string is aligned at its x

#endif // BWPORT_APP_UI_PRINT_MSG_H
