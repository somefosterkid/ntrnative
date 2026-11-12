// Black's sciMain.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"


u32 func_ov175_021df9d8(u32 gameId, u32 **client) {
    u32 *created = NULL;
    u32 result;

    if (client == NULL) {
        return 2;
    }
    *client = NULL;
    result = func_ov175_021df7fc((void **)&created);
    if (result != 0) {
        return result;
    }
    result = func_ov175_021df884((Ov175_021dd1ecWork2 *)created);
    if (result != 0) {
        return result;
    }
    created[0x13] = gameId;
    *client = created;
    return 0;
}

u32 func_ov175_021dfa3c(u32 *param_1) {
    if (param_1 == (u32 *)0x0) {
        return 2;
    }
    if (param_1[0x14] != 0) {
        func_ov175_021df8d4(param_1);
        gsifree(param_1);
    }
    return 0;
}

u32 func_ov175_021dfa6c(Ov175Ctx *param_1) {
    if (param_1 == NULL) {
        return 2;
    }
    if (param_1->unk_50 == 0) {
        return 3;
    }
    func_ov175_021e0764(param_1);
    return 0;
}

s32 func_ov175_021dfa98(s32 param_1) {
    return param_1 + 0x5c;
}

s32 func_ov175_021dfaa0(s32 param_1) {
    return param_1 + 0x84;
}

u32 func_ov175_021dfaa8(s32 param_1, s8 *param_2) {
    func_ov175_021df928(param_1, param_2);
    return 0;
}

u32 func_ov175_021dfab8(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6) {
    if (func_ov175_021e26bc((Ov175Blob *)param_2)) {
        return func_ov175_021e0774((Ov175Job *)param_1, *(u32 *)(param_1 + 0x4c), param_2, param_3, param_4, param_5,
                                                                param_6);
    }
    return 2;
}

u32 func_ov175_021dfb18(Ov175Work *param_1, s8 *param_2, s32 param_3, s32 param_4, s32 param_5, u32 param_6, u32 param_7, u32 param_8) {
    if (param_1 == NULL) {
        return 2;
    }
    if (param_4 == 0) {
        return 2;
    }
    if (param_5 == 0) {
        return 2;
    }
    if (param_1->unk_14[15] == 0) {
        return 3;
    }
    func_ov175_021df948((s32)param_1, param_2);
    return func_ov175_021e0a7c((Ov175Task *)param_1, param_1->unk_14[14], (u8 *)param_1 + 0x5c, (u32)((u8 *)param_1 + 0x84), param_3, (Ov175Blob *)param_4, (u8 *)param_5, param_6, param_7, param_8);
}

u32 func_ov175_021dfbb8(Ov175Ctx *param_1, Ov175Work *param_2, s32 param_3, s32 param_4, s32 param_5, u32 param_6, u32 param_7, u32 param_8) {
    u32 context[22];
    u32 i;
    u8 *p;
    Ov175Sub *sub;

    if (param_1 == NULL) {
        return 2;
    }
    if (param_1->unk_50 == 0) {
        return 3;
    }
    sub = param_2->unk_124;
    p = (u8 *)sub + 8;
    for (i = 4; i != 0; i--) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p += 4;
    }
    func_ov009_02184938(context);
    func_ov009_0218497c((s32 *)context, param_2->unk_124, param_2->unk_120);
    func_ov009_02184a34((s32)((u8 *)sub + 8), (s32 *)context);
    return func_ov175_021e0db8((Ov175Pend *)param_1, param_1->unk_4c, (u8 *)param_1 + 0x5c, (u8 *)param_1 + 0x84, param_2, (u8 *)param_3, (Ov175Blob *)param_4, (u8 *)param_5, param_6, param_7, param_8);
}

u32 func_ov175_021dfc8c(Ov175Work *param_1, s32 param_2, s32 param_3, s32 param_4, u32 *param_5) {
    u32 value;
    u32 result;
    value = 0;
    if (param_1 == NULL) {
        return 2;
    }
    if (param_5 == NULL) {
        return 2;
    }
    result = func_ov175_021dfe2c((u8 *)param_1 + 0x5c, param_2, param_3, param_4, &value);
    if (result == 0) {
        *param_5 = value;
    }
    return result;
}

u32 func_ov175_021dfce8(Ov175Work *param_1) {
    return func_ov175_021e0040(param_1);
}

u32 func_ov175_021dfcf4(u32 *param_1) {
    return func_ov175_021e0070(param_1);
}

u32 func_ov175_021dfd00(u32 *param_1) {
    return func_ov175_021e0080(param_1);
}

u32 func_ov175_021dfd0c(s32 param_1) {
    return func_ov175_021e0000((Ov175Work *)param_1);
}

u32 func_ov175_021dfd18(s32 param_1, s32 param_2, u32 param_3) {
    return func_ov175_021e0098((Ov175Work *)param_1, param_2, param_3);
}

u32 func_ov175_021dfd24(Ov175Work *param_1, s32 param_2, u8 *param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7, s32 param_8) {
    SCResult error = SCResult_NO_ERROR;
    u32 i;
    BOOL add = TRUE;

    for (i = 0; i < param_1->unk_10; i++) {
        if (param_4 == param_1->unk_14[i]) {
            add = FALSE;
            break;
        }
    }
    if (add) {
        param_1->unk_10++;
        if (param_1->unk_10 > 0x40) {
            return SCResult_OUT_OF_MEMORY;
        }
        param_1->unk_14[param_1->unk_10 - 1] = param_4;
    }
    if (error == SCResult_NO_ERROR) {
        error = func_ov175_021dff78(param_1, param_2, param_3);
    }
    if (error == SCResult_NO_ERROR) {
        error = func_ov175_021dffa0(param_1, param_2, param_4);
    }
    if (error == SCResult_NO_ERROR) {
        error = func_ov175_021dffc8(param_1, param_2, param_5);
    }
    if (error == SCResult_NO_ERROR) {
        error = func_ov175_021dfff8(param_1, param_2, param_7, param_8);
    }
    return error;
}

u32 func_ov175_021dfdfc(s32 *param_1, s16 param_2, u32 param_3) {
    return func_ov175_021e02f0((Ov175Work *)param_1, param_2, param_3);
}

u32 func_ov175_021dfe08(s32 *param_1, s16 param_2, u8 param_3) {
    return func_ov175_021e03f8((Ov175Work *)param_1, param_2, param_3);
}

u32 func_ov175_021dfe14(Ov175_021dd1ecWork3 *param_1) {
    return func_ov175_021dff30(param_1);
}

u8 *func_ov175_021dfe20(void) {
    return func_ov175_021df968();
}
