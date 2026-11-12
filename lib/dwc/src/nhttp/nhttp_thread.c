// Black's code at ov114 0x021bbe94-0x021be648. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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
#include "dwc/nhttp_internal.h"

static const char data_ov114_021cdfe0[] = "Content-Disposition: form-data; name=\"";
static const char data_ov114_021ce007[] = "Content-Type: multipart/form-data; boundary=";
static const char data_ov114_021ce034[] = "Content-Type: application/x-www-form-urlencoded\r\n";
static const char data_ov114_021ce068[] = "Content-Type: application/octet-stream\r\nContent-Transfer-Encoding: binary\r\n";
#pragma thumb off
void func_ov114_021bbe94(NHttpThreadInfo *info) {
    func_ov114_021bbeac(info);
}

void func_ov114_021bbea0(NHttpThreadInfo *info) {
    info->queueCreated = TRUE;
}

void func_ov114_021bbeac(NHttpThreadInfo *info) {
    info->queueCreated = FALSE;
}

BOOL func_ov114_021bbeb8(NHttpThreadInfo *info) {
    return info->queueCreated;
}

// Whether the last four bytes that were read (kept in a ring of four) end the header: an empty line, as CR CR, LF LF or
// CR LF CR LF
BOOL func_ov114_021bbec0(const char *tail, int count) {
    char second = tail[(count - 2) & 3];

    if (second == '\r' && tail[(count - 1) & 3] == '\r') {
        return TRUE;
    }
    if (second == '\n' && tail[(count - 1) & 3] == '\n') {
        return TRUE;
    }
    if (tail[(count - 4) & 3] == '\r' && tail[(count - 3) & 3] == '\n' && second == '\r' &&
        tail[(count - 1) & 3] == '\n') {
        return TRUE;
    }
    return FALSE;
}

int func_ov114_021bbf44(NHttpRequest *request, u8 *sendBuffer, int sock, int *count, const u8 *data, u32 size) {
    int remaining;
    int chunk;
    int result;

    remaining = size;
    while (remaining > 0) {
        if (request->canceled != 0) {
            return -1;
        }
        chunk = NHTTP_SEND_BUFFER_SIZE - *count;
        if (remaining <= chunk) {
            chunk = remaining;
        }
        func_ov114_021ba100(sendBuffer + *count, data, chunk);
        *count += chunk;
        data += chunk;
        remaining -= chunk;
        if (*count == NHTTP_SEND_BUFFER_SIZE) {
            result = func_ov114_021ba020(request, sock, sendBuffer, NHTTP_SEND_BUFFER_SIZE, 0);
            if (result <= 0) {
                return result;
            }
            *count -= result;
        }
    }
    remaining = size;
    return remaining;
}

// Reads one byte at a time up to the end of a line, which is CR LF. Returns the number of bytes, or what the socket
// returned if that was not more than 0
int func_ov114_021bc00c(NHttpMutexInfo *mutexInfo, NHttpRequest *request, int sock) {
    char tail[2];
    int count;
    int total = 0;

    tail[0] = 0;
    tail[1] = 0;
    count = 0;
    while (!(tail[count & 1] == '\r' && tail[(count - 1) & 1] == '\n')) {
        int result = func_ov114_021b9f94(mutexInfo, request, sock, &tail[count & 1], 1, 0);

        if (result <= 0) {
            return result;
        }
        total += result;
        count++;
    }
    return total;
}

// Adds the size of what the connection's callback gives for a part of the post data, as it will be sent (a part sent
// as text is longer when it is encoded). Returns FALSE if the callback can't be called
BOOL func_ov114_021bc098(NHttpMutexInfo *mutexInfo, NHttpRequest *request, const char *name, int *length,
                                        int mode) {
    int total = 0;
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);

    if (connection == NULL) {
        return FALSE;
    }
    connection->userBuffer = NULL;
    for (;;) {
        int size;
        const char *buffer;

        if (request->canceled != 0) {
            return FALSE;
        }
        connection->userBufferSize = 0;
        if (func_ov114_021bec74(mutexInfo, connection, name, total) < 0) {
            return FALSE;
        }
        size = connection->userBufferSize;
        buffer = connection->userBuffer;
        if (size == 0) {
            break;
        }
        if (buffer == NULL) {
            return FALSE;
        }
        total += size;
        switch (mode) {
        case NHTTP_SEND_RAW:
        case NHTTP_SEND_PART:
            *length += size;
            break;
        case NHTTP_SEND_ENCODED:
            *length += func_ov114_021ba25c(buffer, size);
            break;
        }
    }
    return TRUE;
}

