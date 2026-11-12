// Black's code at ov009 0x0217a880-0x0217b828. libdwcauth.a's dwc_netcheck.o: the thread (0xc70 bytes) requests the NAS
// 'ac' URL and checks httpresult 200, with HotSpotResponse / message / returncd fields, as the Netcheck thread function of
// the object does, around the Netcheck create / destroy / abort / error functions.

#include "types.h"
#include "stdlib.h"
#include "errno.h"
#include "string.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "dwc/ac.h"
#include "dwc/auth.h"
#include "dwc/dwc.h"
#include "dwc/http.h"

// The work of the thread: its stack, then the state the other functions read and the strings it passes along
typedef struct NetcheckWork {
    u8 stack[0x1000];
    int error;
    int returnCode;
    u8 unk_1008[0x100];
    DWCAcMemFuncs funcs;
    char *html;
    char *hotSpotResponse;
    OSThread thread;
    OSMutex mutex;
} NetcheckWork;

// Black keeps these together (the code reaches the parameters from the same base): 0x4c bytes
static struct {
    DWCHttp *http;
    NetcheckWork *work;
    u32 unk_8;
    u32 unk_c;
    int unk_10;
    int unk_14;
    u8 unk_18[0x18];
    DWCHttpParam param;
} data_ov009_021ace5c;
static DWCAuthParam data_ov009_021acea8;

extern DWCHttpParam data_ov009_021a2358;
// The test server's URL, after a 1 that only code the link dropped reads (pokebw2's sConnTest): one object, as
// two definitions MWCC's size sort would put the URL first
static struct {
    int unk_0;
    const char *url;
} data_ov009_021a26d4 = {1, "http://conntest.nintendowifi.net/"};

#pragma thumb off

void DWCi_Netcheck_StartThread(void);

int DWC_Netcheck_Create(const DWCAcMemFuncs *funcs) {
    DWCAcAllocFunc alloc = funcs->alloc;

    if (data_ov009_021ace5c.work != NULL) {
        return 4;
    }
    data_ov009_021ace5c.work = alloc(0, sizeof(NetcheckWork));
    if (data_ov009_021ace5c.work == NULL) {
        return 4;
    }
    MI_CpuFill8(data_ov009_021ace5c.work, 0, sizeof(NetcheckWork));
    data_ov009_021ace5c.work->returnCode = -99999;
    data_ov009_021ace5c.work->funcs = *funcs;
    if (data_ov009_021ace5c.http != NULL) {
        return 4;
    }
    data_ov009_021ace5c.http = alloc(0, sizeof(DWCHttp));
    if (data_ov009_021ace5c.http == NULL) {
        return 4;
    }
    data_ov009_021ace5c.unk_14 = 0;
    OS_InitMutex(&data_ov009_021ace5c.work->mutex);
    DWCi_Netcheck_StartThread();
    return 0;
}

void DWC_Netcheck_Destroy(void) {
    DWCHttpFreeFunc free = data_ov009_021ace5c.work->funcs.free;

    if (data_ov009_021ace5c.http != NULL) {
        DWC_Http_Destroy(data_ov009_021ace5c.http);
        free(0, data_ov009_021ace5c.http, 0);
        data_ov009_021ace5c.http = NULL;
    }
    DWC_Auth_Destroy();
    if (data_ov009_021ace5c.work == NULL) {
        return;
    }
    if (data_ov009_021ace5c.work->html != NULL) {
        free(0, data_ov009_021ace5c.work->html, 0);
        data_ov009_021ace5c.work->html = NULL;
    }
    if (data_ov009_021ace5c.work->hotSpotResponse != NULL) {
        free(0, data_ov009_021ace5c.work->hotSpotResponse, 0);
        data_ov009_021ace5c.work->hotSpotResponse = NULL;
    }
    free(0, data_ov009_021ace5c.work, 0);
    data_ov009_021ace5c.work = NULL;
}

