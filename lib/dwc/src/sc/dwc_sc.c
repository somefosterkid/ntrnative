// Black's dwc_sc.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

// pokebw2's sScState, as the file's statics: whether the login has ended, the competition client, the functions to
// call when its requests end, and the keys and the certificate that the login gave
static ScResultFunc data_ov175_021e4d18;
static int data_ov175_021e4d08;
static u32 *data_ov175_021e4d0c;
static ScResultFunc data_ov175_021e4d10;
static ScResultFunc data_ov175_021e4d14;
static AuthKeys data_ov175_021e4d1c;
static AuthCertificate data_ov175_021e4f34;

s32 func_ov175_021dee3c(s32 gameId, s32 server) {
    s8 *ptr1;
    s8 *ptr2;
    u32 code;
    s32 result;
    s32 err;

    gsCoreInitialize();
    func_ov175_021df734(server);
    DWCi_GetAuthInfo((s32 *)&ptr1, (s32 *)&ptr2);
    data_ov175_021e4d08 = 0;
    err = func_ov175_021e2510(0, gameId, ptr1, ptr2, (u32)func_ov175_021df4fc, 0);
    if (err != 0) {
        func_ov175_021df4a4(0x8e, "Faild on wsLoginRemoteAuth[%d]\n", err);
        code = func_ov175_021df644(1, 0);
        DWCi_SetError(0x14, code);
        return 7;
    }
    while (data_ov175_021e4d08 == 0) {
        OS_Sleep(10);
        gsCoreThink(0);
    }
    if (DWC_GetLastError(NULL) != 0) {
        return 7;
    }
    result = func_ov175_021df9d8(gameId, &data_ov175_021e4d0c);
    if (result != 0) {
        DWCi_SetError(func_ov175_021df5e4(result), func_ov175_021df644(2, result));
    }
    return result;
}

int func_ov175_021def58(void) {
    u32 result;
    u32 heap;
    u32 code;

    result = func_ov175_021dfa3c(data_ov175_021e4d0c);
    if (result != 0) {
        heap = func_ov175_021df5e4(result);
        code = func_ov175_021df644(3, result);
        DWCi_SetError(heap, code);
    }
    func_ov175_021defc4();
    gsCoreShutdown();
    gsCoreThink(0);
    heap = gsCoreIsShutdown();
    if (heap != 2) {
        gsCoreThink(0);
    }
    return result;
}

void func_ov175_021defc4(void) {
    func_ov114_021bf540(func_ov175_021e22e4());
    func_ov114_021bf540(func_ov175_021dfe20());
}

int func_ov175_021defdc(void) {
    s32 result;
    u32 code;

    result = func_ov175_021dfa6c((Ov175Ctx *)data_ov175_021e4d0c);
    if (result != 0) {
        code = func_ov175_021df644(4, result);
        DWCi_SetError(8, code);
    }
    return result;
}

int func_ov175_021df018(ScResultFunc func, int timeout, void *userData) {
    u32 result;
    u32 code;

    data_ov175_021e4d10 = func;
    result = func_ov175_021dfab8((s32)data_ov175_021e4d0c, (s32)&data_ov175_021e4f34, (u32)&data_ov175_021e4d1c, (u32)func_ov175_021df07c, timeout,
                                 (u32)userData);
    if (result != 0) {
        code = func_ov175_021df644(5, result);
        DWCi_SetError(8, code);
    }
    return result;
}

void func_ov175_021df07c(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    data_ov175_021e4d10(param_3, (void *)param_4);
}

int func_ov175_021df09c(int authoritative, ScResultFunc func, int timeout, void *userData) {
    u32 result;
    data_ov175_021e4d14 = func;
    result = func_ov175_021dfb18((Ov175Work *)data_ov175_021e4d0c, NULL, authoritative, (s32)&data_ov175_021e4f34, (s32)&data_ov175_021e4d1c, (s32)func_ov175_021df110, timeout, (s32)userData);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(7, result));
    }
    return result;
}

void func_ov175_021df110(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    data_ov175_021e4d14(param_3, (void *)param_4);
}

int func_ov175_021df130(ScReport *report, int authoritative, ScResultFunc func, int timeout, void *userData) {
    u32 result;
    u32 code;

    data_ov175_021e4d18 = func;
    result = func_ov175_021dfbb8((Ov175Ctx *)data_ov175_021e4d0c, (Ov175Work *)report, authoritative, (s32)&data_ov175_021e4f34, (s32)&data_ov175_021e4d1c, (u32)func_ov175_021df1a8, timeout, (u32)userData);
    if (result != 0) {
        code = func_ov175_021df644(0xb, result);
        DWCi_SetError(8, code);
    }
    return result;
}

void func_ov175_021df1a8(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    data_ov175_021e4d18(param_3, (void *)param_4);
}

int func_ov175_021df1c8(const char *csid) {
    u32 result;
    u32 code;

    result = func_ov175_021dfaa8((s32)data_ov175_021e4d0c, (s8 *)csid);
    if (result != 0) {
        code = func_ov175_021df644(8, result);
        DWCi_SetError(8, code);
    }
    return result;
}

char *func_ov175_021df20c(void) {
    return (s8 *)func_ov175_021dfa98((s32)data_ov175_021e4d0c);
}

char *func_ov175_021df224(void) {
    return (s8 *)func_ov175_021dfaa0((s32)data_ov175_021e4d0c);
}