// Sends what the connection's callback gives for a part of the post data. Returns 0 if it all went into the send
// buffer, 1 if the socket failed, 2 if the server closed it and 3 if the connection is gone or the request canceled
int func_ov114_021bc160(NHttpMutexInfo *mutexInfo, NHttpRequest *request, u8 *sendBuffer, const char *name, int sock,
                               int *count, int mode) {
    char encoded[3];
    u32 total = 0;
    NHttpConnection *connection;

    connection = func_ov114_021bebe0(mutexInfo, request);
    if (connection == NULL) {
        return 3;
    }
    connection->userBuffer = NULL;
    for (;;) {
        u32 size;
        const char *buffer;
        int result;

        if (request->canceled != 0) {
            return 3;
        }
        connection->userBufferSize = 0;
        if (func_ov114_021bec74(mutexInfo, connection, name, total) < 0) {
            return 3;
        }
        buffer = connection->userBuffer;
        size = connection->userBufferSize;
        if (size == 0) {
            break;
        }
        if (buffer == NULL) {
            return 3;
        }
        total += size;
        switch (mode) {
        case NHTTP_SEND_RAW:
        case NHTTP_SEND_PART:
            result = func_ov114_021bbf44(request, sendBuffer, sock, count, buffer, size);
            if (result < 0) {
                return 1;
            }
            if (result == 0) {
                return 2;
            }
            break;
        case NHTTP_SEND_ENCODED: {
            u32 i;

            for (i = 0; i < size; i++) {
                func_ov114_021ba13c(encoded, 3);
                result = func_ov114_021bbf44(request, sendBuffer, sock, count, encoded, NHTTPi_encodeUrlChar(encoded, buffer[i]));
                if (result < 0) {
                    return 1;
                }
                if (result == 0) {
                    return 2;
                }
            }
            break;
        }
        }
    }
    return 0;
}

// Whether the response has room for what is read, asking the connection's callback for a buffer when it has none or it
// is full
BOOL func_ov114_021bc2fc(NHttpMutexInfo *mutexInfo, NHttpResponse *response) {
    BOOL ready = FALSE;
    BOOL full = func_ov114_021bb2fc(response, response->bufferUsed);

    if (response->bufferSize == 0 || response->buffer == NULL || full) {
        NHttpConnection *connection = func_ov114_021bebf0(mutexInfo, response);

        if (connection != NULL) {
            func_ov114_021bed04(mutexInfo, connection);
            if (response->buffer != NULL && response->bufferSize != 0) {
                if (!func_ov114_021bb2fc(response, response->bufferUsed)) {
                    ready = TRUE;
                }
            }
        }
    } else if (!full) {
        ready = TRUE;
    }
    return ready;
}

// Puts data in the send buffer for the request that runs. Returns 0, or 1 if the socket failed and 2 if the server
// closed it
int func_ov114_021bc384(NHttpThreadWork *work, const void *data, int size) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpThreadInfo *thread = func_ov114_021bef50(root);
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    u8 *sendBuffer = thread->sendBuffer;
    int result;

    if (size == 0) {
        return 0;
    }
    result = func_ov114_021bbf44(request, sendBuffer, state->socket, &work->sendCount, data, size);
    if (result < 0) {
        return 1;
    }
    if (result == 0) {
        return 2;
    }
    return 0;
}

// Sends the Proxy-Authorization header field if the request has the user and the password of the proxy
int func_ov114_021bc40c(NHttpThreadWork *work) {
    NHttpRequest *request = func_ov114_021bef48(func_ov114_021bef08())->current->request;
    int result;

    if (request->proxyAuthorizationSize == 0) {
        return 0;
    }
    result = func_ov114_021bc384(work, "Proxy-Authorization: Basic ", 27);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->proxyAuthorization, request->proxyAuthorizationSize);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    return 0;
}

int func_ov114_021bc488(NHttpThreadWork *work) {
    NHttpRequest *request = func_ov114_021bef48(func_ov114_021bef08())->current->request;
    int result;

    if (request->authorizationSize == 0) {
        return 0;
    }
    result = func_ov114_021bc384(work, "Authorization: Basic ", 21);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->authorization, request->authorizationSize);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    return 0;
}

// Sends the request that asks the proxy to connect to the server (CONNECT), for a secure connection
int func_ov114_021bc504(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    NHttpState *state = func_ov114_021bef3c(root);
    char port[8];
    int portLength = func_ov114_021ba4e4(port, request->port);
    int result;

    result = func_ov114_021bc384(work, "CONNECT ", 8);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->url + 8, request->hostEnd - 8);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, ":", 1);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, port, portLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, " HTTP/1.1\r\n", 11);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "Host: ", 6);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->url + 8, request->hostEnd - 8);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, ":", 1);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, port, portLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "Content-Length: 0\r\nPragma: no-cache\r\n", 37);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc40c(work);
    if (result != 0) {
        return result;
    }
    // The result of the empty line that ends the header is not checked
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (work->sendCount > 0) {
        result = func_ov114_021ba020(request, state->socket, sendBuffer, work->sendCount, 0);
        if (result < 0) {
            return 1;
        }
        if (result == 0) {
            return 2;
        }
    }
    work->sendCount = 0;
    func_ov114_021ba13c(sendBuffer, NHTTP_SEND_BUFFER_SIZE);
    return 0;
}