void DWC_Netcheck_Abort(void) {
    if (data_ov009_021ace5c.work == NULL) {
        return;
    }
    if (data_ov009_021ace5c.http != NULL) {
        DWC_Http_Abort(data_ov009_021ace5c.http);
    }
    DWC_Auth_Abort();
    if (data_ov009_021ace5c.work->thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021ace5c.work->thread)) {
        OS_JoinThread(&data_ov009_021ace5c.work->thread);
    }
    data_ov009_021ace5c.work->returnCode = -7;
}

void DWCi_Netcheck_Thread(void *arg);
void DWCi_Netcheck_SetError(u32 error);

u32 DWC_Netcheck_GetError(void) {
    u32 error;

    OS_LockMutex(&data_ov009_021ace5c.work->mutex);
    error = data_ov009_021ace5c.work->error;
    OS_UnlockMutex(&data_ov009_021ace5c.work->mutex);
    return error;
}

int DWC_Netcheck_GetReturnCode(void) {
    return data_ov009_021ace5c.work->returnCode;
}

void DWCi_Netcheck_StartThread(void) {
    if (data_ov009_021ace5c.work->thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021ace5c.work->thread)) {
        return;
    }
    OS_CreateThread(&data_ov009_021ace5c.work->thread, DWCi_Netcheck_Thread, data_ov009_021ace5c.work,
                    data_ov009_021ace5c.work->stack + sizeof(data_ov009_021ace5c.work->stack), 0x1000, 0x10);
    OS_WakeupThreadDirect(&data_ov009_021ace5c.work->thread);
}

