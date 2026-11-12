// Black's code at ov114 0x021c3a38-0x021c569c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <ctype.h>
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
#include "stdlib.h"

// The other ghttp files of Black still take the connections and the buffers as s32 *
#define ghiAppendDataToBuffer(buffer, data, size) func_ov114_021bfcec((s32 *)(buffer), (s8 *)(data), (size))
#define ghiSendBufferedData(connection) func_ov114_021c011c((s32 *)(connection))
#define ghiResetBuffer(buffer) func_ov114_021c00fc((s32 *)(buffer))
#define ghiDoReceive(connection, buffer, size) func_ov114_021c04d4((connection), (buffer), (size))
#define ghiDecryptReceivedData(connection) func_ov114_021c0378(connection)
#define ghiCallProgressCallback(connection, buffer, bufferLen) func_ov114_021c02c8((s32 *)(connection), (s32)(buffer), (bufferLen))
#define ghiAppendHeaderToBuffer(buffer, name, value) func_ov114_021bff7c((s32 *)(buffer), (s8 *)(name), (s8 *)(value))
#define ghiAppendCharToBuffer(buffer, c) func_ov114_021c0004((s32 *)(buffer), (c))
#define ghiAppendIntToBuffer(buffer, i) func_ov114_021c00c0((s32 *)(buffer), (i))
#define ghiEncryptDataToBuffer(buffer, data, size) func_ov114_021bfdec((s32 *)(buffer), (s8 *)(data), (size))

#pragma thumb off
BOOL func_ov114_021c3a38(GHIConnection *connection) {
    char *url;
    u32 length;
    char saved;
    char *path;
    char *space;

    if (connection == NULL) {
        return FALSE;
    }
    url = connection->url;
    if (url == NULL) {
        return FALSE;
    }
    if (strncmp(url, "http://", 7) == 0) {
        connection->secure = FALSE;
        url += 7;
    } else if (strncmp(url, "https://", 8) == 0) {
        connection->secure = TRUE;
        url += 8;
    } else {
        return FALSE;
    }

    length = strcspn(url, ":/");
    saved = url[length];
    url[length] = '\0';
    connection->serverAddress = func_ov009_0217f1e0(url);
    if (connection->serverAddress == NULL) {
        return FALSE;
    }
    url[length] = saved;
    url += length;

    if (url[0] == ':') {
        url++;
        connection->serverPort = atol(url);
        if (connection->serverPort == 0) {
            return FALSE;
        }
        do {
            url++;
        } while (url[0] != '\0' && url[0] != '/');
    } else if (connection->secure == TRUE) {
        connection->serverPort = 443;
    } else {
        connection->serverPort = 80;
    }

    if (url[0] == '\0') {
        url = "/";
    }
    path = func_ov009_0217f1e0(url);
    connection->requestPath = path;
    space = strchr(path, ' ');
    while (space != NULL) {
        *space = '+';
        path = connection->requestPath;
        space = strchr(path, ' ');
    }
    {
        BOOL valid = FALSE;

        if (path != NULL) {
            valid = TRUE;
        }
        return valid;
    }
}

void func_ov114_021c3bac(GHIConnection *connection) {
    gsDebugFormat(8, 3, 0x10, "Socket Initialization\n");
    ghiCallProgressCallback(connection, NULL, 0);
    func_ov009_0217f25c();
    if (!func_ov114_021c3a38(connection)) {
        connection->completed = TRUE;
        connection->result = 3;
        return;
    }

    if (connection->secure == TRUE && connection->encryptor.engine == 0) {
        gsDebugFormat(8, 0, 2, "Encryption engine not set for HTTPS.  Using default engine\r\n");
        func_ov114_021c109c(connection->request, 5);
    } else if (connection->secure != TRUE && connection->encryptor.engine != 0) {
        func_ov114_021c109c(connection->request, 0);
        gsDebugFormat(8, 0, 2, "Encryption engine set for unsecured URL. Removing encryption.\r\n");
    }

    if (connection->secure == TRUE && connection->encryptor.open == 0) {
        gsDebugFormat(8, 3, 15, "Initializing SSL engine\n");
        if (connection->encryptor.init(connection, &connection->encryptor) == 3) {
            gsDebugFormat(8, 3, 2, "Failed to initialize SSL engine\n");
            connection->completed = TRUE;
            connection->result = 0x11;
            return;
        }
    }
    connection->state = 1;
    ghiCallProgressCallback(connection, NULL, 0);
}

