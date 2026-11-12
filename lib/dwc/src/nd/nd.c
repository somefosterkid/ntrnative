// Black's code at ov114 0x021c5e78-0x021c7ba0 (dwc_nd.c, split from it, holds 0x021c569c-0x021c5e78). Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.
// It is NitroDWC's libdwcnd.a, its 13 members (attr, base64, bgnend, ca, cancel, dwc_nitro, fileapi, filecount, fileget,
// filelist, os_nitro, stdlib, string) in archive order as one translation unit: their .rodata, .data and .bss are one
// sorted run each, so the order of the definitions below (the prebuilt's names) gives Black's layout.

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
#include "dwc/apinfo.h"
#include "dwc/nd_internal.h"
#include "dwc/nhttp_internal.h"
#include "dwc/error.h"

void DWCi_BM_GetWiFiInfo(DWCWiFiInfo *param_1);

// A certificate authority that the server's certificate is checked against (NitroWiFi's CPSCaInfo, socl.h's
// SOCLCaInfo)
typedef struct CPSCaInfo {
    char *dn;
    int modulus_len;
    u8 *modulus;
    int exponent_len;
    u8 *exponent;
} CPSCaInfo;
typedef CPSCaInfo SOCLCaInfo;

// The library's state, which its files share (DWCi_Nd_bgnend.c defines it; the prebuilt's names)
extern void *(*DWCi_Ndi_alloc)(u32 size, s32 align);
extern void (*DWCi_Ndi_free)(void *ptr);
extern void (*DWCi_Ndi_dlcallback)(u32 reason, u32 error, s32 code);
extern s32 DWCi_Ndi_isOpened;
extern s32 DWCi_Ndi_isExistApinfo;
extern s32 DWCi_Ndi_nhttpid;
extern s8 *DWCi_Ndi_authtoken;
extern s8 *DWCi_Ndi_serverURL;
extern s8 DWCi_Ndi_dwcgamecd[9];
extern s8 *DWCi_Ndi_dlattr[3];
extern s8 DWCi_Ndi_macaddr[17];
extern s8 DWCi_Ndi_strApinfo[21];
extern s8 DWCi_Ndi_password[25];
extern s8 DWCi_Ndi_userAgent[33];
extern SOCLCaInfo dwci_nd_ca_nasself;

#pragma thumb off
// DWCi_Nd_attr.c
void func_ov114_021c5e78(void) {
    s8 **attrs = DWCi_Ndi_dlattr;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (attrs[i] != NULL) {
            DWCi_Ndi_free(attrs[i]);
            attrs[i] = NULL;
        }
    }
}

u32 func_ov114_021c5ebc(s32 param_1, s8 *param_2) {
    size_t result;
    u8 *ptr;

    if (*param_2 != 0) {
        result = DWCi_Ndi_strcmp(param_2);
        if (10 < (s32)result) {
            func_ov114_021c659c(5, 0);
            return 0;
        }
        result = DWCi_Ndi_strcmp(param_2);
        if (10 < (s32)result) {
            func_ov114_021c659c(5, 0);
            return 0;
        }
        ptr = DWCi_Ndi_alloc(DWCi_Ndi_getEncodeLenBASE64(result) + 1, 4);
        DWCi_Ndi_dlattr[param_1] = (s8 *)ptr;
        if (ptr == NULL) {
            func_ov114_021c659c(1, 0);
            return 0;
        }
                ;
                DWCi_Ndi_dlattr[param_1][DWCi_Ndi_encodeBASE64(ptr, param_2, result)] = 0;
    }
    return 1;
}

u32 func_ov114_021c5f90(s8 *param_1, s8 *param_2, s8 *param_3) {
    s32 result;

    DWCi_Ndi_lockGlobal();
    func_ov114_021c5e78();
    result = func_ov114_021c5ebc(0, param_1);
    if (result != 0) {
        result = func_ov114_021c5ebc(1, param_2);
        if (result != 0) {
            result = func_ov114_021c5ebc(2, param_3);
            if (result != 0) {
                DWCi_Ndi_unlockGlobal();
                return 1;
            }
        }
    }
    func_ov114_021c5e78();
    DWCi_Ndi_unlockGlobal();
    return 0;
}

// DWCi_Nd_base64.c
static const s8 szBASE64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.-";

s32 func_ov114_021c6004(s8 *dst, s8 *p, s32 n) {
    s32 i;
    s32 j;
    s8 *str;
    s8 *str2;
    s8 val;

    str = dst;
    str2 = p;
    j = 0;
    i = 0;
    if (n > 0) {
        do {
            val = *str2;
            i += 1;
            if (val != 0) {
                *str = val;
                str += 1;
                j += 1;
                str2 += 1;
            } else {
                *str = 0;
                str += 1;
            }
        } while (i < n);
    }
    return j;
}

s32 decodeChar(u32 c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }
    if (c == '.') {
        return 62;
    }
    if (c == '-') {
        return 63;
    }
    return -1;
}

u32 DWCi_Ndi_getDecodeLenBASE64(s32 len) {
    return (len >> 2) * 3;
}

s32 DWCi_Ndi_getEncodeLenBASE64(s32 len) {
    return ((len + 2) / 3) * 4;
}

