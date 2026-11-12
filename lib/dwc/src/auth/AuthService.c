// Black's AuthService.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

Ov175SoapTask *func_ov009_0217fd7c(s8 *url, s8 *action, void *request, void *callback, void *userData);
void func_ov009_0217e978(u8 *, u8 *);
u32 func_ov009_0217e848(u32 *, const char *);
u32 func_ov009_0217cf5c(u32 *, u32 *, s32, u8 *, u32);
u32 *func_ov009_021819b0(const char **, s32);
u32 func_ov009_02181de8(void *);

static const char data_ov175_021e2cc0[] = "010001";
static const char data_ov175_021e2cc8[] = "BF05D63E93751AD4A59A4A7389CF0BE8A22CCDEEA1E7F12C062D6E194472EFDA5184CCECEB4FBADF5EB1D7ABFE91181453972AA971F624AF9BA8F0F82E2869FB7D44BDE8D56EE50977898F3FEE75869622C4981F07506248BD3D092E8EA05C12B2FA37881176084C8F8B8756C4722CDC57D2AD28ACD3AD85934FB48D6B2D2027";

static const char *data_ov175_021e49a4[] = {"ns1=\"http://gamespy.net/AuthService/\""};

char wsAuthServiceURL[128];

u32 func_ov175_021e2290(void) {
    if (data_ov009_021acef8 == 1) {
        if (wsAuthServiceURL[0] == '\0') {
            OS_SNPrintf(wsAuthServiceURL, 0x80, "https://%s.auth.pubsvs.gs.nintendowifi.net/AuthService/AuthService.asmx", &data_ov009_021acefc);
        }
        return 1;
    }
    return 0;
}

s8 *func_ov175_021e22e4(void) {
    if (data_ov009_021acef8 == 1) {
        if (wsAuthServiceURL[0] == '\0') {
            OS_SNPrintf(wsAuthServiceURL, 0x80, "https://%s.auth.pubsvs.gs.nintendowifi.net/AuthService/AuthService.asmx", &data_ov009_021acefc);
        }
        return wsAuthServiceURL;
    }
    gsDebugFormat(0, 3, 1, "No availability\r\n");
    return NULL;
}

void func_ov175_021e2354(s32 param_1, u32 param_2, u32 *param_3, Ov175Cb *param_4) {
    Ov175Auth a;
    Ov175Blob *blob = &a.blob;
    u8 ctx[0x58];

    blob->unk_0 = 0;
    memset(&a, 0, sizeof(a));
    if (param_1 == 0) {
        if (func_ov009_021834d0(param_3) == 0 || func_ov009_021834e8(param_3, "LoginRemoteAuthResult") == 0) {
            a.status = 0x65;
        } else if (func_ov009_02183a24(param_3, "responseCode", &a.res) == 0) {
            a.status = 0x65;
        } else if (a.res != 0) {
            a.status = 7;
        } else if (func_ov009_021835d0(param_3, "certificate") == 0 ||
                   func_ov175_021e2a5c(blob, param_3) == 0 ||
                   func_ov009_02183560(param_3) == 0 ||
                   func_ov009_02183e50(param_3, "peerkeyprivate", (LargeIntBuf *)a.bufC) == 0) {
            a.status = 0x65;
        } else {
            memcpy(a.bufA, blob->unk_78, 0x104);
            func_ov009_02184938((u32 *)ctx);
            func_ov009_0217e978(a.bufC, ctx);
            func_ov009_02184a34((s32)a.digest, (s32 *)ctx);
            blob->unk_0 = func_ov175_021e26bc(blob);
            if (blob->unk_0 == 0) {
                a.status = 0x66;
            }
        }
    } else if (param_1 == 0x12) {
        a.status = 0x6b;
    } else {
        a.status = 0x64;
    }
    if (param_4->unk_0 != NULL) {
        param_4->unk_0(param_1, &a.status, param_4->unk_4);
    }
    gsifree(param_4);
}

u32 func_ov175_021e2510(u32 param_1, u32 param_2, s8 *param_3, s8 *param_4, u32 param_5, u32 param_6) {
    Ov175Pair *pair;
    u32 *handle;

    if (func_ov175_021e2290() == 0) {
        return 0x6a;
    }
    pair = gsimalloc(8);
    if (pair == NULL) {
        return 0x68;
    }
    pair->unk_0 = param_5;
    pair->unk_4 = param_6;
    handle = func_ov009_021819b0(data_ov175_021e49a4, 1);
    if (handle == NULL) {
        return 0x68;
    }
    if (func_ov009_021829ac(handle, "ns1", "LoginRemoteAuth") == 0 ||
        func_ov009_02182cf0(handle, "ns1", "version", 1) == 0 ||
        func_ov009_02182cf0(handle, "ns1", "partnercode", param_1) == 0 ||
        func_ov009_02182cf0(handle, "ns1", "namespaceid", param_2) == 0 ||
        func_ov009_02182ac0(handle, "ns1", "authtoken", param_3) == 0 ||
        func_ov009_02182ac0(handle, "ns1", "challenge", param_4) == 0 ||
        func_ov009_02182a2c(handle, "ns1", "LoginRemoteAuth") == 0 ||
        func_ov009_02181de8(handle) == 0) {
        func_ov009_02181d84(handle);
        return 0x68;
    }
    if (func_ov009_0217fd7c(wsAuthServiceURL, "SOAPAction: \"http://gamespy.net/AuthService/LoginRemoteAuth\"", handle, func_ov175_021e2354, pair) != NULL) {
        return 0;
    }
    func_ov009_02181d84(handle);
    gsifree(pair);
    return 0x68;
}

