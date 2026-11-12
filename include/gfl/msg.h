// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/msg.h, the lines in use so far.
#ifndef BWPORT_GFL_MSG_H
#define BWPORT_GFL_MSG_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Message files (msgdata.c): a block of messages for each language, encrypted with a key from each message's ID.

// Loads a file of a message archive. With preload set, all its text is read at once, and otherwise each message is
// read from the archive when it is loaded
MsgData *GFL_MsgSysLoadData(BOOL preload, u16 arcId, u16 fileId, HeapID heapId);
void GFL_MsgDataFree(MsgData *msgData);
void GFL_MsgDataLoadStrbuf(MsgData *msgData, u32 messageId, StrBuf *strbuf);
StrBuf *GFL_MsgDataLoadStrbufNew(MsgData *msgData, u32 messageId);
// A message file already in memory
MsgData *GFL_MsgDataCreateFromHandle(void *file, HeapID heapId);
u32 GFL_MsgDataGetLineCount(MsgData *msgData);
// Copies a message to a buffer of size characters, cut short to fit, with the terminator
void GFL_MsgDataLoadRawStr(MsgData *msgData, u32 messageId, u16 *dest, u32 size);
// The language that messages are loaded in, when the file has it
void func_020461d4(u8 langId);
u8 GFL_MsgDataGetDefaultLangID(void);

#endif // BWPORT_GFL_MSG_H