void func_ov114_021c3d10(GHIConnection *connection) {
    char *host;

    if (connection->hostLookup != NULL) {
        connection->state = 2;
        ghiCallProgressCallback(connection, NULL, 0);
        return;
    }
    gsDebugFormat(8, 3, 0x10, "Host Lookup\n");
    host = connection->proxyAddress;
    if (host == NULL) {
        host = data_ov114_021d3b2c;
        if (host == NULL) {
            host = connection->serverAddress;
        }
    }
    connection->serverIp = inet_addr(host);
    if (connection->serverIp == -1) {
        if (func_ov009_0217f150(host, (GSIResolveHostnameHandle *)&connection->hostLookup) == -1) {
            gsDebugFormat(8, 3, 1, "Thread Creation Failed\n");
            connection->hostLookup = NULL;
            connection->completed = TRUE;
            connection->result = 4;
            return;
        }
    }
    if (connection->serverIp == -1) {
        connection->state = 2;
        ghiCallProgressCallback(connection, NULL, 0);
    } else {
        connection->state = 3;
        ghiCallProgressCallback(connection, NULL, 0);
    }
}

void func_ov114_021c3e10(s32 *param_1) {
    s32 result;

    result = func_ov009_0217f1cc((GSIResolveHostnameHandle)param_1[0x73]);
    param_1[7] = result;
    if (result == -1) {
        gsDebugFormat(8, 3, 1, (s8 *)"Error resolving hostname\n");
        param_1[0x73] = 0;
        param_1[0x49] = 1;
        param_1[0x10] = 4;
        return;
    }
    if (result == 0) {
        param_1[4] = 2;
        func_ov114_021c02c8(param_1, 0, 0);
        return;
    }
    param_1[0x73] = 0;
    gsDebugFormat(8, 3, 0x10, (s8 *)"DNS lookup complete\n");
    param_1[4] = 3;
    func_ov114_021c02c8(param_1, 0, 0);
}

void func_ov114_021c3ec0(GHIConnection *connection) {
    int result;

    gsDebugFormat(8, 3, 0x10, "Connecting\n");
    if (connection->socket == -1) {
        connection->socket = socket(2, 1, 0);
        if (connection->socket == -1) {
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = GOAGetLastError(connection->socket);
            return;
        }
        if (!func_ov009_0217ee50(connection->socket, 0)) {
            connection->completed = TRUE;
            connection->result = 5;
            connection->socketError = GOAGetLastError(connection->socket);
            return;
        }
        if (connection->throttle != 0) {
            SetReceiveBufferSize(connection->socket, data_ov114_021ce5b0);
        }
        if (connection->encryptor.engine == 4) {
            if (connection->encryptor.start(connection, &connection->encryptor) == 3) {
                connection->completed = TRUE;
                connection->result = 0x11;
                return;
            }
            connection->encryptor.engine = 0;
        }

        {
            SOCSockAddrIn address = {0};

            address.family = 2;
            if (connection->proxyAddress != NULL) {
                address.port = SOC_HtoNs(connection->proxyPort);
            } else if (data_ov114_021d3b2c != NULL) {
                address.port = SOC_HtoNs(data_ov114_021d3b28);
            } else {
                address.port = SOC_HtoNs(connection->serverPort);
            }
            address.addr.addr = connection->serverIp;
            result = connect(connection->socket, (SOCKADDR *)&address, 8);
        }
        if (result == -1) {
            int error = GOAGetLastError(connection->socket);

            if (error != -6 && error != -26 && error != -76) {
                connection->completed = TRUE;
                connection->result = 6;
                connection->socketError = error;
                return;
            }
        }
    }

    {
        int ready;
        int failed;

        result = GSISocketSelect(connection->socket, NULL, &ready, &failed);
        if (result == -1 || (result == 1 && failed != 0)) {
            connection->completed = TRUE;
            connection->result = 6;
            if (result == -1) {
                connection->socketError = GOAGetLastError(connection->socket);
            } else {
                connection->socketError = 0;
            }
            return;
        }
        if (result == 1 && ready != 0) {
            if (connection->encryptor.engine == 0) {
                connection->state = 5;
            } else {
                connection->state = 4;
            }
            ghiCallProgressCallback(connection, NULL, 0);
        }
    }
}

