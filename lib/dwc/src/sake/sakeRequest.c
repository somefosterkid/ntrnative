// Black's sakeRequest.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

Ov175SoapTask *func_ov009_0217fd7c(s8 *url, s8 *action, void *request, void *callback, void *userData);
u32 *func_ov009_021819b0(char **namespaces, s32 count);
u32 func_ov009_02181de8(void *);

// The declaration of the message's namespace
// pokebw2's GSI_SAKE_SERVICE_NAMESPACES
static const char *data_ov175_021e370c[] = { "ns1=\"http://gamespy.net/sake\"" };

// The URL of the storage server, which gdb.c sets up once it knows the server it uses
char sakeiSoapUrl[128];

Ov175Op *func_ov175_021dd56c(Ov175Data *param_1, s32 param_2, void *param_3, Ov175ReqCb param_4, void *param_5) {
    Ov175Op *dest;
    param_1->unk_48 = 0;
    if (param_3 == NULL) {
        param_1->unk_48 = 3;
        return NULL;
    }
    if (param_1->unk_0 == 0 || param_1->unk_24 == 0) {
        param_1->unk_48 = 1;
        return NULL;
    }
    dest = gsimalloc(sizeof(Ov175Op));
    if (dest == NULL) {
        param_1->unk_48 = 2;
        return NULL;
    }
    memset(dest, 0, sizeof(Ov175Op));
    dest->unk_0 = param_1;
    dest->unk_4 = param_2;
    dest->unk_8 = param_3;
    dest->unk_10 = param_4;
    dest->unk_14 = param_5;
    return dest;
}

void func_ov175_021dd604(Ov175Op *param_1) {
    gsifree(param_1);
}

u32 func_ov175_021dd610(s32 param_1) {
    switch (param_1) {
    case 0:
        return 0;
    case 1:
        return 6;
    case 0x12:
        return 0x17;
    default:
        return 4;
    }
}

u32 func_ov175_021dd64c(s8 *param_1) {
    s32 result;
    u32 val;
    result = strcmp(param_1, "Success");
    if (result == 0) {
        return 0;
    }
    result = strcmp(param_1, "SecretKeyInvalid");
    if (result == 0) {
        return 1;
    }
    result = strcmp(param_1, "ServiceDisabled");
    if (result == 0) {
        return 2;
    }
    result = strcmp(param_1, "DatabaseUnavailable");
    if (result == 0) {
        return 7;
    }
    result = strcmp(param_1, "LoginTicketInvalid");
    if (result == 0) {
        return 8;
    }
    result = strcmp(param_1, "LoginTicketExpired");
    if (result == 0) {
        return 9;
    }
    result = strcmp(param_1, "TableNotFound");
    if (result == 0) {
        return 10;
    }
    result = strcmp(param_1, "RecordNotFound");
    if (result == 0) {
        return 0xb;
    }
    result = strcmp(param_1, "FieldNotFound");
    if (result == 0) {
        return 0xc;
    }
    result = strcmp(param_1, "FieldTypeInvalid");
    if (result == 0) {
        return 0xd;
    }
    result = strcmp(param_1, "NoPermission");
    if (result == 0) {
        return 0xe;
    }
    result = strcmp(param_1, "RecordLimitReached");
    if (result == 0) {
        return 0xf;
    }
    result = strcmp(param_1, "AlreadyRated");
    if (result == 0) {
        return 0x10;
    }
    result = strcmp(param_1, "NotRateable");
    if (result == 0) {
        return 0x11;
    }
    result = strcmp(param_1, "NotOwned");
    if (result == 0) {
        return 0x12;
    }
    result = strcmp(param_1, "FilterInvalid");
    if (result == 0) {
        return 0x13;
    }
    result = strcmp(param_1, "SortInvalid");
    if (result == 0) {
        return 0x14;
    }
    result = strcmp(param_1, "TargetFilterInvalid");
    if (result == 0) {
        val = 0x15;
    } else {
        val = 0x16;
    }
    return val;
}

