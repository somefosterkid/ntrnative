// Black's code at ov009 0x0217b828-0x0217c070. libdwcauth.a's dwc_prof.o: the code posts wtype / words / wregion / wenc
// UTF-16LE to the NAS '/pr' and parses 'prwords', as the CheckProfanity and Prof_ParseResponse functions of
// the object do; the two 0x10 allocators at the end are its Prof_Alloc / Prof_Free.

#include "types.h"
#include "dwc/dwc.h"
#include "system/version.h"
#include "dwc/http.h"
#include "dwc/auth.h"
#include "string.h"
#include "wstring.h"
#include "stdlib.h"
#include "errno.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "gs/dwc_nastime.h"
#include "dwc/ac.h"

// The request's work: the HTTP handle, then the URL and where the answer goes
typedef struct DWCProfWork {
    DWCHttp http;
    BOOL httpCreated;
    char url[0x101];
    char *result;
    int *badWordCount;
    int wordCount;
} DWCProfWork;

BOOL DWCi_CheckProfanityAsync(const u16 **words, int wordCount, const char *reserved, int timeout, char *result, int *badWordCount, const char *region);
BOOL DWCi_Prof_ParseResponse(void);
void *DWCi_Prof_Alloc(int name, u32 size);
void DWCi_Prof_Free(int name, void *ptr, int size);

static DWCProfWork *data_ov009_021acef0 = NULL;

extern DWCHttpParam data_ov009_021a2358;

BOOL DWCi_CheckProfanityAsync(const u16 **words, int wordCount, const char *reserved, int timeout, char *result,
                              int *badWordCount, const char *region) {
    DWCHttpParam param;
    u8 calInfo[0x94];
    int total;
    int i;
    int pos;
    u16 *body = NULL;
    char *str;

    if (words == NULL || wordCount <= 0 || wordCount > 50 || result == NULL || badWordCount == NULL) {
        return FALSE;
    }
    total = 0;
    for (i = 0; i < wordCount; i++) {
        total += wcslen(words[i]) + 1;
    }
    if (total - 1 <= 0 || total - 1 > 500) {
        return FALSE;
    }
    if (data_ov009_021acef0 != NULL) {
        return FALSE;
    }
    MI_CpuFill8(result, 0, wordCount);
    data_ov009_021acef0 = DWC_Alloc(0, sizeof(DWCProfWork));
    if (data_ov009_021acef0 == NULL) {
        goto error;
    }
    MI_CpuFill8(data_ov009_021acef0, 0, sizeof(DWCProfWork));
    data_ov009_021acef0->httpCreated = FALSE;
    MI_CpuFill8(calInfo, 0, sizeof(calInfo));
    data_ov009_021acef0->result = result;
    data_ov009_021acef0->badWordCount = badWordCount;
    data_ov009_021acef0->wordCount = wordCount;
    strncpy(data_ov009_021acef0->url, data_ov009_021a2358.url, 0x100);
    data_ov009_021acef0->url[0x100] = '\0';
    str = strstr(data_ov009_021acef0->url, "/ac\0");
    if (str == NULL) {
        goto error;
    }
    MI_CpuCopy8("/pr", str, 4);
    param.url = data_ov009_021acef0->url;
    param.unk_04 = 0;
    param.bufSize = 0x400;
    param.alloc = DWCi_Prof_Alloc;
    param.free = DWCi_Prof_Free;
    param.unk_14 = 0;
    if (timeout <= 0) {
        param.timeout = 10000;
    } else {
        param.timeout = timeout;
    }
    if (DWC_Http_Create(&data_ov009_021acef0->http, &param) != 0) {
        goto error;
    }
    data_ov009_021acef0->httpCreated = TRUE;
    if (!DWC_Auth_GetCalInfoFromNVRAM(calInfo)) {
        goto error;
    }
    if (!DWC_Auth_SetCalInfoToHttp(&data_ov009_021acef0->http, calInfo, data_ov009_021ace70)) {
        goto error;
    }
    if (DWC_Http_Add_PostBase64Item(&data_ov009_021acef0->http, "wregion", region, 1) != 0) {
        goto error;
    }
    if (DWC_Http_Add_PostBase64Item(&data_ov009_021acef0->http, "wenc", "UTF-16LE", 8) != 0) {
        goto error;
    }
    if (reserved != NULL && DWC_Http_Add_PostBase64Item(&data_ov009_021acef0->http, "wtype", reserved, 4) != 0) {
        goto error;
    }
    if (DWC_Http_Add_Body(&data_ov009_021acef0->http, "&wtype=") != 0) {
        goto error;
    }
    pos = 0;
    body = DWC_Alloc(0, total * 2);
    if (body == NULL) {
        goto error;
    }
    for (i = 0; i < wordCount; i++) {
        wcscpy(&body[pos], words[i]);
        pos += wcslen(words[i]);
        if (pos >= total - 1) {
            break;
        }
        body[pos] = '\t';
        pos++;
    }
    if (DWC_Http_Add_PostBase64Item(&data_ov009_021acef0->http, "words", (const char *)body, (total - 1) * 2) != 0) {
        goto error;
    }
    DWC_Free(0, body, 0);
    body = NULL;
    if (DWC_Http_FinishHeader(&data_ov009_021acef0->http) != 0) {
        goto error;
    }
    DWC_Http_StartThread(&data_ov009_021acef0->http, OS_GetThreadPriority(OS_GetCurrentThread()) - 1);
    if (data_ov009_021acef0->http.thread.id != 0) {
        return TRUE;
    }

error:
    if (body != NULL) {
        DWC_Free(0, body, 0);
    }
    if (data_ov009_021acef0 != NULL) {
        if (data_ov009_021acef0->httpCreated == TRUE) {
            DWC_Http_Destroy(&data_ov009_021acef0->http);
        }
        DWC_Free(0, data_ov009_021acef0, 0);
        data_ov009_021acef0 = NULL;
    }
    return FALSE;
}