void func_ov114_021c4150(GHIConnection *connection) {
    int size;
    char buffer[0x401];
    int result;

    if (connection->encryptor.unk_0C == 0) {
        gsDebugFormat(8, 3, 0x10, "Securing Session\n");
        if (connection->encryptor.start != NULL) {
            if (connection->encryptor.start(connection, &connection->encryptor) == 3) {
                connection->completed = TRUE;
                connection->result = 0x11;
                return;
            }
        }
        if (connection->encryptor.active != 0) {
            connection->state = 5;
            ghiCallProgressCallback(connection, NULL, 0);
            return;
        }
    }

    if (connection->encryptor.unk_1C != 0) {
        if (connection->encryptor.start != NULL) {
            if (connection->encryptor.start(connection, &connection->encryptor) == 3) {
                connection->completed = TRUE;
                connection->result = 0x11;
                return;
            }
        }
        if (connection->encryptor.active != 0) {
            connection->state = 5;
            ghiCallProgressCallback(connection, NULL, 0);
        }
        return;
    }

    if (connection->sendBuffer.pos < connection->sendBuffer.length) {
        if (!ghiSendBufferedData(connection)) {
            return;
        }
        if (connection->sendBuffer.pos < connection->sendBuffer.length) {
            return;
        }
        ghiResetBuffer(&connection->sendBuffer);
    }

    size = sizeof(buffer);
    result = ghiDoReceive(connection, buffer, &size);
    if (result == 2 || result == 3) {
        connection->completed = TRUE;
        connection->result = 0x11;
        return;
    }
    if (result != 0) {
        return;
    }
    if (!ghiAppendDataToBuffer(&connection->decodeBuffer, buffer, size)) {
        return;
    }
    if (!ghiDecryptReceivedData(connection)) {
        connection->completed = TRUE;
        connection->result = 0x11;
        return;
    }
    if (connection->encryptor.active != 0) {
        connection->state = 5;
        ghiCallProgressCallback(connection, NULL, 0);
    }
}

