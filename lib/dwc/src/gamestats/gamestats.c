// Black's code at ov114 0x021c9858-0x021ca494. The name is pokebw2's file between dpw_battle_tower.c and crypt.c (16
// functions there, 16 here).

#include "types.h"
#include "dwc/dwc.h"
#include "dwc/gamestats.h"
#include "dwc/base.h"
#include "dwc/ext.h"
#include <string.h>
#include <stdlib.h>
#include "gs/nonport.h"
#include "gfl/ghttpEncryption.h"


// The state of the client
typedef struct GamestatsClient {
    int state;
    BOOL active;
    int request;
    BOOL configured;
    char gameName[0x20];
    char secret[0x14];
    // The scrambling of the payload: a generator of numbers with a multiplier, an increment and a modulus, and the key
    // that the checksum of the payload is xored with
    u32 multiplier;
    u32 increment;
    u32 modulus;
    u32 hashKey;
    // The text of the URL being sent, where its hash and its end are, and the answer to the first request
    char *url;
    char *hashField;
    char *urlEnd;
    const char *answer;
    int answerSize;
    // The answer to the second request, once checked, without the hash that ends it
    const char *body;
    int bodySize;
    GamestatsCheckFunc check;
    // The hash that the second request carries
    char hash[0x29];
} GamestatsClient;

static u32 data_ov114_021d4228;
static const char *data_ov114_021cf7a8 = NULL;
static GamestatsClient data_ov114_021cf7ac = { 2 };
void func_0205a29c(void *param_1);
u32 func_ov114_021ca240(char *param_1, u32 param_2, u8 *param_3, s32 param_4, GamestatsCheckFunc param_5, s32 param_6);

#pragma thumb off

void func_ov114_021c9b64(char *param_1, int param_2, int param_3, void *param);
void func_ov114_021ca06c(void);

u32 b64size(u32 len) {
    u32 flag = (len % 3 != 0) ? 1 : 0;
    return (len / 3 + flag) * 4;
}

void my_randinit(u32 seed) {
    data_ov114_021d4228 = seed | seed << 0x10;
}

u8 func_ov114_021c98a4(void) {
    u32 seed = data_ov114_021d4228;
    seed = (data_ov114_021cf7ac.multiplier * seed + data_ov114_021cf7ac.increment) % data_ov114_021cf7ac.modulus;
    data_ov114_021d4228 = seed;
    return ((s32)seed >> 16) & 0xff;
}

u32 func_ov114_021c98e0(s8 *param_1, const u8 *param_2, s32 param_3, u32 param_4, u32 param_5, u32 param_6) {
    typedef struct { u8 b[20]; } K;
    s32 count;
    s32 i;

    data_ov114_021cf7ac.configured = 0;
    if (strlen(param_1) >= 0x20) {
        return 0;
    }
    if (strlen(param_2) != 0x14) {
        return 0;
    }
    if (((param_3 - 5) & 7) != 0) {
        return 0;
    }
    if ((param_4 & 1) == 0) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 0x20; i++) {
        if ((param_5 >> i & 1) == 1) {
            count++;
        }
    }
    if (count != 1) {
        return 0;
    }
    strncpy(data_ov114_021cf7ac.gameName, param_1, 0x20);
    *(K *)data_ov114_021cf7ac.secret = *(K *)param_2;
    data_ov114_021cf7ac.multiplier = param_3;
    data_ov114_021cf7ac.increment = param_4;
    data_ov114_021cf7ac.modulus = param_5;
    data_ov114_021cf7ac.hashKey = param_6;
    data_ov114_021cf7ac.configured = 1;
    return 1;
}

