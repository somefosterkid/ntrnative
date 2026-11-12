// Black's sciReport.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.

#include "dwc/webservices_internal.h"


u32 func_ov175_021dfe2c(u8 *unused, u32 param_1, s32 param_2, s32 param_3, u32 *param_5) {
    Ov175Work *work;
    Ov175_021dd1ecWork4 *buf;
    u16 *hbuf;
    s32 t20;
    s32 t16 = param_2 * 16;
    t20 = t16 + param_2 * 4;
    work = gsimalloc(0x128);
    if (work == NULL) {
        return 5;
    }
    memset(work, 0, 0x128);
    buf = gsimalloc(0x10000);
    if (buf == NULL) {
        gsifree(work);
        return 5;
    }
    memset(buf, 0, 0x10000);
    work->unk_114[0] = 0;
    work->unk_114[2] = 0x10000;
    memset(buf, 0, 0x44);
    buf->unk_00 = 0x2000000;
    buf->unk_04 = ((param_1 >> 24) & 0xff) | ((param_1 >> 8) & 0xff00) | ((param_1 << 8) & 0xff0000) | ((param_1 << 24) & 0xff000000);
    buf->unk_2c = t20;
    buf->unk_30 = t16;
    buf->unk_34 = (param_2 + param_3) * 4;
    hbuf = (u16 *)buf;
    buf->unk_20 = param_2;
    buf->unk_22 = param_3;
    work->unk_0 = 1;
    work->unk_124 = (Ov175Sub *)buf;
    work->unk_4 = -1;
    work->unk_10 = 0;
    *param_5 = (u32)work;
    return 0;
}

u32 func_ov175_021dff30(Ov175_021dd1ecWork3 *param_1) {
    param_1->unk_00 = 0;
    param_1->unk_08 = 0;
    param_1->unk_04 = 0;
    param_1->unk_10 = 0;
    param_1->unk_0c = 0;
    gsifree((void *)param_1->unk_124);
    param_1->unk_124 = 0;
    param_1->unk_11c = 0;
    param_1->unk_118 = 0;
    param_1->unk_120 = 0;
    gsifree(param_1);
    return 0;
}

u32 func_ov175_021dff78(Ov175Work *param_1, s32 param_2, u8 *param_3) {
    param_2 = param_2 * 0x14 + 0x44;
    func_ov175_021e05ac(param_2 + (u8 *)param_1->unk_124, param_3);
    return 0;
}

u32 func_ov175_021dffa0(Ov175Work *param_1, s32 param_2, u32 param_3) {
    int val;
    s32 *offsetPtr;
    u32 offset;
    offset = 0x14;
    val = param_2 * offset;
    offset = val;
    offsetPtr = &offset;
    func_ov175_021e054c((u8 *)param_1->unk_124 + (*offsetPtr + 0x54), param_3);
    return 0;
}

u32 func_ov175_021dffc8(Ov175Work *param_1, s32 param_2, u32 param_3) {
    Ov175Sub *sub;

    sub = param_1->unk_124;
    func_ov175_021e054c((u8 *)sub + (sub->unk_2c + 0x44 + sub->unk_30 + param_2 * 4), param_3);
    return 0;
}

u32 func_ov175_021dfff8(Ov175Work *param_1, s32 param_2, s32 param_3, s32 param_4) {
    return 0;
}

u32 func_ov175_021e0000(Ov175Work *param_1) {
    s32 offset;
    Ov175Sub *sub;
    offset = param_1->unk_120;
    sub = param_1->unk_124;
    param_1->unk_8 = 0;
    param_1->unk_4 = offset;
    param_1->unk_120 = offset + 2;
    func_ov175_021e0504((u8 *)sub + offset, 0);
    sub->unk_3c += 2;
    return 0;
}

u32 func_ov175_021e0040(Ov175Work *param_1) {
    Ov175Sub *sub;
    sub = param_1->unk_124;
    param_1->unk_0 = 2;
    param_1->unk_120 = sub->unk_2c + 0x44 + sub->unk_30 + sub->unk_34;
    return 0;
}

u32 func_ov175_021e0070(u32 *param_1) {
    *param_1 = 3;
    return 0;
}

u32 func_ov175_021e0080(u32 *param_1) {
    *param_1 = 4;
    return 0;
}

u32 func_ov175_021e0090(void) {
    return 0;
}

u32 func_ov175_021e0098(Ov175Work *param_1, s32 param_2, u32 param_3) {
    Ov175Sub *sub;

    sub = param_1->unk_124;
    func_ov175_021e0090();
    if (param_2 != 0) {
        sub->unk_1c |= 1;
    }
    sub->unk_18 = Ov175Swap32(param_3);
    sub->unk_1c = Ov175Swap32(sub->unk_1c);
    sub->unk_20 = Ov175Swap16(sub->unk_20);
    sub->unk_22 = Ov175Swap16(sub->unk_22);
    sub->unk_2a = 0;
    sub->unk_24 = Ov175Swap16(sub->unk_24);
    sub->unk_26 = Ov175Swap16(sub->unk_26);
    sub->unk_28 = Ov175Swap16(sub->unk_28);
    sub->unk_2c = Ov175Swap32(sub->unk_2c);
    sub->unk_30 = Ov175Swap32(sub->unk_30);
    sub->unk_34 = Ov175Swap32(sub->unk_34);
    sub->unk_38 = Ov175Swap32(sub->unk_38);
    sub->unk_3c = Ov175Swap32(sub->unk_3c);
    sub->unk_40 = Ov175Swap32(sub->unk_40);
    return 0;
}

u32 func_ov175_021e02f0(Ov175Work *param_1, int param_2, u32 param_3) {
    Ov175Sub *sub = param_1->unk_124;

    if (param_1->unk_0 == 2) {
        sub->unk_24++;
        sub->unk_38 += 8;
    } else if (param_1->unk_0 == 3) {
        sub->unk_26++;
        sub->unk_3c += 8;
        param_1->unk_8++;
        func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_4, param_1->unk_8);
    } else if (param_1->unk_0 == 4) {
        sub->unk_28++;
        sub->unk_40 += 8;
        param_1->unk_8++;
        func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_4, param_1->unk_8);
    }
    func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_120, param_2);
    param_1->unk_120 += 2;
    func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_120, 0);
    param_1->unk_120 += 2;
    func_ov175_021e054c((u8 *)param_1->unk_124 + param_1->unk_120, param_3);
    param_1->unk_120 += 4;
    return 0;
}

u32 func_ov175_021e03f8(Ov175Work *param_1, int param_2, u32 param_3) {
    Ov175Sub *sub = param_1->unk_124;

    if (param_1->unk_0 == 2) {
        sub->unk_24++;
        sub->unk_38 += 5;
    } else if (param_1->unk_0 == 3) {
        sub->unk_26++;
        sub->unk_3c += 5;
        param_1->unk_8++;
        func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_4, param_1->unk_8);
    } else if (param_1->unk_0 == 4) {
        sub->unk_28++;
        sub->unk_40 += 5;
        param_1->unk_8++;
        func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_4, param_1->unk_8);
    }
    func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_120, param_2);
    param_1->unk_120 += 2;
    func_ov175_021e0504((u8 *)param_1->unk_124 + param_1->unk_120, 2);
    param_1->unk_120 += 2;
    func_ov175_021e04fc((u8 *)param_1->unk_124 + param_1->unk_120, param_3);
    param_1->unk_120 += 1;
    return 0;
}