void func_ov175_021dd84c(s32 param_1, u32 param_2, u32 *param_3, Ov175Op *param_4) {
    u8 buf[0x20];
    s32 result;
    s32 value = 0;

    if (param_4 == NULL || param_4->unk_0 == 0 || param_4->unk_20 == NULL) {
        return;
    }
    result = func_ov175_021dd610(param_1);
    if (result == 0) {
        if (func_ov009_021834d0(param_3) == 0 || func_ov009_021834e8(param_3, param_4->unk_20->unk_c) == 0 || func_ov009_0218372c(param_3, param_4->unk_20->unk_10, buf, 0x20) == 0) {
            result = 5;
        } else {
            result = func_ov175_021dd64c(buf);
            if (result == 0 && param_4->unk_20->unk_0 != 0) {
                param_4->unk_c = gsimalloc(param_4->unk_20->unk_0);
                if (param_4->unk_c != NULL) {
                    param_4->unk_1c = param_3;
                    result = param_4->unk_20->unk_1c(param_4);
                    if (result == 0) {
                        value = (s32)param_4->unk_c;
                    }
                } else {
                    result = 6;
                }
            }
        }
    }
    if (param_4->unk_10 != NULL) {
        param_4->unk_10(param_4->unk_0, param_4, result, param_4->unk_8, (void *)value, param_4->unk_14);
    }
    if (param_4->unk_20->unk_20 != NULL) {
        param_4->unk_20->unk_20(param_4);
    }
    gsifree(param_4->unk_c);
    param_4->unk_c = NULL;
    func_ov175_021dd604(param_4);
}

u32 func_ov175_021dd98c(Ov175Op *param_1) {
    Ov175Desc *desc;
    u32 result;

    desc = param_1->unk_20;
    result = desc->unk_14(param_1);
    if (result != 0) {
        return result;
    }
    param_1->unk_18 = func_ov009_021819b0((char **)data_ov175_021e370c, 1);
    if (param_1->unk_18 == NULL) {
        return 2;
    }
    func_ov009_021829ac(param_1->unk_18, "ns1", param_1->unk_20->unk_4);
    func_ov009_02182cf0(param_1->unk_18, "ns1", "gameid", param_1->unk_0->unk_14);
    func_ov009_02182ac0(param_1->unk_18, "ns1", "secretKey", param_1->unk_0->unk_18);
    func_ov009_02182ac0(param_1->unk_18, "ns1", "loginTicket", param_1->unk_0->unk_2c.unk_0);
    result = desc->unk_18(param_1);
    if (result != 0) {
        func_ov009_02181d84(param_1->unk_18);
        param_1->unk_18 = NULL;
        if (desc->unk_20 != NULL) {
            desc->unk_20(param_1);
        }
        return result;
    }
    func_ov009_02182a2c(param_1->unk_18, "ns1", param_1->unk_20->unk_4);
    func_ov009_02181de8(param_1->unk_18);
    return 0;
}

void func_ov175_021dda98(Ov175Op *param_1) {
    Ov175SoapTask *task;

    if (sakeiSoapUrl[0] == '\0') {
        OS_SNPrintf(sakeiSoapUrl, 0x80, "http://%s.sake.gs.nintendowifi.net/SakeStorageServer/StorageServer.asmx", param_1->unk_0->unk_4);
    }
    task = func_ov009_0217fd7c(sakeiSoapUrl, param_1->unk_20->unk_8, param_1->unk_18, func_ov175_021dd84c, param_1);
    if (task != NULL) {
        param_1->unk_20->unk_24 = task->unk_24;
    } else {
        param_1->unk_20->unk_24 = NULL;
    }
}

s32 func_ov175_021ddb10(Ov175Op *param_1, Ov175Desc *param_2) {
    s32 result;

    param_1->unk_20 = param_2;
    result = func_ov175_021dd98c(param_1);
    if (result != 0) {
        return result;
    }
    func_ov175_021dda98(param_1);
    return 0;
}
