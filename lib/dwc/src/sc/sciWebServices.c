// Black's sciWebServices.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"

Ov175SoapTask *func_ov009_0217fd7c(s8 *url, s8 *action, void *request, void *callback, void *userData);
u32 *func_ov009_021819b0(s32, s32);
u32 func_ov009_02181de8(void *);

static const char *data_ov175_021e41b8[] = { "gsc=\"http://gamespy.net/competition/\"" };

u32 func_ov175_021e06dc(Ov175_021dd1ecWork2 *param_1, Ov175_021dd1ecWork2 *param_2) {
    if (gsCoreIsShutdown() == 0) {
        param_1->unk_00 = (u32)param_2;
        param_1->unk_04 = 0;
        param_1->unk_08 = 0;
        param_1->unk_0c = 0;
        param_1->unk_2c = 0;
        param_1->unk_28 = 0;
        param_1->unk_30 = 0;
        param_1->unk_14 = 0;
        param_1->unk_10 = 0;
        param_1->unk_18 = 0;
        param_1->unk_48 = 1;
        return 0;
    }
    return 4;
}

void func_ov175_021e0730(u32 *param_1) {
    param_1[0x12] = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[0xb] = 0;
    param_1[10] = 0;
    param_1[0xc] = 0;
    param_1[5] = 0;
    param_1[4] = 0;
    param_1[6] = 0;
    *param_1 = 0;
}

void func_ov175_021e0764(void) {
    gsCoreThink(0);
}

u32 func_ov175_021e0774(Ov175Job *param_1, u32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, u32 param_7) {
    u32 *handle;
    u32 ret;

    if (param_1->unk_14 != 0) {
        ret = 6;
    } else {
        handle = func_ov009_021819b0((s32)data_ov175_021e41b8, 1);
        if (handle == NULL) {
            ret = 5;
        } else {
            if (func_ov009_021829ac(handle, "gsc", "CreateSession") == 0 || func_ov009_021829ac(handle, "gsc", "certificate") == 0 || func_ov175_021e2874((Ov175Blob *)param_3, "gsc", handle) == 0 || func_ov009_02182a2c(handle, "gsc", "certificate") == 0 || func_ov009_02182e90(handle, "gsc", "proof", (u8 *)param_4 + 0x208, 0x10) == 0 || func_ov009_02182cf0(handle, "gsc", "gameid", param_2) == 0 || func_ov009_02182a2c(handle, "gsc", "CreateSession") == 0 || func_ov009_02181de8(handle) == 0) {
                func_ov009_02181d84(handle);
                ret = 7;
            } else {
                param_1->unk_4 = param_5;
                param_1->unk_2c = param_7;
                param_1->unk_14 = 1;
                func_ov009_0217fd7c(scServiceURL, "SOAPAction: \"http://gamespy.net/competition/CreateSession\"", handle, func_ov175_021e08e8, param_1);
                ret = 0;
            }
        }
    }
    return ret;
}

void func_ov175_021e08e8(s32 param_1, u32 param_2, u32 *param_3, Ov175Job *param_4) {
    u32 found;
    s32 code;
    u8 buf1[0xff];
    u8 buf2[0xff];

    code = 7;
    if (param_4->unk_48 == 0) {
        return;
    }
    if (param_1 == 0) {
        found = 0;
        if (func_ov009_021834d0(param_3) == 0) {
            code = 9;
        } else {
            if (func_ov009_021834e8(param_3, "CreateSessionResponse") == 0 &&
                func_ov009_021834e8(param_3, "CreateMatchlessSessionResponse") == 0) {
                code = 9;
            }
        }
        if (func_ov009_021834e8(param_3, "CreateSessionResult") == 0 &&
            func_ov009_021834e8(param_3, "CreateMatchlessSessionResult") == 0) {
            code = 9;
        }
        if (func_ov009_02183a24(param_3, "result", &found) == 0) {
            code = 9;
        } else if (code != 9) {
            if (found == 0) {
                if (func_ov009_0218372c(param_3, "csid", buf1, 0xff) == 0 || func_ov009_0218372c(param_3, "ccid", buf2, 0xff) == 0) {
                    code = 9;
                } else {
                    func_ov175_021df928(param_4->unk_0, buf1);
                    func_ov175_021df948(param_4->unk_0, buf2);
                    code = 0;
                }
            } else {
                code = 9;
            }
        }
    }
    param_4->unk_14 = 0;
    if (param_4->unk_4 == 0) {
        return;
    }
    ((void (*)(s32, s32, s32, s32))param_4->unk_4)(param_4->unk_0, param_1, code, param_4->unk_2c);
    param_4->unk_2c = 0;
    param_4->unk_4 = 0;
}

u32 func_ov175_021e0a7c(Ov175Task *param_1, u32 param_2, u32 *param_3, u32 param_4, u32 param_5, Ov175Blob *param_6, u8 *param_7, u32 param_8, u32 param_9, u32 param_10) {
    u32 *handle;
    u32 ret;

    if (param_1->unk_10 != 0) {
        ret = 6;
    } else {
        handle = func_ov009_021819b0((s32)data_ov175_021e41b8, 1);
        if (handle == NULL) {
            ret = 5;
        } else {
            if (func_ov009_021829ac(handle, "gsc", "SetReportIntention") == 0 || func_ov009_021829ac(handle, "gsc", "certificate") == 0 || func_ov175_021e2874(param_6, "gsc", handle) == 0 || func_ov009_02182a2c(handle, "gsc", "certificate") == 0 || func_ov009_02182e90(handle, "gsc", "proof", param_7 + 0x208, 0x10) == 0 || func_ov009_02182ac0(handle, "gsc", "csid", param_3) == 0 || func_ov009_02182ac0(handle, "gsc", "ccid", (s8 *)param_4) == 0 || func_ov009_02182cf0(handle, "gsc", "gameid", param_2) == 0 || func_ov009_02182cf0(handle, "gsc", "authoritative", param_5 != 0) == 0 || func_ov009_02182a2c(handle, "gsc", "SetReportIntention") == 0 || func_ov009_02181de8(handle) == 0) {
                func_ov009_02181d84(handle);
                ret = 7;
            } else {
                param_1->unk_8 = (void (*)(s32, s32, s32, s32))param_8;
                param_1->unk_28 = param_10;
                param_1->unk_10 = 1;
                func_ov009_0217fd7c(scServiceURL, "SOAPAction: \"http://gamespy.net/competition/SetReportIntention\"", handle, func_ov175_021e0c68, param_1);
                ret = 0;
            }
        }
    }
    return ret;
}