BOOL func_ov114_021c99d0(char *param_1, s32 param_2) {
    typedef struct { u8 b[20]; } K;
    typedef struct { K key; char data[1]; } Buf;
    char hex[41];
    u8 digest[20];
    char table[] = "0123456789abcdef";
    s32 i;
    Buf *ptr;
    s32 len;
    s32 n;
    Buf *tail;

    if (param_2 <= 0x28) {
        return FALSE;
    }
    len = param_2 - 0x28;
    n = b64size(len);
    ptr = DWC_Alloc(8, n + 0x29);
    if (ptr == NULL) {
        return FALSE;
    }
    ptr->key = *(K *)data_ov114_021cf7ac.secret;
    B64Encode(param_1, ptr->data, len, 2);
    tail = (Buf *)((char *)ptr + 0x14 + b64size(len));
    tail->key = *(K *)data_ov114_021cf7ac.secret;
    n = b64size(len);
    MATH_CalcSHA1(digest, ptr, n + 0x28);
    DWC_Free(8, ptr, 0);
    for (i = 0; i < 20; i++) {
        hex[2 * i] = table[digest[i] >> 4];
        hex[2 * i + 1] = table[digest[i] & 0xf];
    }
    hex[40] = 0;
    if (strncmp(param_1 + (param_2 - 0x28), hex, 0x28) != 0) {
        return FALSE;
    }
    data_ov114_021cf7ac.body = param_1;
    data_ov114_021cf7ac.bodySize = len;
    return TRUE;
}



#pragma thumb on

#pragma thumb off
void func_ov114_021c9b60(int state, const char *buffer, int length, int received, int total, void *param) {
}

void func_ov114_021c9b64(char *param_1, int param_2, int param_3, void *param) {
    u32 n;
    GamestatsCheckFunc check;

    data_ov114_021cf7ac.request = -1;
    if (data_ov114_021cf7ac.state == 1) {
        return;
    }
    if (param_3 == 0) {
        switch (data_ov114_021cf7ac.state) {
        case 5:
            if (param_2 == 0x20) {
                u8 shaIn[0x34];
                u8 *digest = data_ov114_021cf7ac.hashField + 0x14;
                char table[] = "0123456789abcdef";
                s32 i;

                __memcpy(shaIn, data_ov114_021cf7ac.secret, 0x14);
                memcpy(shaIn + 0x14, param_1, 0x20);
                MATH_CalcSHA1(digest, shaIn, 0x34);
                for (i = 0; i < 20; i++) {
                    data_ov114_021cf7ac.hashField[2 * i] = table[digest[i] >> 4];
                    data_ov114_021cf7ac.hashField[2 * i + 1] = table[digest[i] & 0xf];
                }
                n = strlen("&hash=");
                *(data_ov114_021cf7ac.hashField - n) = '&';
                strncpy(data_ov114_021cf7ac.hash, data_ov114_021cf7ac.hashField, 0x29);
                data_ov114_021cf7ac.state = 6;
            } else {
                func_ov114_021ca06c();
            }
            func_0205a29c(param_1);
            break;
        case 7:
            n = strlen("error:");
            if (strncmp(param_1, "error:", n) == 0) {
                func_0205a29c(param_1);
                func_ov114_021ca06c();
                return;
            }
            if (data_ov114_021cf7ac.answer != NULL) {
                func_0205a29c(data_ov114_021cf7ac.answer);
                data_ov114_021cf7ac.answer = NULL;
            }
            if (func_ov114_021c99d0(param_1, param_2) == 0) {
                func_0205a29c(param_1);
                func_ov114_021ca06c();
                return;
            }
            check = data_ov114_021cf7ac.check;
            if (check != NULL && check(param_1, param_2) == 0) {
                func_0205a29c(param_1);
                func_ov114_021ca06c();
                return;
            }
            data_ov114_021cf7ac.answer = param_1;
            data_ov114_021cf7ac.answerSize = param_2;
            data_ov114_021cf7ac.state = 8;
            break;
        }
    } else {
        func_ov114_021ca06c();
    }
}