u32 func_ov175_021df23c(u32 param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 result;
    result = func_ov175_021dfc8c((Ov175Work *)data_ov175_021e4d0c, param_1, param_2, param_3, (u32 *)param_4);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(9, result));
    }
    return result;
}

u32 func_ov175_021df298(Ov175Work *param_1) {
    u32 result;
    result = func_ov175_021dfce8(param_1);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(10, result));
    }
    return result;
}

u32 func_ov175_021df2c8(u32 *param_1) {
    u32 result;
    result = func_ov175_021dfcf4(param_1);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(10, result));
    }
    return result;
}

u32 func_ov175_021df2f8(u32 *param_1) {
    u32 result;
    result = func_ov175_021dfd00(param_1);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(10, result));
    }
    return result;
}

u32 func_ov175_021df328(s32 param_1) {
    u32 result;
    result = func_ov175_021dfd0c(param_1);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(10, result));
    }
    return result;
}

u32 func_ov175_021df358(Ov175Work *param_1, s32 param_2, u8 *param_3, s32 param_4, s32 param_5, Ov175Req *param_6, s32 param_7) {
    u32 result;
    result = func_ov175_021dfd24(param_1, param_2, param_3, param_4, param_5, param_6->unk_18, (s32)param_6, param_7);
    if (result != 0) {
        DWCi_SetError(8, func_ov175_021df644(10, result));
    }
    return result;
}

u32 func_ov175_021df3b0(u32 param_1, s32 param_2, u32 param_3) {
    u32 result;
    u32 heap;
    result = func_ov175_021dfd18(param_1, param_2, param_3);
    if (result != 0) {
        heap = func_ov175_021df5e4(result);
        DWCi_SetError(heap, func_ov175_021df644(10, result));
    }
    return result;
}

int func_ov175_021df3e8(ScReport *report, u8 key, int value) {
    u32 result;
    u32 heap;
    result = func_ov175_021dfdfc((s32 *)report, key, value);
    if (result != 0) {
        heap = func_ov175_021df5e4(result);
        DWCi_SetError(heap, func_ov175_021df644(10, result));
    }
    return result;
}

int func_ov175_021df420(ScReport *report, u8 key, u8 value) {
    u32 result;
    u32 heap;
    result = func_ov175_021dfe08((s32 *)report, key, value);
    if (result != 0) {
        heap = func_ov175_021df5e4(result);
        DWCi_SetError(heap, func_ov175_021df644(10, result));
    }
    return result;
}

int func_ov175_021df458(ScReport *report) {
    u32 result;
    u32 heap;
    result = func_ov175_021dfe14((Ov175_021dd1ecWork3 *)((u32 *)report));
    if (result != 0) {
        heap = func_ov175_021df5e4(result);
        DWCi_SetError(heap, func_ov175_021df644(13, result));
    }
    return result;
}

int func_ov175_021df490(const AuthCertificate **certificate) {
    *certificate = &data_ov175_021e4f34;
    return 0;
}

void func_ov175_021df4a4(s32 param_1, char *param_2, ...) {
    char buf[0x100];
    va_list args;
    s32 len;

    len = OS_SNPrintf(buf, 0x100, "Sc[%d]> ", param_1);
    va_start(args, param_2);
    OS_VSNPrintf(buf + len, 0x100 - len, param_2, args);
    va_end(args);
}

void func_ov175_021df4fc(s32 param_1, Ov175_021dd1ecWork6 *param_2) {
    u32 code;
    s32 heap;

    if (param_1 != 0) {
        func_ov175_021df4a4(0x315, "Failed on player login, GHTTP error: %d\n", param_1);
        if (param_1 == 1) {
            code = func_ov175_021df644(1, 5);
            heap = 9;
        } else {
            code = func_ov175_021df644(1, 7);
            heap = 0x14;
        }
        DWCi_SetError(heap, code);
        func_ov114_021bf8c0(param_1);
    } else {
        if (param_2->unk_00 != 0) {
            func_ov175_021df4a4(0x323, "Failed on player login, Login result: %d\n", param_2->unk_00);
            func_ov175_021df65c(param_2->unk_00);
        } else {
            memcpy(&data_ov175_021e4f34, &param_2->unk_08, sizeof(AuthCertificate));
            memcpy(&data_ov175_021e4d1c, &param_2->unk_388, sizeof(AuthKeys));
            func_ov175_021df4a4(0x32b, "Player '%s' logged in.\r\n", param_2->unk_47);
        }
    }
    data_ov175_021e4d08 = 1;
}

u32 func_ov175_021df5e4(u32 param_1) {
    switch(param_1) {
    case 0:
        return 0;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        return 9;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        break;
    case 0xb:
        break;
    case 0xc:
        break;
    case 0xd:
        break;
    case 0xe:
    }
    return 0x14;
}

s32 func_ov175_021df644(s32 param_1, s32 param_2) {
    return -43000 - (param_1 * 100 + param_2);
}

s32 func_ov175_021df65c(s32 param_1) {
    s32 heap;
    u32 code;

    heap = 0x14;
    code = 0xffff5808;
    if (param_1 == 0) {
        return 0;
    }
    switch (param_1) {
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x69:
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        code -= 8;
        break;
    case 0x68:
        code -= 6;
        heap = 9;
        break;
    default:
        code -= 0xf;
        break;
    }
    DWCi_SetError(heap, code);
    return param_1;
}
