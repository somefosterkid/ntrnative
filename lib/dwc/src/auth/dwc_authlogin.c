// Black's code at ov009 0x02178ce0-0x02178f4c: the NAS login the game starts over libdwcauth.a's dwc_auth.o: it starts
// DWC_Auth_Create with the caller's parameters, polls it, and gives the answer. Not in NitroDWC: a source
// file of its own, since these functions reach 0x021ab1a4 from its own address, not from dwc_auth.c's first static.

#include <string.h>
#include "types.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "dwc/auth.h"
#include "dwc/util_wifiidtool.h"

// The NAS login's state at 0x021ab1a4: whether it ended, its error, whether it succeeded, two values the caller
// gives and the tick it started at; the parameters (0x021ab1c0) and the HTTP work (0x021ab208) follow it
typedef struct DWCAuthLoginState {
    int unk_0;
    int unk_4;
    int unk_8;
    u64 unk_c;
    OSTick tick;
} DWCAuthLoginState;

static DWCAuthLoginState data_ov009_021ab1a4;
static DWCAuthParam data_ov009_021ab1c0;
// The HTTP work DWC_Auth_Create runs the login on (0x1c50 bytes, what DWCi_Http's cleanup clears)
static u8 data_ov009_021ab208[0x1c50];

#pragma thumb off
void func_ov009_02178ce0(void) {
    data_ov009_021ab1a4.unk_0 = 0;
    data_ov009_021ab1a4.unk_8 = 0;
    data_ov009_021ab1a4.unk_4 = 0;
    MI_CpuFill8(data_ov009_021ab208, 0, sizeof(data_ov009_021ab208));
}

BOOL func_ov009_02178d14(const u16 *ingamesn, const char *gsbrcd, u64 param_3, DWCHttpAllocFunc alloc, DWCHttpFreeFunc free) {
    func_ov009_02178ce0();
    MI_CpuCopy8(ingamesn, data_ov009_021ab1c0.ingamesn, sizeof(data_ov009_021ab1c0.ingamesn));
    MI_CpuCopy8(gsbrcd, data_ov009_021ab1c0.gsbrcd, sizeof(data_ov009_021ab1c0.gsbrcd));
    data_ov009_021ab1c0.alloc = alloc;
    data_ov009_021ab1c0.free = free;
    data_ov009_021ab1a4.tick = OS_GetTick();
    data_ov009_021ab1a4.unk_c = param_3;
    return DWC_Auth_Create(&data_ov009_021ab1c0, data_ov009_021ab208) == 0;
}

void func_ov009_02178da4(void) {
    DWC_Auth_Abort();
    DWC_Auth_Destroy();
}

void func_ov009_02178db4(void) {
    DWCAuthResult retryResult;
    DWCAuthResult authResult;
    u32 error;
    u32 field;
    u32 result;

    error = DWC_Auth_GetError();
    if (error == 0x15) {
        DWC_Auth_GetResult(&retryResult);
        if (retryResult.returncode == 0x4e22) {
            DWC_Auth_Destroy();
            func_ov009_02178ce0();
            result = DWC_Auth_Create(&data_ov009_021ab1c0, data_ov009_021ab208);
            if (result == 0) {
                return;
            }
            field = 0xffffb17c;
        } else {
            data_ov009_021ab1a4.unk_0 = 1;
            data_ov009_021ab1a4.unk_8 = 1;
            return;
        }
    } else {
        if (error == 0) {
            return;
        }
        DWC_Auth_GetResult(&authResult);
        DWC_Auth_Destroy();
        field = authResult.returncode;
    }
    data_ov009_021ab1a4.unk_0 = 1;
    data_ov009_021ab1a4.unk_8 = 0;
    data_ov009_021ab1a4.unk_4 = field;
}

u32 func_ov009_02178e70(void) {
    return data_ov009_021ab1a4.unk_0;
}

void *func_ov009_02178e80(void) {
    return (void *)data_ov009_021ab1a4.unk_8;
}

u32 func_ov009_02178e90(void) {
    return data_ov009_021ab1a4.unk_4;
}

void func_ov009_02178ea0(char *token, char *challenge) {
    DWCAuthResult result;

    DWC_Auth_GetResult(&result);
    strncpy(token, result.token, sizeof(result.token));
    token[300] = 0;
    strncpy(challenge, result.challenge, sizeof(result.challenge));
    challenge[8] = 0;
}

void func_ov009_02178ef4(void) {
    DWC_Auth_Destroy();
}

s64 func_ov009_02178f00(void) {
    DWCAuthWiFiId id;

    func_02058fb4(&id);
    if (id.flag != 0) {
        return (s64)id.uId;
    }
    return (s64)id.notAttestedId;
}

u32 func_ov009_02178f30(void) {
    DWCAuthWiFiId id;

    func_02058fb4(&id);
    return id.flag;
}