static inline void CopyBytes(u8 *dst, u8 *src, s32 n) {
    s32 i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
}
u32 func_ov114_021c9d9c(u8 *param_1, u8 *param_2, s32 param_3, u8 *param_4, s32 param_5) {
    s32 len;
    u8 *ptr;
    long i;
    u32 sum;

    len = param_5 + param_3;
    sum = 0;
    ptr = DWC_Alloc(8, len + 4);
    if (ptr == NULL) {
        return 2;
    }
    for (i = 0; i < param_3; i++) {
        ptr[i + 4] = param_2[i];
    }
    for (i = 0; i < param_5; i++) {
        ptr[i + param_3 + 4] = param_4[i];
    }
    for (i = 0; i < len; i++) {
        sum += ptr[i + 4];
    }
    my_randinit(sum);
    for (i = 0; i < len; i++) {
        ptr[i + 4] ^= func_ov114_021c98a4();
    }
    sum ^= data_ov114_021cf7ac.hashKey;
    ptr[0] = sum >> 24;
    ptr[1] = sum >> 16;
    ptr[2] = sum >> 8;
    ptr[3] = sum;
    B64Encode(ptr, param_1, len + 4, 2);
    DWC_Free(8, ptr, 0);
    param_1[b64size(len + 4)] = 0;
    return 0;
}

void func_ov114_021c9ee4(int server, const char *key) {
    char secret[0x15];
    char seed[9] = "";
    u32 seed1;
    u32 seed2;
    u32 seed3;
    u32 seed4;

    data_ov114_021cf7ac.request = -1;
    data_ov114_021cf7ac.url = NULL;
    data_ov114_021cf7ac.hashField = NULL;
    data_ov114_021cf7ac.urlEnd = NULL;
    data_ov114_021cf7ac.answer = NULL;
    data_ov114_021cf7ac.answerSize = 0;
    switch (server) {
    case 0:
        data_ov114_021cf7a8 = "http://gamestats2.gs.nintendowifi.net/";
        break;
    case 1:
        data_ov114_021cf7a8 = "http://sdkdev.gamespy.com/games/";
        break;
    case 2:
        data_ov114_021cf7a8 = "http://ishikawa.servebeer.com/games/";
        break;
    }
    strncpy(secret, key, 0x14);
    secret[0x14] = 0;
    seed1 = strtoul(strncpy(seed, key + 0x14, 8), NULL, 16);
    seed2 = strtoul(strncpy(seed, key + 0x1c, 8), NULL, 16);
    seed3 = strtoul(strncpy(seed, key + 0x24, 8), NULL, 16);
    seed4 = strtoul(strncpy(seed, key + 0x2c, 8), NULL, 16);
    func_ov114_021c98e0(key + 0x34, secret, seed1, seed2, seed3, seed4);
    func_ov114_021bf558(NULL);
    data_ov114_021cf7ac.state = 3;
    data_ov114_021cf7ac.active = 1;
}


#pragma thumb on

#pragma thumb off
void func_ov114_021ca06c(void) {
    BOOL BVar1;

    BVar1 = DWCi_IsError();
    if (BVar1 == 0) {
        DWCi_SetError(6, 0xfffea048);
    }
    data_ov114_021cf7ac.state = 1;
}

int func_ov114_021ca0a0(void) {
    s32 result;

    switch (data_ov114_021cf7ac.state) {
    case 1:
        func_ov114_021ca06c();
        break;
    case 4:
        result = func_ov114_021bf6c4(data_ov114_021cf7ac.url, 0, 0, func_ov114_021c9b60, func_ov114_021c9b64, 0);
        data_ov114_021cf7ac.request = result;
        if (result >= 0) {
            data_ov114_021cf7ac.state = 5;
            break;
        }
        func_ov114_021ca06c();
        break;
    case 5:
        if (func_ov114_021bf5b8() == 0) {
            func_ov114_021ca06c();
        }
        break;
    case 6:
        result = func_ov114_021bf6c4(data_ov114_021cf7ac.url, 0, 0, func_ov114_021c9b60, func_ov114_021c9b64, 0);
        data_ov114_021cf7ac.request = result;
        if (result >= 0) {
            data_ov114_021cf7ac.state = 7;
            break;
        }
        func_ov114_021ca06c();
        break;
    case 7:
        if (func_ov114_021bf5b8() == 0) {
            func_ov114_021ca06c();
        }
        break;
    case 0:
    case 2:
    case 3:
    case 8:
        break;
    }
    return data_ov114_021cf7ac.state;
}