s32 DWCi_Ndi_encodeBASE64(u8 *encstr, s8 *srcbin, s32 length) {
    u8 c[3];
    u8 base0, base1, base2, base3;
    s32 n;
    s32 len = 0;
    static const s8 pad = '*';

    n = func_ov114_021c6004(c, srcbin, length > 3 ? 3 : length);
    while (n > 0) {
        if (length < 3) {
            c[2] = 0;
        }
        if (length < 2) {
            c[1] = 0;
        }
        base0 = c[0] >> 2;
        base1 = ((c[0] << 4) | (c[1] >> 4)) & 0x3f;
        base2 = ((c[1] << 2) | (c[2] >> 6)) & 0x3f;
        base3 = c[2] & 0x3f;
        *encstr++ = szBASE64[base0];
        *encstr++ = szBASE64[base1];
        *encstr++ = n > 1 ? szBASE64[base2] : pad;
        *encstr++ = n > 2 ? szBASE64[base3] : pad;
        srcbin += n;
        len += 4;
        length -= n;
        if (length == 0) {
            break;
        }
        n = func_ov114_021c6004(c, srcbin, length > 3 ? 3 : length);
    }
    *encstr = 0;
    return len;
}

s32 DWCi_Ndi_decodeBASE64(u8 *dest, s8 *src, s32 srcSize) {
    u8 in[4];
    char bytes[3];
    int value;
    int n;
    int length = 0;

    n = func_ov114_021c6004(in, src, 4);
    value = 0;
    while (n > 0 && value >= 0) {
        int i;

        bytes[0] = 0;
        bytes[1] = 0;
        bytes[2] = 0;
        value = decodeChar(in[0]);
        if (value >= 0) {
            bytes[0] = value << 2;
            value = decodeChar(in[1]);
            if (value >= 0) {
                bytes[0] |= value >> 4;
                bytes[1] = value << 4;
                value = decodeChar(in[2]);
                if (value >= 0) {
                    bytes[1] |= value >> 2;
                    bytes[2] = value << 6;
                    value = decodeChar(in[3]);
                    if (value >= 0) {
                        bytes[2] |= value;
                    }
                }
            }
        }
        for (i = 0; i < n - 1; i++) {
            *dest = bytes[i];
            dest++;
        }
        src += n;
        length += n - 1;
        srcSize -= 4;
        if (srcSize <= 0) {
            break;
        }
        n = func_ov114_021c6004(in, src, 4);
    }
    *dest = '\0';
    return length;
}

// DWCi_Nd_bgnend.c
// No reference gives the order of these definitions (the prebuilt's version has DWCi_Ndi_error, which Black's
// lacks): this one, with fileapi's and os_NITRO's, lays the .bss out as Black's.
static void (*cleanupCallback)(void);
void *(*DWCi_Ndi_alloc)(u32 size, s32 align);
s32 DWCi_Ndi_nhttpid;
s32 DWCi_Ndi_isExistApinfo;
s8 *DWCi_Ndi_serverURL;
void (*DWCi_Ndi_dlcallback)(u32 reason, u32 error, s32 code);
void (*DWCi_Ndi_free)(void *ptr);
s32 DWCi_Ndi_isOpened;
s8 *DWCi_Ndi_authtoken;
s8 DWCi_Ndi_macaddr[17];
s8 *DWCi_Ndi_dlattr[3];
s8 DWCi_Ndi_strApinfo[21];
s8 DWCi_Ndi_password[25];
s8 DWCi_Ndi_userAgent[33];
s8 DWCi_Ndi_dwcgamecd[9];

void *strAlloc(s8 *src) {
    u32 len;
    void *result;

    len = DWCi_Ndi_strcmp(src);
    result = DWCi_Ndi_alloc(len + 1, 4);
    if (result != NULL) {
        DWCi_Ndi_memcpy(result, src, len);
        *(u8 *)((s32)result + len) = 0;
    }
    return result;
}

BOOL func_ov114_021c6348(NHttpAllocFunc alloc, NHttpFreeFunc free, s8 *url, s8 *token, s8 *gameCode, s8 *password, void (*onResult)(u32, u32, s32)) {
    DWCi_Ndi_alloc = alloc;
    DWCi_Ndi_free = free;
    DWCi_Ndi_dlcallback = onResult;
    DWCi_Ndi_dlattr[0] = NULL;
    DWCi_Ndi_dlattr[1] = NULL;
    DWCi_Ndi_dlattr[2] = NULL;
    DWCi_Ndi_isExistApinfo = FALSE;
    DWCi_Ndi_nhttpid = 0;
    DWCi_Ndi_authtoken = NULL;
    DWCi_Ndi_serverURL = NULL;
    func_ov114_021c79c0(DWCi_Ndi_macaddr);
    DWCi_Ndi_getUserAgent(DWCi_Ndi_userAgent);
    DWCi_Ndi_serverURL = strAlloc(url);
    if (DWCi_Ndi_serverURL != NULL) {
        DWCi_Ndi_authtoken = strAlloc(token);
        if (DWCi_Ndi_authtoken != NULL) {
            DWCi_Ndi_dwcgamecd[DWCi_Ndi_encodeBASE64((u8 *)DWCi_Ndi_dwcgamecd, gameCode, 4)] = '\0';
            DWCi_Ndi_password[DWCi_Ndi_encodeBASE64(DWCi_Ndi_password, password, 16)] = '\0';
            if (DWCi_Ndi_initHttpCallback() && DWCi_Ndi_initLockGlobal()) {
                if (func_ov114_021be648(alloc, free, 17) != -1) {
                    DWCi_Ndi_isExistApinfo = DWCi_Ndi_getApInfo(DWCi_Ndi_strApinfo);
                    DWCi_Ndi_isOpened = TRUE;
                    return TRUE;
                }
                DWCi_Ndi_exitLockGlobal();
            }
            DWCi_Ndi_free(DWCi_Ndi_authtoken);
            DWCi_Ndi_free(DWCi_Ndi_serverURL);
            func_ov114_021c659c(8, 0);
            return FALSE;
        }
    }
    if (DWCi_Ndi_authtoken != NULL) {
        DWCi_Ndi_free(DWCi_Ndi_authtoken);
    }
    if (DWCi_Ndi_serverURL != NULL) {
        DWCi_Ndi_free(DWCi_Ndi_serverURL);
    }
    func_ov114_021c659c(1, 0);
    return FALSE;
}