// Reads the proxy's answer to CONNECT. TRUE if the status is 200
BOOL func_ov114_021bc740(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = reqInfo->current->request;
    NHttpResponse *response = request->response;
    int count;
    BOOL ok = FALSE;
    int received = 0;
    char buffer[0x200];

    for (;;) {
        int i;
        BOOL found;

        count = func_ov114_021b9f94(mutexInfo, request, state->socket, buffer + received, 0x200 - received, 0);
        received += count;
        response->statusCode = NHTTPi_strToInt(buffer + 9, 3);
        if (func_ov114_021ba150(buffer, "HTTP/", 5) == 0 && buffer[8] == ' ' && response->statusCode == 200) {
            ok = TRUE;
        }
        for (i = 0, found = FALSE; i < received; i++) {
            if (i > 1 && buffer[i - 1] == '\r' && buffer[i] == '\r') {
                found = TRUE;
            } else if (i > 1 && buffer[i - 1] == '\n' && buffer[i] == '\n') {
                found = TRUE;
            } else if (i > 3 && buffer[i - 3] == '\r' && buffer[i - 2] == '\n' && buffer[i - 1] == '\r' &&
                       buffer[i] == '\n') {
                found = TRUE;
            }
        }
        if (found) {
            return ok ? TRUE : FALSE;
        }
        if (count < 0) {
            return FALSE;
        }
        if (received >= 0x200) {
            count = func_ov114_021b9f94(mutexInfo, request, state->socket, sendBuffer, 1, 0);
            if (count < 0) {
                return FALSE;
            }
            if (count != 0) {
                return FALSE;
            }
        }
    }
}

// Sends the header fields that the request was given, taking each off the list
int func_ov114_021bc924(NHttpThreadWork *work) {
    NHttpRequest *request = func_ov114_021bef48(func_ov114_021bef08())->current->request;
    NHttpHeader *header = NHTTPi_getHdrFromList(&request->headers);
    int result;

    if (header != NULL) {
        do {
            result = func_ov114_021bc384(work, header->name, func_ov114_021ba118(header->name));
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, ": ", 2);
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, header->value, func_ov114_021ba118(header->value));
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, "\r\n", 2);
            if (result != 0) {
                return result;
            }
            func_ov114_021ba850(header);
            header = NHTTPi_getHdrFromList(&request->headers);
        } while (header != NULL);
    }
    return 0;
}

// Sends a post that is one block of data (the body of the request), with its Content-Length
int func_ov114_021bc9f0(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    NHttpState *state = func_ov114_021bef3c(root);
    int length = 0;
    char text[12];
    int textLength;
    int result;

    if (request->rawData == NULL) {
        if (!func_ov114_021bc098(mutexInfo, request, NULL, &length, NHTTP_SEND_RAW)) {
            return 3;
        }
    } else {
        length = request->rawSize;
    }
    textLength = func_ov114_021ba4e4(text, length);
    result = func_ov114_021bc384(work, "Content-Length: ", 16);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, text, textLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    if (request->rawData == NULL) {
        result =
            func_ov114_021bc160(mutexInfo, request, sendBuffer, NULL, state->socket, &work->sendCount, NHTTP_SEND_RAW);
        if (result != 0) {
            return result;
        }
    } else {
        result = func_ov114_021bc384(work, request->rawData, request->rawSize);
        if (result != 0) {
            return result;
        }
    }
    return 0;
}

// Sends a post as multipart/form-data: each part goes between boundaries with its name, its content type if it is
// binary and its data
int func_ov114_021bcb74(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    NHttpThreadInfo *thread = func_ov114_021bef50(root);
    NHttpState *state = func_ov114_021bef3c(root);
    int length = 0;
    NHttpHeader *part;
    char text[12];
    int textLength;
    int result;

    part = request->postData;
    if (part != NULL) {
        do {
            length += NHTTP_BOUNDARY_SIZE + 2;
            length += func_ov114_021ba118(part->name) + 41;
            if (part->binary) {
                length += 75;
            }
            length += 2;
            if (part->value == NULL) {
                if (!func_ov114_021bc098(mutexInfo, request, part->name, &length, NHTTP_SEND_PART)) {
                    return 3;
                }
            } else {
                length += part->length;
            }
            length += 2;
            if (part == request->postData->prev) {
                break;
            }
            part = part->next;
        } while (part != NULL);
    }
    length += NHTTP_BOUNDARY_SIZE + 4;
    textLength = func_ov114_021ba4e4(text, length);
    result = func_ov114_021bc384(work, data_ov114_021ce007, sizeof(data_ov114_021ce007) - 1);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->boundary + 2, NHTTP_BOUNDARY_SIZE - 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "Content-Length: ", 16);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, text, textLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    part = request->postData;
    if (part != NULL) {
        do {
            result = func_ov114_021bc384(work, request->boundary, NHTTP_BOUNDARY_SIZE);
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, "\r\n", 2);
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, data_ov114_021cdfe0, sizeof(data_ov114_021cdfe0) - 1);
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, part->name, func_ov114_021ba118(part->name));
            if (result != 0) {
                return result;
            }
            result = func_ov114_021bc384(work, "\"\r\n", 3);
            if (result != 0) {
                return result;
            }
            if (part->binary) {
                result = func_ov114_021bc384(work, data_ov114_021ce068, sizeof(data_ov114_021ce068) - 1);
                if (result != 0) {
                    return result;
                }
            }
            result = func_ov114_021bc384(work, "\r\n", 2);
            if (result != 0) {
                return result;
            }
            if (part->value == NULL) {
                result = func_ov114_021bc160(mutexInfo, request, thread->sendBuffer, part->name, state->socket,
                                             &work->sendCount, NHTTP_SEND_PART);
                if (result != 0) {
                    return result;
                }
            } else {
                result = func_ov114_021bc384(work, part->value, part->length);
                if (result != 0) {
                    return result;
                }
            }
            result = func_ov114_021bc384(work, "\r\n", 2);
            if (result != 0) {
                return result;
            }
            if (part == request->postData->prev) {
                break;
            }
            part = part->next;
        } while (part != NULL);
    }
    result = func_ov114_021bc384(work, request->boundary, NHTTP_BOUNDARY_SIZE);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "--\r\n", 4);
    if (result != 0) {
        return result;
    }
    return 0;
}

