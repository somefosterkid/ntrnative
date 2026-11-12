// Black's code at ov009 0x0217956c-0x0217a880. libdwcauth.a's dwc_http.o: the code is DWC_Http_*'s HTTP client over CPS
// (POST /%s HTTP/1.0 headers, Content-Length, a thread, base64 items) in the object's order.

#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/ssl.h"
#include "dwc/http.h"
#include "dwc/dwc.h"
#include "stddef.h"
#include "string.h"
#include "stdlib.h"

void DWCi_Http_DestroyResult(s32 param_1, s32 param_2, s32 param_3);
s32 DWCi_Http_AllocBuffer(DWCHttp *param_1, DWCHttpBuffer *param_2, s32 param_3);
void DWCi_Http_FreeBuffer(s32 param_1, s32 *param_2);
BOOL DWCi_Http_ReallocBuffer(DWCHttp *http, DWCHttpBuffer *buf, int size);
u32 DWCi_Http_AddResult(DWCHttp *param_1, DWCHttpParseResult *param_2, const char *param_3, const char *param_4);
BOOL DWCi_Http_CheckHeaderReceived(DWCHttp *http);
int DWCi_Http_CPSCallback(int result, CPSSslConnection *con, int level);

// The root certificates the connection accepts (libdwcauth's dwc_ca_*.c files)
extern CPSCaInfo data_ov009_021a3380;
extern CPSCaInfo data_ov009_021a3254;
extern CPSCaInfo data_ov009_021a3534;
extern CPSCaInfo DWCca_verisign_secureserver;
extern CPSCaInfo data_ov009_021a2a3c;
extern CPSCaInfo data_ov009_021a2b54;
extern CPSCaInfo DWCca_cybertrust_gteroot;
extern CPSCaInfo data_ov009_021a28f0;
extern CPSCaInfo DWCca_thawte_server;
extern CPSCaInfo DWCca_thawte_premiumserver;
extern CPSCaInfo DWCca_globalsign_rootcar2;
extern CPSCaInfo DWCca_nas_self;

void *data_ov009_021a2590[12] = {
    &data_ov009_021a3380,
    &data_ov009_021a3254,
    &data_ov009_021a3534,
    &DWCca_verisign_secureserver,
    &data_ov009_021a2a3c,
    &data_ov009_021a2b54,
    &DWCca_cybertrust_gteroot,
    &data_ov009_021a28f0,
    &DWCca_thawte_server,
    &DWCca_thawte_premiumserver,
    &DWCca_globalsign_rootcar2,
    &DWCca_nas_self,
};

BOOL DWCi_Http_SetHostinfo(DWCHttp *http, const char *url);
int DWCi_Http_WriteBasicHeader(DWCHttp *http);


int DWC_Http_Create(DWCHttp *http, DWCHttpParam *config) {
    DWCHttpAllocFunc alloc = config->alloc;

    MI_CpuFill8(http, 0, sizeof(DWCHttp));
    http->content_len = -1;
    http->receivedbody_len = -1;
    http->param = *config;
    http->lowrecvbuf = alloc(0, 0xb68);
    if (http->lowrecvbuf == NULL) {
        http->error = 1;
        return 1;
    }
    http->lowsendbuf = alloc(0, 0x5ea);
    if (http->lowsendbuf == NULL) {
        http->error = 1;
        return 1;
    }
    if (DWCi_Http_AllocBuffer(http, &http->rep, http->param.bufSize) == 0) {
        http->error = 1;
        return 1;
    }
    if (DWCi_Http_SetHostinfo(http, config->url) == 0) {
        http->error = 1;
        return 1;
    }
    http->error = DWCi_Http_WriteBasicHeader(http);
    if (http->error == 0) {
        http->initflag = 0xff;
    }
    return http->error;
}

