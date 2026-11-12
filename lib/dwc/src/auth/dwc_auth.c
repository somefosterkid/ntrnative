// Black's code at ov009 0x02177ec8-0x02178f4c. Black's DWC links libdwcauth.a's dwc_auth.o here: the code does what its
// DWC_Auth_* / DWCi_Auth_* functions do (a thread that posts to the NAS 'ac' URL and parses httpresult, returncd,
// token, challenge, datetime).

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wstring.h>
#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "dwc/auth.h"
#include "dwc/util_wifiidtool.h"
#include "gs/dwc_nastime.h"

// The work DWC_Auth_Create allocates (0x13f4 bytes): the thread's stack, the error, the NAS's answer, the copy of the
// parameters, the HTTP handle the caller gave, the thread, and the mutex over the error and the abort flag
typedef struct DWCAuthWork {
    u8 stack[0x1000];
    u32 unk_1000;
    int error;
    DWCAuthResult result;
    DWCAuthParam param;
    u8 unk_1214[0x100];
    DWCHttp *http;
    OSThread thread;
    OSMutex mutex;
    int aborted;
} DWCAuthWork;

// The file's state: a flag, the work, and a second flag DWC_Auth_Create clears
typedef struct DWCAuthState {
    int unk_0;
    DWCAuthWork *work;
    int unk_8;
} DWCAuthState;

DWCHttpParam data_ov009_021a2358 = { "", 0, 0x1000, NULL, NULL, 0, 20000 };
static char data_ov009_021a2374[] = "https://nas.nintendowifi.net/ac";
// The statics in the order of Black's .bss
static DWCAuthState data_ov009_021ab184;
static DWCWiFiInfo data_ov009_021ab190;

#pragma thumb off
void DWC_Auth_SetCustomNas(const char *nasaddr) {
    data_ov009_021a2358.url = nasaddr;
}
#pragma thumb on

#pragma thumb off
const char *func_ov009_02177ed8(void) {
    return data_ov009_021a2358.url;
}

int DWCi_Auth_Start(int first);
void DWCi_Auth_StartThread(void);
u32 DWC_Auth_Create(DWCAuthParam *param, void *http) {
    DWCHttpAllocFunc alloc = param->alloc;

    if (data_ov009_021ab184.work != NULL) {
        return 2;
    }
    data_ov009_021ab184.work = alloc(0, sizeof(DWCAuthWork));
    if (data_ov009_021ab184.work == NULL) {
        return 2;
    }
    MI_CpuFill8(data_ov009_021ab184.work, 0, sizeof(DWCAuthWork));
    data_ov009_021ab184.work->http = http;
    data_ov009_021ab184.unk_8 = 0;
    MI_CpuFill8(&data_ov009_021ab184.work->result, 0, sizeof(DWCAuthResult));
    data_ov009_021ab184.work->result.returncode = 0x4e84;
    data_ov009_021ab184.work->param = *param;
    data_ov009_021ab184.work->param.ingamesn[DWCAUTH_INGAMESN_LEN - 1] = 0;
    data_ov009_021ab184.work->param.gsbrcd[DWCAUTH_GSBRCDBYTES - 1] = 0;
    data_ov009_021ab184.work->error = DWCi_Auth_Start(1);
    if (data_ov009_021ab184.work->error != 0) {
        return data_ov009_021ab184.work->error;
    }
    data_ov009_021ab184.unk_0 = 0;
    DWCi_Auth_StartThread();
    return 0;
}

void DWCi_Auth_Thread(void *arg);
void DWCi_Auth_StartThread(void) {
    OS_InitMutex(&data_ov009_021ab184.work->mutex);
    data_ov009_021ab184.work->aborted = 0;
    if (data_ov009_021ab184.work->thread.id != 0 && OS_IsThreadTerminated(&data_ov009_021ab184.work->thread) == 0) {
        return;
    }
    OS_CreateThread(&data_ov009_021ab184.work->thread, DWCi_Auth_Thread, &data_ov009_021ab184.work, data_ov009_021ab184.work->stack + sizeof(data_ov009_021ab184.work->stack), sizeof(data_ov009_021ab184.work->stack), 0x10);
    OS_WakeupThreadDirect(&data_ov009_021ab184.work->thread);
}
#pragma thumb reset

#pragma thumb on

#pragma thumb on