u32 func_ov175_021e26b8(u32 param_1) {
}

u32 func_ov175_021e26bc(Ov175Blob *param_1) {
    u32 tmp;
    u8 digest[16];
    u32 keyB[65];
    u32 keyA[65];
    u32 ctx[22];

    func_ov009_02184938(ctx);
    tmp = func_ov175_021e26b8(param_1->unk_4);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_8);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_c);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_10);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_14);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_18);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    tmp = func_ov175_021e26b8(param_1->unk_1c);
    func_ov009_0218497c((s32 *)ctx, &tmp, 4);
    func_ov009_0218497c((s32 *)ctx, param_1->unk_20, strlen(param_1->unk_20));
    func_ov009_0218497c((s32 *)ctx, param_1->unk_3f, strlen(param_1->unk_3f));
    func_ov009_0218497c((s32 *)ctx, param_1->unk_54, strlen(param_1->unk_54));
    func_ov009_0217e978(param_1->unk_78, ctx);
    func_ov009_0217e978(param_1->unk_17c, ctx);
    func_ov009_0218497c((s32 *)ctx, param_1->unk_300, 0x80);
    func_ov009_02184a34((s32)digest, (s32 *)ctx);
    func_ov009_0217e848(keyA, data_ov175_021e2cc8);
    func_ov009_0217e848(keyB, data_ov175_021e2cc0);
    return func_ov009_0217cf5c(keyA, digest, 0x10, param_1->unk_280, 0x80) == 0;
}

u32 func_ov175_021e2874(Ov175Blob *param_1, s8 *param_2, u32 *param_3) {
    if (func_ov009_02182cf0(param_3, param_2, "length", param_1->unk_4) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "version", param_1->unk_8) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "partnercode", param_1->unk_c) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "namespaceid", param_1->unk_10) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "userid", param_1->unk_14) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "profileid", param_1->unk_18) == 0 ||
        func_ov009_02182cf0(param_3, param_2, "expiretime", param_1->unk_1c) == 0 ||
        func_ov009_02182b68(param_3, param_2, "profilenick", param_1->unk_20) == 0 ||
        func_ov009_02182b68(param_3, param_2, "uniquenick", param_1->unk_3f) == 0 ||
        func_ov009_02182b68(param_3, param_2, "cdkeyhash", param_1->unk_54) == 0 ||
        func_ov009_021830d0(param_3, param_2, "peerkeymodulus", (LargeIntBuf *)param_1->unk_78) == 0 ||
        func_ov009_021830d0(param_3, param_2, "peerkeyexponent", (LargeIntBuf *)param_1->unk_17c) == 0 ||
        func_ov009_02182e90(param_3, param_2, "serverdata", param_1->unk_300, 0x80) == 0 ||
        func_ov009_02182e90(param_3, param_2, "signature", param_1->unk_280, 0x80) == 0) {
        return 0;
    }
    return 1;
}

u32 func_ov175_021e2a5c(Ov175Blob *param_1, u32 *param_2) {
    u8 buf[0x104];
    u32 len;

    if (func_ov009_02183a24(param_2, "length", &param_1->unk_4) == 0 ||
        func_ov009_02183a24(param_2, "version", &param_1->unk_8) == 0 ||
        func_ov009_02183a24(param_2, "partnercode", &param_1->unk_c) == 0 ||
        func_ov009_02183a24(param_2, "namespaceid", &param_1->unk_10) == 0 ||
        func_ov009_02183a24(param_2, "userid", &param_1->unk_14) == 0 ||
        func_ov009_02183a24(param_2, "profileid", &param_1->unk_18) == 0 ||
        func_ov009_02183a24(param_2, "expiretime", &param_1->unk_1c) == 0 ||
        func_ov009_0218372c(param_2, "profilenick", param_1->unk_20, 0x1f) == 0 ||
        func_ov009_0218372c(param_2, "uniquenick", param_1->unk_3f, 0x15) == 0 ||
        func_ov009_0218372c(param_2, "cdkeyhash", param_1->unk_54, 0x21) == 0 ||
        func_ov009_0218372c(param_2, "peerkeymodulus", buf, 0x101) == 0 ||
        func_ov009_0217e848(param_1->unk_78, buf) == 0 ||
        func_ov009_0218372c(param_2, "peerkeyexponent", buf, 0x101) == 0 ||
        func_ov009_0217e848(param_1->unk_17c, buf) == 0 ||
        func_ov009_021837a4(param_2, "serverdata", param_1->unk_300, 0x80, &len) == 0 ||
        func_ov009_021837a4(param_2, "signature", param_1->unk_280, 0x80, &len) == 0) {
        return 0;
    }
    return 1;
}