void func_ov175_021e0c68(s32 param_1, u32 param_2, u32 *param_3, Ov175Task *param_4) {
    u32 found;
    s32 code;
    u8 buf[0x100];

    if (param_4->unk_48 == 0) {
        return;
    }
    if (param_1 == 0) {
        code = 0;
        found = 0;
        if (func_ov009_021834d0(param_3) == 0 || func_ov009_021834e8(param_3, "SetReportIntentionResponse") == 0 || func_ov009_021834e8(param_3, "SetReportIntentionResult") == 0 || func_ov009_02183a24(param_3, "result", &found) == 0 || func_ov009_0218372c(param_3, "ccid", buf, 0xff) == 0) {
            code = 9;
        } else if (found != 0) {
            code = 8;
        } else {
            func_ov175_021df948(param_4->unk_0, buf);
        }
    } else {
        code = 7;
    }
    param_4->unk_10 = 0;
    if (param_4->unk_8 == NULL) {
        return;
    }
    param_4->unk_8(param_4->unk_0, param_1, code, param_4->unk_28);
    param_4->unk_28 = 0;
    param_4->unk_8 = NULL;
}

void func_ov175_021e0d84(GHIPost *post, Ov175Pend *pend) {
    func_ov114_021c24b4(post, "report", pend->unk_40, pend->unk_44, "report", "application/bin");
}

u32 func_ov175_021e0db8(Ov175Pend *param_1, u32 param_2, s8 *param_3, s8 *param_4, Ov175Work *param_5, u8 *param_6, Ov175Blob *param_7, u8 *param_8, u32 param_9, u32 param_10, u32 param_11) {
    u32 *handle;
    u32 ret;

    if (param_1->unk_18 != 0) {
        return 6;
    }
    if ((u32)param_5->unk_120 < 0x44) {
        return 0xc;
    }
    handle = func_ov009_021819b0((s32)data_ov175_021e41b8, 1);
    if (handle == NULL) {
        return 5;
    }
    if (func_ov009_021829ac(handle, "gsc", "SubmitReport") == 0 || func_ov009_021829ac(handle, "gsc", "certificate") == 0 || func_ov175_021e2874(param_7, "gsc", handle) == 0 || func_ov009_02182a2c(handle, "gsc", "certificate") == 0 || func_ov009_02182e90(handle, "gsc", "proof", param_8 + 0x208, 0x10) == 0 || func_ov009_02182ac0(handle, "gsc", "csid", param_3) == 0 || func_ov009_02182ac0(handle, "gsc", "ccid", param_4) == 0 || func_ov009_02182cf0(handle, "gsc", "gameid", param_2) == 0 || func_ov009_02182cf0(handle, "gsc", "authoritative", param_6 != 0) == 0 || func_ov009_02182a2c(handle, "gsc", "SubmitReport") == 0 || func_ov009_02181de8(handle) == 0) {
        func_ov009_02181d84(handle);
        ret = 5;
    } else {
        param_1->unk_40 = param_5->unk_124;
        param_1->unk_44 = param_5->unk_120;
        param_1->unk_c = (void *)param_9;
        param_1->unk_30 = param_11;
        param_1->unk_18 = 1;
        func_ov009_0217fe68((u32)scServiceURL, (u32)"SOAPAction: \"http://gamespy.net/competition/SubmitReport\"", (u32)handle, (u32)func_ov175_021e0fe8, (u32)func_ov175_021e0d84, (u32)param_1);
        ret = 0;
    }
    return ret;
}

void func_ov175_021e0fe8(s32 param_1, s32 param_2, u32 *param_3, Ov175Call *param_4) {
    u32 sp0;
    s32 (*func)(u32, s32, s32, s32);
    s32 val;

    if (param_4->unk_48 == 0) {
        return;
    }
    if (param_1 == 0) {
        val = 0;
        sp0 = 0;
        if ((func_ov009_021834d0(param_3) == 0) || (func_ov009_021834e8(param_3, "SubmitReportResponse") == 0) || (func_ov009_021834e8(param_3, "SubmitReportResult") == 0) || (func_ov009_02183a24(param_3, "result", &sp0) == 0)) {
            val = 9;
        } else if (sp0 != 0) {
            if (sp0 == 7) {
                val = 0xC;
            } else {
                val = 8;
            }
        }
    } else {
        val = 7;
    }
    func = *(s32 (**)(u32, s32, s32, s32))((u8 *)param_4 + 0xC);
    param_4->unk_18 = 0;
    if (func == NULL) {
        return;
    }
    func(param_4->unk_0, param_1, val, param_4->unk_30);
    param_4->unk_30 = 0;
    *(s32 (**)(u32, s32, s32, s32))((u8 *)param_4 + 0xC) = NULL;
}