// Sends a post as application/x-www-form-urlencoded: name=value pairs with & between them, URL encoded
int func_ov114_021bcf24(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    NHttpState *state = func_ov114_021bef3c(root);
    int length = 0;
    NHttpHeader *part;
    char text[12];
    int textLength;
    int result;

    part = request->postData;
    if (part != NULL) {
        do {
            length = length + NHTTPi_getUrlEncodedSize(part->name) + 1;
            if (part->value == NULL) {
                if (!func_ov114_021bc098(mutexInfo, request, part->name, &length, NHTTP_SEND_ENCODED)) {
                    return 3;
                }
            } else {
                length += NHTTPi_getUrlEncodedSize(part->value);
            }
            if (part == request->postData->prev) {
                break;
            }
            length += 1;
            part = part->next;
        } while (part != NULL);
    }
    textLength = func_ov114_021ba4e4(text, length);
    result = func_ov114_021bc384(work, data_ov114_021ce034, sizeof(data_ov114_021ce034) - 1);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "Content-Length: ", 16);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, text, textLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    part = request->postData;
    if (part != NULL) {
        do {
            int i;

            for (i = 0; part->name[i] != 0; i++) {
                result = func_ov114_021bc384(work, text, NHTTPi_encodeUrlChar(text, part->name[i]));
                if (result != 0) {
                    return result;
                }
            }
            result = func_ov114_021bc384(work, "=", 1);
            if (result != 0) {
                return result;
            }
            if (part->value == NULL) {
                result = func_ov114_021bc160(mutexInfo, request, sendBuffer, part->name, state->socket,
                                             &work->sendCount, NHTTP_SEND_ENCODED);
                if (result != 0) {
                    return result;
                }
            } else {
                for (i = 0; part->value[i] != 0; i++) {
                    result = func_ov114_021bc384(work, text, NHTTPi_encodeUrlChar(text, part->value[i]));
                    if (result != 0) {
                        return result;
                    }
                }
            }
            if (part == request->postData->prev) {
                break;
            }
            result = func_ov114_021bc384(work, "&", 1);
            if (result != 0) {
                return result;
            }
            part = part->next;
        } while (part != NULL);
    }
    return 0;
}

void func_ov114_021bd220(NHttpThreadWork *work) {
    work->id = -1;
    func_ov114_021ba13c(work->hostName, sizeof(work->hostName));
    func_ov114_021ba13c(work->unk_104, sizeof(work->unk_104));
    work->address = -1;
    work->prevAddress = -1;
    work->sendCount = 0;
    work->keepAlive = FALSE;
    work->chunked = FALSE;
    work->retry = 0;
    work->contentLength = 0;
    work->error = 0;
}

// Ends the request that runs: closes its socket unless the connection is kept, records how it ended, destroys the
// request and tells the user's callback
void func_ov114_021bd270(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = reqInfo->current->request;
    NHttpResponse *response = request->response;
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);

    if (request->canceled != 0) {
        work->error = 8;
        work->keepAlive = FALSE;
    }
    if (work->keepAlive == FALSE) {
        if (state->socket >= 0) {
            if (func_ov114_021b9cfc(mutexInfo, request, state->socket) < 0) {
                work->error = 10;
            }
            state->socket = -1;
        }
    }
    if (work->error == 0) {
        response->succeeded = TRUE;
    } else {
        response->succeeded = FALSE;
        func_ov114_021ba874(state, work->error);
        if (response->buffer == work->unk_104) {
            response->buffer = NULL;
            response->bufferSize = 0;
        }
    }
    if (connection != NULL) {
        connection->status = work->error;
    }
    func_ov114_021befcc(mutexInfo);
    func_ov114_021ba850(reqInfo->current);
    reqInfo->current = NULL;
    func_ov114_021befd8(mutexInfo);
    func_ov114_021bb934(mutexInfo, request);
    if (connection != NULL && response->succeeded) {
        connection->stage = NHTTP_STAGE_DONE;
    }
    func_ov114_021bee1c(mutexInfo, connection);
    if (connection != NULL) {
        func_ov114_021bea98(connection);
    }
    func_ov114_021bf1b0(connection);
}

