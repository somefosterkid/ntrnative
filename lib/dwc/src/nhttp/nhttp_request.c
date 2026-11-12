// Black's code at ov114 0x021bb3c0-0x021bbb28. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

#define NHTTP_URL_LENGTH_MAX 0x1ff
#define NHTTP_URL_BUFFER_SIZE 0x200
#define NHTTP_ERROR_NO_MEMORY 1
#define NHTTP_ERROR_BAD_URL 4
#define NHTTP_ERROR_BAD_STATE 11

// The length of the scheme in front of the host: http:// or https://
static inline int NHTTPi_schemeLength(NHttpRequest *request) {
    return request->https ? 8 : 7;
}

#pragma thumb off
void func_ov114_021bb3c0(NHttpReqInfo *info) {
    info->current = NULL;
}

NHttpRequest *func_ov114_021bb3cc(NHttpState *state, const char *url, int method, void *buffer, int bufferSize, void *param,
                                  int unused, NHttpReleaseFunc release) {
    char text[NHTTP_URL_BUFFER_SIZE];
    int length;
    int schemeLength;
    int restLength;
    int sawSlash;
    int out;
    const char *rest;
    int decodedLength;
    int in;
    int escapes;
    int escapeState;
    char c;
    NHttpRequest *request = NULL;

    if (method != NHTTP_METHOD_GET && method != NHTTP_METHOD_POST && method != NHTTP_METHOD_HEAD) {
        func_ov114_021ba874(state, NHTTP_ERROR_BAD_STATE);
        goto fail;
    }
    request = (NHttpRequest *)func_ov114_021ba820(sizeof(NHttpRequest), 4);
    if (request == NULL) {
        func_ov114_021ba874(state, NHTTP_ERROR_NO_MEMORY);
        goto fail;
    }
    func_ov114_021ba13c(request, sizeof(NHttpRequest));
    request->response = (NHttpResponse *)func_ov114_021ba820(sizeof(NHttpResponse), 4);
    if (request->response == NULL) {
        func_ov114_021ba874(state, NHTTP_ERROR_NO_MEMORY);
        goto fail;
    }
    func_ov114_021ba13c(request->response, sizeof(NHttpResponse));
    request->response->buffer = buffer;
    request->response->bufferSize = bufferSize;
    request->response->unk_2C = unused;
    request->response->release = release;
    length = func_ov114_021ba124(url, NHTTP_URL_LENGTH_MAX);
    if (length <= 7) {
        func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
        goto fail;
    }
    func_ov114_021ba13c(text, NHTTP_URL_BUFFER_SIZE);
    func_ov114_021ba100(text, url, length);
    request->port = 80;
    schemeLength = 7;
    if (func_ov114_021ba150(text, "http://", 7) != 0) {
        schemeLength = 8;
        if (func_ov114_021ba150(text, "https://", 8) != 0) {
            func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
            goto fail;
        }
        request->https = 1;
        request->port = 443;
    }
    rest = text + schemeLength;
    restLength = length - schemeLength;
    if (restLength <= 0) {
        func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
        goto fail;
    }
    in = 0;
    escapes = 0;
    escapeState = 0;
    while (in < restLength && rest[in] != '/') {
        if (escapeState == 2) {
            escapeState--;
        } else if (escapeState == 1) {
            c = func_ov114_021ba35c(rest + (in - 1), 2);
            escapeState--;
            if (c < 0) {
                func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
                goto fail;
            }
            if (c == '/') {
                escapes--;
                break;
            }
        } else if (rest[in] == '%') {
            escapeState = 2;
            escapes++;
        }
        in++;
    }
    if (escapeState != 0) {
        func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
        goto fail;
    }
    decodedLength = schemeLength + restLength - escapes * 2;
    request->url = (char *)func_ov114_021ba820(decodedLength + 1, 4);
    if (request->url == NULL) {
        func_ov114_021ba874(state, NHTTP_ERROR_NO_MEMORY);
        goto fail;
    }
    func_ov114_021ba13c(request->url, decodedLength + 1);
    func_ov114_021ba100(request->url, text, schemeLength);
    in = 0;
    out = 0;
    escapeState = 0;
    sawSlash = 0;
    if (restLength > 0) {
        do {
            if (escapeState == 2) {
                escapeState--;
            } else if (escapeState == 1) {
                c = func_ov114_021ba35c(rest + (in - 1), 2);
                request->url[schemeLength + out - 1] = c;
                escapeState--;
                if (c == '/') {
                    sawSlash = 1;
                }
            } else {
                c = rest[in];
                if (c == '/') {
                    sawSlash = 1;
                }
                if (!sawSlash && c == '%') {
                    escapeState = 2;
                } else {
                    request->url[schemeLength + out] = c;
                }
                out++;
            }
            in++;
        } while (in < restLength);
    }
    request->url[schemeLength + out] = 0;
    rest = request->url + schemeLength;
    for (in = 0; in < out; in++) {
        if (rest[in] == '/' || rest[in] == ':') {
            request->hostEnd = in + schemeLength;
            break;
        }
    }
    if (in == out) {
        request->hostEnd = in + schemeLength;
        request->pathStart = request->hostEnd;
    } else if (rest[in] == '/') {
        request->pathStart = request->hostEnd;
    } else if (rest[in] == ':') {
        for (; in < out; in++) {
            if (rest[in] == '/') {
                request->pathStart = in + schemeLength;
                break;
            }
        }
        if (in == out) {
            request->pathStart = in + schemeLength;
        } else {
            int portStart = request->hostEnd + 1;
            int port = NHTTPi_strtonum(request->url + portStart, request->pathStart - portStart);

            if (port < 0) {
                port = request->port;
            } else if (port > 0xffff) {
                func_ov114_021ba874(state, NHTTP_ERROR_BAD_URL);
                goto fail;
            }
            request->port = (u16)port;
        }
    }
    decodedLength = request->hostEnd - NHTTPi_schemeLength(request);
    request->host = (char *)func_ov114_021ba820(decodedLength + 1, 4);
    if (request->host == NULL) {
        func_ov114_021ba874(state, NHTTP_ERROR_NO_MEMORY);
        goto fail;
    }
    func_ov114_021ba13c(request->host, decodedLength + 1);
    func_ov114_021ba100(request->host, request->url + NHTTPi_schemeLength(request), decodedLength);
    func_ov114_021ba100(request->boundary, data_ov114_021cdfc8, NHTTP_BOUNDARY_SIZE);
    request->method = method;
    request->ssl = NULL;
    request->unk_B0 = 0;
    request->unk_B4 = 0;
    request->unk_B8 = 0;
    request->unk_BC = 0;
    request->rootCerts = NULL;
    request->rootCertCount = 0;
    request->unk_CC = 0;
    request->verifyOption = 0;
    request->skipVerify = 0;
    request->response->userParam = param;
    request->useProxy = 0;
    request->unk_248 = 0;
    return request;

fail:
    if (request != NULL) {
        if (request->url != NULL) {
            func_ov114_021ba850(request->url);
        }
        if (request->host != NULL) {
            func_ov114_021ba850(request->host);
        }
        if (request->response != NULL) {
            func_ov114_021ba850(request->response);
        }
        func_ov114_021ba850(request);
    }
    return NULL;
}