void func_ov114_021c4384(GHIConnection *connection) {
    GHIBuffer *buffer;
    const char *method;
    char length[16];

    gsDebugFormat(8, 3, 0x10, "Sending Request\n");
    if (connection->sendBuffer.length == 0) {
        if (connection->encryptor.engine == 0 || connection->encryptor.mode == 0) {
            buffer = &connection->sendBuffer;
        } else {
            buffer = &connection->encodeBuffer;
        }

        if (connection->post != NULL && !connection->postingState.finished) {
            method = "POST ";
        } else {
            method = "HEAD ";
            if (connection->type != 3) {
                method = "GET ";
            }
        }
        ghiAppendDataToBuffer(buffer, method, 0);
        if (connection->proxyAddress != NULL || data_ov114_021d3b2c != NULL) {
            ghiAppendDataToBuffer(buffer, connection->url, 0);
        } else {
            ghiAppendDataToBuffer(buffer, connection->requestPath, 0);
        }
        ghiAppendDataToBuffer(buffer, " HTTP/1.1\r\n", 0);

        if (connection->serverPort == 80) {
            ghiAppendHeaderToBuffer(buffer, "Host", connection->serverAddress);
        } else {
            ghiAppendDataToBuffer(buffer, "Host: ", 0);
            ghiAppendDataToBuffer(buffer, connection->serverAddress, 0);
            ghiAppendCharToBuffer(buffer, ':');
            ghiAppendIntToBuffer(buffer, connection->serverPort);
            ghiAppendDataToBuffer(buffer, "\r\n", 2);
        }

        if (connection->headers == NULL || strstr(connection->headers, "User-Agent") == NULL) {
            ghiAppendHeaderToBuffer(buffer, "User-Agent", "GameSpyHTTP/1.0");
        }
        if (connection->keepAlive) {
            ghiAppendHeaderToBuffer(buffer, "Connection", "Keep-Alive");
        } else {
            ghiAppendHeaderToBuffer(buffer, "Connection", "close");
        }

        if (connection->post != NULL && !connection->postingState.finished) {
            OS_SPrintf(length, "%d", connection->postingState.totalBytes);
            ghiAppendHeaderToBuffer(buffer, "Content-Length", length);
            ghiAppendHeaderToBuffer(buffer, "Content-Type", func_ov114_021c25c8(connection));
        }
        if (connection->headers != NULL) {
            ghiAppendDataToBuffer(buffer, connection->headers, 0);
        }
        ghiAppendDataToBuffer(buffer, "\r\n", 2);

        if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
            if (!ghiEncryptDataToBuffer(&connection->sendBuffer, buffer->data, buffer->length)) {
                connection->completed = TRUE;
                connection->result = 0x11;
                return;
            }
            ghiResetBuffer(buffer);
        }
    }

    if (ghiSendBufferedData(connection) && connection->sendBuffer.pos >= connection->sendBuffer.length) {
        ghiResetBuffer(&connection->sendBuffer);
        if (connection->post != NULL && !connection->postingState.finished) {
            connection->state = 6;
        } else {
            connection->state = 7;
        }
        ghiCallProgressCallback(connection, NULL, 0);
    }
}

void func_ov114_021c4684(s32 *arg0) {
    s32 result;
    s32 val;
    s32 sStack_18;
    gsDebugFormat(8, 3, 0x10, (s8 *)"Posting\n");
    val = arg0[0x5c];
    result = func_ov114_021c3834((GHIConnection *)arg0);
    if (result == 0) {
        sStack_18 = 0;
        func_ov114_021c2bb0((GHIConnection *)arg0);
        if (GSISocketSelect(arg0[0x14], &sStack_18, (s32 *)0x0, (s32 *)0x0) != 1) {
            return;
        }
        if (sStack_18 != 0) {
            arg0[4] = 8;
            func_ov114_021c02c8(arg0, 0, 0);
            return;
        }
        return;
    }
    if (result == 3) {
        arg0[0x60] = 0;
        return;
    }
    if (val != arg0[0x5c]) {
        func_ov114_021c0318(arg0);
    }
    if (result != 1) {
        return;
    }
    func_ov114_021c2bb0((GHIConnection *)arg0);
    arg0[0x61] = 1;
    arg0[4] = 7;
    func_ov114_021c02c8(arg0, 0, 0);
}

void func_ov114_021c4764(s32 *param_1) {
    s32 result;
    s32 aiStack_14 [1];
    s32 iStack_18;


    gsDebugFormat(8, 3, 0x10, (s8 *)"Waiting\n");
    result = GSISocketSelect(param_1[0x14], aiStack_14, (s32 *)0x0, &iStack_18);
    if ((result == -1) || ((result == 1 && (iStack_18 != 0)))) {
        param_1[0x49] = 1;
        param_1[0x10] = 5;
        if (result == -1) {
            result = GOAGetLastError(param_1[0x14]);
        }
        else {
            result = 0;
        }
        param_1[0x15] = result;
        return;
    }
    if (result != 1) {
        return;
    }
    if (aiStack_14[0] != 0) {
        param_1[4] = 8;
        func_ov114_021c02c8(param_1, 0, 0);
        return;
    }
}