int DWC_Http_FinishHeader(DWCHttp *http) {
    char buf[8];
    int len;

    if (DWC_Http_Add_HeaderItem(http, "Connection", "close") != 0) {
        return 1;
    }
    len = strlen(strstr(http->req.buffer, "\r\n\r\n") + 4);
    if (len != 0) {
        OS_SNPrintf(buf, 7, "%d", len);
        if (DWC_Http_Add_HeaderItem(http, "Content-Length", buf) != 0) {
            return 1;
        }
    }
    return 0;
}

static s32 data_ov009_021ace58;

void DWCi_Http_Thread(void *arg);
void DWC_Http_StartThread(DWCHttp *http, u32 priority) {
    http->abort = 0;
    OS_InitMutex(&http->mutex);
    OS_InitMutex(&http->content_len_mutex);
    if (http->param.unk_14 == 1) {
        data_ov009_021ace58 = 1;
    } else {
        data_ov009_021ace58 = 0;
    }
    if (http->thread.id == 0 || OS_IsThreadTerminated(&http->thread)) {
        OS_CreateThread(&http->thread, DWCi_Http_Thread, http, http->stack + 0x1000, 0x1000, priority);
        OS_WakeupThreadDirect(&http->thread);
    }
}


#pragma thumb off
void DWC_Http_Abort(void *http) {
    u8 *param_1 = http;

    if (*(u8 *)(param_1 + 0x1000) != 0xff) {
        return;
    }
    OS_LockMutex((OSMutex *)(param_1 + 0x1c34));
    *(u32 *)(param_1 + 0x1c4c) = 1;
    OS_UnlockMutex((OSMutex *)(param_1 + 0x1c34));
}
#pragma thumb on

#pragma thumb off
BOOL DWCi_Http_Yield(DWCHttp *http) {
    if (http->ssl_enabled == TRUE) {
        OS_GetLowEntropyData(http->lowentropydata);
        func_ov009_02162248(http->lowentropydata, sizeof(http->lowentropydata));
    }
    OS_LockMutex(&http->mutex);
    if (http->abort == TRUE) {
        OS_UnlockMutex(&http->mutex);
        return FALSE;
    }
    OS_UnlockMutex(&http->mutex);
    OS_Sleep(10);
    return TRUE;
}
#pragma thumb on

#pragma thumb off
void DWCi_Http_InitCpsSocket(DWCHttp *http) {
    MI_CpuFill8(&http->soc, 0, sizeof(CPSSoc));
    http->soc.rcvbuf_size = 0xB68;
    http->soc.rcvbuf = http->lowrecvbuf;
    http->soc.sndbuf_size = 0x5EA;
    http->soc.sndbuf = http->lowsendbuf;
    func_ov009_0215aa5c(&http->soc);
}
#pragma thumb on

#pragma thumb off
CPSInAddr DWCi_Http_Resolve(DWCHttp *http) {
    return func_ov009_0215c910(http->hostname);
}

BOOL DWCi_Http_CheckHeaderReceived(DWCHttp *http) {
    DWCHttpBuffer *response = &http->rep;
    char *text = response->buffer;
    char *body;
    char *lineEnd;
    char *lengthText;
    char saved;
    int labelLength;

    if (strstr(text, "\r\n\r\n") == NULL) {
        return FALSE;
    }
    body = strstr(text, "\r\n\r\n") + 4;
    OS_LockMutex(&http->content_len_mutex);
    http->receivedbody_len = response->write_index - body;
    OS_UnlockMutex(&http->content_len_mutex);
    lengthText = strstr(response->buffer, "Content-Length: ");
    if (lengthText == NULL) {
        return TRUE;
    }
    labelLength = strlen("Content-Length: ");
    lineEnd = strstr(lengthText + labelLength, "\r\n");
    saved = *lineEnd;
    *lineEnd = 0;
    OS_LockMutex(&http->content_len_mutex);
    http->content_len = atol(lengthText + labelLength);
    OS_UnlockMutex(&http->content_len_mutex);
    *lineEnd = saved;
    return TRUE;
}