void func_ov114_021c64e0(void) {
    func_ov114_021c5e78();
    DWCi_Ndi_exitLockGlobal();
    DWCi_Ndi_exitHttpCallback();
    DWCi_Ndi_free(DWCi_Ndi_authtoken);
    DWCi_Ndi_free(DWCi_Ndi_serverURL);
    cleanupCallback();
}

void func_ov114_021c651c(void (*callback)(void)) {
    DWCi_Ndi_isOpened = FALSE;
    cleanupCallback = callback;
    func_ov114_021c654c(DWCi_Ndi_nhttpid);
    func_ov114_021be680(func_ov114_021c64e0);
}

static inline BOOL destroyNhttp(s32 *req) {
    func_ov114_021bf3bc(*req);
    if (func_ov114_021bf184(*req) != 0) {
        return FALSE;
    }
    *req = 0;
    return TRUE;
}

/* FAKEMATCH(volatile): the original keeps param_1 in its stack home (stmdb {r0-r3}, reloads, the dead
   store kept); what did that in Nintendo's source is unknown (its address passed to a call?). pokebw2's
   ReleaseRequest has the same code and row (docs/nonmatching-functions.md: `...` with and without va_start,
   `int *handle = &request`, a static inline taking `int *`, a struct parameter all fail; Black 1's aletheia writes
   `void *volatile handle`); round 37: the address passed to the static inline is propagated away (18 lines) */
BOOL func_ov114_021c654c(volatile s32 param_1) {
    BOOL result = TRUE;

    DWCi_Ndi_lockGlobal();
    if (param_1 != 0) {
        func_ov114_021bf3bc(param_1);
        if (func_ov114_021bf184(param_1) != 0) {
            result = FALSE;
        } else {
            param_1 = 0;
        }
    }
    DWCi_Ndi_unlockGlobal();
    return result;
}

void func_ov114_021c659c(u32 param_1, s32 param_2) {
    switch (param_1) {
    case 0:
    case 6:
        break;
    case 1:
        DWCi_SetError(9, -31001);
        break;
    case 3:
        DWCi_SetError(14, -31020);
        break;
    case 4:
        DWCi_SetError(13, -31040);
        break;
    case 7:
        DWCi_SetError(13, -31000 - param_2);
        break;
    case 5:
        DWCi_SetError(13, -31040);
        break;
    case 8:
        DWCi_SetError(9, -31009);
        break;
    case 2:
        DWCi_SetError(13, -31010);
        break;
    default:
        DWCi_SetError(9, -31009);
        break;
    }
}

// DWCi_Nd_ca.c: Nintendo's certificate authority
static u8 ca_exponent[3] = { 0x01, 0x00, 0x01 };
static s8 ca_name[79] = "US, Washington, Nintendo of America Inc, NOA, Nintendo CA, ca@noa.nintendo.com";
static u8 ca_modulo[128] = {
    0xb3, 0xcd, 0x79, 0x97, 0x77, 0x5d, 0x8a, 0xaf, 0x86, 0xa8, 0xe8, 0xd7, 0x73, 0x1c, 0x77, 0xdf,
    0x10, 0x90, 0x1f, 0x81, 0xf8, 0x41, 0x9e, 0x21, 0x55, 0xdf, 0xbc, 0xfc, 0x63, 0xfb, 0x19, 0x43,
    0xf1, 0xf6, 0xc4, 0x72, 0x42, 0x49, 0xbd, 0xad, 0x44, 0x68, 0x4e, 0xf3, 0xda, 0x1d, 0xe6, 0x4d,
    0xd8, 0xf9, 0x59, 0x88, 0xdc, 0xae, 0x3e, 0x9b, 0x38, 0x09, 0xca, 0x7f, 0xff, 0xdc, 0x24, 0xa2,
    0x44, 0x78, 0x78, 0x49, 0x93, 0xd4, 0x84, 0x40, 0x10, 0xb8, 0xec, 0x3e, 0xdb, 0x2d, 0x93, 0xc8,
    0x11, 0xc8, 0xfd, 0x78, 0x2d, 0x61, 0xad, 0x31, 0xae, 0x86, 0x26, 0xb0, 0xfd, 0x5a, 0x3f, 0xa1,
    0x3d, 0xbf, 0xe2, 0x4b, 0x49, 0xec, 0xce, 0x66, 0x98, 0x58, 0x26, 0x12, 0xc0, 0xfb, 0xf4, 0x77,
    0x65, 0x1b, 0xea, 0xfb, 0xcb, 0x7f, 0xe0, 0x8c, 0xcb, 0x02, 0xa3, 0x4e, 0x5e, 0x8c, 0xea, 0x9b,
};
SOCLCaInfo dwci_nd_ca_nasself = {
    (char *)ca_name,
    sizeof(ca_modulo),
    ca_modulo,
    sizeof(ca_exponent),
    ca_exponent,
};

