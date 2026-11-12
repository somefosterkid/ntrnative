// Black's code at ov114 0x021c109c-0x021c1d30. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

u32 data_ov114_021d3b44[8];
void *data_ov114_021d3b40;
// pokebw2's sghiEncyptorSetRootCAList
static char data_ov114_021ce6ae[] = "ghiEncyptorSetRootCAList";
// pokebw2's sghiEncryptorGetRootCAList
static char data_ov114_021ce6f9[] = "ghiEncryptorGetRootCAList";
// pokebw2's sghiEncryptorSetTwlRootCA
static char data_ov114_021ce6e0[] = "ghiEncryptorSetTwlRootCA";
// pokebw2's sghiEncryptorCleanupTwlRootCA
static char data_ov114_021ce72e[] = "ghiEncryptorCleanupTwlRootCA";
// pokebw2's sSslAuthCallback
static char data_ov114_021ce686[] = "SslAuthCallback";
// pokebw2's sghiTwlSSLInit
static char data_ov114_021ce678[] = "ghiTwlSSLInit";
// pokebw2's sghiEncryptorSslInitFunc
static char data_ov114_021ce696[] = "ghiEncryptorSslInitFunc";
// pokebw2's sghiEncryptorSslStartFunc
static char data_ov114_021ce6c7[] = "ghiEncryptorSslStartFunc";
// pokebw2's sghiEncryptorSslCleanupFunc
static char data_ov114_021ce713[] = "ghiEncryptorSslCleanupFunc";
#pragma thumb off
u32 func_ov114_021c109c(s32 param_1, s32 param_2) {
    s32 *ptr;
    s32 result;

    ptr = func_ov114_021c0da4(param_1);
    if (ptr == (s32 *)0x0) {
        return 0;
    }
    if (param_2 == 5) {
        param_2 = 4;
    }
    if (ptr[0x66] == param_2) {
        return 1;
    }
    if (ptr[0x65] != 0 && ptr[0x66] != param_2) {
        return 0;
    }
    if ((param_2 == 0) && (result = strncmp((u8 *)ptr[5], "https://", 8), result == 0))
    {
        return 0;
    }
    ptr[0x66] = param_2;
    if (param_2 != 0) {
        ptr[0x65] = 0;
        ptr[0x6d] = (s32)func_ov114_021c1920;
        ptr[0x6f] = (s32)func_ov114_021c1a78;
        ptr[0x6e] = (s32)func_ov114_021c1c98;
        ptr[0x70] = (s32)func_ov114_021c1c88;
        ptr[0x71] = (s32)func_ov114_021c1c90;
        ptr[0x67] = 0;
        ptr[0x68] = 0;
        ptr[0x69] = 0;
        ptr[0x6a] = 1;
        ptr[0x6b] = 0;
        ptr[0x6c] = 0;
        ptr[0x72] = 1;
        return 1;
    }
    ptr[0x65] = 0;
    return 1;
}

s32 func_ov114_021c1184(s8 *param_1, s8 *param_2) {
    if (param_2 == NULL || param_1 == NULL || strcmp(param_1, "") == 0) {
        gsDebugFormat(8, 4, 1, "%s(@%s:%d):SSL - Null pointer or empty url string; url=0x%X, theRootCAList=0x%X\n", "ghttpEncryption.c", data_ov114_021ce6ae, 0x74, param_1, param_2);
        return 0;
    }
    return func_ov114_021c137c(param_1, (GHICertList *)param_2);
}

u32 func_ov114_021c1210(const char *url) {
    return func_ov114_021c1500(url);
}

void func_ov114_021c121c(u32 *param_1, u32 *param_2) {
    strcmp((s8 *)*param_1, (s8 *)*param_2);
}

