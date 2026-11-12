// Black's code at ov114 0x021bf520-0x021bfb90. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/ghttp_internal.h"

typedef struct DWCGHTTPState {
    s32 startCount;
    DWCGHTTPEntry *entries;
} DWCGHTTPState;

static DWCGHTTPState data_ov114_021d3b20;
static void *data_ov114_021ce540 = data_ov009_021a2f2c;
static struct {
    void **head;
    s32 value;
} data_ov114_021ce544 = { &data_ov114_021ce540, 1 };
#pragma thumb off
BOOL func_ov114_021bf520(u32 *param_1) {
    u32 result = func_ov114_021c2344(param_1, (s32)&data_ov114_021ce544);

    return result == 1;
}

BOOL func_ov114_021bf540(const char *url) {
    return func_ov114_021c2350(url) == 1 ? TRUE : FALSE;
}

u32 func_ov114_021bf558(const char *gamename) {
    func_ov114_021c1ff8();
    data_ov114_021d3b20.startCount = data_ov114_021d3b20.startCount + 1;
    return 1;
}

u32 func_ov114_021bf57c(void) {
    if (data_ov114_021d3b20.startCount <= 0) {
        return 1;
    }
    func_ov114_021c204c();
    data_ov114_021d3b20.startCount = data_ov114_021d3b20.startCount - 1;
    if (data_ov114_021d3b20.startCount == 0) {
        func_ov114_021bfb20();
    }
    return 1;
}

u32 func_ov114_021bf5b8(void) {
    BOOL BVar1;

    BVar1 = DWCi_IsError();
    if (BVar1 == 0) {
        func_ov114_021c2270();
        return 1;
    }
    return 0;
}

BOOL func_ov114_021bf5d8(int request, int result, char *buffer, int length, void *param) {
    DWCGHTTPEntry *entry = param;
    DWCGHTTPCompletedFunc completed;
    BOOL freeBuffer;

    completed = entry->param.completed;
    freeBuffer = entry->param.freeBuffer;
    if (completed != NULL) {
        if (result == 0) {
            completed(buffer, length, result, entry->param.param);
        } else if (result == 0x12) {
            completed(NULL, 0, result, entry->param.param);
        } else {
            func_ov114_021bf8c0(result);
            completed(NULL, 0, result, entry->param.param);
        }
    }
    if (result != 0 || freeBuffer == TRUE) {
        if (entry != NULL && entry->buffer != NULL) {
            DWC_Free(6, entry->buffer, 0);
        } else {
            freeBuffer = TRUE;
        }
    }
    func_ov114_021bfa70(entry);
    return freeBuffer != FALSE ? TRUE : FALSE;
}

void func_ov114_021bf67c(int request, int state, const char *buffer, int length, int received, int total, void *param) {
    DWCGHTTPEntry *entry = param;
    DWCGHTTPProgressFunc progress = entry->param.progress;

    if (progress != NULL) {
        progress(state, buffer, length, received, total, entry->param.param);
    }
}

s32 func_ov114_021bf6c4(const char *url, int bufferSize, BOOL freeBuffer, DWCGHTTPProgressFunc progress, DWCGHTTPCompletedFunc completed, void *param) {
    return func_ov114_021bf6ec(url, bufferSize, freeBuffer, NULL, progress, completed, param);
}

s32 func_ov114_021bf6ec(const char *url, int bufferSize, BOOL freeBuffer, GHIPost **post, DWCGHTTPProgressFunc progress, DWCGHTTPCompletedFunc completed, void *param) {
    char *buffer = NULL;
    DWCGHTTPParam entryParam;
    DWCGHTTPEntry *entry;
    int request;

    if (DWCi_IsError()) {
        return -1;
    }
    entryParam.param = param;
    entryParam.completed = completed;
    entryParam.progress = progress;
    entryParam.freeBuffer = freeBuffer;
    entry = func_ov114_021bfa08(&entryParam);
    if (entry == NULL) {
        func_ov114_021bf8c0(-5);
        completed(NULL, 0, -5, param);
        return -5;
    }

    if (bufferSize > 0) {
        buffer = DWC_Alloc(6, bufferSize);
        if (buffer == NULL) {
            func_ov114_021bf8c0(-5);
            completed(NULL, 0, -5, param);
            func_ov114_021bfa70(entry);
            return -5;
        }
        entry->buffer = buffer;
    }

    request = func_ov114_021c20a4(url, NULL, buffer, bufferSize, post == NULL ? NULL : *post, FALSE, FALSE,
                                  progress == NULL ? NULL : func_ov114_021bf67c, func_ov114_021bf5d8, entry);
    if (request < 0) {
        func_ov114_021bf8c0(request);
        completed(NULL, 0, request, param);
        if (entry->buffer != NULL) {
            DWC_Free(6, entry->buffer, 0);
        }
        func_ov114_021bfa70(entry);
    }
    entry->request = request;
    func_ov114_021c22ec(request, 1);
    return request;
}