// DWCi_Nd_cancel.c
BOOL func_ov114_021c666c(void) {
    u32 sp4;
    u32 sp0;
    s32 val;
    s32 flag;

    if (DWCi_Ndi_isOpened == 0) {
        return 0;
    }
    flag = 0;
    sp4 = 0;
    sp0 = 0;
    func_ov114_021c5e14(&sp4, &sp0);
    if (sp4 == sp0) {
        return 0;
    }
    if (DWCi_Ndi_prepareHttpCallback() == 0) {
        DWCi_Ndi_lockGlobal();
        val = DWCi_Ndi_nhttpid;
        DWCi_Ndi_unlockGlobal();
        if (func_ov114_021bf374(val) == 0) {
            flag = 1;
        }
        return flag;
    }
    DWCi_Ndi_unprepareHttpCallback();
    return 0;
}

u32 DWCi_Ndi_getApInfo(u8 *apinfo) {
    s8 strAp[13] = "00:0000000-00";
    static DWCApInfo ap;

    if (func_0205cd24(&ap)) {
        if (ap.apType >= 0 && ap.apType < 10) {
            strAp[1] = '0' + ap.apType;
        } else if (ap.apType >= 10 && ap.apType < 100) {
            DWCi_Ndi_intToStr(strAp, ap.apType);
        }
        if (ap.apType == 4 || ap.apType == 8) {
            if (ap.area >= 0 && ap.area < 10) {
                strAp[3] = '0' + ap.area;
            }
            DWCi_Ndi_memcpy(strAp + 4, ap.spotInfo, 9);
        }
        DWCi_Ndi_encodeBASE64(apinfo, strAp, 13);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov114_021c67fc(s8 *gamecd, s8 *userId) {
    DWCWiFiInfo info;
    s8 digits[13];

    DWCi_Ndi_memclr(&info, sizeof(info));
    DWCi_BM_GetWiFiInfo(&info);
    if (info.attestedUserId == 0) {
        return FALSE;
    }
    if (DWCi_Ndi_intToStrFigure(digits, info.attestedUserId, 13) < 0) {
        return FALSE;
    }
    DWCi_Ndi_encodeBASE64(userId, digits, 13);
    DWCi_Ndi_encodeBASE64(gamecd, (s8 *)(HW_ROM_HEADER_BUF + 0xc), 4);
    return TRUE;
}

// The events of the HTTP client's callback (pokebw2's)
enum {
    NHTTP_EVENT_BUFFER_FULL = 2,
    NHTTP_EVENT_AFTER_BUFFER_FULL,
    NHTTP_EVENT_ANSWER,
};

void DWCi_Ndi_getUserAgent(s8 *userAgent) {
    OS_SNPrintf(userAgent, 0x21, "Nitro WiFi SDK/%d.%d", 5, 3);
}

// DWCi_Nd_fileapi.c
SOCLCaInfo *ca_array[1] = { &dwci_nd_ca_nasself };
static s8 rhgamecode[9];
static const s8 DWCiNd_action_list[9] = "bGlzdA**";
static const s8 DWCiNd_action_count[9] = "Y291bnQ*";
static const s8 DWCiNd_action_contents[13] = "Y29udGVudHM*";
static s8 userid[21];
static s8 listnumRecvbuf[128];
static s8 data_ov114_021d3ccc[0x59];

int func_ov114_021c68bc(int request, int event, void *arg) {
    NdTask *task;
    int kind;
    DWCNdCallback callback;
    OSMessageQueue *waiter;
    int status;
    int serverError;
    int error;
    int bodySize;
    s8 *statusText;
    int length;
    s8 *body;
    int start;
    int size;
    int pos;
    s8 code[4];
    BOOL isCode;

    serverError = -1;
    task = (NdTask *)func_ov114_021bf444(request);
    kind = task->kind;
    callback = task->callback;
    waiter = task->waiter;
    switch (event) {
    case NHTTP_EVENT_ANSWER:
        if (func_ov114_021bf488(request) == 8) {
            error = DWC_ND_ERROR_CANCELED;
        } else if (func_ov114_021be7fc(request, (s8 *)"HTTPSTATUSCODE", (char **)&statusText) != 3) {
            error = DWC_ND_ERROR_HTTP;
        } else {
            status = DWCi_Ndi_strToInt(statusText, 3);
            if (status == 302) {
                isCode = FALSE;
                length = func_ov114_021bf3e4(request, (char **)&body, &bodySize);
                if (length < 0) {
                    error = DWC_ND_ERROR_HTTP;
                } else {
                    pos = 0;
                    while (pos < length) {
                        start = pos;
                        while (body[pos] != '=' && pos < length) {
                            pos++;
                        }
                        size = pos - start;
                        pos++;
                        if (size == 8) {
                            isCode = DWCi_Ndi_strncmp(body + start, (s8 *)"returncd", 8) == 0;
                        }
                        start = pos;
                        while (body[pos] != '&' && body[pos] != '\r' && body[pos] != '\0' && pos < length) {
                            pos++;
                        }
                        size = pos - start;
                        pos++;
                        if (isCode) {
                            if (size == 4) {
                                size = DWCi_Ndi_decodeBASE64((u8 *)code, body + start, size);
                                code[size] = '\0';
                                serverError = DWCi_Ndi_strToInt(code, size);
                            }
                            break;
                        }
                    }
                    if (serverError < 0) {
                        error = DWC_ND_ERROR_HTTP;
                    } else {
                        error = DWC_ND_ERROR_DLSERVER;
                    }
                }
            } else if (status <= 0 || status != 200) {
                error = DWC_ND_ERROR_HTTP;
            } else {
                switch (kind) {
                case DWC_ND_CBREASON_GETFILELISTNUM:
                    error = DWCi_Ndi_response_count(request, task) ? DWC_ND_ERROR_NONE : DWC_ND_ERROR_HTTP;
                    break;
                case DWC_ND_CBREASON_GETFILELIST:
                    error = DWCi_Ndi_response_list(request, task) ? DWC_ND_ERROR_NONE : DWC_ND_ERROR_HTTP;
                    break;
                case DWC_ND_CBREASON_GETFILE:
                    error = func_ov114_021c7614(request, (s32)task) ? DWC_ND_ERROR_NONE : DWC_ND_ERROR_HTTP;
                    break;
                }
            }
        }
        func_ov114_021c6b44((s32 *)task);
        DWCi_Ndi_unprepareHttpCallback();
        if (callback != NULL) {
            callback(kind, error, serverError);
        }
        if (waiter != NULL) {
            DWCi_Ndi_kickThread(waiter);
        }
        break;
    case NHTTP_EVENT_BUFFER_FULL:
        error = DWC_ND_ERROR_BUFFULL;
        ((NHttpBufferEvent *)arg)->used = 0;
        func_ov114_021c659c(error, serverError);
        if (callback != NULL) {
            callback(kind, error, serverError);
        }
        break;
    case NHTTP_EVENT_AFTER_BUFFER_FULL:
        break;
    }
    return 0;
}

void func_ov114_021c6b44(s32 *param_1) {
    if (param_1 == NULL) {
        return;
    }
    switch (param_1[0]) {
    case 1:
        break;
    case 2:
        if (param_1[7] != 0) {
            DWCi_Ndi_free((void *)param_1[7]);
            param_1[7] = 0;
        }
        if (param_1[6] != 0) {
            DWCi_Ndi_free((void *)param_1[6]);
            param_1[6] = 0;
        }
        DWCi_Ndi_free((void *)param_1[5]);
        param_1[5] = 0;
        break;
    case 3:
        DWCi_Ndi_free((void *)param_1[3]);
        param_1[3] = 0;
        break;
    }
    DWCi_Ndi_free(param_1);
}

void freeHttp(NdTask *task, s32 request) {
    func_ov114_021c654c(request);
    DWCi_Ndi_unprepareHttpCallback();
    func_ov114_021c6b44((s32 *)task);
}

BOOL func_ov114_021c6c0c(NdTask **taskOut, s32 *requestOut, const char *action, void *buffer, u32 bufferSize, s8 *logData) {
    NdTask *task;
    s32 request;
    u8 decoded[0x100];

    if (!func_ov114_021c67fc(rhgamecode, userid)) {
        func_ov114_021c659c(ND_ERROR_FATAL, 0);
        return FALSE;
    }
    task = DWCi_Ndi_alloc(sizeof(NdTask), 4);
    if (task == NULL) {
        func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
        return FALSE;
    }
    task->callback = DWCi_Ndi_dlcallback;
    task->waiter = NULL;
    if (!DWCi_Ndi_prepareHttpCallback()) {
        func_ov114_021c6b44((s32 *)task);
        func_ov114_021c659c(DWC_ND_ERROR_BUSY, 0);
        return FALSE;
    }
    if (!func_ov114_021c654c(DWCi_Ndi_nhttpid)) {
        return FALSE;
    }
    request = func_ov114_021bf04c((const char *)DWCi_Ndi_serverURL, 1, buffer, bufferSize, func_ov114_021c68bc, task);
    if (request == 0) {
        DWCi_Ndi_unprepareHttpCallback();
        func_ov114_021c6b44((s32 *)task);
        func_ov114_021c659c(DWC_ND_ERROR_HTTP, 0);
        return FALSE;
    }
    *taskOut = task;
    *requestOut = request;
    if (func_ov114_021be9c8(request, ca_array, 1) == -1) {
        goto fail;
    }
    if (func_ov114_021be8b8(request, 0) == -1) {
        goto fail;
    }
    if (func_ov114_021be6bc(request, "User-Agent", (const char *)DWCi_Ndi_userAgent) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"gamecd", DWCi_Ndi_dwcgamecd) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"rhgamecd", rhgamecode) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"passwd", DWCi_Ndi_password) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"token", DWCi_Ndi_authtoken) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"userid", userid) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"macadr", DWCi_Ndi_macaddr) == -1) {
        goto fail;
    }
    if (func_ov114_021be748(request, (s8 *)"action", (s8 *)action) == -1) {
        goto fail;
    }
    if (DWCi_Ndi_dlattr[0] != NULL && func_ov114_021be748(request, (s8 *)"attr1", DWCi_Ndi_dlattr[0]) == -1) {
        goto fail;
    }
    if (DWCi_Ndi_dlattr[1] != NULL && func_ov114_021be748(request, (s8 *)"attr2", DWCi_Ndi_dlattr[1]) == -1) {
        goto fail;
    }
    if (DWCi_Ndi_dlattr[2] != NULL && func_ov114_021be748(request, (s8 *)"attr3", DWCi_Ndi_dlattr[2]) == -1) {
        goto fail;
    }
    if (DWCi_Ndi_isExistApinfo && func_ov114_021be748(request, (s8 *)"apinfo", DWCi_Ndi_strApinfo) == -1) {
        goto fail;
    }
    if (logData != NULL) {
        s32 length = DWCi_Ndi_strcmp(logData);

        if (length > 0x40) {
            func_ov114_021c659c(ND_ERROR_BAD_PARAMETER, 0);
            freeHttp(task, request);
            return FALSE;
        }
        DWCi_Ndi_memclr(data_ov114_021d3ccc, 0x59);
        DWCi_Ndi_encodeBASE64((u8 *)data_ov114_021d3ccc, logData, length);
        if (func_ov114_021be748(request, (s8 *)"optionallogdata", data_ov114_021d3ccc) == -1) {
            goto fail;
        }
    }
    DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_dwcgamecd, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, rhgamecode, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_password, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_authtoken, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, userid, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_macaddr, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, (s8 *)action, 0x80);
    DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_strApinfo, 0x80);
    if (DWCi_Ndi_dlattr[0] != NULL) {
        DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_dlattr[0], 0x80);
    }
    if (DWCi_Ndi_dlattr[1] != NULL) {
        DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_dlattr[1], 0x80);
    }
    if (DWCi_Ndi_dlattr[2] != NULL) {
        DWCi_Ndi_decodeBASE64(decoded, DWCi_Ndi_dlattr[2], 0x80);
    }
    if (logData != NULL) {
        DWCi_Ndi_decodeBASE64(decoded, data_ov114_021d3ccc, 0x80);
    }
    return TRUE;