void DWCi_Netcheck_Thread(void *arg) {
    DWCAuthResult authResult;
    u8 calInfo[0x94];
    DWCWiFiInfo wifiInfo;
    char returnCode[4];
    DWCHttpAllocFunc alloc = data_ov009_021ace5c.work->funcs.alloc;
    DWCHttpFreeFunc free = data_ov009_021ace5c.work->funcs.free;
    char *url = NULL;
    char *data = NULL;
    char *wait = NULL;
    int httpResult;
    int sleepMs;
    int urlLen;
    int dataLen;
    int waitLen;
    int len;
    int code;
    char *answer;

    for (;;) {
        data_ov009_021ace5c.param.url = data_ov009_021a26d4.url;
        data_ov009_021ace5c.param.unk_04 = 1;
        data_ov009_021ace5c.param.bufSize = 0x1000;
        data_ov009_021ace5c.param.alloc = alloc;
        data_ov009_021ace5c.param.free = free;
        data_ov009_021ace5c.param.timeout = 20000;
        data_ov009_021ace5c.work->returnCode = -2;
        if (DWC_Http_Create(data_ov009_021ace5c.http, &data_ov009_021ace5c.param) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        if (DWC_Http_FinishHeader(data_ov009_021ace5c.http) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        DWC_Http_StartThread(data_ov009_021ace5c.http, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (data_ov009_021ace5c.http->thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021ace5c.http->thread)) {
            OS_JoinThread(&data_ov009_021ace5c.http->thread);
        }
        switch (data_ov009_021ace5c.http->error) {
        case 2:
            data_ov009_021ace5c.work->returnCode = -1;
        default:
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(3);
            goto done;
        case 8:
            if (DWC_Http_ParseResult(data_ov009_021ace5c.http, 0) != 1) {
                DWC_Http_Destroy(data_ov009_021ace5c.http);
                DWCi_Netcheck_SetError(2);
                goto done;
            }
            httpResult = atol(DWC_Http_GetResult(data_ov009_021ace5c.http, "httpresult"));
            if (errno == 0x22) {
                DWCi_Netcheck_SetError(2);
                goto done;
            }
            switch (httpResult) {
            case 200:
                data_ov009_021ace5c.unk_c = data_ov009_021ace5c.http->hostip;
                break;
            case 302:
                data_ov009_021ace5c.unk_14 = 1;
                if (data_ov009_021ace5c.work->hotSpotResponse != NULL) {
                    data_ov009_021ace5c.work->returnCode = -6;
                    DWC_Http_Destroy(data_ov009_021ace5c.http);
                    data_ov009_021ace5c.param.url = data_ov009_021a2358.url;
                    data_ov009_021ace5c.param.unk_04 = 0;
                    data_ov009_021ace5c.param.bufSize = 0x200;
                    data_ov009_021ace5c.param.alloc = alloc;
                    data_ov009_021ace5c.param.free = free;
                    data_ov009_021ace5c.param.timeout = 20000;
                    if (strcmp(data_ov009_021ace5c.param.url, "https://nas.nintendowifi.net/ac") != 0) {
                        data_ov009_021ace5c.param.unk_14 = 1;
                    }
                    if (DWC_Http_Create(data_ov009_021ace5c.http, &data_ov009_021ace5c.param) != 0) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(1);
                        goto done;
                    }
                    if (!DWC_Auth_GetCalInfoFromNVRAM(calInfo) ||
                        !DWC_Auth_SetCalInfoToHttp(data_ov009_021ace5c.http, calInfo, 1)) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(8);
                        goto done;
                    }
                    if (DWC_Http_Add_PostBase64Item(data_ov009_021ace5c.http, "action", "message", 7) != 0 ||
                        DWC_Http_Add_PostBase64Item(data_ov009_021ace5c.http, "HotSpotResponse",
                                                    data_ov009_021ace5c.work->hotSpotResponse,
                                                    strlen(data_ov009_021ace5c.work->hotSpotResponse)) != 0) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(8);
                        goto done;
                    }
                    free(0, data_ov009_021ace5c.work->hotSpotResponse, 0);
                    data_ov009_021ace5c.work->hotSpotResponse = NULL;
                    if (DWC_Http_FinishHeader(data_ov009_021ace5c.http) != 0) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(1);
                        goto done;
                    }
                    DWC_Http_StartThread(data_ov009_021ace5c.http,
                                         OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
                    if (data_ov009_021ace5c.http->thread.id != 0 &&
                        !OS_IsThreadTerminated(&data_ov009_021ace5c.http->thread)) {
                        OS_JoinThread(&data_ov009_021ace5c.http->thread);
                    }
                    switch (data_ov009_021ace5c.http->error) {
                    case 2:
                        data_ov009_021ace5c.work->returnCode = -1;
                    default:
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(3);
                        goto done;
                    case 8:
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(7);
                        goto done;
                    }
                } else {
                    answer = data_ov009_021ace5c.http->rep.buffer;
                    if (answer == NULL) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(2);
                        goto done;
                    }
                    data_ov009_021ace5c.work->html = alloc(0, strlen(answer) + 1);
                    if (data_ov009_021ace5c.work->html == NULL) {
                        DWC_Http_Destroy(data_ov009_021ace5c.http);
                        DWCi_Netcheck_SetError(4);
                        goto done;
                    }
                    strncpy(data_ov009_021ace5c.work->html, answer, strlen(answer) + 1);
                }
                break;
            default:
                DWC_Http_Destroy(data_ov009_021ace5c.http);
                DWCi_Netcheck_SetError(10);
                goto done;
            }
            break;
        }

        DWC_Http_Destroy(data_ov009_021ace5c.http);
        DWCi_BM_GetWiFiInfo(&wifiInfo);
        if (wifiInfo.attestedUserId == 0) {
            data_ov009_021ace5c.work->returnCode = -3;
            data_ov009_021acea8.ingamesn[0] = 0;
            data_ov009_021acea8.gsbrcd[0] = 0;
            data_ov009_021acea8.alloc = data_ov009_021ace5c.work->funcs.alloc;
            data_ov009_021acea8.free = data_ov009_021ace5c.work->funcs.free;
            if (DWC_Auth_Create(&data_ov009_021acea8, data_ov009_021ace5c.http) != 0) {
                DWCi_Netcheck_SetError(5);
                goto done;
            }
            DWC_Auth_Join();
            if (DWC_Auth_GetError() != 0x15) {
                if (DWC_Auth_GetError() == 9) {
                    data_ov009_021ace5c.work->returnCode = -1;
                } else {
                    DWC_Auth_GetResult(&authResult);
                    if (data_ov009_021ace5c.unk_10 == 1 && (authResult.returncode == -0x5b06 || DWC_Auth_GetError() == 0xb)) {
                        data_ov009_021ace5c.work->returnCode = 0;
                        DWC_Auth_Destroy();
                        DWCi_Netcheck_SetError(0xb);
                        goto done;
                    }
                    data_ov009_021ace5c.work->returnCode = authResult.returncode;
                }
                DWC_Auth_Destroy();
                DWCi_Netcheck_SetError(6);
                goto done;
            }
            DWC_Auth_Destroy();
        }

        if (httpResult == 200) {
            data_ov009_021ace5c.work->returnCode = 0;
            DWCi_Netcheck_SetError(0xb);
            goto done;
        }

        data_ov009_021ace5c.work->returnCode = -4;
        data_ov009_021ace5c.param.url = data_ov009_021a2358.url;
        data_ov009_021ace5c.param.unk_04 = 0;
        data_ov009_021ace5c.param.bufSize = 0x1000;
        data_ov009_021ace5c.param.alloc = alloc;
        data_ov009_021ace5c.param.free = free;
        data_ov009_021ace5c.param.timeout = 40000;
        if (strcmp(data_ov009_021ace5c.param.url, "https://nas.nintendowifi.net/ac") != 0) {
            data_ov009_021ace5c.param.unk_14 = 1;
        }
        if (DWC_Http_Create(data_ov009_021ace5c.http, &data_ov009_021ace5c.param) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        if (!DWC_Auth_GetCalInfoFromNVRAM(calInfo) || !DWC_Auth_SetCalInfoToHttp(data_ov009_021ace5c.http, calInfo, 1)) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(8);
            goto done;
        }
        if (DWC_Http_Add_PostBase64Item(data_ov009_021ace5c.http, "action", "parse", 5) != 0 ||
            DWC_Http_Add_PostBase64Item(data_ov009_021ace5c.http, "HTML", data_ov009_021ace5c.work->html,
                                        strlen(data_ov009_021ace5c.work->html)) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(8);
            goto done;
        }
        free(0, data_ov009_021ace5c.work->html, 0);
        data_ov009_021ace5c.work->html = NULL;
        if (DWC_Http_FinishHeader(data_ov009_021ace5c.http) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        DWC_Http_StartThread(data_ov009_021ace5c.http, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (data_ov009_021ace5c.http->thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021ace5c.http->thread)) {
            OS_JoinThread(&data_ov009_021ace5c.http->thread);
        }
        switch (data_ov009_021ace5c.http->error) {
        case 3:
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            if (data_ov009_021ace5c.unk_10 == 1) {
                data_ov009_021ace5c.work->returnCode = 0;
                DWCi_Netcheck_SetError(0xb);
            } else {
                DWCi_Netcheck_SetError(3);
            }
            goto done;
        case 2:
            data_ov009_021ace5c.work->returnCode = -1;
        default:
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(3);
            goto done;
        case 8:
            break;
        }

        if (DWC_Http_ParseResult(data_ov009_021ace5c.http, 0) != 1) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(2);
            goto done;
        }
        httpResult = atol(DWC_Http_GetResult(data_ov009_021ace5c.http, "httpresult"));
        if (errno == 0x22) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(2);
            goto done;
        }
        if (httpResult != 200) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            if (data_ov009_021ace5c.unk_10 == 1 && httpResult == 302) {
                data_ov009_021ace5c.work->returnCode = 0;
                DWCi_Netcheck_SetError(0xb);
            } else {
                DWCi_Netcheck_SetError(2);
            }
            goto done;
        }
        len = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "returncd", returnCode, 4);
        if (len <= 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        code = atol(returnCode);
        if (errno == 0x22) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        if (data_ov009_021ace5c.unk_10 == 1 && code == 0x72) {
            data_ov009_021ace5c.work->returnCode = 0;
            DWCi_Netcheck_SetError(0xb);
            goto done;
        }
        if (code >= 0x64) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(6);
            goto done;
        }
        urlLen = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "url", NULL, 0);
        if (urlLen <= 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        dataLen = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "data", NULL, 0);
        if (dataLen <= 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        waitLen = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "wait", NULL, 0);
        url = alloc(0, urlLen + 1);
        if (url == NULL) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(4);
            goto done;
        }
        data = alloc(0, dataLen + 1);
        if (data == NULL) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(4);
            goto done;
        }
        if (waitLen > 0) {
            wait = alloc(0, waitLen + 1);
            if (wait == NULL) {
                DWC_Http_Destroy(data_ov009_021ace5c.http);
                DWCi_Netcheck_SetError(4);
                goto done;
            }
        }
        len = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "url", url, urlLen + 1);
        if (len < 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        url[len] = 0;
        len = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "data", data, dataLen + 1);
        if (len < 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(9);
            goto done;
        }
        data[len] = 0;
        sleepMs = 0;
        if (waitLen > 0) {
            len = DWC_Http_GetBase64DecodedResult(data_ov009_021ace5c.http, "wait", wait, waitLen + 1);
            if (len < 0) {
                DWC_Http_Destroy(data_ov009_021ace5c.http);
                DWCi_Netcheck_SetError(9);
                goto done;
            }
            wait[len] = 0;
            sleepMs = atol(wait);
            if (errno == 0x22) {
                DWC_Http_Destroy(data_ov009_021ace5c.http);
                DWCi_Netcheck_SetError(9);
                goto done;
            }
            sleepMs *= 1000;
            if (sleepMs > 180000) {
                sleepMs = 180000;
            }
        }
        DWC_Http_Destroy(data_ov009_021ace5c.http);
        data_ov009_021ace5c.work->returnCode = -5;
        data_ov009_021ace5c.param.url = url;
        data_ov009_021ace5c.param.unk_04 = 0;
        data_ov009_021ace5c.param.bufSize = 0x1000;
        data_ov009_021ace5c.param.alloc = alloc;
        data_ov009_021ace5c.param.free = free;
        data_ov009_021ace5c.param.timeout = 120000;
        if (DWC_Http_Create(data_ov009_021ace5c.http, &data_ov009_021ace5c.param) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        if (DWC_Http_Add_Body(data_ov009_021ace5c.http, data) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(8);
            goto done;
        }
        if (DWC_Http_FinishHeader(data_ov009_021ace5c.http) != 0) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(1);
            goto done;
        }
        DWC_Http_StartThread(data_ov009_021ace5c.http, OS_GetThreadPriority(OSi_ThreadInfo.current) - 1);
        if (data_ov009_021ace5c.http->thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021ace5c.http->thread)) {
            OS_JoinThread(&data_ov009_021ace5c.http->thread);
        }
        switch (data_ov009_021ace5c.http->error) {
        case 2:
            data_ov009_021ace5c.work->returnCode = -1;
        default:
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(3);
            goto done;
        case 8:
            break;
        }
        if (DWC_Http_ParseResult(data_ov009_021ace5c.http, 1) != 1) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(2);
            goto done;
        }
        answer = data_ov009_021ace5c.http->rep.buffer;
        if (answer == NULL) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(2);
            goto done;
        }
        data_ov009_021ace5c.work->hotSpotResponse = alloc(0, strlen(answer) + 1);
        if (data_ov009_021ace5c.work->hotSpotResponse == NULL) {
            DWC_Http_Destroy(data_ov009_021ace5c.http);
            DWCi_Netcheck_SetError(4);
            goto done;
        }
        strncpy(data_ov009_021ace5c.work->hotSpotResponse, answer, strlen(answer) + 1);
        DWC_Http_Destroy(data_ov009_021ace5c.http);
        OS_Sleep(sleepMs);
    }
done:
    if (url != NULL) {
        free(0, url, 0);
    }
    if (data != NULL) {
        free(0, data, 0);
    }
    if (wait != NULL) {
        free(0, wait, 0);
    }
}

void DWCi_Netcheck_SetError(u32 error) {
    OS_LockMutex(&data_ov009_021ace5c.work->mutex);
    data_ov009_021ace5c.work->error = error;
    OS_UnlockMutex(&data_ov009_021ace5c.work->mutex);
}
#pragma thumb on
