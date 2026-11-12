// Black's code at ov114 0x021c0368-0x021c08b0. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

// The proxy (ghiProxyAddress, ghiProxyPort)
char *data_ov114_021d3b2c;
u16 data_ov114_021d3b28;

u32 data_ov114_021ce5ac = 250;
s32 data_ov114_021ce5b0 = 125;
#pragma thumb off
void func_ov114_021c0368(void) {
}

void func_ov114_021c036c(void) {
}

void func_ov114_021c0370(void) {
}

void func_ov114_021c0374(void) {
}

BOOL func_ov114_021c0378(GHIConnection *connection) {
    int inSize = 0;
    int outSize = 0;
    int result;

    do {
        do {
            const char *in = connection->decodeBuffer.data + connection->decodeBuffer.pos;
            char *out;

            inSize = connection->decodeBuffer.length - connection->decodeBuffer.pos;
            out = connection->recvBuffer.data + connection->recvBuffer.length;
            outSize = connection->recvBuffer.size - connection->recvBuffer.length;
            result = connection->encryptor.decrypt(connection, &connection->encryptor, in, &inSize, out, &outSize);
            if (result == 2) {
                if (!func_ov114_021bfb90((s32 *)&connection->recvBuffer, connection->recvBuffer.sizeIncrement)) {
                    return FALSE;
                }
            } else if (result == 3) {
                return FALSE;
            }
        } while (result == 2 && outSize == 0);

        if (inSize > connection->decodeBuffer.length) {
            gsDebugFormat(8, 4, 1, "ghiDecryptReceivedData read past the end of connection->decodeBuffer! (%d\x5c%d bytes)\r\n",
                          inSize, connection->decodeBuffer.length);
            return FALSE;
        }
        connection->decodeBuffer.pos += inSize;
        connection->recvBuffer.length += outSize;
    } while (outSize > 0);

    if (connection->decodeBuffer.pos > 0xff) {
        int remaining = connection->decodeBuffer.length - connection->decodeBuffer.pos;

        if (remaining == 0) {
            func_ov114_021c00fc((s32 *)&connection->decodeBuffer);
        } else {
            memmove(connection->decodeBuffer.data, connection->decodeBuffer.data + connection->decodeBuffer.pos,
                    remaining);
            connection->decodeBuffer.pos = 0;
            connection->decodeBuffer.length = remaining;
        }
    }
    return TRUE;
}

int func_ov114_021c04d4(GHIConnection *connection, char *buffer, int *size) {
    int max = *size - 1;
    int received;
    int error;

    if (connection->throttle != 0) {
        u32 now = current_time();

        if (now < connection->lastThrottleTime + data_ov114_021ce5ac) {
            return 1;
        }
        connection->lastThrottleTime = now;
        if (max >= data_ov114_021ce5b0) {
            max = data_ov114_021ce5b0;
        }
    }

    if (connection->encryptor.engine != 0 && connection->encryptor.active == 1 &&
        connection->encryptor.established == 1) {
        int length = max;

        if (func_ov114_021c1c80(connection, &connection->encryptor, buffer, &length) == 1) {
            if (length == -1) {
                return 1;
            }
            received = length;
        } else {
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = 0;
            connection->closing = TRUE;
            return 3;
        }
    } else {
        received = recv(connection->socket, buffer, max, 0);
        if (received == -1) {
            error = GOAGetLastError(connection->socket);
            if (error == -56) {
                connection->closing = TRUE;
                return 2;
            }
            if (error == -6 || error == -26 || error == -76) {
                return 1;
            }
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = error;
            connection->closing = TRUE;
            return 3;
        }
    }

    if (received == 0) {
        connection->closing = TRUE;
        return 2;
    }
    buffer[received] = '\0';
    *size = received;
    gsDebugFormat(8, 0, 0x20, "Received %d bytes\n", received);
    return 0;
}

int func_ov114_021c0684(GHIConnection *connection, const char *data, int size) {
    int sent;

    if (data == NULL || size == 0) {
        return 0;
    }

    if (connection->encryptor.engine != 0 && connection->encryptor.active == 1 &&
        connection->encryptor.established == 1) {
        int count = 0;

        if (func_ov114_021c1c78(connection, &connection->encryptor, data, size, &count) == 1) {
            if (count == -1) {
                return -2;
            }
            sent = count;
        } else {
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = 0;
            return -1;
        }
    } else {
        sent = send(connection->socket, data, size, 0);
        if (sent == -1) {
            int error = GOAGetLastError(connection->socket);

            if (error == -6 || error == -26 || error == -76) {
                return 0;
            }
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = error;
            return -1;
        }
    }

    if (connection->state == 6 && !connection->postingState.isXml) {
        connection->postingState.bytesPosted += sent;
    }
    return sent;
}

s32 func_ov114_021c07b8(s32 *param_1, u8 *param_2, s32 param_3) {
    s32 rcode = 0;

    if (param_1[0x198 / 4] != 0 && param_1[0x1a4 / 4] == 1 && param_1[0x1a8 / 4] == 1) {
        if (!func_ov114_021bfdec(&param_1[22], param_2, param_3)) {
            return 0;
        }
        if (!func_ov114_021c011c(param_1)) {
            return 0;
        }
        if (param_1[26] < param_1[25]) {
            return 2;
        }
        func_ov114_021c00fc(&param_1[22]);
        return 1;
    }
    if (param_1[26] >= param_1[25]) {
        rcode = func_ov114_021c0684((GHIConnection *)param_1, (const char *)param_2, param_3);
        switch (rcode) {
        case -1:
            return 0;
        case -2:
            return 2;
        }
        if (rcode == param_3) {
            return 1;
        }
    }
    if (!func_ov114_021bfcec(&param_1[22], param_2 + rcode, param_3 - rcode)) {
        return 0;
    }
    return 2;
}