// Takes the request that was sent first. FALSE (after waiting for a message) when there is none
BOOL func_ov114_021bd3b8(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpQueueNode *node;

    func_ov114_021befcc(mutexInfo);
    node = func_ov114_021baec8(func_ov114_021bef40(root));
    if (node != NULL) {
        NHttpReqInfo *reqInfo = func_ov114_021bef48(root);

        work->id = node->id;
        reqInfo->current = node;
    } else {
        work->id = -1;
    }
    func_ov114_021befd8(mutexInfo);
    if (work->id < 0) {
        func_ov114_021b9bf4(&func_ov114_021bef50(root)->queue);
        return FALSE;
    }
    return TRUE;
}

// Looks the host of the request up, or its proxy, unless it is the host of the last request
BOOL func_ov114_021bd430(NHttpThreadWork *work) {
    NHttpRequest *request = func_ov114_021bef48(func_ov114_021bef08())->current->request;
    char *host = request->host;

    if (request->useProxy) {
        host = request->proxyHost;
    }
    if (func_ov114_021ba118(host) == 0 || func_ov114_021ba130(host, work->hostName) != 0) {
        work->address = func_ov114_021ba098(request, host);
        if (work->address == 0) {
            if (request->useProxy) {
                work->error = 12;
                return FALSE;
            }
            work->error = 4;
            return FALSE;
        }
    } else {
        work->address = work->prevAddress;
    }
    func_ov114_021ba13c(work->hostName, sizeof(work->hostName));
    func_ov114_021ba100(work->hostName, host, func_ov114_021ba124(host, 0xff));
    work->port = request->port;
    if (request->useProxy) {
        work->port = request->proxyPort;
    }
    if (work->address != work->prevAddress || work->port != work->prevPort || request->https == 1) {
        work->keepAlive = FALSE;
    }
    work->prevAddress = work->address;
    work->prevPort = work->port;
    return TRUE;
}

// Opens the socket and connects it to the host or the proxy, unless the connection of the last request is kept
BOOL func_ov114_021bd548(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpRequest *request = reqInfo->current->request;
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);

    if (work->keepAlive == TRUE) {
        if (func_ov114_021b9ca4(state->socket) == -1) {
            work->keepAlive = FALSE;
        }
    }
    if (work->keepAlive == FALSE) {
        if (state->socket >= 0) {
            if (func_ov114_021b9cfc(mutexInfo, request, state->socket) < 0) {
                state->socket = -1;
                work->error = 10;
                return FALSE;
            }
        }
        state->socket = func_ov114_021b9ce4(request);
        if (state->socket < 0) {
            work->error = 3;
            return FALSE;
        }
        func_ov114_021befcc(mutexInfo);
        reqInfo->current->socket = state->socket;
        func_ov114_021befd8(mutexInfo);
        if (request->canceled != 0) {
            return FALSE;
        }
        if (func_ov114_021b9d8c(state, mutexInfo, request, state->socket, work->address, work->port) < 0) {
            if (request->useProxy) {
                work->error = 13;
                return FALSE;
            }
            if (func_ov114_021ba884(state) != 0) {
                work->error = 14;
                return FALSE;
            }
            work->error = 5;
            return FALSE;
        }
    } else {
        func_ov114_021befcc(mutexInfo);
        reqInfo->current->socket = state->socket;
        func_ov114_021befd8(mutexInfo);
    }
    return TRUE;
}

// For a secure connection through a proxy: asks the proxy to connect and makes the connection secure. 0 to go on, 1
// when it failed, 2 when the proxy closed the connection
int func_ov114_021bd6dc(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpRequest *request = reqInfo->current->request;
    int result;

    work->error = 10;
    work->sendCount = 0;
    if (request->https && request->useProxy) {
        result = func_ov114_021bc504(work);
        if (result != 0) {
            return result;
        }
        if (!func_ov114_021bc740(work)) {
            return 1;
        }
        result = func_ov114_021b9e3c(state, mutexInfo, request, state->socket);
        if (result != 0) {
            if (result == -1004) {
                if (func_ov114_021ba884(state) != 0) {
                    work->error = 16;
                }
                return 1;
            }
            if (result == -1005) {
                if (func_ov114_021ba884(state) != 0) {
                    work->error = 17;
                }
                return 1;
            }
            if (func_ov114_021ba884(state) != 0) {
                work->error = 14;
            }
            return 1;
        }
    }
    return 0;
}

