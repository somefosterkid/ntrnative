// Black's code at ov114 0x021bfb90-0x021c0244. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

#pragma thumb off
s32 func_ov114_021bfb90(s32 *param_1, s32 param_2) {
    s32 newSize;
    s32 ptr;

    if (param_1 == NULL) {
        return 0;
    }
    if (param_2 <= 0) {
        return 0;
    }
    newSize = param_1[2] + param_2;
    ptr = (s32)gsirealloc((void *)param_1[1], newSize);
    if (ptr == 0) {
        return 0;
    }
    param_1[1] = ptr;
    param_1[2] = newSize;
    return 1;
}

s32 func_ov114_021bfbd0(s32 param_1, s32 *param_2, s32 param_3, s32 param_4) {
    if (param_1 == 0) {
        return 0;
    }
    if (param_2 == NULL) {
        return 0;
    }
    if (param_3 <= 0) {
        return 0;
    }
    if (param_4 <= 0) {
        return 0;
    }
    param_2[0] = param_1;
    param_2[1] = 0;
    param_2[2] = 0;
    param_2[3] = 0;
    param_2[4] = 0;
    param_2[5] = param_4;
    param_2[6] = 0;
    param_2[7] = 0;
    param_2[8] = 0;
    if (func_ov114_021bfb90(param_2, param_3) == 0) {
        return 0;
    }
    *(s8 *)param_2[1] = 0;
    return 1;
}

s32 func_ov114_021bfc54(s32 param_1, s32 *param_2, s8 *param_3, s32 param_4) {
    if (param_1 == 0) {
        return 0;
    }
    if (param_2 == NULL) {
        return 0;
    }
    if (param_3 == NULL) {
        return 0;
    }
    if (param_4 <= 0) {
        return 0;
    }
    param_2[0] = param_1;
    param_2[1] = (s32)param_3;
    param_2[2] = param_4;
    param_2[3] = 0;
    param_2[4] = 0;
    param_2[5] = 0;
    param_2[6] = 1;
    param_2[7] = 1;
    param_2[8] = 0;
    *param_3 = 0;
    return 1;
}

void func_ov114_021bfcb4(s32 *param_1) {
    if (param_1 == NULL || param_1[1] == 0) {
        return;
    }
    if (param_1[7] == 0) {
        gsifree((void *)param_1[1]);
    }
    memset(param_1, 0, 0x24);
}

u32 func_ov114_021bfcec(s32 *param_1, s8 *param_2, u32 param_3) {
    s32 size;
    s32 used;

    if (param_1 == NULL) {
        return 0;
    }
    if (param_2 == NULL) {
        return 0;
    }
    if ((s32)param_3 < 0) {
        return 0;
    }
    if (param_1[8] != 0) {
        return 0;
    }
    if (param_3 == 0) {
        param_3 = strlen(param_2);
    }
    size = param_1[2];
    used = param_1[3] + param_3;
    while (used >= size) {
        if (param_1[6] != 0) {
            ((s32 *)param_1[0])[73] = 1;
            ((s32 *)param_1[0])[16] = 2;
            return 0;
        }
        if (!func_ov114_021bfb90(param_1, param_1[5])) {
            ((s32 *)param_1[0])[73] = 1;
            ((s32 *)param_1[0])[16] = 1;
            return 0;
        }
        size = param_1[2];
    }
    memcpy((u8 *)param_1[1] + param_1[3], param_2, param_3);
    param_1[3] = used;
    ((s8 *)param_1[1])[used] = 0;
    return 1;
}