void DWCi_Http_Thread(void *arg) {
    DWCHttp *http = (DWCHttp *)arg;
    CPSSoc *soc = &http->soc;
    CPSSslConnection *con = &http->con;
    DWCHttpBuffer *rep = &http->rep;
    u32 copy_length;
    u32 ip;
    u8 *s;
    s32 len;
    u64 lastactivetime;
    u64 nowtime;
    s32 timeout;
    u32 tcpconnect;
    s32 header_received = FALSE;

    timeout = http->param.timeout;
    if (timeout <= 0) {
        timeout = 60000;
    }
    DWCi_Http_InitCpsSocket(http);
    ip = DWCi_Http_Resolve(http);
    if (ip == 0) {
        http->error = DWCHTTP_E_DNSERR;
        return;
    }
    http->hostip = ip;
    func_ov009_0215ab10();
    if (http->ssl_enabled == TRUE) {
        MI_CpuFill8(con, 0, sizeof(CPSSslConnection));
        con->auth_callback = DWCi_Http_CPSCallback;
        con->server_name = http->hostname;
        soc->con = con;
        CPS_SetRootCa(data_ov009_021a2590, 12);
        CPS_SetSsl(TRUE);
    }
    func_ov009_0215aab0(0, http->port, ip);
    tcpconnect = func_ov009_0215ac60();
    if (tcpconnect != 0) {
        http->error = DWCHTTP_E_CONERR;
        func_ov009_0215ab5c();
        func_ov009_0215aa70();
        return;
    }
    s = (u8 *)http->req.buffer;
    len = func_ov009_0215b470(s, strlen((const char *)s));
    if (len <= 0) {
        http->error = DWCHTTP_E_SENDERR;
        goto exit;
    }
    func_ov009_0215b584();
    if (!DWCi_Http_Yield(http)) {
        http->error = DWCHTTP_E_ABORT;
        goto exit;
    }
    rep->write_index = rep->buffer;
    rep->buffer_tail = rep->buffer + rep->length;
    lastactivetime = OS_GetTick();
    while (1) {
        if (data_ov009_021a82bc == 0) {
            http->error = DWCHTTP_E_SENDERR;
            goto exit;
        }
        len = func_ov009_0215b504();
        if (len < 0) {
            goto finish;
        }
        if (len > 0) {
            lastactivetime = OS_GetTick();
            s = func_ov009_0215aeb0((u32 *)&len);
            if (s == NULL) {
                goto finish;
            }
            copy_length = (len < rep->buffer_tail - 1 - rep->write_index) ? len : rep->buffer_tail - 1 - rep->write_index;
            MI_CpuCopy8(s, rep->write_index, copy_length);
            rep->write_index += copy_length;
            *rep->write_index = '\0';
            if (header_received == TRUE) {
                OS_LockMutex(&http->content_len_mutex);
                http->receivedbody_len += copy_length;
                OS_UnlockMutex(&http->content_len_mutex);
            } else {
                header_received = DWCi_Http_CheckHeaderReceived(http);
            }
            if (len > copy_length) {
                func_ov009_0215b064(len);
                goto finish;
            }
            func_ov009_0215b064(copy_length);
        }
        if (http->content_len >= 0 && http->receivedbody_len >= http->content_len) {
            goto finish;
        }
        nowtime = OS_GetTick();
        if (OS_TicksToMilliSeconds(nowtime - lastactivetime) > timeout) {
            http->error = DWCHTTP_E_RECVTOUT;
            goto exit;
        }
        if (!DWCi_Http_Yield(http)) {
            http->error = DWCHTTP_E_ABORT;
            goto exit;
        }
    }
finish:
    func_ov009_0215ad00();
    func_ov009_0215ad3c();
    func_ov009_0215ab5c();
    func_ov009_0215aa70();
    http->error = DWCHTTP_E_FINISH;
    return;
exit:
    func_ov009_0215ad00();
    func_ov009_0215ad3c();
    func_ov009_0215ab5c();
    func_ov009_0215aa70();
}