#pragma thumb off
void DWC_Auth_Abort(void) {
    if (data_ov009_021ab184.work == NULL) {
        return;
    }
    OS_LockMutex(&data_ov009_021ab184.work->mutex);
    data_ov009_021ab184.work->aborted = 1;
    OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
    if (data_ov009_021ab184.work->http != NULL) {
        DWC_Http_Abort(data_ov009_021ab184.work->http);
    }
    if (data_ov009_021ab184.work->thread.id == 0) {
        return;
    }
    if (OS_IsThreadTerminated(&data_ov009_021ab184.work->thread) != 0) {
        return;
    }
    OS_JoinThread(&data_ov009_021ab184.work->thread);
}
#pragma thumb on

#pragma thumb off
void DWC_Auth_Destroy(void) {
    DWCHttpFreeFunc free;

    if (data_ov009_021ab184.work == NULL) {
        return;
    }
    free = data_ov009_021ab184.work->param.free;
    if (data_ov009_021ab184.work->http != NULL) {
        DWC_Http_Destroy(data_ov009_021ab184.work->http);
    }
    free(0, data_ov009_021ab184.work, 0);
    data_ov009_021ab184.work = NULL;
}
#pragma thumb on

#pragma thumb off
void DWC_Auth_Join(void) {
    if (data_ov009_021ab184.work->thread.id == 0) {
        return;
    }
    if (OS_IsThreadTerminated(&data_ov009_021ab184.work->thread) != 0) {
        return;
    }
    OS_JoinThread(&data_ov009_021ab184.work->thread);
}
#pragma thumb on

#pragma thumb off
u32 DWC_Auth_GetError(void) {
    u32 error;
    if (data_ov009_021ab184.work == NULL) {
        return 0x16;
    }
    OS_LockMutex(&data_ov009_021ab184.work->mutex);
    error = data_ov009_021ab184.work->error;
    OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
    return error;
}

void DWC_Auth_GetResult(DWCAuthResult *param_1) {
    int auth;
    s32 *buf = (s32 *)param_1;
    s32 *origBuf;

    ;
    if (data_ov009_021ab184.work == NULL) {
        MI_CpuFill8(param_1, 0, 0x1c4);
    }
    auth = 0x4e84;
    MI_CpuCopy8(&data_ov009_021ab184.work->result, param_1, 0x1c4);
    origBuf = buf;
    if (0x4e20 - 1 + 1 > *origBuf || *buf >= 0x7530) {
        *buf = 0x5206;
    }
    if (*buf >= auth) {
        *origBuf = -(*origBuf);
    }
}

int DWCi_Auth_Prepare_FirstPost(DWCHttp *http, const char *gsbrcd, const u16 *ingamesn);
static inline void DWCi_Auth_SetHttpAlloc(DWCHttpParam *param, DWCAuthWork *work) {
    param->alloc = work->param.alloc;
    param->free = work->param.free;
}

int DWCi_Auth_Start(int first) {
    DWCAuthWork *work;

    if (strcmp(data_ov009_021a2358.url, data_ov009_021a2374) != 0) {
        data_ov009_021a2358.unk_14 = 1;
    }
    work = data_ov009_021ab184.work;
    data_ov009_021a2358.alloc = work->param.alloc;
    data_ov009_021a2358.free = work->param.free;
    if (DWC_Http_Create(work->http, &data_ov009_021a2358) != 0) {
        return 4;
    }
    if (first == 1) {
        DWCi_BM_GetWiFiInfo(&data_ov009_021ab190);
    }
    data_ov009_021ab184.work->error = DWCi_Auth_Prepare_FirstPost(data_ov009_021ab184.work->http, data_ov009_021ab184.work->param.gsbrcd, data_ov009_021ab184.work->param.ingamesn);
    if (data_ov009_021ab184.work->error != 0) {
        return 4;
    }
    if (DWC_Http_FinishHeader(data_ov009_021ab184.work->http) != 0) {
        return 4;
    }
    DWC_Http_StartThread(data_ov009_021ab184.work->http, OS_GetThreadPriority(OS_GetCurrentThread()) - 1);
    return 0;
}