GHICertList *func_ov114_021c1230(s8 *param_1) {
    GHIRootCAEntry *found;
    GHIRootCAEntry key;
    s32 index;
    s32 count;

    if (param_1 == NULL || data_ov114_021d3b40 == 0 || strcmp(param_1, "") == 0) {
        gsDebugFormat(8, 4, 1, "%s(@%s:%d):SSL - Null pointer or empty URL string; url=0x%X, ghttpCertList=0x%X\n", "ghttpEncryption.c", data_ov114_021ce6f9, 0x312, param_1, data_ov114_021d3b40);
        return 0;
    }
    key.url = func_ov114_021c1020(param_1);
    count = 1;
    index = ArraySearch(data_ov114_021d3b40, &key, (int (*)(const void *, const void *))func_ov114_021c121c, 0, 1);
    gsifree(key.url);
    if (index == -1) {
        gsDebugFormat(8, 4, 1, "%s(@%s:%d):SSL - Could not find Url in the Cert List. url=%s\n", "ghttpEncryption.c", data_ov114_021ce6f9, 0x324 - count, param_1);
        return 0;
    }
    found = (GHIRootCAEntry *)ArrayNth(data_ov114_021d3b40, index);
    return found->certList;
}

void func_ov114_021c135c(s32 *param_1) {
    param_1[1] = 0;
    gsifree((void *)*param_1);
    *param_1 = 0;
}

BOOL func_ov114_021c137c(const char *url, GHICertList *rootCA) {
    GHIRootCAEntry entry;
    int index;

    if (url == NULL || rootCA == NULL || strcmp(url, "") == 0) {
        gsDebugFormat(8, 4, 1, "%s(@%s:%d):SSL - Null pointer or empty url string; url=0x%X, theRootCAList=0x%X\n", "ghttpEncryption.c", data_ov114_021ce6e0, 837, url, rootCA);
        return FALSE;
    }
    if (data_ov114_021d3b40 == NULL) {
        data_ov114_021d3b40 = ArrayNew(sizeof(GHIRootCAEntry), 1, (void (*)(void *))func_ov114_021c135c);
        if (data_ov114_021d3b40 == NULL) {
            gsDebugFormat(8, 2, 1, "%s(@%s:%d):SSL - Out of memory. Cannot allocate memory for ghttpCertList\n", "ghttpEncryption.c", data_ov114_021ce6e0, 852);
            return FALSE;
        }
    }
    entry.url = func_ov114_021c1020((s8 *)url);
    entry.certList = rootCA;
    entry.refCount = 1;
    index = ArraySearch(data_ov114_021d3b40, &entry, (int (*)(const void *, const void *))func_ov114_021c121c, 0, TRUE);
    if (index == -1) {
        ArrayInsertSorted(data_ov114_021d3b40, &entry, (int (*)(const void *, const void *))func_ov114_021c121c);
    } else {
        gsifree(entry.url);
        ((GHIRootCAEntry *)ArrayNth(data_ov114_021d3b40, index))->refCount++;
    }
    return TRUE;
}

BOOL func_ov114_021c1500(const char *url) {
    BOOL result = TRUE;
    GHIRootCAEntry key;
    int index;

    if (url == NULL || data_ov114_021d3b40 == NULL || strcmp(url, "") == 0) {
        gsDebugFormat(8, 4, 1, "%s(@%s:%d):SSL - Null pointer or empty url string; url=0x%X, ghttpCertList=0x%X\n", "ghttpEncryption.c", data_ov114_021ce72e, 891, url, data_ov114_021d3b40);
        return FALSE;
    }
    key.url = func_ov114_021c1020((s8 *)url);
    index = ArraySearch(data_ov114_021d3b40, &key, (int (*)(const void *, const void *))func_ov114_021c121c, 0, TRUE);
    if (index == -1) {
        gsDebugFormat(8, 4, result, "%s(@%s:%d):SSL - Could not find Url in the Cert List. url=%s\n", "ghttpEncryption.c", data_ov114_021ce72e, 906, url);
        result = FALSE;
    } else {
        GHIRootCAEntry *entry = ArrayNth(data_ov114_021d3b40, index);

        entry->refCount--;
        if (entry->refCount <= 0) {
            ArrayDeleteAt(data_ov114_021d3b40, index);
            if (ArrayLength(data_ov114_021d3b40) == 0) {
                ArrayFree(data_ov114_021d3b40);
                data_ov114_021d3b40 = NULL;
            }
        }
    }
    gsifree(key.url);
    return result;
}