BOOL func_ov114_021c4830(GHIConnection *connection) {
    int major;
    int minor;
    int code;
    int textEnd;
    char c;

    if (STD_TSScanf(connection->recvBuffer.data, "HTTP/%d.%d %d%n", &major, &minor, &code, &textEnd) != 3 ||
        major < 1 || code < 100 || code >= 600) {
        connection->completed = TRUE;
        connection->result = 7;
        return FALSE;
    }

    for (; (c = connection->recvBuffer.data[textEnd]) != '\0' && isspace(c); textEnd++) {
    }
    connection->httpMajor = major;
    connection->httpMinor = minor;
    connection->statusCode = code;
    connection->statusTextPos = textEnd;
    return TRUE;
}

void func_ov114_021c492c(GHIConnection *connection) {
    int size;
    char buffer[0x400];
    int result;
    char *lineEnd;
    int lineLength;

    gsDebugFormat(8, 3, 0x10, "Receiving Status\n");
    size = sizeof(buffer);
    result = ghiDoReceive(connection, buffer, &size);
    if (result == 3 || result == 1) {
        return;
    }
    if (result == 0) {
        if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
            if (!ghiAppendDataToBuffer(&connection->decodeBuffer, buffer, size)) {
                return;
            }
            if (!ghiDecryptReceivedData(connection)) {
                connection->completed = TRUE;
                connection->result = 0x11;
                return;
            }
        } else {
            if (!ghiAppendDataToBuffer(&connection->recvBuffer, buffer, size)) {
                return;
            }
        }
    }

    lineEnd = strstr(connection->recvBuffer.data, "\r\n");
    if (lineEnd != NULL) {
        lineEnd[0] = '\0';
        lineLength = lineEnd - connection->recvBuffer.data;
        if (!func_ov114_021c4830(connection)) {
            return;
        }
        connection->headersPos = lineLength + 2;
        if (connection->statusCode == 100 && connection->postingState.isXml != 0) {
            connection->postingState.isXml = FALSE;
            ghiResetBuffer(&connection->recvBuffer);
            connection->state = 6;
            ghiCallProgressCallback(connection, NULL, 0);
            gsDebugFormat(8, 0, 0x10, "Got HTTP continue\r\n");
            return;
        }
        connection->state = 9;
        ghiCallProgressCallback(connection, NULL, 0);
    } else if (result == 2) {
        connection->completed = TRUE;
        connection->result = 7;
        connection->socketError = GOAGetLastError(connection->socket);
    }
}

BOOL func_ov114_021c4b18(GHIConnection *connection, const char *data, int size) {
    const char *delivered = NULL;
    int deliveredSize = 0;

    connection->bytesReceived += size;
    if (connection->bytesReceived == connection->totalSize || connection->closing != 0) {
        connection->completed = TRUE;
    }
    if (connection->type == 0) {
        if (!ghiAppendDataToBuffer(&connection->fileBuffer, data, size)) {
            return FALSE;
        }
        delivered = connection->fileBuffer.data;
        deliveredSize = connection->fileBuffer.length;
    } else if (connection->type == 1) {
        if (size != 0) {
            connection->completed = TRUE;
            connection->result = 13;
            return FALSE;
        }
        delivered = data;
        deliveredSize = size;
    } else if (connection->type == 2) {
        delivered = data;
        deliveredSize = size;
    }
    ghiCallProgressCallback(connection, delivered, deliveredSize);
    return TRUE;
}

