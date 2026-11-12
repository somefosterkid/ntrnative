// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_menuwork.h: the prototypes of
// Black's named functions and the types they need.
#ifndef BWPORT_SYSTEM_BMP_MENUWORK_H
#define BWPORT_SYSTEM_BMP_MENUWORK_H

#include "types.h"
#include "stddef.h"
#include "gfl/heap.h"
#include "struct_decls.h"

ListMenuOption *ListMenuCore_CreateOptionList(u32 count, u32 heapId);
// Adds an option after the last one, with a message or a copy of text, unless the array is full
void ListMenuCore_AppendMsgOption(ListMenuOption *options, MsgData *msgData, u32 messageId, intptr_t value,
                                  HeapID heapId);
// The number of options in the list
u32 ListMenuCore_GetFirstFreeIndex(const ListMenuOption *options);

// An option of a list menu: its text, and the value the menu returns when it is chosen (a number, or a pointer such
// as the rows of the menu the option opens: the Union Room's)
struct ListMenuOption {
    StrBuf *text;
    intptr_t value;
};

#define LIST_MENU_END ((StrBuf *)-1)

void ListMenuCore_AppendStrBufOption(ListMenuOption *options, const StrBuf *text, intptr_t value, HeapID heapId);
// Frees the options' strings, leaving the array empty
void ListMenuCore_FreeStrBufs(ListMenuOption *options);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Frees the options' strings and the array
void ListMenuCore_FreeOptionList(ListMenuOption *options);

#endif // BWPORT_SYSTEM_BMP_MENUWORK_H