fail:
    freeHttp(task, request);
    func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
    return FALSE;
}

BOOL func_ov114_021c70d4(NdTask *task, s32 param_2) {
    DWCi_Ndi_lockGlobal();
    if (func_ov114_021bf308(param_2) != 0) {
        func_ov114_021c659c(3, 0);
        DWCi_Ndi_unlockGlobal();
        return 0;
    }
    DWCi_Ndi_nhttpid = param_2;
    DWCi_Ndi_unlockGlobal();
    return 1;
}

// The number of files on the server: the prebuilt's DWCi_NdGetFileListNumAsync (setupHttp with action_count and
// listnumRecvbuf, kind 1, the count's address, runHttp). None of Black's code calls it, so Black's link drops it;
// its use keeps DWCiNd_action_count in the shared .rodata, where Black has it.
BOOL DWCi_NdGetFileListNumAsync(int *entrynum) {
    NdTask *task;
    s32 request;

    if (!func_ov114_021c6c0c(&task, &request, DWCiNd_action_count, listnumRecvbuf, sizeof(listnumRecvbuf), NULL)) {
        return FALSE;
    }
    task->kind = DWC_ND_CBREASON_GETFILELISTNUM;
    task->target.number = entrynum;
    return func_ov114_021c70d4(task, request) ? TRUE : FALSE;
}