u32 DWCi_Auth_ParseHttp(void);
void DWCi_Auth_SetError(u32 param_1);
void DWCi_Auth_Thread(void *arg) {
    int retry = 0;
    int first;
    u32 ret;
    OSTick start;

    while (TRUE) {
        if (data_ov009_021ab184.work->http->thread.id != 0 && OS_IsThreadTerminated(&data_ov009_021ab184.work->http->thread) == 0) {
            OS_JoinThread(&data_ov009_021ab184.work->http->thread);
        }
        if (data_ov009_021ab184.work->http->error != 8) {
            data_ov009_021ab184.work->result.returncode = 0x4e84;
            if (data_ov009_021ab184.work->http->error == 7) {
                DWCi_Auth_SetError(0x14);
                return;
            }
            if (retry > 2) {
                if (data_ov009_021ab184.work->http->error == 2) {
                    DWCi_Auth_SetError(9);
                    return;
                }
                if (data_ov009_021ab184.work->http->error == 3) {
                    DWCi_Auth_SetError(0xb);
                    return;
                }
                DWCi_Auth_SetError(0xd);
                return;
            }
            retry++;
            first = 1;
        } else {
            ret = DWCi_Auth_ParseHttp();
            switch (ret) {
                case 0x15:
                    DWCi_Auth_SetError(0x15);
                    return;
                case 0x10:
                    if (retry > 2) {
                        DWCi_Auth_SetError(0x10);
                        return;
                    }
                    retry++;
                    first = 0;
                    break;
                case 0x11:
                    DWCi_Auth_SetError(ret);
                    return;
                default:
                    if (retry >= 2) {
                        DWCi_Auth_SetError(ret);
                        return;
                    }
                    retry++;
                    first = 1;
                    break;
            }
        }
        start = OS_GetTick();
        while (OS_TicksToMilliSeconds(OS_GetTick() - start) < 5000) {
            OS_LockMutex(&data_ov009_021ab184.work->mutex);
            if (data_ov009_021ab184.work->aborted == 1) {
                data_ov009_021ab184.work->result.returncode = 0x4e84;
                OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
                DWCi_Auth_SetError(0x14);
                return;
            }
            OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
            OS_Sleep(5000);
        }
        DWC_Http_Destroy(data_ov009_021ab184.work->http);
        OS_LockMutex(&data_ov009_021ab184.work->mutex);
        data_ov009_021ab184.work->error = DWCi_Auth_Start(first);
        if (data_ov009_021ab184.work->error != 0) {
            data_ov009_021ab184.work->result.returncode = 0x4e84;
            OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
            return;
        }
        OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
    }
}


u32 DWCi_Auth_FillResult(void);
u32 DWCi_Auth_ParseHttp(void) {
    DWCHttpAllocFunc alloc = data_ov009_021ab184.work->param.alloc;
    DWCHttpFreeFunc free = data_ov009_021ab184.work->param.free;
    void *wifi_id;

    if (DWC_Http_ParseResult(data_ov009_021ab184.work->http, 0) != 1) {
        data_ov009_021ab184.work->result.returncode = 0x4e84;
        return 0xe;
    }

    if (DWCi_Auth_FillResult() != 0) {
        return 0xe;
    }

    if (data_ov009_021ab184.work->result.returncode < 0x4e84) {
        if (data_ov009_021ab184.work->result.returncode == 0x4e22) {
            wifi_id = alloc(0, 0x101f);
            if (wifi_id == 0) {
                data_ov009_021ab184.work->result.returncode = 0x4e84;
                return 2;
            }
            if (DWCi_AUTH_UpDateWiFiID(&data_ov009_021ab190, (void *)(((uintptr_t)wifi_id + 0x1f) & ~0x1f)) != 1) {
                free(0, wifi_id, 0);
                data_ov009_021ab184.work->result.returncode = 0x4e84;
                return 0xf;
            }
            free(0, wifi_id, 0);
        }
        return 0x15;
    }

    switch (data_ov009_021ab184.work->result.returncode) {
        case 0x4e88:
            DWCi_AUTH_RemakeWiFiID(&data_ov009_021ab190);
            data_ov009_021ab184.work->result.returncode = 0x4e88;
            return 0x10;
        case 0x4e8c:
            wifi_id = alloc(0, 0x1000);
            if (wifi_id == 0) {
                data_ov009_021ab184.work->result.returncode = 0x4e8c;
                return 0x11;
            }
            DWCi_AUTH_MakeWiFiID((void *)(((uintptr_t)wifi_id + 0x1f) & ~0x1f));
            free(0, wifi_id, 0);
            data_ov009_021ab184.work->result.returncode = 0x4e8c;
            return 0x11;
        default:
            return 0x12;
    }
}

