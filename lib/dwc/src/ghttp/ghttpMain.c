// Black's code at ov114 0x021c1d30-0x021c235c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

extern u32 data_ov114_021ce5ac;
extern s32 data_ov114_021ce5b0;
static s32 data_ov114_021d3b64;
static u8 data_ov114_021ced74[0x14] = {
    0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x43, 0x6c, 0x6f, 0x73, 0x69, 0x6e, 0x67, 0x0a, 0x00
};
#pragma thumb off
void func_ov114_021c1d30(s32 *param_1) {
    s32 statusMajor = param_1[0x118 / 4] / 100;

    switch (statusMajor) {
    case 1:
    case 2:
    case 3:
        return;
    case 4:
        switch (param_1[0x118 / 4]) {
        case 401:
            param_1[0x40 / 4] = 9;
            break;
        case 403:
            param_1[0x40 / 4] = 10;
            break;
        case 404:
        case 410:
            param_1[0x40 / 4] = 11;
            break;
        default:
            param_1[0x40 / 4] = 8;
            break;
        }
        break;
    case 5:
        param_1[0x40 / 4] = 12;
        break;
    }
}

s32 func_ov114_021c1dec(s32 *param_1) {
    GHIConnection *connection = (GHIConnection *)param_1;
    BOOL completed;

    if (connection->processing) {
        return FALSE;
    }
    connection->processing = TRUE;

    if (connection->state == 0) {
        func_ov114_021c3bac(connection);
    }
    if (connection->state == 1) {
        func_ov114_021c3d10(connection);
    }
    if (connection->state == 2) {
        func_ov114_021c3e10((s32 *)connection);
    }
    if (connection->state == 3) {
        func_ov114_021c3ec0(connection);
    }
    if (connection->state == 4) {
        func_ov114_021c4150(connection);
    }
    if (connection->state == 5) {
        func_ov114_021c4384(connection);
    }
    if (connection->state == 6) {
        func_ov114_021c4684((s32 *)connection);
    }
    if (connection->state == 7) {
        func_ov114_021c4764((s32 *)connection);
    }
    if (connection->state == 8) {
        func_ov114_021c492c(connection);
    }
    if (connection->state == 9) {
        func_ov114_021c4e7c(connection);
    }
    if (connection->state == 10) {
        func_ov114_021c5430(connection);
    }
    if (connection->state == 11) {
        gsDebugFormat(8, 3, 0x10, data_ov114_021ced74);
    }

    if (connection->redirectUrl != NULL) {
        func_ov114_021c0e58(connection);
    }

    completed = connection->completed;
    if (connection->result == 0x12 && !completed) {
        if (!CanReceiveOnSocket(connection->socket)) {
            connection->completed = TRUE;
        }
    }

    if (connection->completed) {
        if (connection->encryptor.secure == TRUE) {
            func_ov114_021c1d30((s32 *)connection);
            if (!func_ov114_021c0b4c((s32 *)connection)) {
                connection->processing = FALSE;
                connection->state = 11;
                return FALSE;
            }
            func_ov114_021c0244((s32 *)connection);
            if (!func_ov114_021c0c14(connection)) {
                connection->processing = FALSE;
                return FALSE;
            }
        } else {
            if (!func_ov114_021c0b4c((s32 *)connection)) {
                connection->processing = FALSE;
                connection->state = 11;
                return FALSE;
            }
            func_ov114_021c0244((s32 *)connection);
            if (!func_ov114_021c0c14(connection)) {
                connection->processing = FALSE;
                return FALSE;
            }
        }
    } else {
        connection->processing = FALSE;
    }
    return completed;
}

void func_ov114_021c1ff8(void) {
    func_ov114_021c0370();
    data_ov114_021d3b64 = data_ov114_021d3b64 + 1;
    if (data_ov114_021d3b64 == 1) {
        func_ov114_021c0368();
        data_ov114_021ce5b0 = 0x7d;
        data_ov114_021ce5ac = 0xfa;
    } else {
        func_ov114_021c0374();
    }
}

void func_ov114_021c204c(void) {
    func_ov114_021c0370();
    data_ov114_021d3b64 = data_ov114_021d3b64 + -1;
    if (data_ov114_021d3b64 == 0) {
        func_ov114_021c0fb0();
        if (data_ov114_021d3b2c != (void *)0x0) {
            gsifree(data_ov114_021d3b2c);
            data_ov114_021d3b2c = (void *)0x0;
        }
        func_ov114_021c0374();
        func_ov114_021c036c();
        return;
    }
    func_ov114_021c0374();
}