BOOL func_ov114_021c7124(DWCNdFileInfo *files, u32 offset, u32 count) {
    NdTask *task;
    s32 request;
    s32 bufferSize = count << 8;
    s8 *buffer;
    s8 *offsetParam;
    s8 *countParam;
    s8 number[12];
    s32 length;
    s32 countLength;

    if (bufferSize < 0x80) {
        bufferSize = 0x80;
    }
    buffer = DWCi_Ndi_alloc(bufferSize, 4);
    if (buffer == NULL) {
        func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
        return FALSE;
    }
    length = DWCi_Ndi_intToStr(number, offset);
    offsetParam = DWCi_Ndi_alloc(DWCi_Ndi_getEncodeLenBASE64(length) + 1, 4);
    if (offsetParam == NULL) {
        DWCi_Ndi_free(buffer);
        func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
        return FALSE;
    }
    offsetParam[DWCi_Ndi_encodeBASE64((u8 *)offsetParam, number, length)] = '\0';
    countLength = DWCi_Ndi_intToStr(number, count);
    countParam = DWCi_Ndi_alloc(DWCi_Ndi_getEncodeLenBASE64(countLength) + 1, 4);
    if (countParam == NULL) {
        DWCi_Ndi_free(offsetParam);
        DWCi_Ndi_free(buffer);
        func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
        return FALSE;
    }
    countParam[DWCi_Ndi_encodeBASE64((u8 *)countParam, number, countLength)] = '\0';
    if (!func_ov114_021c6c0c(&task, &request, DWCiNd_action_list, buffer, bufferSize, NULL)) {
        DWCi_Ndi_free(countParam);
        DWCi_Ndi_free(offsetParam);
        DWCi_Ndi_free(buffer);
        return FALSE;
    }
    if (func_ov114_021be748(request, (s8 *)"offset", offsetParam) != -1 &&
        func_ov114_021be748(request, (s8 *)"num", countParam) != -1) {
        task->kind = DWC_ND_CBREASON_GETFILELIST;
        task->target.files = files;
        task->offsetParam = (char *)offsetParam;
        task->countParam = (char *)countParam;
        task->count = count;
        task->buffer = (char *)buffer;
        if (!func_ov114_021c70d4(task, request)) {
            DWCi_Ndi_free(countParam);
            DWCi_Ndi_free(offsetParam);
            DWCi_Ndi_free(buffer);
            return FALSE;
        }
        return TRUE;
    }
    DWCi_Ndi_free(countParam);
    DWCi_Ndi_free(offsetParam);
    DWCi_Ndi_free(buffer);
    freeHttp(task, request);
    func_ov114_021c659c(DWC_ND_ERROR_ALLOC, 0);
    return FALSE;
}