u32 DWCi_Auth_FillResult(void) {
    char *end;
    int code;
    int ret;
    char *str;
    DWCAuthWork *work;

    end = NULL;
    end = DWC_Http_GetResult(data_ov009_021ab184.work->http, "Date");
    if (end != NULL) {
        func_ov009_02168250(end);
    }
    end = NULL;
    errno = 0;
    str = DWC_Http_GetResult(data_ov009_021ab184.work->http, "httpresult");
    code = atol(str);
    if (errno == 0x22) {
        data_ov009_021ab184.work->result.returncode = 0x4e85;
        return 0xc;
    }
    if (code != 200) {
        data_ov009_021ab184.work->result.returncode = code + 23000;
        return 0x12;
    }
    if (DWC_Http_GetBase64DecodedResult(data_ov009_021ab184.work->http, "returncd", data_ov009_021ab184.work->result.retcode, 4) <= 0) {
        data_ov009_021ab184.work->result.returncode = 0x4e85;
        return 0xe;
    }
    ret = strtol(data_ov009_021ab184.work->result.retcode, &end, 10);
    work = data_ov009_021ab184.work;
    if (end != work->result.retcode + strlen(work->result.retcode)) {
        work->result.returncode = 0x4e85;
        return 0xc;
    }
    work->result.returncode = ret + 20000;
    if (ret < 100) {
        data_ov009_021ab184.work->result.token[0] = 0;
        data_ov009_021ab184.work->result.locator[0] = 0;
        data_ov009_021ab184.work->result.challenge[0] = 0;
        data_ov009_021ab184.work->result.datetime[0] = 0;
        data_ov009_021ab184.work->result.cookie[0] = 0;
        DWC_Http_GetBase64DecodedResult(data_ov009_021ab184.work->http, "token", data_ov009_021ab184.work->result.token, 0x12d);
        DWC_Http_GetBase64DecodedResult(data_ov009_021ab184.work->http, "locator", data_ov009_021ab184.work->result.locator, 0x33);
        DWC_Http_GetBase64DecodedResult(data_ov009_021ab184.work->http, "challenge", data_ov009_021ab184.work->result.challenge, 9);
        DWC_Http_GetBase64DecodedResult(data_ov009_021ab184.work->http, "datetime", data_ov009_021ab184.work->result.datetime, 0xf);
        DWC_Http_GetRawResult(data_ov009_021ab184.work->http, "Set-Cookie", data_ov009_021ab184.work->result.cookie, 0x41);
        data_ov009_021ab184.work->result.cookie[0x2b] = 0;
        if (ret != 0x28) {
            data_ov009_021ab184.unk_8 = 1;
        } else {
            data_ov009_021ab184.unk_8 = 2;
        }
    }
    return 0;
}


int DWCi_Auth_Prepare_FirstPost(DWCHttp *param_1, const char *param_2, const u16 *param_3) {
    u8 cal_info[0x94];

    if (data_ov009_021ab190.attestedUserId == 0) {
        if (DWC_Http_Add_PostBase64Item(param_1, "action", "acctcreate", strlen((char *)"acctcreate")) != 0) {
            return 8;
        }
    } else {
        if (DWC_Http_Add_PostBase64Item(param_1, "action", "login", strlen((char *)"login")) != 0) {
            return 8;
        }
        if (DWC_Http_Add_PostBase64Item(param_1, "gsbrcd", param_2, strlen((char *)param_2)) != 0) {
            return 8;
        }
    }

    if (DWC_Auth_GetCalInfoFromWiFiInfo((NasAuthInfo *)cal_info, &data_ov009_021ab190) == 0) {
        return 5;
    }

    if (DWC_Auth_SetCalInfoToHttp(param_1, cal_info, data_ov009_021ace70) == 0) {
        return 8;
    }

    if (data_ov009_021ab184.unk_0 == 1) {
        if (DWC_Http_Add_PostBase64Item(param_1, "iswfc", "Y", strlen((char *)"Y")) != 0) {
            return 8;
        }
    }

    if (wcslen(param_3) != 0) {
        if (DWC_Http_Add_PostBase64Item(param_1, "ingamesn", param_3, wcslen(param_3) * 2) != 0) {
            return 8;
        }
    }

    return 0;
}
#pragma thumb on

#pragma thumb reset

#pragma thumb reset

#pragma thumb reset

#pragma thumb on

#pragma thumb off
void DWCi_Auth_SetError(u32 param_1) {
    OS_LockMutex(&data_ov009_021ab184.work->mutex);
    data_ov009_021ab184.work->error = param_1;
    OS_UnlockMutex(&data_ov009_021ab184.work->mutex);
}

u32 DWC_SetAuthServer(u32 param_1) {
    const char *url;

    switch (param_1) {
        case 0:
            url = "https://nas.test.nintendowifi.net/ac";
            DWC_Auth_SetCustomNas(url);
            break;
        case 2:
            url = "https://nas.dev.nintendowifi.net/ac";
            DWC_Auth_SetCustomNas(url);
            break;
        case 1:
            url = "https://nas.nintendowifi.net/ac";
            DWC_Auth_SetCustomNas(url);
            break;
        default:
            return 0;
    }
    return 1;
}
#pragma thumb on

#pragma thumb on
