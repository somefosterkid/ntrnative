// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/msgsearch.h: the lines in use so far.
#ifndef BWPORT_APP_UI_MSGSEARCH_H
#define BWPORT_APP_UI_MSGSEARCH_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// msgsearch.c: A search of message files for the strings that start with a prefix
typedef struct MsgSearch MsgSearch;

// A string found: its message file and its line
typedef struct {
    u32 file;
    u32 line;
} MsgSearchResult;

// pokebw2's names by Black 2's overlay 139 addresses: func_ov139_0219a438, func_ov139_0219a490 and func_ov139_0219a4a4
MsgSearch *func_ov067_021b6c74(MsgData **files, u32 count, HeapID heapId);
void func_ov067_021b6ccc(MsgSearch *search);
// Fills results with the strings of a file that start with prefix, up to max, and returns how many it found
u32 func_ov067_021b6ce0(MsgSearch *search, u32 file, u32 unused, StrBuf *prefix, MsgSearchResult *results, u32 max);

#endif // BWPORT_APP_UI_MSGSEARCH_H