// Sends the request line, the header fields and the post data. 0 when it is all sent, 1 or 3 when it failed and 2
// when the server closed the connection
int func_ov114_021bd7e8(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    int urlLength = func_ov114_021ba118(request->url);
    int result = 0;
    int schemeLength;

    work->error = 10;
    if (connection != NULL) {
        connection->stage = NHTTP_STAGE_SENDING;
    }
    work->sendCount = 0;
    switch (request->method) {
    case NHTTP_METHOD_GET:
        result = func_ov114_021bc384(work, "GET ", 4);
        break;
    case NHTTP_METHOD_POST:
        result = func_ov114_021bc384(work, "POST ", 5);
        break;
    case NHTTP_METHOD_HEAD:
        result = func_ov114_021bc384(work, "HEAD ", 5);
        break;
    }
    if (result != 0) {
        return result;
    }
    if (request->useProxy && !request->https) {
        result = func_ov114_021bc384(work, request->url, urlLength);
        if (result != 0) {
            return result;
        }
    } else if (urlLength > request->pathStart) {
        result = func_ov114_021bc384(work, request->url + request->pathStart, urlLength - request->pathStart);
        if (result != 0) {
            return result;
        }
    } else {
        result = func_ov114_021bc384(work, "/", 1);
        if (result != 0) {
            return result;
        }
    }
    result = func_ov114_021bc384(work, " HTTP/1.1\r\n", 11);
    if (result != 0) {
        return result;
    }
    schemeLength = request->https ? 8 : 7;
    result = func_ov114_021bc384(work, "Host: ", 6);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, request->url + schemeLength, request->hostEnd - schemeLength);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc384(work, "\r\n", 2);
    if (result != 0) {
        return result;
    }
    if (request->useProxy && !request->https) {
        result = func_ov114_021bc40c(work);
        if (result != 0) {
            return result;
        }
    }
    result = func_ov114_021bc488(work);
    if (result != 0) {
        return result;
    }
    result = func_ov114_021bc924(work);
    if (result != 0) {
        return result;
    }
    if (request->method == NHTTP_METHOD_POST) {
        BOOL multipart;

        if (request->rawPost) {
            result = func_ov114_021bc9f0(work);
        } else {
            if (request->postMode == NHTTP_POST_AUTO) {
                NHttpHeader *part = request->postData;

                multipart = FALSE;
                if (part != NULL) {
                    do {
                        if (part->binary) {
                            multipart = TRUE;
                            break;
                        }
                        if (part == request->postData->prev) {
                            break;
                        }
                        part = part->next;
                    } while (part != NULL);
                }
            } else if (request->postMode == NHTTP_POST_MULTIPART) {
                multipart = TRUE;
            } else {
                multipart = FALSE;
            }
            if (multipart) {
                result = func_ov114_021bcb74(work);
            } else {
                result = func_ov114_021bcf24(work);
            }
        }
        if (result != 0) {
            if (result == 3) {
                work->error = 3;
                return result;
            }
            return result;
        }
    } else {
        result = func_ov114_021bc384(work, "\r\n", 2);
        if (result != 0) {
            return result;
        }
    }
    result = 0;
    if (work->sendCount > 0) {
        int sent = func_ov114_021ba020(request, state->socket, sendBuffer, work->sendCount, 0);

        work->sendCount = 0;
        func_ov114_021ba13c(sendBuffer, NHTTP_SEND_BUFFER_SIZE);
        if (sent < 0) {
            result = 1;
        }
        if (sent == 0) {
            result = 2;
        }
    }
    work->sendCount = 0;
    func_ov114_021ba13c(sendBuffer, NHTTP_SEND_BUFFER_SIZE);
    return result;
}

// Reads the header of the answer a byte at a time, up to its end, into the response (its first part and then blocks)
BOOL func_ov114_021bdb68(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    NHttpResponse *response = request->response;
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);
    NHttpState *state = func_ov114_021bef3c(root);
    char tail[4] = { 0, 0, 0, 0 };
    NHttpHeaderBlock *block;

    if (connection != NULL) {
        connection->stage = NHTTP_STAGE_RECV_HEADER;
    }
    response->headerSize = 0;
    func_ov114_021ba13c(work->statusLine, sizeof(work->statusLine));
    block = response->blocks;
    work->recvCount = 0;
    for (;;) {
        int count;

        if (request->canceled != 0) {
            return FALSE;
        }
        if (work->recvCount < NHTTP_HEADER_SIZE) {
            count = func_ov114_021b9f94(mutexInfo, request, state->socket, response->header + work->recvCount, 1, 0);
            tail[work->recvCount & 3] = response->header[work->recvCount];
        } else {
            int offset = work->recvCount & (NHTTP_HEADER_BLOCK_SIZE - 1);

            if (offset == 0) {
                if (block != NULL) {
                    block->next = func_ov114_021ba820(sizeof(NHttpHeaderBlock), 4);
                    block = block->next;
                } else {
                    block = func_ov114_021ba820(sizeof(NHttpHeaderBlock), 4);
                    response->blocks = block;
                }
                if (block == NULL) {
                    work->error = 1;
                    return FALSE;
                }
                block->next = NULL;
            }
            count = func_ov114_021b9f94(mutexInfo, request, state->socket, block->data + offset, 1, 0);
            tail[work->recvCount & 3] = block->data[offset];
        }
        if (count <= 0) {
            work->error = 10;
            return FALSE;
        }
        work->recvCount += count;
        if (func_ov114_021bbec0(tail, work->recvCount)) {
            break;
        }
    }
    response->headerSize = work->recvCount;
    if (response->headerSize == 0) {
        work->error = 7;
        return FALSE;
    }
    return TRUE;
}