s32 func_ov114_021c20a4(const char *url, const char *headers, char *buffer, int bufferSize, GHIPost *post, BOOL throttle, BOOL blocking, DWCGHTTPRawProgressFunc progress, DWCGHTTPRawCompletedFunc completed, void *param) {
    GHIConnection *connection;
    BOOL ready;

    if (url == NULL || url[0] == '\0') {
        return -1;
    }
    if (bufferSize < 0) {
        return -1;
    }
    if (buffer != NULL && bufferSize == 0) {
        return -1;
    }
    if (data_ov114_021d3b64 == 0) {
        func_ov114_021c1ff8();
    }

    connection = func_ov114_021c0998();
    if (connection == NULL) {
        return -1;
    }
    connection->type = 0;
    connection->url = func_ov009_0217f1e0(url);
    if (connection->url == NULL) {
        func_ov114_021c0d40((s32 *)connection);
        return -1;
    }
    if (headers != NULL && headers[0] != '\0') {
        connection->headers = func_ov009_0217f1e0(headers);
        if (connection->headers == NULL) {
            func_ov114_021c0d40((s32 *)connection);
            return -1;
        }
    }
    connection->post = post;
    connection->blocking = blocking;
    connection->progressCallback = progress;
    connection->completedCallback = completed;
    connection->callbackParam = param;
    connection->throttle = throttle;
    connection->userBuffer = buffer != NULL;
    if (connection->userBuffer) {
        ready = func_ov114_021bfc54((s32)connection, (s32 *)&connection->fileBuffer, (s8 *)buffer, bufferSize);
    } else {
        ready = func_ov114_021bfbd0((s32)connection, (s32 *)&connection->fileBuffer, 0x800, 0x800);
    }
    if (!ready) {
        func_ov114_021c0d40((s32 *)connection);
        return -1;
    }
    if (post != NULL) {
        if (!func_ov114_021c2a78(connection)) {
            func_ov114_021c0d40((s32 *)connection);
            return -1;
        }
    }

    if (blocking) {
        if (!func_ov114_021c1dec((s32 *)connection)) {
            do {
                func_ov009_0217f2bc(10);
            } while (!func_ov114_021c1dec((s32 *)connection));
        }
        return 0;
    }
    return connection->request;
}

void func_ov114_021c2270(void) {
    func_ov114_021c0df8((u8 *)func_ov114_021c1dec);
}

u32 func_ov114_021c2284(s32 param_1) {
    s32 *node = func_ov114_021c0da4(param_1);

    if (node != NULL) {
        func_ov114_021c1dec(node);
        return 1;
    }
    return 0;
}

void func_ov114_021c22a4(s32 param_1) {
    s32 *node = func_ov114_021c0da4(param_1);

    if (node == NULL) {
        return;
    }
    node[16] = 0x12;
    node[4] = 0xb;
    node[73] = 1;
    func_ov114_021c0b4c(node);
}

s32 func_ov114_021c22d4(s32 param_1) {
    s32 *node = func_ov114_021c0da4(param_1);

    if (node != NULL) {
        return node[4];
    }
    return 0;
}

void func_ov114_021c22ec(s32 param_1, s32 param_2) {
    s32 *ptr;

    ptr = func_ov114_021c0da4(param_1);
    if (ptr != (s32 *)0x0) {
        ptr[0x62] = param_2;
    }
}

GHIPost *func_ov114_021c2304(void) {
    return func_ov114_021c23e4();
}

void func_ov114_021c2310(GHIPost *post, BOOL autoFree) {
    if (post == NULL) {
        return;
    }
    func_ov114_021c245c(post, autoFree);
}

void func_ov114_021c2324(int request, GHIPost *post) {
    if (post == NULL) {
        return;
    }
    func_ov114_021c2490(request, post);
}

BOOL func_ov114_021c2338(GHIPost *post, SoapWriter *writer) {
    return func_ov114_021c2584(post, writer);
}

u32 func_ov114_021c2344(u32 *param_1, s32 param_2) {
    return func_ov114_021c1184((s8 *)param_1, (s8 *)param_2);
}

u32 func_ov114_021c2350(const char *url) {
    return func_ov114_021c1210(url);
}