int func_ov114_021c4bd8(GHIConnection *connection) {
    int size;

    if (STD_TSScanf(connection->chunkHeader, "%x", &size) != 1) {
        return -1;
    }
    return size;
}

void func_ov114_021c4c00(GHIConnection *connection, const char *data, int size) {
    int count;

    if (size != 0 && connection->chunkHeaderLength < 10) {
        count = 10 - connection->chunkHeaderLength;
        if (count >= size) {
            count = size;
        }
        memcpy(connection->chunkHeader + connection->chunkHeaderLength, data, count);
        connection->chunkHeaderLength += count;
        connection->chunkHeader[connection->chunkHeaderLength] = '\0';
    }
}

BOOL func_ov114_021c4c50(GHIConnection *connection, const char *data, int size) {
    if (connection->chunked) {
        while (size > 0) {
            if (connection->chunkState == 0) {
                const char *lineEnd = strchr(data, '\n');
                int chunkSize;

                if (lineEnd != NULL) {
                    func_ov114_021c4c00(connection, data, lineEnd - data);
                    size -= lineEnd + 1 - data;
                    data = lineEnd + 1;
                    chunkSize = func_ov114_021c4bd8(connection);
                    connection->chunkRemaining = chunkSize;
                    if (chunkSize == -1) {
                        connection->completed = TRUE;
                        connection->result = 7;
                        return FALSE;
                    }
                    if (chunkSize == 0) {
                        connection->chunkState = 3;
                        gsDebugFormat(8, 0, 0x20, "Reading footer\n");
                    } else {
                        connection->chunkState = 1;
                        gsDebugFormat(8, 0, 0x20, "Reading %d byte chunk\n", chunkSize);
                    }
                } else {
                    func_ov114_021c4c00(connection, data, size);
                    return TRUE;
                }
            } else if (connection->chunkState == 1) {
                int count = connection->chunkRemaining;

                if (count >= size) {
                    count = size;
                }
                gsDebugFormat(8, 0, 0x20, "Read %d bytes of chunk\n", count);
                if (!func_ov114_021c4b18(connection, data, count)) {
                    return FALSE;
                }
                data += count;
                size -= count;
                connection->chunkRemaining -= count;
                if (connection->chunkRemaining == 0) {
                    connection->chunkState = 2;
                }
            } else if (connection->chunkState == 2) {
                const char *lineEnd = strchr(data, '\n');

                if (lineEnd == NULL) {
                    return TRUE;
                }
                size -= lineEnd + 1 - data;
                data = lineEnd + 1;
                connection->chunkHeader[0] = '\0';
                connection->chunkHeaderLength = 0;
                connection->chunkRemaining = 0;
                connection->chunkState = 0;
                gsDebugFormat(8, 0, 0x20, "Read chunk footer\n");
            } else if (connection->chunkState == 3) {
                connection->completed = TRUE;
                gsDebugFormat(8, 0, 0x20, "Finished reading chunks\n");
                return TRUE;
            } else {
                return FALSE;
            }
        }
        return TRUE;
    }
    return func_ov114_021c4b18(connection, data, size);
}

#pragma thumb off
void func_ov114_021c4e68(void *pointer) {
    if (pointer != NULL) {
        gsifree(pointer);
    }
}