#pragma thumb on

#pragma thumb off
void DWC_Http_Destroy(void *param_1) {
    void (*func)();

    func = *(void (**)())((s32)(u8 *)param_1 + 0x1014);
    if (param_1 == (void *)0x0) {
        return;
    }
    DWCi_Http_DestroyResult((s32)param_1, (s32)(u8 *)param_1 + 0x1a74, 0x20);
    DWCi_Http_FreeBuffer((s32)param_1, (s32 *)((s32)(u8 *)param_1 + 0x1a44));
    DWCi_Http_FreeBuffer((s32)param_1, (s32 *)((s32)(u8 *)param_1 + 0x1a34));
    if (*(s32 *)((s32)(u8 *)param_1 + 0x1a08) != 0) {
        (*func)(0, *(s32 *)((s32)(u8 *)param_1 + 0x1a08), 0);
        *(u32 *)((s32)(u8 *)param_1 + 0x1a08) = 0;
    }
    if (*(s32 *)((s32)(u8 *)param_1 + 0x1a0c) != 0) {
        (*func)(0, *(s32 *)((s32)(u8 *)param_1 + 0x1a0c), 0);
        *(u32 *)((s32)(u8 *)param_1 + 0x1a0c) = 0;
    }
    MI_CpuFill8(param_1, 0, 0x1c50);
}

int DWCi_Http_WriteBasicHeader(DWCHttp *http) {
    DWCHttpBuffer *req = &http->req;
    const char *fmt = "POST /%s HTTP/1.0\r\nContent-type: application/x-www-form-urlencoded\r\nHost: %s\r\n\r\n";
    int len;

    if (http->param.unk_04 != 0) {
        fmt = "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n";
    }
    len = strlen(http->hostname) + (strlen(fmt) - 4 + strlen(http->filepath));
    if (DWCi_Http_AllocBuffer(http, &http->req, len + 0x400) != 1) {
        return 1;
    }
    req->write_index += OS_SNPrintf(req->write_index, req->length, fmt, http->filepath, http->hostname);
    return 0;
}

#pragma thumb on

#pragma thumb off
int DWC_Http_Add_HeaderItem(DWCHttp *http, const char *label, const char *data) {
    s32 val;
    s32 result;
    s8 *str;
    s8 *src;
    u64 val2;
    void *buf;

    buf = (u8 *)http + 0x1A34;
    result = strlen(data) + ((strlen("%s: %s\r\n") - 4) + strlen(label));
    val = *(s32 *)((u8 *)(u8 *)buf + 8) - *(s32 *)((u8 *)(u8 *)buf + 4);
    if (((s32) (result + 1) > val) && (DWCi_Http_ReallocBuffer(http, buf, (result - val) + 1) == 0)) {
        return 1;
    }
    str = strstr(*(s8 **)((u8 *)(u8 *)http + 0x1A34), "\r\n\r\n");
    src = str + 2;
    val2 = *(s8 *)((u8 *)str + 2);
    memmove(&src[result], src, strlen(src) + 1);
    src[OS_SNPrintf(src, result + 1, "%s: %s\r\n", label, data)] = val2;
    *(s32 *)((u8 *)(u8 *)buf + 4) = (s32) (*(s32 *)((u8 *)(u8 *)buf + 4) + result);
    return 0;
}

int DWC_Http_Add_PostBase64Item(DWCHttp *http, const char *key, const char *value, int len) {
    int enclen;
    DWCHttpBuffer *req = &http->req;
    const char *fmt = "%s=";
    int size;
    int rest;
    char *p;

    if (http->num_postitem != 0) {
        fmt = "&%s=";
    }
    http->num_postitem++;
    enclen = func_02059b40(value, len, NULL, 0);
    size = enclen + (strlen(fmt) - 2 + strlen(key));
    rest = req->buffer_tail - req->write_index;
    if (size > rest) {
        if (DWCi_Http_ReallocBuffer(http, req, size - rest + 1) == FALSE) {
            return 1;
        }
        rest = req->buffer_tail - req->write_index;
    }
    req->write_index += OS_SNPrintf(req->write_index, rest, fmt, key);
    if (func_02059b40(value, len, req->write_index, req->buffer_tail - req->write_index - 1) < 0) {
        return 1;
    }
    p = req->write_index;
    req->write_index = p + enclen;
    p[enclen] = '\0';
    return 0;
}