BOOL func_ov009_0217bc44(const u16 **words, int count, const char *reserved, int timeout, char *result, int *badWordCount, int region) {
    BOOL BVar1;
    char asStack_c[4];
    s32 sStack_8;

    if (region == 0x80) {
        asStack_c[0] = 0x41;
    }
    else {
        asStack_c[0] = *(s8 *)0x02fffe0f;
    }
    sStack_8 = timeout;
    BVar1 = DWCi_CheckProfanityAsync(words, count, reserved, timeout, result, badWordCount, asStack_c);
    return BVar1;
}

int DWC_CheckProfanityProcess(void) {
    int result;
    u32 code;

    if (data_ov009_021acef0 == NULL || data_ov009_021acef0->http.thread.id == 0) {
        return 0;
    }
    if (OS_IsThreadTerminated(&data_ov009_021acef0->http.thread) == TRUE) {
        code = DWC_AC_GetApType();
        if (code == 0xff || code > 99) {
            code = 99;
        }
        switch (data_ov009_021acef0->http.error) {
        case DWCHTTP_E_FINISH:
            if (!DWCi_Prof_ParseResponse()) {
                result = 3;
            } else {
                result = 2;
            }
            break;
        case DWCHTTP_E_DNSERR:
            DWCi_SetError(8, -52100 - code);
            result = 3;
            break;
        case DWCHTTP_E_CONERR:
        case 4:
        case DWCHTTP_E_RECVTOUT:
            DWCi_SetError(0x12, -33010);
            result = 3;
            break;
        default:
            DWCi_SetError(0x12, -33030);
            result = 3;
            break;
        }
    } else {
        return 1;
    }
    DWC_Http_Destroy(&data_ov009_021acef0->http);
    DWC_Free(0, data_ov009_021acef0, 0);
    data_ov009_021acef0 = NULL;
    return result;
}


BOOL DWCi_Prof_ParseResponse(void) {
    char *names[8] = { "prwordsJ", "prwordsE", "prwordsP", "prwordsK", "prwordsC" };
    BOOL hasRegion = FALSE;
    u32 i;
    DWCHttp *http = &data_ov009_021acef0->http;
    char *str;
    long value;
    char returnCode[4];
    char flags[0x33];
    int len;
    int j;

    if (DWC_Http_ParseResult(http, 0) == 0) {
        goto error;
    }
    str = DWC_Http_GetResult(http, "Date");
    if (str != NULL) {
        func_ov009_02168250(str);
    }
    str = DWC_Http_GetResult(http, "httpresult");
    if (str == NULL) {
        goto error;
    }
    value = atol(str);
    if (errno == ERANGE || value != 200) {
        goto error;
    }
    if (DWC_Http_GetBase64DecodedResult(http, "returncd", returnCode, 4) <= 0) {
        goto error;
    }
    value = atol(returnCode);
    if (errno == ERANGE) {
        goto error;
    }
    if (value == 1 || (value >= 100 && value != 100)) {
        DWCi_SetError(0x11, -33000 - value);
        return FALSE;
    }
    for (i = 0; i < 7; i++) {
        if (names[i] == NULL) {
            continue;
        }
        len = DWC_Http_GetBase64DecodedResult(http, names[i], flags, 0x33);
        if (len < 0) {
            goto error;
        }
        if (len == 0) {
            continue;
        }
        hasRegion = TRUE;
        if (len < data_ov009_021acef0->wordCount) {
            goto error;
        }
        for (j = 0; j < data_ov009_021acef0->wordCount; j++) {
            if (flags[j] == '1') {
                data_ov009_021acef0->result[j] |= (char)(1 << i);
            }
        }
    }
    len = DWC_Http_GetBase64DecodedResult(http, "prwords", flags, 0x33);
    if (len < 0 || len == 0) {
        goto error;
    }
    if (len < data_ov009_021acef0->wordCount) {
        goto error;
    }
    *data_ov009_021acef0->badWordCount = 0;
    for (j = 0; j < data_ov009_021acef0->wordCount; j++) {
        if (flags[j] == '1') {
            (*data_ov009_021acef0->badWordCount)++;
            if (hasRegion) {
                data_ov009_021acef0->result[j] |= 0x80;
            } else {
                data_ov009_021acef0->result[j] = 1;
            }
        }
    }
    return TRUE;

error:
    DWCi_SetError(0x11, -33020);
    return FALSE;
}

#pragma thumb on

#pragma thumb off
void *DWCi_Prof_Alloc(int name, u32 size) {
    return DWC_Alloc(0, size);
}
#pragma thumb on

#pragma thumb off
void DWCi_Prof_Free(int name, void *ptr, int size) {
    DWC_Free(0, ptr, size);
}
#pragma thumb on