void func_ov114_021c4e7c(GHIConnection *connection) {
    int size;
    int headersPos;
    int result;
    char *buffer;
    char *body;
    int bodyLength;
    char *contentLength;
    char *data;
    char *headersEnd;
    char *headers;
    BOOL skipCR = TRUE;

    buffer = gsimalloc(0x1000);
    if (buffer == NULL) {
        connection->completed = TRUE;
        connection->result = 1;
        return;
    }
    size = 0x1000;
    result = ghiDoReceive(connection, buffer, &size);
    if (result == 3) {
        func_ov114_021c4e68(buffer);
        return;
    }
    if (result == 1) {
        func_ov114_021c4e68(buffer);
        return;
    }
    if (result == 0) {
        if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
            if (!ghiAppendDataToBuffer(&connection->decodeBuffer, buffer, size)) {
                func_ov114_021c4e68(buffer);
                return;
            }
            if (!ghiDecryptReceivedData(connection)) {
                connection->completed = TRUE;
                connection->result = 0x11;
                func_ov114_021c4e68(buffer);
                return;
            }
        } else {
            if (!ghiAppendDataToBuffer(&connection->recvBuffer, buffer, size)) {
                func_ov114_021c4e68(buffer);
                return;
            }
        }
    }

    data = connection->recvBuffer.data;
    headersPos = connection->headersPos;
    headers = data + headersPos;
    if (connection->statusCode / 100 == 1 &&
        (strncmp(headers, "\r\n", 2) == 0 || strncmp(headers, "\n\n", 2) == 0)) {
        skipCR = FALSE;
        headersEnd = headers;
    } else {
        headersEnd = strstr(headers, "\r\n\r\n");
    }
    if (headersEnd == NULL) {
        headersEnd = strstr(headers, "\n\n");
    }
    if (headersEnd != NULL) {
        if (skipCR == TRUE) {
            headersEnd += 2;
        }
        headersEnd[0] = '\0';
        body = headersEnd + 2;
        bodyLength = connection->recvBuffer.length - (body - connection->recvBuffer.data);
        connection->recvBuffer.length = headersEnd - connection->recvBuffer.data + 1;
        connection->recvBuffer.pos = connection->recvBuffer.length;

        if (connection->statusCode / 100 == 1) {
            if (bodyLength != 0) {
                memmove(connection->recvBuffer.data, body, bodyLength + 1);
                connection->recvBuffer.length = bodyLength;
            } else {
                ghiResetBuffer(&connection->recvBuffer);
            }
            if (connection->postingState.isXml != 0) {
                connection->postingState.isXml = FALSE;
                connection->state = 6;
                ghiCallProgressCallback(connection, NULL, 0);
            }
            connection->state = 8;
            ghiCallProgressCallback(connection, NULL, 0);
            func_ov114_021c4e68(buffer);
            return;
        }

        if (connection->statusCode / 100 == 3) {
            char *location;

            if (connection->redirectCount > 10) {
                connection->completed = TRUE;
                connection->result = 11;
                func_ov114_021c4e68(buffer);
                return;
            }
            location = strstr(headers, "Location:");
            if (location != NULL) {
                char *end;

                location += 9;
                while (isspace(location[0])) {
                    location++;
                }
                end = location;
                while (end[0] != '\0' && !isspace(end[0])) {
                    end++;
                }
                end[0] = '\0';
                if (location[0] == '/') {
                    connection->redirectUrl = gsimalloc(strlen(connection->serverAddress) + 14 + strlen(location));
                    if (connection->redirectUrl == NULL) {
                        connection->completed = TRUE;
                        connection->result = 1;
                    }
                    OS_SPrintf(connection->redirectUrl, "http://%s:%d%s", connection->serverAddress,
                               connection->serverPort, location);
                } else {
                    connection->redirectUrl = func_ov009_0217f1e0(location);
                    if (connection->redirectUrl == NULL) {
                        connection->completed = TRUE;
                        connection->result = 1;
                    }
                }
                func_ov114_021c4e68(buffer);
                return;
            }
        }

        contentLength = strstr(headers, "Content-Length:");
        if (contentLength != NULL) {
            char maxValue[11] = "2147483647";
            char *digits = contentLength + 16;
            char *digitsEnd = digits;
            int maxLength = strlen(maxValue);
            int digitsLength;

            for (; digitsEnd != NULL && digitsEnd[0] != '\0' && digitsEnd[0] != '\n' && digitsEnd[0] != '\r' &&
                   digitsEnd[0] != ' ';
                 digitsEnd++) {
            }
            digitsLength = digitsEnd - digits;
            if (digitsLength > maxLength) {
                connection->completed = TRUE;
                connection->result = 16;
                func_ov114_021c4e68(buffer);
                return;
            }
            if (maxLength == digitsLength && strncmp(digits, maxValue, digitsLength) >= 0) {
                connection->completed = TRUE;
                connection->result = 16;
                func_ov114_021c4e68(buffer);
                return;
            }
            connection->totalSize = atol(digits);
        }

        connection->chunked = strstr(headers, "Transfer-Encoding: chunked") != NULL ? TRUE : FALSE;
        if (connection->chunked) {
            connection->chunkHeader[0] = '\0';
            connection->chunkHeaderLength = 0;
            connection->chunkRemaining = 0;
            connection->chunkState = 0;
        }
        if (connection->type == 3 || connection->type == 4) {
            connection->completed = TRUE;
            func_ov114_021c4e68(buffer);
            return;
        }
        connection->state = 10;
        if (contentLength != NULL && connection->totalSize == 0) {
            connection->completed = TRUE;
            func_ov114_021c4e68(buffer);
            return;
        }
        if (bodyLength > 0) {
            func_ov114_021c4c50(connection, body, bodyLength);
        }
    } else if (result == 2) {
        connection->completed = TRUE;
        connection->result = 7;
        connection->socketError = GOAGetLastError(connection->socket);
    }
    func_ov114_021c4e68(buffer);
}