void func_ov114_021bf880(s32 param_1);
void DWCi_RankingSessionCancel(void) {
    if (data_ov114_021cf7ac.request >= 0) {
        func_ov114_021bf880(data_ov114_021cf7ac.request);
    }
    data_ov114_021cf7ac.state = 0;
}

#pragma thumb on

#pragma thumb off
void func_ov114_021ca1b4(void) {
    if (data_ov114_021cf7ac.active == 0) {
        return;
    }
    if (data_ov114_021cf7ac.url != (void *)0x0) {
        DWC_Free(8, data_ov114_021cf7ac.url, 0);
        data_ov114_021cf7ac.url = (void *)0x0;
    }
    if (data_ov114_021cf7ac.answer != 0) {
        func_0205a29c(data_ov114_021cf7ac.answer);
        data_ov114_021cf7ac.answerSize = 0;
        data_ov114_021cf7ac.answer = 0;
    }
    func_ov114_021bf57c();
    data_ov114_021cf7ac.state = 2;
    data_ov114_021cf7ac.active = 0;
}

void *func_ov114_021ca228(int *size) {
    *size = data_ov114_021cf7ac.bodySize;
    return data_ov114_021cf7ac.body;
}

u32 func_ov114_021ca240(char *param_1, u32 param_2, u8 *param_3, s32 param_4, GamestatsCheckFunc param_5, s32 param_6) {
    char pidbuf[0x10];
    u32 info[2];
    u32 total;
    u32 n;
    s32 r;
    char *buf;

    if (data_ov114_021cf7ac.configured != 1) {
        return 3;
    }
    if (data_ov114_021cf7ac.url != NULL) {
        DWC_Free(8, data_ov114_021cf7ac.url, 0);
        data_ov114_021cf7ac.url = NULL;
    }
    total = b64size(param_4 + 0xc)
        + (strlen("&data=")
        + ((strlen("&hash=")
        + (OS_SNPrintf(pidbuf, 0x10, "%d", param_2)
        + (strlen("?pid=")
        + (strlen(param_1)
        + (strlen(data_ov114_021cf7a8) + strlen(data_ov114_021cf7ac.gameName)))))) + 0x29));
    data_ov114_021cf7ac.url = DWC_Alloc(8, total);
    if (data_ov114_021cf7ac.url == NULL) {
        return 2;
    }
    OS_SPrintf(data_ov114_021cf7ac.url, "%s%s%s?pid=%d&hash=%s&data=", data_ov114_021cf7a8, data_ov114_021cf7ac.gameName, param_1, param_2, "0000000000000000000000000000000000000000", "");
    buf = data_ov114_021cf7ac.url;
    n = strlen(buf);
    data_ov114_021cf7ac.urlEnd = buf + n;
    n = strlen("&data=");
    data_ov114_021cf7ac.hashField = data_ov114_021cf7ac.urlEnd - n - 0x28;
    info[0] = param_2;
    info[1] = param_4;
    r = func_ov114_021c9d9c(data_ov114_021cf7ac.urlEnd, (u8 *)info, 8, param_3, param_4);
    if (r == 2) {
        DWC_Free(8, data_ov114_021cf7ac.url, 0);
        data_ov114_021cf7ac.url = NULL;
        return 2;
    }
    if (param_6 != 0) {
        memcpy(data_ov114_021cf7ac.hashField, data_ov114_021cf7ac.hash, 0x28);
        data_ov114_021cf7ac.state = 6;
    } else {
        n = strlen("&hash=");
        *(data_ov114_021cf7ac.hashField - n) = 0;
        data_ov114_021cf7ac.state = 4;
    }
    data_ov114_021cf7ac.check = param_5;
    return 0;
}


#pragma thumb on

#pragma thumb off
int func_ov114_021ca470(const char *page, int pid, const void *payload, int size, GamestatsCheckFunc check) {
    return func_ov114_021ca240(page, pid, (u8 *)payload, size, check, 0);
}
#pragma thumb on