int DWC_Http_Add_Body(DWCHttp *http, const char *text) {
    int length;
    DWCHttpBuffer *request = &http->req;
    int room;
    int written;

    length = strlen(text);
    room = request->buffer_tail - request->write_index;
    if (length > room) {
        if (DWCi_Http_ReallocBuffer(http, request, length - room + 1) == 0) {
            return 1;
        }
        room = request->buffer_tail - request->write_index;
    }
    written = OS_SNPrintf(request->write_index, room, "%s", text);
    if (written != length) {
        return 1;
    }
    request->write_index += written;
    return 0;
}


#pragma thumb on

#pragma thumb off
int DWCi_Http_CPSCallback(int result, CPSSslConnection *con, int level) {
    if ((result & 0x8000) != 0) {
        result = result & 0xffff7fff;
    }
    return result;
}
#pragma thumb on

#pragma thumb off
s32 DWCi_Http_AllocBuffer(DWCHttp *param_1, DWCHttpBuffer *param_2, s32 param_3) {
    s32 (*func)(s32, s32);
    s32 result;

    func = *(s32 (**)(s32, s32))((u8 *)param_1 + 0x1010);
    if (param_3 == 0) {
        return 0;
    }
    result = func(0, param_3);
    *(s32 *)param_2 = result;
    if (result == 0) {
        return 0;
    }
    *(s32 *)((u8 *)param_2 + 4) = result;
    *(s32 *)((u8 *)param_2 + 8) = (s32) (result + param_3);
    *(s32 *)((u8 *)param_2 + 0xC) = param_3;
    return 1;
}
#pragma thumb on

#pragma thumb off
void DWCi_Http_FreeBuffer(s32 http, s32 *buf) {
    s32 (*func)(s32, s32, s32);
    s32 val;

    val = *buf;
    func = *(s32 (**)(s32, s32, s32))((u8 *)http + 0x1014);
    if (val != 0) {
        func(0, val, 0);
    }
    MI_CpuFill8(buf, 0U, 0x10U);
}

BOOL DWCi_Http_ReallocBuffer(DWCHttp *http, DWCHttpBuffer *buffer, int extra) {
    DWCHttpAllocFunc alloc = http->param.alloc;
    DWCHttpFreeFunc free = http->param.free;
    char *start;

    if (extra <= 0) {
        return FALSE;
    }
    start = alloc(0, buffer->length + extra);
    if (start == NULL) {
        return FALSE;
    }
    MI_CpuCopy8(buffer->buffer, start, buffer->length);
    free(0, buffer->buffer, 0);
    if (start == NULL) {
        return FALSE;
    }
    buffer->write_index += start - buffer->buffer;
    buffer->length += extra;
    buffer->buffer = start;
    buffer->buffer_tail = start + buffer->length;
    return TRUE;
}


BOOL DWCi_Http_SetHostinfo(DWCHttp *http, const char *url) {
    char *port = NULL;
    char *path;
    char *p;

    if (strlen(url) >= DWCHTTP_MAXURL) {
        return FALSE;
    }
    strncpy(http->url, url, DWCHTTP_MAXURL);
    if (strlen(url) != strlen(http->url)) {
        return FALSE;
    }
    if (strstr(http->url, "http://") != NULL) {
        http->hostname = http->url + 7;
        http->ssl_enabled = FALSE;
        http->port = 80;
    } else {
        p = strstr(http->url, "https://");
        if (p == NULL) {
            return FALSE;
        }
        http->hostname = p + 8;
        http->ssl_enabled = TRUE;
        http->port = 443;
    }
    p = strstr(http->hostname, ":");
    if (p != NULL) {
        *p = '\0';
        port = p + 1;
    }
    p = strstr(http->hostname, "/");
    path = NULL;
    if (p != NULL) {
        *p = '\0';
        path = p + 1;
    }
    http->filepath = path;
    if (port != NULL) {
        http->port = atol(port);
    }
    return TRUE;
}