BOOL func_ov114_021c73c0(DWCNdFileInfo *file, void *buffer, u32 size, s8 *logData) {
    NdTask *task;
    s32 request;
    s32 nameLength;
    s32 paramSize;
    u8 *nameParam;

    if (size < file->size) {
        func_ov114_021c659c(5, 0);
        return FALSE;
    }
    nameLength = DWCi_Ndi_strcmp((s8 *)file->fileName);
    if (nameLength > 0x20) {
        func_ov114_021c659c(5, 0);
        return FALSE;
    }
    if (nameLength > 0) {
        paramSize = DWCi_Ndi_getEncodeLenBASE64(nameLength);
    } else {
        paramSize = 0;
    }
    nameParam = DWCi_Ndi_alloc(paramSize + 1, 4);
    if (nameParam == NULL) {
        func_ov114_021c659c(1, 0);
        return FALSE;
    }
    if (!func_ov114_021c6c0c(&task, &request, DWCiNd_action_contents, buffer, file->size, logData)) {
        DWCi_Ndi_free(nameParam);
        return FALSE;
    }
    if (paramSize > 0) {
        nameParam[DWCi_Ndi_encodeBASE64(nameParam, (s8 *)file->fileName, nameLength)] = '\0';
        if (func_ov114_021be748(request, (s8 *)"contents", (s8 *)nameParam) == -1) {
            goto fail;
        }
    }
    task->kind = 3;
    task->target.fileName = (char *)nameParam;
    task->count = file->size;
    if (!func_ov114_021c70d4(task, request)) {
        DWCi_Ndi_free(nameParam);
        return FALSE;
    }
    return TRUE;

fail:
    freeHttp(task, request);
    DWCi_Ndi_free(nameParam);
    func_ov114_021c659c(1, 0);
    return FALSE;
}

BOOL func_ov114_021c757c(u32 *param_1, s32 *param_2) {
    s32 result;

    if (DWCi_Ndi_isOpened == 0) {
        return 0;
    }
    result = func_ov114_021bf4b0(DWCi_Ndi_nhttpid, param_1, param_2);
    return result == 0;
}

BOOL DWCi_Ndi_response_count(s32 res, void *hrq) {
    char *sp4;
    int sp0;
    s32 result;
    s32 flag;

    result = func_ov114_021bf3e4(res, &sp4, &sp0);
    if (result >= 0) {
        **(s32 **)((u8 *)hrq + 0xC) = DWCi_Ndi_strToInt((s8 *)sp4, result);
        if ((s32) **(s32 **)((u8 *)hrq + 0xC) >= 0) {
            flag = 1;
        } else {
            flag = 0;
        }
        return flag;
    }
    return 0;
}

BOOL func_ov114_021c7614(s32 param_1, s32 param_2) {
    char *uStack_c;
    int uStack_10;
    s32 result;

    result = func_ov114_021bf3e4(param_1, &uStack_c, &uStack_10);
    return result == *(s32 *)(param_2 + 0x10);
}

BOOL DWCi_Ndi_response_list(s32 request, NdTask *task) {
    BOOL result = FALSE;
    s8 *body;
    s32 bodySize;
    s8 *header;
    s32 length;
    s32 fileIndex;
    s32 pos;
    s32 field;
    s32 start;
    BOOL sawReturn;
    DWCNdFileInfo *files;

    DWCi_Ndi_memclr(task->target.files, sizeof(DWCNdFileInfo) * task->count);
    if (func_ov114_021be7fc(request, (s8 *)"Content-Length", (char **)&header) > 0 && header[0] == '0' && header[1] == '\0') {
        return TRUE;
    }
    length = func_ov114_021bf3e4(request, (char **)&body, &bodySize);
    if (length <= 0) {
        goto done;
    }
    fileIndex = 0;
    pos = 0;
    if (task->count <= 0) {
        goto done;
    }
    do {
        DWCNdFileInfo *file;

        if (pos >= length) {
            result = TRUE;
            break;
        }
        files = task->target.files;
        file = &files[fileIndex];
        field = 0;
        start = pos;
        sawReturn = FALSE;
        result = FALSE;
        for (; pos < length; pos++) {
            s8 c = body[pos];

            if (c == '\r') {
                sawReturn = TRUE;
                continue;
            }
            if (sawReturn) {
                if (c == '\n') {
                    if (field == 5) {
                        s32 size = pos - 1 - start;
                        s32 value;

                        if (size > 8) {
                            goto done;
                        }
                        value = DWCi_Ndi_strToInt(body + start, size);
                        if (value < 0) {
                            goto done;
                        }
                        file->size = value;
                    }
                    pos++;
                    result = field >= 5;
                    break;
                }
            } else if (c == '\t') {
                switch (field) {
                case 0: {
                    u32 size = pos - start;

                    if (size > 0x20) {
                        goto done;
                    }
                    DWCi_Ndi_memcpy(file->fileName, body + start, size);
                    break;
                }
                case 1: {
                    u8 decoded[0x66 + 1];
                    u32 size = pos - start;
                    u32 decodedSize;

                    if (DWCi_Ndi_getDecodeLenBASE64(size) > 0x66) {
                        goto done;
                    }
                    decodedSize = DWCi_Ndi_decodeBASE64(decoded, body + start, size);
                    if (decodedSize > 0x66) {
                        decodedSize = 0x66;
                    }
                    DWCi_Ndi_memcpy(file->description, decoded, decodedSize);
                    file->description[0x33 - 1] = 0;
                    break;
                }
                case 2: {
                    u32 size = pos - start;

                    if (size > 10) {
                        goto done;
                    }
                    DWCi_Ndi_memcpy(file->param1, body + start, size);
                    break;
                }
                case 3: {
                    u32 size = pos - start;

                    if (size > 10) {
                        goto done;
                    }
                    DWCi_Ndi_memcpy(file->param2, body + start, size);
                    break;
                }
                case 4: {
                    u32 size = pos - start;

                    if (size > 10) {
                        goto done;
                    }
                    DWCi_Ndi_memcpy(file->param3, body + start, size);
                    break;
                }
                case 5: {
                    s32 size = pos - start;
                    s32 value;

                    if (size > 8) {
                        goto done;
                    }
                    value = DWCi_Ndi_strToInt(body + start, size);
                    if (value < 0) {
                        goto done;
                    }
                    file->size = value;
                    break;
                }
                }
                field++;
                start = pos + 1;
            }
            sawReturn = FALSE;
        }
        fileIndex++;
    } while (fileIndex < task->count);
done:
    return result;
}