int func_ov114_021c1668(u32 result, GHISslCertInfo *info, int length) {
    gsDebugFormat(8, 4, 8, "%s(@%s:%d):SSL - RECEIVED CERTFICATE\n", "ghttpEncryption.c", data_ov114_021ce686, 938);
    gsDebugFormat(8, 4, 8, "l:%d\n\ts:%s\n\tCN=%s\n\ti:%s\n", length, info->subject, info->commonName, info->issuer);
    if (result & 0x8000) {
        gsDebugFormat(8, 4, 2, "%s(@%s:%d): Certificate is out-of-date - Ignoring\r\n", "ghttpEncryption.c", data_ov114_021ce686, 946);
        result &= ~0x8000;
    }
    if (result & 0x4000) {
        gsDebugFormat(8, 4, 2, "%s(@%s:%d): SSL: Server name does not match - Ignoring\r\n", "ghttpEncryption.c", data_ov114_021ce686, 954);
        result &= ~0x4000;
    }
    switch ((u8)result) {
    case 1:
        gsDebugFormat(8, 4, 2, "%s(@%s:%d): SSL: No root CA installed.(DO NOT IGNORE)\r\n", "ghttpEncryption.c", data_ov114_021ce686, 963);
        break;
    case 2:
        gsDebugFormat(8, 4, 2, "%s(@%s:%d): SSL: Bad signature.(DO NOT IGNORE)\n", "ghttpEncryption.c", data_ov114_021ce686, 969);
        break;
    case 3:
        gsDebugFormat(8, 4, 2, "%s(@%s:%d):SSL: Unknown signature algorithm\n", "ghttpEncryption.c", data_ov114_021ce686, 975);
        break;
    case 4:
        gsDebugFormat(8, 4, 2, "%s(@%s:%d):SSL: Unknown public key alrorithm\n", "ghttpEncryption.c", data_ov114_021ce686, 981);
        break;
    }
    return result;
}

GHISslConnection *func_ov114_021c1804(const char *url) {
    GHISslConnection *ssl = NULL;
    GHICertList *rootCA = func_ov114_021c1230((s8 *)url);

    if (rootCA != NULL) {
        if (rootCA->certs != NULL && rootCA->count > 0) {
            OS_GetLowEntropyData(data_ov114_021d3b44);
            func_ov009_02162248(data_ov114_021d3b44, 0x20);
            ssl = gsimalloc(sizeof(GHISslConnection));
            if (ssl != NULL) {
                memset(ssl, 0, sizeof(GHISslConnection));
                ssl->unk_800 = 0;
                ssl->certs = rootCA->certs;
                ssl->certCount = rootCA->count;
                ssl->authCallback = func_ov114_021c1668;
            } else {
                gsDebugFormat(8, 2, 2, "%s(@%s:%d): TWL Failed to allocate memory for ssl connection\r\n", "ghttpEncryption.c", data_ov114_021ce678, 1020);
            }
        } else {
            gsDebugFormat(8, 2, 2, "%s(@%s:%d): TWL Uninitialized certificate list\r\n", "ghttpEncryption.c", data_ov114_021ce678, 1027);
        }
    } else {
        gsDebugFormat(8, 2, 2, "%s(@%s:%d): TWL Uninitialized certificate list\r\n", "ghttpEncryption.c", data_ov114_021ce678, 1034);
    }
    return ssl;
}

