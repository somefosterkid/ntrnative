// Black's code at ov009 0x0217c070-0x0217c690. libdwcauth.a's dwc_svl.o: the code posts action=SVCLOC and svc to the
// NAS and parses svchost and servicetoken, as the Svl_Init / GetTokenAsync / Process and ParseHttp functions of the object do.

#include "types.h"
#include "dwc/dwc.h"
#include "dwc/dwc_svl.h"
#include "dwc/http.h"
#include "dwc/auth.h"
#include "string.h"
#include "errno.h"
#include "gs/dwc_nastime.h"
#include "nitro/mi.h"
#include "stdlib.h"

typedef struct DWCSvlResult {
    BOOL status;
    char svlhost[0x41];
    char svltoken[0x12d];
} DWCSvlResult;

typedef struct DWCSvlLocator {
    void *(*alloc)(int, u32);
    void (*free)(int, void *, int);
    DWCHttp http;
    DWCSvlResult *result;
} DWCSvlLocator;

BOOL DWCi_Svl_ParseHttp(void);
static DWCSvlLocator *data_ov009_021acef4;

BOOL DWC_Svl_Init(void *(*alloc)(int, u32), void (*free)(int, void *, int)) {
    if (data_ov009_021acef4 != NULL) {
        DWCi_SetError(9, 0xffffa1dc);
        return 0;
    }
    data_ov009_021acef4 = (*alloc)(0, sizeof(DWCSvlLocator));
    if (data_ov009_021acef4 == NULL) {
        DWCi_SetError(9, 0xffffa1dc);
        return 0;
    }
    MI_CpuFill8(data_ov009_021acef4, 0, sizeof(DWCSvlLocator));
    data_ov009_021acef4->alloc = alloc;
    data_ov009_021acef4->free = free;
    return 1;
}

#pragma thumb off
void DWC_Svl_Cleanup(void) {
    if (data_ov009_021acef4 == NULL) {
        return;
    }
    DWC_Http_Destroy(&data_ov009_021acef4->http);
    data_ov009_021acef4->free(0, data_ov009_021acef4, 0);
    data_ov009_021acef4 = NULL;
}

BOOL DWC_Svl_GetTokenAsync(const char *svl, void *result) {
    DWCHttpParam param;
    u8 cal_info[0x94];
    DWCSvlLocator *locator;

    data_ov009_021acef4->result = result;
    MI_CpuFill8(result, 0, sizeof(DWCSvlResult));
    if (data_ov009_021acef4->http.thread.id != 0 && !OS_IsThreadTerminated(&data_ov009_021acef4->http.thread)) {
        OS_Terminate();
    }
    locator = data_ov009_021acef4;
    param.url = data_ov009_021a2358.url;
    param.unk_04 = 0;
    param.bufSize = 0x1000;
    param.alloc = locator->alloc;
    param.free = locator->free;
    if (strcmp(data_ov009_021a2358.url, "https://nas.nintendowifi.net/ac") != 0) {
        param.unk_14 = 1;
    } else {
        param.unk_14 = 0;
    }
    param.timeout = 20000;
    if (DWC_Http_Create(&locator->http, &param) != 0) {
        DWCi_SetError(9, -24100);
        return FALSE;
    }
    if (!DWC_Auth_GetCalInfoFromNVRAM(cal_info) || !DWC_Auth_SetCalInfoToHttp(&data_ov009_021acef4->http, cal_info, 0)) {
        DWC_Http_Destroy(&data_ov009_021acef4->http);
        DWCi_SetError(9, -24100);
        return FALSE;
    }
    if (DWC_Http_Add_PostBase64Item(&data_ov009_021acef4->http, "action", "SVCLOC", 6) != 0
        || DWC_Http_Add_PostBase64Item(&data_ov009_021acef4->http, "svc", svl, 4) != 0) {
        DWC_Http_Destroy(&data_ov009_021acef4->http);
        DWCi_SetError(9, -24100);
        return FALSE;
    }
    if (DWC_Http_FinishHeader(&data_ov009_021acef4->http) != 0) {
        DWC_Http_Destroy(&data_ov009_021acef4->http);
        DWCi_SetError(9, -24100);
        return FALSE;
    }
    DWC_Http_StartThread(&data_ov009_021acef4->http, OS_GetThreadPriority(OS_GetCurrentThread()) - 1);
    if (data_ov009_021acef4->http.thread.id == 0) {
        DWC_Http_Destroy(&data_ov009_021acef4->http);
        DWCi_SetError(9, -24100);
        return FALSE;
    }
    return TRUE;
}


u32 DWC_Svl_Process(void) {
    if (data_ov009_021acef4 == NULL) {
        return 0;
    }
    if (data_ov009_021acef4->http.thread.id == 0) {
        return 1;
    }
    if (OS_IsThreadTerminated(&data_ov009_021acef4->http.thread) == TRUE) {
        switch (data_ov009_021acef4->http.error) {
        case DWCHTTP_E_FINISH:
            if (DWCi_Svl_ParseHttp() == 0) {
                DWC_Http_Destroy(&data_ov009_021acef4->http);
                return 4;
            }
            DWC_Http_Destroy(&data_ov009_021acef4->http);
            return 3;
        case DWCHTTP_E_ABORT:
            DWC_Http_Destroy(&data_ov009_021acef4->http);
            return 5;
        default:
            DWC_Http_Destroy(&data_ov009_021acef4->http);
            DWCi_SetError(0x10, -24101);
            return 4;
        }
    }
    return 2;
}

BOOL DWCi_Svl_ParseHttp(void) {
    DWCSvlLocator *locator = data_ov009_021acef4;
    char text[4];
    const char *date;
    const char *httpResult;
    long status;
    long returnCode;

    if (DWC_Http_ParseResult(&locator->http, 0) == 0) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    date = DWC_Http_GetResult(&locator->http, "Date");
    if (date != NULL) {
        func_ov009_02168250((char *)date);
    }
    httpResult = DWC_Http_GetResult(&locator->http, "httpresult");
    if (httpResult == NULL) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    status = atol(httpResult);
    if (errno == ERANGE) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    if (status != 200) {
        DWCi_SetError(0xf, -25000 - status);
        return FALSE;
    }
    if (DWC_Http_GetBase64DecodedResult(&locator->http, "returncd", text, 4) <= 0) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    returnCode = atol(text);
    if (errno == ERANGE) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    if (returnCode >= 100) {
        DWCi_SetError(0xf, -24000 - returnCode);
        return FALSE;
    }
    if (DWC_Http_GetBase64DecodedResult(&locator->http, "svchost", data_ov009_021acef4->result->svlhost, 0x40) < 0
        || DWC_Http_GetBase64DecodedResult(&locator->http, "servicetoken", data_ov009_021acef4->result->svltoken, 0x12c) < 0
        || DWC_Http_GetBase64DecodedResult(&locator->http, "statusdata", text, 1) < 0) {
        DWCi_SetError(0xf, -25101);
        return FALSE;
    }
    data_ov009_021acef4->result->status = text[0] == 'Y' ? 1 : 0;
    return TRUE;
}


#pragma thumb on