// DWCi_Nd_os_NITRO.c
static OSMutex g_MutexAttr;
static OSMutex g_MutexHttp;
static s32 g_rightHTTPComm;

u32 DWCi_Ndi_initLockGlobal(void) {
    OS_InitMutex(&g_MutexAttr);
    return 1;
}

void DWCi_Ndi_exitLockGlobal(void) {
}

void DWCi_Ndi_lockGlobal(void) {
    OS_LockMutex(&g_MutexAttr);
}

void DWCi_Ndi_unlockGlobal(void) {
    OS_UnlockMutex(&g_MutexAttr);
}

u32 DWCi_Ndi_initHttpCallback(void) {
    OS_InitMutex(&g_MutexHttp);
    g_rightHTTPComm = 0;
    return 1;
}

void DWCi_Ndi_exitHttpCallback(void) {
}

BOOL DWCi_Ndi_prepareHttpCallback(void) {
    BOOL flag;

    OS_LockMutex(&g_MutexHttp);
    flag = g_rightHTTPComm == 0;
    if (flag) {
        g_rightHTTPComm = 1;
    }
    OS_UnlockMutex(&g_MutexHttp);
    return flag;
}

void DWCi_Ndi_unprepareHttpCallback(void) {
    OS_LockMutex(&g_MutexHttp);
    g_rightHTTPComm = 0;
    OS_UnlockMutex(&g_MutexHttp);
}

void DWCi_Ndi_kickThread(OSMessageQueue *threadEvent) {
    OS_SendMessage(threadEvent, (void *)0x0, 0);
}

void func_ov114_021c79c0(u8 *param_1) {
    s32 i;
    u8 mac[6];
    s8 hex[14];

    OS_GetMacAddress(mac);
    for (i = 0; i < 6; i++) {
        DWCi_Ndi_hexToStrFigure(hex + i * 2, mac[i], 2);
    }
    DWCi_Ndi_encodeBASE64(param_1, hex, 0xc);
}

u32 DWCi_Ndi_strcmp(s8 *param_1) {
    return strlen(param_1);
}

s32 DWCi_Ndi_strncmp(s8 *p1, s8 *p2, s32 size) {
    return strncmp(p1, p2, size);
}

void DWCi_Ndi_memclr(void *p, u32 size) {
    MI_CpuFill8(p, 0, size);
}

void DWCi_Ndi_memcpy(void *dst, void *src, u32 size) {
    MI_CpuCopy8(src, dst, size);
}

s32 DWCi_Ndi_strToInt(s8 *p, s32 len) {
    return NHTTPi_strToInt(p, len);
}

s32 DWCi_Ndi_intToStr(s8 *p, u32 n) {
    return func_ov114_021ba4e4(p, n);
}

s32 DWCi_Ndi_intToStrFigure(s8 *p, u64 n, s32 figure) {
    s32 i;
    s32 b;
    u32 x;
    // The powers of ten from 10^16 down to 10
    static const u64 rdx[] = {
        10000000000000000ULL,
        1000000000000000ULL,
        100000000000000ULL,
        10000000000000ULL,
        1000000000000ULL,
        100000000000ULL,
        10000000000ULL,
        1000000000ULL,
        100000000ULL,
        10000000ULL,
        1000000ULL,
        100000ULL,
        10000ULL,
        1000ULL,
        100ULL,
        10ULL,
    };
    s32 len = 0;

    if (n >= 100000000000000000ULL) {
        return -1;
    }
    for (i = 0, b = 0; i < 16; i++) {
        u64 power = rdx[i];

        if (n >= power) {
            x = n / power;
            n -= x * power;
            p[len] = x + '0';
            b = 1;
            len++;
        } else if (b || 17 - i <= figure) {
            p[len] = '0';
            len++;
        }
    }
    p[len] = n + '0';
    return len + 1;
}

s32 DWCi_Ndi_hexToStrFigure(s8 *p, u32 n, s32 figure) {
    s32 i;
    u32 x;
    s32 len = 0;

    if (figure > 8) {
        return -1;
    }
    for (i = 1; i <= figure; i++) {
        x = (n >> ((figure - i) * 4)) & 0xf;
        if (x < 10) {
            p[len] = x + '0';
            len++;
        } else {
            p[len] = x + 'a' - 10;
            len++;
        }
    }
    return len;
}