#pragma thumb on

#pragma thumb off
u32 DWCi_Http_AddResult(DWCHttp *param_1, DWCHttpParseResult *param_2, const char *param_3, const char *param_4) {
    s32 (*allocFn)(s32, s32);
    s32 (*freeFn)(s32, s32, s32);
    int entryOfs;
    u32 idx;
    u8 *entries;
    s32 oldLabel;
    s32 oldValue;


    allocFn = *(s32 (**)(s32, s32))((u8 *)param_1 + 0x1010);
    freeFn = *(s32 (**)(s32, s32, s32))((u8 *)param_1 + 0x1014);
    if ((s32) param_2->index > (s32) param_2->len) {
        return 0U;
    }
    *(s32 *)((u8 *)param_2->entry + (param_2->index * 8)) = allocFn(0, strlen(param_3) + 1);
    if (*(s32 *)((u8 *)param_2->entry + (param_2->index * 8)) != 0) {
        *(s32 *)((u8 *)(((u8 *)param_2->entry + (param_2->index * 8))) + 4) = allocFn(0, strlen(param_4) + 1);
        idx = param_2->index;
        entries = (u8 *)param_2->entry;
        entryOfs = idx * 8;
        if (*(s32 *)((u8 *)(entries + entryOfs) + 4) != 0) {
            strcpy(*(char **)(entries + (idx * 8)), param_3);
            strcpy(*(s8 **)((u8 *)(((u8 *)param_2->entry + (param_2->index * 8))) + 4), param_4);
            param_2->index = (s32) (param_2->index + 1);
            return 1U;
        }
    }
    oldLabel = *(s32 *)((u8 *)param_2->entry + (param_2->index * 8));
    if (oldLabel != 0) {
        freeFn(0, oldLabel, 0);
        *(s32 *)((u8 *)param_2->entry + (param_2->index * 8)) = 0;
    }
    oldValue = *(s32 *)((u8 *)(((u8 *)param_2->entry + (param_2->index * 8))) + 4);
    if (oldValue != 0) {
        freeFn(0, oldValue, 0);
        *(s32 *)((u8 *)(((u8 *)param_2->entry + (param_2->index * 8))) + 4) = 0;
    }
    return 0U;
}