s32 func_ov114_021c1920(GHIConnection *connection, GHIEncryptor *encryptor) {
    GHIEncryptorState *state;

    gsDebugFormat(8, 4, 15, "%s(@%s:%d)\r\n", "ghttpEncryption.c", data_ov114_021ce696, 1047);
    encryptor->state = gsimalloc(sizeof(GHIEncryptorState));
    if (encryptor->state == NULL) {
        gsDebugFormat(8, 2, 2, "%s(@%s:%d):Failed to allocate SSL interface (out of memory: %d bytes)\r\n", "ghttpEncryption.c", data_ov114_021ce696, 1061, 8);
        return 3;
    }
    memset(encryptor->state, 0, sizeof(GHIEncryptorState));
    state = encryptor->state;
    state->ssl = func_ov114_021c1804(connection->url);
    state->started = FALSE;
    if (state->ssl == NULL) {
        gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL) certificate is uninitialized\r\n", "ghttpEncryption.c", data_ov114_021ce696, 1073);
        return 3;
    }
    encryptor->open = TRUE;
    encryptor->unk_0C = 0;
    encryptor->active = FALSE;
    encryptor->mode = 0;
    encryptor->established = TRUE;
    encryptor->unk_1C = 1;
    gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL engine) initialized\r\n", "ghttpEncryption.c", data_ov114_021ce696, 1085);
    return 1;
}

s32 func_ov114_021c1a78(GHIConnection *connection, GHIEncryptor *encryptor) {
    GHIEncryptorState *state = encryptor->state;
    int error;

    gsDebugFormat(8, 4, 15, "%s(@%s:%d)\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1100);
    if (!state->started) {
        error = func_ov009_0215fc80(connection->socket, state->ssl);
        if (error < 0) {
            switch (error) {
            case -28:
                gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL) Invalid processing. Socket config is invalid\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1113);
                break;
            case -33:
                gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL) Cannot create any more socket descriptors.\r\n" "ghttpEncryption.c", data_ov114_021ce6c7, 1118);
                break;
            case -39:
                gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL) Socket library is not initialized.\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1124);
                break;
            case -42:
                gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL)Insufficient resources.\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1129);
                break;
            default:
                gsDebugFormat(8, 4, 15, "%s(@%s:%d):GameSpy SSL (TWL) SOC_EnableSsl failed (Unknown Error = %d)\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1135, error);
                break;
            }
            connection->completed = TRUE;
            connection->result = 6;
            connection->socketError = error;
            return 3;
        }
        state->started = TRUE;
    }
    encryptor->unk_0C = 1;
    encryptor->active = TRUE;
    gsDebugFormat(8, 4, 15, "%s(@%s:%d)return success\r\n", "ghttpEncryption.c", data_ov114_021ce6c7, 1154);
    return 1;
}

int func_ov114_021c1c78(GHIConnection *connection, GHIEncryptor *encryptor, const char *data, int size, int *sent) {
    return 1;
}

int func_ov114_021c1c80(GHIConnection *connection, GHIEncryptor *encryptor, char *buffer, int *size) {
    return 1;
}

u32 func_ov114_021c1c88(void) {
    return 3;
}

u32 func_ov114_021c1c90(void) {
    return 3;
}

u32 func_ov114_021c1c98(u32 param_1, u32 *param_2) {
    u32 *ptr;

    gsDebugFormat(8, 4, 0xf, "%s(@%s:%d):GameSpy SSL (Twl) engine cleanup called\r\n", "ghttpEncryption.c", data_ov114_021ce713, 0x4d5);
    if (param_2 != (u32 *)0x0) {
        ptr = (u32 *)*param_2;
        if (ptr != (u32 *)0x0) {
            if ((void *)*ptr != (void *)0x0) {
                gsifree((void *)*ptr);
            }
            gsifree(ptr);
            *param_2 = 0;
        }
        param_2[2] = 0;
        param_2[3] = 0;
        param_2[4] = 0;
    }
    return 1;
}