// Reads the status line and the header fields that matter: the status, how long the body is, whether the connection
// stays open and whether the body is in chunks
BOOL func_ov114_021bdd34(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpRequest *request = func_ov114_021bef48(root)->current->request;
    NHttpResponse *response = request->response;
    u8 *sendBuffer = func_ov114_021bef50(root)->sendBuffer;
    int colon;
    int start;
    int length;

    if (!func_ov114_021bb210(response, work->statusLine, 0, sizeof(work->statusLine))) {
        work->error = 7;
        return FALSE;
    }
    if (func_ov114_021ba150(work->statusLine, "HTTP/", 5) != 0) {
        work->error = 7;
        return FALSE;
    }
    if (work->statusLine[8] != ' ') {
        work->error = 7;
        return FALSE;
    }
    response->statusCode = NHTTPi_strToInt(work->statusLine + 9, 3);
    if (response->statusCode < 0) {
        work->error = 7;
        return FALSE;
    }
    if (func_ov114_021bafa0(response, NHTTP_FIRST_FIELD, response->headerSize, &colon, NULL) < 0) {
        work->error = 7;
        return FALSE;
    }
    length = func_ov114_021bbbc8(response, "Content-Length", &start);
    work->contentLength = length;
    if (length == 0) {
        work->error = 0;
        return FALSE;
    }
    if (length > NHTTP_SEND_BUFFER_SIZE) {
        work->error = 7;
        return FALSE;
    }
    if (length > 0) {
        if (!func_ov114_021bb210(response, (char *)sendBuffer, start, length)) {
            work->error = 7;
            return FALSE;
        }
        work->contentLength = NHTTPi_strToInt((char *)sendBuffer, work->contentLength);
        if (work->contentLength < 0) {
            work->error = 7;
            return FALSE;
        }
        response->contentLength = work->contentLength;
    } else {
        response->contentLength = -1;
    }
    if (request->https != 0) {
        work->keepAlive = FALSE;
    } else {
        length = func_ov114_021bbbc8(response, "Connection", &start);
        if (length == 0) {
            work->error = 7;
            work->keepAlive = FALSE;
            return FALSE;
        }
        if (length > NHTTP_SEND_BUFFER_SIZE) {
            work->keepAlive = FALSE;
        } else if (length > 0) {
            if (func_ov114_021bb12c(response, start, start + length, "Keep-Alive", 0) == 0) {
                work->keepAlive = TRUE;
            } else {
                work->keepAlive = FALSE;
            }
        } else {
            work->keepAlive = FALSE;
        }
    }
    length = func_ov114_021bbbc8(response, "Transfer-Encoding", &start);
    work->chunked = length;
    if (length == 0) {
        work->error = 7;
        return FALSE;
    }
    if (length > NHTTP_SEND_BUFFER_SIZE) {
        work->chunked = FALSE;
    } else {
        work->chunked =
            length > 0 ? func_ov114_021bb12c(response, start, start + length, "chunked", ';') == 0 : FALSE;
    }
    work->error = 0;
    response->headerDone = TRUE;
    return TRUE;
}