int DWC_Http_ParseResult(DWCHttp *http, int flag) {
    DWCHttpParseResult table;
    char savedColon;
    char savedEnd;
    char *response;
    char *bodyStart;
    char *bodyEnd;
    char *space;
    char *status;
    char *line;
    char saved;

    table.entry = http->labelvalue;
    table.len = DWCHTTP_MAX_LABELVALUE;
    table.index = 0;
    MI_CpuFill8(http->labelvalue, 0, sizeof(http->labelvalue));
    response = http->rep.buffer;
    bodyStart = strstr(response, "\r\n\r\n");
    if (bodyStart == NULL) {
        return 0;
    }
    bodyEnd = bodyStart + 4 + strlen(bodyStart + 4);
    space = strstr(response, " ");
    if (space == NULL) {
        return 0;
    }
    status = space + 1;
    saved = status[3];
    status[3] = 0;
    if (DWCi_Http_AddResult(http, &table, "httpresult", status) != 1) {
        status[3] = saved;
        return 0;
    }
    status[3] = saved;
    if (flag == 1 || strncmp(status, "200", 3) != 0) {
        return 1;
    }
    line = strstr(status + 4, "\r\n");
    if (line == NULL) {
        return 0;
    }
    line += 2;
    while (line[0] != '\r' && line[1] != '\n') {
        char *colon;
        char *eol;
        char *value;

        colon = strstr(line, ": ");
        if (colon == NULL) {
            break;
        }
        savedColon = *colon;
        *colon = 0;
        value = colon + 2;
        eol = strstr(value, "\r\n");
        if (eol == NULL) {
            *colon = savedColon;
            break;
        }
        savedEnd = *eol;
        *eol = 0;
        if (DWCi_Http_AddResult(http, &table, line, value) != 1) {
            *colon = savedColon;
            *eol = savedEnd;
            return 0;
        }
        line = value + strlen(value) + 2;
        *colon = savedColon;
        *eol = savedEnd;
    }
    {
        char *equals;
        char *value;
        char *name;
        char savedEquals;
        char *amp;

        for (name = bodyStart + 4; name < bodyEnd; ) {
            equals = strstr(name, "=");
            if (equals == NULL) {
                break;
            }
            savedEquals = *equals;
            *equals = 0;
            value = equals + 1;
            amp = strstr(value, "&");
            if (amp == NULL) {
                amp = strstr(value, "\r\n");
            }
            if (amp != NULL) {
                savedEnd = *amp;
                *amp = 0;
            }
            if (DWCi_Http_AddResult(http, &table, name, value) != 1) {
                *equals = savedEquals;
                if (amp != NULL) {
                    *amp = savedEnd;
                }
                return 0;
            }
            name = value + strlen(value) + 1;
            *equals = savedEquals;
            if (amp != NULL) {
                *amp = savedEnd;
            }
        }
    }
    return 1;
}

char *DWC_Http_GetResult(DWCHttp *http, const char *key) {
    int i;

    for (i = 0; i < DWCHTTP_MAX_LABELVALUE; i++) {
        if (http->labelvalue[i].label == NULL) {
            break;
        }
        if (strcmp(key, http->labelvalue[i].label) == 0) {
            return http->labelvalue[i].value;
        }
    }
    return NULL;
}


#pragma thumb on

#pragma thumb off
int DWC_Http_GetBase64DecodedResult(DWCHttp *http, const char *key, char *buf, int size) {
    s32 result;
    s8 *src;

    src = DWC_Http_GetResult(http, key);
    if (src == NULL) {
        return 0;
    }
    result = func_02059cbc(src, (s32) strlen(src), (u8 *) buf, (u32) size);
    if (result == -1) {
        return result;
    }
    if ((u32) result < (u32) size) {
        ((u8 *)buf)[result] = 0;
    }
    return result;
}
#pragma thumb on

#pragma thumb off
BOOL DWC_Http_GetRawResult(DWCHttp *http, const char *key, char *buf, int size) {
    char *str;
    size_t len;

    str = DWC_Http_GetResult(http, key);
    if (str == NULL) {
        return 0;
    }
    len = strlen(str);
    if ((s32)len < size) {
        strcpy(buf, str);
        return 1;
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
void DWCi_Http_DestroyResult(s32 param_1, s32 param_2, s32 param_3) {
    s32 (*func)(s32, s32, s32);
    s32 val;
    int flag;
    s32 val2;
    s32 *ptr;
    s32 i;

    func = *(s32 (**)(s32, s32, s32))((u8 *)param_1 + 0x1014);
    val2 = param_2;
    i = 0;
    ptr = &val2;
    if (param_3 <= 0) {
        return;
    }
    do {
        val = *(s32 *)(val2 + i * 8);
        flag = 0;
        if (val != flag) {
            func(0, val, 0);
            *(s32 *)(param_2 + (i * 8)) = 0;
        }
        ;
        if (*(s32 *)((u8 *)(val2 + i * 8) + 4) != 0) {
            func(flag, *(s32 *)((u8 *)(val2 + i * 8) + 4), 0);
            *(s32 *)((u8 *)(*ptr + i * 8) + 4) = 0;
        }
        i += 1;
    } while (i < param_3);
}
#pragma thumb on
