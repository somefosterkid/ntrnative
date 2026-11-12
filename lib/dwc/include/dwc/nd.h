// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/nd.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_ND_H
#define BWPORT_DWC_ND_H

#include "types.h"

// A file the server offers, 0xb0 bytes
typedef struct {
    // The name that the file is asked for with, and a text about it in UTF-16
    char fileName[0x22];
    u16 description[0x33];
    char param1[11];
    // Mystery Gift's gifts give the games they are for here, as a hexadecimal mask of 1 << GAME_VERSION
    char param2[11];
    char param3[11];
    u8 unkA9[3];
    u32 size;
} DWCNdFileInfo;

// What a callback reports on: the first call, the file counts, a list and a file. The library's end is ours
typedef enum {
    DWC_ND_CBREASON_INITIALIZE,
    DWC_ND_CBREASON_GETFILELISTNUM,
    DWC_ND_CBREASON_GETFILELIST,
    DWC_ND_CBREASON_GETFILE,
    ND_CBREASON_CLEANED_UP,
} DWCNdCallbackReason;

// The errors of the download library; ND_ERROR_BAD_PARAMETER (a text that is too long) and ND_ERROR_FATAL (the library
// could not be set up) are ours
enum {
    DWC_ND_ERROR_NONE,
    DWC_ND_ERROR_ALLOC,
    DWC_ND_ERROR_BUSY,
    DWC_ND_ERROR_HTTP,
    DWC_ND_ERROR_BUFFULL,
    ND_ERROR_BAD_PARAMETER,
    DWC_ND_ERROR_CANCELED,
    DWC_ND_ERROR_DLSERVER,
    ND_ERROR_FATAL,
};

// The callback of every asynchronous call, with its result and the code of a failure of the server (-1 when there is none)
typedef void (*DWCNdCallback)(u32 reason, u32 error, int serverError);

// pokebw2's DWC_NdInitAsync
BOOL func_ov114_021c5928(DWCNdCallback callback, const char *gameCode, const char *password);
// pokebw2's DWC_NdSetAttr
BOOL func_ov114_021c5c04(const char *attr1, const char *attr2, const char *attr3);
// pokebw2's DWC_NdGetFileListAsync
BOOL func_ov114_021c5c38(DWCNdFileInfo *files, u32 offset, u32 count);
// pokebw2's DWC_NdGetFileAsync
BOOL func_ov114_021c5cfc(DWCNdFileInfo *file, void *buffer, u32 size);
// pokebw2's DWC_NdCleanupAsync
BOOL func_ov114_021c5b80(void);
// pokebw2's NdReportResults
// Gives the game the callback of what has ended since the last call; not a name of pret's
void func_ov114_021c5ab8(void);
// pokebw2's DWC_NdGetProgress
BOOL func_ov114_021c5e14(u32 *received, u32 *contentLength);
// pokebw2's DWC_NdCancelAsync
BOOL func_ov114_021c5db4(void);

#endif // BWPORT_DWC_ND_H