BOOL func_ov114_021bdff8(NHttpThreadWork *work) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpRequest *request = (func_ov114_021bef48(root))->current->request;
    NHttpResponse *response = request->response;
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);
    u8 *scratch = (func_ov114_021bef50(root))->sendBuffer;
    int count;

    if (request->method == 2 || response->statusCode == 204 || response->statusCode == 304 ||
        (response->statusCode >= 100 && response->statusCode < 200)) {
        return TRUE;
    }
    func_ov114_021bef60(connection, 0);
    if (connection != NULL) {
        connection->stage = 4;
    }
    if (work->contentLength >= 0) {
        func_ov114_021bef60(connection, work->contentLength);
        if (work->contentLength > 0) {
            do {
                if (work->error != 6 && !func_ov114_021bc2fc(mutexInfo, response)) {
                    work->error = 6;
                    response->buffer = work->unk_104;
                    response->bufferSize = sizeof(work->unk_104);
                }
                if (work->error == 6) {
                    count = func_ov114_021bb344(mutexInfo, request, state->socket, 0, work->contentLength, 0);
                } else {
                    count = func_ov114_021bb344(mutexInfo, request, state->socket, response->bufferUsed,
                                                work->contentLength, 0);
                }
                if (count < 0) {
                    return FALSE;
                }
                if (count == 0) {
                    break;
                }
                if (work->error != 6) {
                    response->bufferUsed += count;
                    response->bodyReceived += count;
                }
                work->contentLength -= count;
            } while (work->contentLength > 0);
        }
        if (work->error != 6) {
            if (work->contentLength != 0) {
                work->error = func_ov114_021bb2fc(response, response->bufferUsed) ? 6 : 10;
            } else {
                work->error = 0;
            }
        }
    } else {
        int chunkSize;

        work->error = 10;
        chunkSize = 10;
        if (work->chunked) {
            chunkSize = -1;
            for (;;) {
                char ring[2];

                ring[0] = 0;
                ring[1] = 0;
                work->recvCount = 0;
                while (work->recvCount < NHTTP_SEND_BUFFER_SIZE) {
                    int length;
                    char c;

                    count = func_ov114_021b9f94(mutexInfo, request, state->socket, scratch + work->recvCount, 1, 0);
                    if (count < 0) {
                        return FALSE;
                    }
                    length = work->recvCount;
                    c = ring[length & 1] = scratch[length];
                    if (c == ';' || (c == '\n' && ring[(length - 1) & 1] == '\r')) {
                        if (c == '\n') {
                            length--;
                        } else if (func_ov114_021bc00c(mutexInfo, request, state->socket) <= 0) {
                            return FALSE;
                        }
                        if (length == 0) {
                            return FALSE;
                        }
                        chunkSize = func_ov114_021ba35c((char *)scratch, length);
                        if (chunkSize < 0) {
                            return FALSE;
                        }
                        break;
                    }
                    work->recvCount++;
                }
                if (work->recvCount == NHTTP_SEND_BUFFER_SIZE) {
                    work->error = 7;
                    return FALSE;
                }
                if (chunkSize > 0) {
                    func_ov114_021bef60(connection, chunkSize);
                    while (chunkSize > 0) {
                        if (work->error != 6 && !func_ov114_021bc2fc(mutexInfo, response)) {
                            work->error = 6;
                            response->buffer = work->unk_104;
                            response->bufferSize = sizeof(work->unk_104);
                        }
                        if (work->error == 6) {
                            count = func_ov114_021bb344(mutexInfo, request, state->socket, 0, chunkSize, 0);
                        } else {
                            count = func_ov114_021bb344(mutexInfo, request, state->socket, response->bufferUsed,
                                                        chunkSize, 0);
                        }
                        if (count <= 0) {
                            return FALSE;
                        }
                        response->bufferUsed += count;
                        response->bodyReceived += count;
                        chunkSize -= count;
                        if (chunkSize == 0) {
                            if (func_ov114_021b9f94(mutexInfo, request, state->socket, scratch, 2, 0) <= 0) {
                                return FALSE;
                            }
                        }
                    }
                } else {
                    func_ov114_021bc00c(mutexInfo, request, state->socket);
                    work->error = 0;
                    break;
                }
            }
        } else {
            for (;;) {
                if (!func_ov114_021bc2fc(mutexInfo, response)) {
                    work->error = 6;
                    response->buffer = work->unk_104;
                    response->bufferSize = sizeof(work->unk_104);
                }
                if (work->error == 6) {
                    count = func_ov114_021bb310(mutexInfo, request, state->socket, 0, 0);
                } else {
                    count = func_ov114_021bb310(mutexInfo, request, state->socket, response->bufferUsed, 0);
                }
                if (count < 0) {
                    return FALSE;
                }
                if (count == 0) {
                    if (work->error != 6) {
                        work->error = 0;
                    }
                    break;
                }
                response->bufferUsed += count;
                response->bodyReceived += count;
            }
        }
    }
    {
        NHttpConnection *owner = func_ov114_021bebf0(mutexInfo, response);

        if (work->error == 0 && owner != NULL) {
            func_ov114_021bed90(mutexInfo, owner);
        }
    }
    return TRUE;
}

// The thread: takes each request that is sent and runs it, and sleeps when there is none, until the library ends
void func_ov114_021be4d4(void) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpThreadWork work;

    func_ov114_021bd220(&work);
    if (state->quit == 0) {
        do {
            if (work.retry == 0) {
                if (!func_ov114_021bd3b8(&work)) {
                    continue;
                }
                if (reqInfo->current->request->canceled != 0) {
                    func_ov114_021bd270(&work);
                    continue;
                }
                if (!func_ov114_021bd430(&work)) {
                    func_ov114_021bd270(&work);
                    continue;
                }
            }
            if (work.retry == 1) {
                work.retry = 0;
            }
            if (!func_ov114_021bd548(&work)) {
                func_ov114_021bd270(&work);
                continue;
            }
            switch (func_ov114_021bd6dc(&work)) {
            case 0:
                break;
            case 2:
                work.retry = 1;
                continue;
            case 1:
                func_ov114_021bd270(&work);
                continue;
            }
            switch (func_ov114_021bd7e8(&work)) {
            case 0:
                break;
            case 2:
                work.retry = 1;
                continue;
            case 1:
            case 3:
                func_ov114_021bd270(&work);
                continue;
            }
            if (reqInfo->current->request->canceled != 0) {
                func_ov114_021bd270(&work);
                continue;
            }
            if (!func_ov114_021bdb68(&work)) {
                func_ov114_021bd270(&work);
                continue;
            }
            if (!func_ov114_021bdd34(&work)) {
                func_ov114_021bd270(&work);
                continue;
            }
            func_ov114_021bdff8(&work);
            func_ov114_021bd270(&work);
        } while (state->quit == 0);
    }
}
