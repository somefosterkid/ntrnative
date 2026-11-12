// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync/gsync_download.h: the lines in use
// so far.
#ifndef BWPORT_APP_GSYNC_GSYNC_DOWNLOAD_H
#define BWPORT_APP_GSYNC_GSYNC_DOWNLOAD_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's GSyncDownload_Free
void func_ov123_021d5b70(GSyncDownload *dl);
// pokebw2's GSyncDownload_GetBuffer
void *func_ov123_021d5ba0(GSyncDownload *dl);
// pokebw2's GSyncDownload_Init
BOOL func_ov123_021d5bd4(GSyncDownload *dl);
// pokebw2's GSyncDownload_IsSucceeded
// Whether the last call ended without an error, or with one or by timing out
BOOL func_ov123_021d5c0c(GSyncDownload *dl);
// pokebw2's GSyncDownload_IsFailed
BOOL func_ov123_021d5c2c(GSyncDownload *dl);
// pokebw2's GSyncDownload_SetAttr
// Sets the attributes the files are listed by: a string and a number
BOOL func_ov123_021d5c64(GSyncDownload *dl, const char *attr1, int attr2);
// pokebw2's GSyncDownload_ResetTimer
void func_ov123_021d5c9c(GSyncDownload *dl);
// pokebw2's GSyncDownload_GetFileList
BOOL func_ov123_021d5cf8(GSyncDownload *dl);
// pokebw2's GSyncDownload_GetFile
BOOL func_ov123_021d5d24(GSyncDownload *dl);
// pokebw2's GSyncDownload_Cleanup
BOOL func_ov123_021d5d58(GSyncDownload *dl);
// pokebw2's GSyncDownload_GetFileSize
u32 func_ov123_021d5d8c(GSyncDownload *dl);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's GSyncDownload_Create
GSyncDownload *func_ov123_021d5b10(HeapID heapId, u32 size);
// pokebw2's GSyncDownload_Main
void func_ov123_021d5d7c(GSyncDownload *dl);

#endif // BWPORT_APP_GSYNC_GSYNC_DOWNLOAD_H
