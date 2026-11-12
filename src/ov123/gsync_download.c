// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov199/gsync_download.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Game Sync's download of a file from the Dream World's server, through NitroDWC's download library, with a timeout.
// The name is the ROM's string, from GFL_HeapAllocate's calls, and the work's name, data_ov123_021dac20, from an assert.
// Function names are ours.

#include "types.h"
#include "app/gsync/gsync_download.h"
#include "gfl/dwc_rap.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "dwc/nd.h"

void func_0200f7dc(void);
void func_ov009_02156de0(u32 (*callback)(void *, int, int, int), void *);

// How long a call may take, in frames
#define GSYNC_DOWNLOAD_TIMEOUT 3600
// The buffer's room beyond the file's size
#define GSYNC_DOWNLOAD_BUFFER_EXTRA 0x40

struct GSyncDownload {
    DWCNdFileInfo file;
    // Set by the download library's callback once a call ends, with its error
    BOOL done;
    u32 error;
    void *buffer;
    u32 bufferSize;
    u32 unkC0;
    BOOL cleanedUp;
    BOOL cancelled;
    // Counts the frames of the call since it started, while it is not 0
    int timer;
    BOOL timedOut;
};

static void func_ov123_021d5ac0(u32 reason, u32 error, int serverError);
static u32 func_ov123_021d5ba8(void *work, int a1, int event, int a3);
static BOOL func_ov123_021d5cac(GSyncDownload *dl);

// pokebw2's _pDLWork
static GSyncDownload *data_ov123_021dac20;

// pokebw2's GSyncDownload_OnNdEvent
static void func_ov123_021d5ac0(u32 reason, u32 error, int serverError) {
    u32 code = error;

    switch (error) {
    case DWC_ND_ERROR_NONE:
        break;
    case DWC_ND_ERROR_HTTP:
        code = error;
        break;
    case DWC_ND_ERROR_CANCELED:
        data_ov123_021dac20->cancelled = TRUE;
        break;
    case ND_ERROR_FATAL:
        func_0200f7dc();
        break;
    }
    data_ov123_021dac20->done = TRUE;
    data_ov123_021dac20->error = code;
}

// pokebw2's GSyncDownload_Create
GSyncDownload *func_ov123_021d5b10(HeapID heapId, u32 size) {
    GSyncDownload *dl = GFL_HeapAllocate(heapId, sizeof(GSyncDownload), TRUE, "gsync_download.c", 136);

    GFL_ASSERT(!data_ov123_021dac20);
    data_ov123_021dac20 = dl;
    dl->buffer = GFL_HeapAllocate(heapId, size + GSYNC_DOWNLOAD_BUFFER_EXTRA, FALSE, "gsync_download.c", 140);
    dl->bufferSize = size;
    return dl;
}

// pokebw2's GSyncDownload_Free
void func_ov123_021d5b70(GSyncDownload *dl) {
    if (data_ov123_021dac20 != NULL) {
        func_ov009_02156de0(NULL, NULL);
        GFL_HeapFree(dl->buffer);
        GFL_HeapFree(dl);
        data_ov123_021dac20 = NULL;
    }
}

// pokebw2's GSyncDownload_GetBuffer
void *func_ov123_021d5ba0(GSyncDownload *dl) {
    return dl->buffer;
}

// pokebw2's GSyncDownload_OnDwcEvent
static u32 func_ov123_021d5ba8(void *work, int a1, int event, int a3) {
    GSyncDownload *dl = work;

    if (!dl->cleanedUp && func_ov114_021c5b80()) {
        dl->cleanedUp = TRUE;
    }
    if (!dl->done) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSyncDownload_Init
BOOL func_ov123_021d5bd4(GSyncDownload *dl) {
    dl->done = FALSE;
    if (!func_ov114_021c5928(func_ov123_021d5ac0, "IRAO", "WX9x7Zh6J3aBC4zQ")) {
        return FALSE;
    }
    func_ov009_02156de0(func_ov123_021d5ba8, dl);
    return TRUE;
}

// pokebw2's GSyncDownload_IsSucceeded
BOOL func_ov123_021d5c0c(GSyncDownload *dl) {
    if (data_ov123_021dac20->done && dl->error == DWC_ND_ERROR_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSyncDownload_IsFailed
BOOL func_ov123_021d5c2c(GSyncDownload *dl) {
    if (dl->timedOut && dl->cancelled) {
        return TRUE;
    }
    if (data_ov123_021dac20->done && dl->error != DWC_ND_ERROR_NONE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's GSyncDownload_SetAttr
BOOL func_ov123_021d5c64(GSyncDownload *dl, const char *attr1, int attr2) {
    BOOL result = FALSE;
    char buf[20];

    dl->done = FALSE;
    STD_TSNPrintf(buf, sizeof(buf), "%d", attr2);
    if (func_ov114_021c5c04(attr1, buf, "")) {
        result = TRUE;
    }
    return result;
}

// pokebw2's GSyncDownload_ResetTimer
void func_ov123_021d5c9c(GSyncDownload *dl) {
    dl->timer = 0;
    dl->timedOut = FALSE;
}

// Counts a frame of the call, and cancels it once it takes too long. Returns whether it goes on
// pokebw2's GSyncDownload_UpdateTimer
static BOOL func_ov123_021d5cac(GSyncDownload *dl) {
    if (dl->timedOut) {
        return FALSE;
    }
    if (dl->timer != 0) {
        dl->timer++;
        if (dl->timer > GSYNC_DOWNLOAD_TIMEOUT) {
            dl->timedOut = TRUE;
            if (!func_ov114_021c5db4()) {
                dl->cancelled = TRUE;
            }
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's GSyncDownload_GetFileList
BOOL func_ov123_021d5cf8(GSyncDownload *dl) {
    BOOL result = TRUE;

    dl->done = FALSE;
    dl->timer = 1;
    dl->timedOut = FALSE;
    if (!func_ov114_021c5c38(&dl->file, 0, 1)) {
        result = FALSE;
    }
    return result;
}

// pokebw2's GSyncDownload_GetFile
BOOL func_ov123_021d5d24(GSyncDownload *dl) {
    BOOL result = TRUE;

    dl->done = FALSE;
    dl->timer = 1;
    dl->timedOut = FALSE;
    if (!func_ov114_021c5cfc(&dl->file, dl->buffer, dl->bufferSize + GSYNC_DOWNLOAD_BUFFER_EXTRA)) {
        result = FALSE;
    }
    return result;
}

// pokebw2's GSyncDownload_Cleanup
BOOL func_ov123_021d5d58(GSyncDownload *dl) {
    if (!dl->cleanedUp) {
        dl->done = FALSE;
        if (!func_ov114_021c5b80()) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's GSyncDownload_Main
void func_ov123_021d5d7c(GSyncDownload *dl) {
    if (func_ov123_021d5cac(dl)) {
        func_ov114_021c5ab8();
    }
}

// pokebw2's GSyncDownload_GetFileSize
u32 func_ov123_021d5d8c(GSyncDownload *dl) {
    return dl->file.size;
}