void func_ov114_021bf880(s32 param_1) {
    DWCGHTTPEntry *entry;

    func_ov114_021c22a4(param_1);
    entry = func_ov114_021bfaf0(param_1);
    if (entry == NULL) {
        return;
    }
    if (entry->buffer != NULL) {
        DWC_Free(6, entry->buffer, 0);
    }
    func_ov114_021bfa70(entry);
}

s32 func_ov114_021bf8c0(s32 param_1) {
    s32 code;
    s32 detail;

    detail = 0xfffe8130;
    code = 7;
    if (param_1 == 0) {
        return 0;
    }
    switch (param_1) {
    case -7:
        detail -= 0x320;
        break;
    case -6:
        detail += 0xfffffcd6;
        break;
    case -5:
        detail -= 0x348;
        break;
    case -4:
    case -3:
    case -2:
        detail -= 0x334;
        break;
    case -1:
        detail += 0xfffffcc2;
        break;
    case 1:
    case 20:
        detail -= 1;
        code = 9;
        break;
    case 2:
        detail -= 0x348;
        break;
    case 3:
        detail += 0xfffffcae;
        break;
    case 4:
        detail -= 0x1e;
        break;
    case 5:
        detail -= 0x32;
        break;
    case 6:
    case 11:
    case 12:
        detail -= 0x14;
        break;
    case 7:
        detail -= 0x35c;
        break;
    case 8:
    case 9:
    case 10:
        detail += 0xfffffc9a;
        break;
    case 13:
    case 14:
        detail -= 0x370;
        break;
    case 15:
        detail += 0xfffffc86;
        break;
    case 16:
        detail -= 0x384;
        break;
    case 17:
        detail += 0xfffffc72;
        break;
    default:
        break;
    }
    DWCi_SetError(code, detail);
    return param_1;
}

DWCGHTTPEntry *func_ov114_021bfa08(const DWCGHTTPParam *param) {
    DWCGHTTPEntry *entry = DWC_Alloc(6, sizeof(DWCGHTTPEntry));

    if (entry == NULL) {
        return NULL;
    }
    entry->param = *param;
    entry->next = NULL;
    entry->buffer = NULL;
    if (data_ov114_021d3b20.entries == NULL) {
        data_ov114_021d3b20.entries = entry;
        return entry;
    }
    entry->next = data_ov114_021d3b20.entries;
    data_ov114_021d3b20.entries = entry;
    return entry;
}

void func_ov114_021bfa70(void *param_1) {
    DWCGHTTPEntry *previous = data_ov114_021d3b20.entries;
    DWCGHTTPEntry *current;

    if (previous == NULL) {
        return;
    }
    if (previous == param_1) {
        DWCGHTTPEntry *next = previous->next;

        DWC_Free(6, previous, 0);
        data_ov114_021d3b20.entries = next;
        return;
    }
    current = previous->next;
    if (current != NULL) {
        do {
            if (current != param_1) {
                previous = current;
            } else {
                DWCGHTTPEntry *next = previous->next;

                previous->next = next->next;
                DWC_Free(6, next, 0);
                return;
            }
            current = current->next;
        } while (current != NULL);
    }
}

DWCGHTTPEntry *func_ov114_021bfaf0(int request) {
    DWCGHTTPEntry *entry = data_ov114_021d3b20.entries;

    while (entry != NULL && entry->request != request) {
        entry = entry->next;
    }
    return entry;
}

void func_ov114_021bfb20(void) {
    DWCGHTTPEntry *entry = data_ov114_021d3b20.entries;

    if (entry != NULL) {
        do {
            DWCGHTTPEntry *current = entry;

            entry = entry->next;
            if (current->buffer != NULL) {
                DWC_Free(6, current->buffer, 0);
            }
            DWC_Free(6, current, 0);
        } while (entry != NULL);
    }
    data_ov114_021d3b20.entries = NULL;
}