s32 func_ov114_021bfdec(s32 *param_1, s8 *param_2, u32 param_3) {
    GHIBuffer *buffer = (GHIBuffer *)param_1;
    const char *data = (const char *)param_2;
    int size = param_3;
    int offset = 0;
    int available = 0;

    if (buffer == NULL) {
        return FALSE;
    }
    if (data == NULL) {
        return FALSE;
    }
    if (size < 0) {
        return FALSE;
    }
    if (buffer->readOnly) {
        return FALSE;
    }
    if (buffer->connection->encryptor.engine == 0 || !buffer->connection->encryptor.active) {
        return func_ov114_021bfcec(param_1, param_2, size);
    }
    if (size == 0) {
        size = strlen(data);
    }
    if (size == 0) {
        return TRUE;
    }
    available = buffer->size - buffer->length;
    do {
        int chunk = (size < 0x3f01) ? size : 0x3f01;
        int result = buffer->connection->encryptor.encrypt(buffer->connection, &buffer->connection->encryptor, data + offset,
                                                           size, buffer->data + buffer->length, &available);

        if (result == 2) {
            if (!func_ov114_021bfb90(param_1, buffer->sizeIncrement)) {
                return FALSE;
            }
            available = buffer->size - buffer->length;
        } else if (result == 1) {
            offset += chunk;
            buffer->length = buffer->size - available;
        } else {
            gsDebugFormat(8, 4, 2, "ghiEncryptDataToBuffer encountered unhandled return code: %d\r\n", result);
            return FALSE;
        }
    } while (offset < size);
    return TRUE;
}

BOOL func_ov114_021bff7c(s32 *param_1, s8 *param_2, s8 *param_3) {
    u32 result;
    if (func_ov114_021bfcec(param_1, param_2, 0) == 0) {
        return 0;
    }
    if (func_ov114_021bfcec(param_1, ": ", 2) == 0) {
        return 0;
    }
    if (func_ov114_021bfcec(param_1, param_3, 0) == 0) {
        return 0;
    }
    result = func_ov114_021bfcec(param_1, "\r\n", 2);
    return result != 0;
}

s32 func_ov114_021c0004(s32 *param_1, s8 param_2) {
    if (param_1 == NULL) {
        return 0;
    }
    if (param_1[8] != 0) {
        return 0;
    }
    if (param_1[3] + 1 >= param_1[2]) {
        if (param_1[6] != 0) {
            ((s32 *)param_1[0])[73] = 1;
            ((s32 *)param_1[0])[16] = 2;
            return 0;
        }
        if (!func_ov114_021bfb90(param_1, param_1[5])) {
            ((s32 *)param_1[0])[73] = 1;
            ((s32 *)param_1[0])[16] = 1;
            return 0;
        }
    }
    ((s8 *)param_1[1])[param_1[3]] = param_2;
    param_1[3] = param_1[3] + 1;
    ((s8 *)param_1[1])[param_1[3]] = 0;
    return 1;
}

void func_ov114_021c00c0(s32 *param_1, u32 param_2) {
    u8 auStack_20 [16];

    OS_SPrintf(auStack_20, "%d", param_2);
    func_ov114_021bfcec(param_1, (s8 *)auStack_20, 0);
}

void func_ov114_021c00fc(s32 *param_1) {
    param_1[3] = 0;
    param_1[4] = 0;
    if (param_1[8] == 0) {
        *(u8 *)param_1[1] = 0;
    }
}

BOOL func_ov114_021c011c(s32 *param_1) {
    GHIConnection *connection = (GHIConnection *)param_1;
    int ready;
    int failed;

    do {
        int sent;

        if (connection->encryptor.engine == 0 || connection->encryptor.mode == 1) {
            int result = GSISocketSelect(connection->socket, 0, &ready, &failed);

            if (result == -1 || (result == 1 && failed != 0)) {
                connection->completed = TRUE;
                connection->result = 5;
                if (result == -1) {
                    connection->socketError = GOAGetLastError(connection->socket);
                } else {
                    connection->socketError = 0;
                }
                return FALSE;
            }
            if (result < 1 || ready == 0) {
                return TRUE;
            }
        }
        sent = func_ov114_021c0684(connection, connection->sendBuffer.data + connection->sendBuffer.pos,
                                   connection->sendBuffer.length - connection->sendBuffer.pos);
        switch (sent) {
        case -1:
            return FALSE;
        case -2:
            return TRUE;
        }
        connection->sendBuffer.pos += sent;
    } while (connection->sendBuffer.pos < connection->sendBuffer.length);
    return TRUE;
}