void func_ov114_021c5430(GHIConnection *connection) {
    gsi_time startTime = current_time();
    int size = 0;
    gsi_time elapsed = 0;
    char *buffer;
    int result;

    buffer = gsimalloc(0x2000);
    if (buffer == NULL) {
        connection->completed = TRUE;
        connection->result = 1;
        return;
    }
    gsDebugFormat(8, 3, 0x10, "Receiving File\n");

    while (!connection->completed && elapsed < connection->maxRecvTime) {
        size = 0x2000;
        result = ghiDoReceive(connection, buffer, &size);
        if (result == 1) {
            func_ov114_021c4e68(buffer);
            if (connection->lastRecvTime == 0) {
                connection->lastRecvTime = current_time();
                return;
            }
            if (current_time() - connection->lastRecvTime < 30000) {
                return;
            }
            gsDebugFormat(8, 0, 1, "Receive File Idle Timeout %d ===== \r\n", current_time() - connection->lastRecvTime);
            connection->lastRecvTime = 0;
            connection->completed = TRUE;
            connection->result = 19;
            return;
        }
        connection->lastRecvTime = 0;
        if (result == 3) {
            func_ov114_021c4e68(buffer);
            return;
        }
        if (result == 2) {
            connection->completed = TRUE;
            if (connection->totalSize > 0 && connection->bytesReceived < connection->totalSize) {
                connection->result = 15;
            }
            func_ov114_021c4e68(buffer);
            return;
        }

        if (connection->encryptor.engine != 0 && connection->encryptor.mode == 1) {
            int received;

            if (!ghiAppendDataToBuffer(&connection->decodeBuffer, buffer, size)) {
                func_ov114_021c4e68(buffer);
                return;
            }
            connection->recvBuffer.length = connection->recvBuffer.pos;
            if (!ghiDecryptReceivedData(connection)) {
                connection->completed = TRUE;
                connection->result = 0x11;
                func_ov114_021c4e68(buffer);
                return;
            }
            received = connection->recvBuffer.length - connection->recvBuffer.pos;
            if (received != 0) {
                if (!func_ov114_021c4c50(connection, connection->recvBuffer.data + connection->recvBuffer.pos, received)) {
                    func_ov114_021c4e68(buffer);
                    return;
                }
            }
        } else {
            if (!func_ov114_021c4c50(connection, buffer, size)) {
                func_ov114_021c4e68(buffer);
                return;
            }
        }
        elapsed = current_time() - startTime;
    }
    func_ov114_021c4e68(buffer);
}