// Destroys a request after the connection that has it lets go of its response
void func_ov114_021bb8b4(NHttpRoot *root, NHttpRequest *request) {
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);

    if (connection != NULL) {
        connection->response = NULL;
    }
    func_ov114_021ba850(request->response);
    func_ov114_021bb934(mutexInfo, request);
}

// Frees a ring of headers or post data from its last part
void func_ov114_021bb8f0(NHttpHeader *list) {
    if (list != NULL) {
        do {
            if (list != list->prev) {
                NHttpHeader *last = list->prev;
                NHttpHeader *before = last->prev;

                func_ov114_021ba850(last);
                list->prev = before;
            } else {
                func_ov114_021ba850(list);
                list = NULL;
            }
        } while (list != NULL);
    }
}

BOOL func_ov114_021bb934(NHttpMutexInfo *mutexInfo, NHttpRequest *request) {
    NHttpConnection *connection = func_ov114_021bebe0(mutexInfo, request);

    if (connection != NULL) {
        connection->request = NULL;
    }
    func_ov114_021bb8f0(request->headers);
    func_ov114_021bb8f0(request->postData);
    func_ov114_021ba850(request->url);
    func_ov114_021ba850(request->host);
    func_ov114_021ba850(request);
    return TRUE;
}

// Sends a request to the thread, which takes it when the requests before it are done. Returns its id, or -1
int func_ov114_021bb97c(NHttpRoot *root, NHttpRequest *request) {
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpThreadInfo *thread = func_ov114_021bef50(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    int id;

    if (request->sent != 0) {
        func_ov114_021ba874(state, NHTTP_ERROR_BAD_STATE);
        return -1;
    }
    func_ov114_021befcc(mutexInfo);
    id = func_ov114_021bacfc(func_ov114_021bef40(root), request);
    if (id >= 0) {
        request->sent = 1;
        func_ov114_021b9c08(&thread->queue);
    } else {
        func_ov114_021ba874(state, NHTTP_ERROR_NO_MEMORY);
    }
    func_ov114_021befd8(mutexInfo);
    return id;
}

// Cancels the request that has an id: the one that runs is told to stop and its socket is shut down, one that waits is
// taken out of the list. The running node's request is volatile (nhttp_internal.h), so it is loaded again at each use
BOOL func_ov114_021bba14(NHttpRoot *root, int id) {
    BOOL canceled = FALSE;
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpQueueNode *current = reqInfo->current;

    func_ov114_021befcc(mutexInfo);
    if (current != NULL && current->id == id && current->request->canceled == 0) {
        current->request->canceled = 1;
        func_ov114_021ba068(mutexInfo, current->request, current->socket);
        canceled = TRUE;
    }
    if (!canceled) {
        canceled = func_ov114_021bade0(func_ov114_021bef40(root), mutexInfo, id);
    }
    func_ov114_021befd8(mutexInfo);
    return canceled;
}

// Cancels the running request, as func_ov114_021bba14 does, and frees every request that waits
void func_ov114_021bbaac(NHttpRoot *root) {
    NHttpReqInfo *reqInfo = func_ov114_021bef48(root);
    NHttpListInfo *list = func_ov114_021bef40(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpQueueNode *current = reqInfo->current;

    func_ov114_021befcc(mutexInfo);
    if (current != NULL && current->request->canceled == 0) {
        current->request->canceled = 1;
        func_ov114_021ba068(mutexInfo, current->request, current->socket);
    }
    func_ov114_021bae90(list, mutexInfo);
    func_ov114_021befd8(mutexInfo);
}
