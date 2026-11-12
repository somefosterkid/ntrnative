// Black's sakeRequestModify.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.
// Black lacks sakeiStartDeleteRecordRequest: the linker dead-stripped it with sakeMain.c's sakeDeleteRecord, and kept
// the description of the delete request it passes (and the two functions the description names).

#include "dwc/webservices_internal.h"


u32 func_ov175_021ddf74(Ov175Op *param_1);
u32 func_ov175_021ddfbc(Ov175Op *param_1);
u32 func_ov175_021ddff8(Ov175Op *param_1);
u32 func_ov175_021de038(Ov175Op *param_1);
u32 func_ov175_021de078(Ov175Op *param_1);
u32 func_ov175_021de0e8(Ov175Op *param_1);
u32 func_ov175_021de100(Ov175Op *param_1);

// The descriptions of the three requests (GameSpy's SAKEIRequestInfo, pokebw2's sCreateRecordType and so on)
static Ov175Desc data_ov175_021e39a0 = {
    4,
    "CreateRecord",
    "SOAPAction: \"http://gamespy.net/sake/CreateRecord\"",
    "CreateRecordResponse",
    "CreateRecordResult",
    func_ov175_021ddf74,
    func_ov175_021ddfbc,
    func_ov175_021ddff8,
    NULL,
    NULL,
};

static Ov175Desc data_ov175_021e3978 = {
    0,
    "UpdateRecord",
    "SOAPAction: \"http://gamespy.net/sake/UpdateRecord\"",
    "UpdateRecordResponse",
    "UpdateRecordResult",
    func_ov175_021de038,
    func_ov175_021de078,
    NULL,
    NULL,
    NULL,
};

static Ov175Desc data_ov175_021e39c8 = {
    0,
    "DeleteRecord",
    "SOAPAction: \"http://gamespy.net/sake/DeleteRecord\"",
    "DeleteRecordResponse",
    "DeleteRecordResult",
    func_ov175_021de0e8,
    func_ov175_021de100,
    NULL,
    NULL,
    NULL,
};

u32 func_ov175_021ddb38(Ov175Rec *param_1, s32 param_2) {
    s32 i;
    for (i = 0; i < param_2; i++) {
        if (param_1[i].unk_0 == NULL || param_1[i].unk_0[0] == 0) {
            return 7;
        }
        if (param_1[i].unk_4 >= 10) {
            return 8;
        }
        if (param_1[i].unk_4 == 4 && param_1[i].v.sp == NULL) {
            return 9;
        }
        if (param_1[i].unk_4 == 5 && param_1[i].v.hp == NULL) {
            return 9;
        }
        if (param_1[i].unk_4 == 8) {
            if (param_1[i].v.bin.len < 0) {
                return 9;
            }
            if (param_1[i].v.bin.data == NULL && param_1[i].v.bin.len > 0) {
                return 9;
            }
        }
    }
    return 0;
}

void func_ov175_021ddbe4(Ov175Op *param_1, Ov175Rec *param_2, s32 param_3) {
    s32 i;
    func_ov009_021829ac(param_1->unk_18, "ns1", "values");
    for (i = 0; i < param_3; i++) {
        func_ov009_021829ac(param_1->unk_18, "ns1", "RecordField");
        func_ov009_02182ac0(param_1->unk_18, "ns1", "name", param_2[i].unk_0);
        func_ov009_021829ac(param_1->unk_18, "ns1", "value");
        if (param_2[i].unk_4 == 0) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "byteValue");
            func_ov009_02182cf0(param_1->unk_18, "ns1", "value", param_2[i].v.b);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "byteValue");
        } else if (param_2[i].unk_4 == 1) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "shortValue");
            func_ov009_02182cf0(param_1->unk_18, "ns1", "value", param_2[i].v.h);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "shortValue");
        } else if (param_2[i].unk_4 == 2) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "intValue");
            func_ov009_02182cf0(param_1->unk_18, "ns1", "value", param_2[i].v.w);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "intValue");
        } else if (param_2[i].unk_4 == 9) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "int64Value");
            func_ov009_02182d78(param_1->unk_18, "ns1", "value", param_2[i].v.i64);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "int64Value");
        } else if (param_2[i].unk_4 == 3) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "floatValue");
            func_ov009_02182dfc(param_1->unk_18, "ns1", "value", param_2[i].v.f);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "floatValue");
        } else if (param_2[i].unk_4 == 4) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "asciiStringValue");
            func_ov009_02182ac0(param_1->unk_18, "ns1", "value", param_2[i].v.sp);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "asciiStringValue");
        } else if (param_2[i].unk_4 == 5) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "unicodeStringValue");
            func_ov009_02182c10(param_1->unk_18, "ns1", "value", param_2[i].v.hp);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "unicodeStringValue");
        } else if (param_2[i].unk_4 == 6) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "booleanValue");
            func_ov009_02182cf0(param_1->unk_18, "ns1", "value", (param_2[i].v.w != 0));
            func_ov009_02182a2c(param_1->unk_18, "ns1", "booleanValue");
        } else if (param_2[i].unk_4 == 7) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "dateAndTimeValue");
            func_ov009_02182ff8(param_1->unk_18, "ns1", "value", param_2[i].v.w);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "dateAndTimeValue");
        } else if (param_2[i].unk_4 == 8) {
            func_ov009_021829ac(param_1->unk_18, "ns1", "binaryDataValue");
            func_ov009_02182f40(param_1->unk_18, "ns1", "value", (const u8 *)param_2[i].v.bin.data, param_2[i].v.bin.len);
            func_ov009_02182a2c(param_1->unk_18, "ns1", "binaryDataValue");
        }
        func_ov009_02182a2c(param_1->unk_18, "ns1", "value");
        func_ov009_02182a2c(param_1->unk_18, "ns1", "RecordField");
    }
    func_ov009_02182a2c(param_1->unk_18, "ns1", "values");
}

u32 func_ov175_021ddf74(Ov175Op *param_1) {
    Ov175Inner *inner;
    s32 n;
    inner = param_1->unk_8;
    if (inner->unk_0 == 0) {
        return 4;
    }
    n = inner->unk_8;
    if (n < 0) {
        return 6;
    }
    if (inner->unk_4 == 0 && n > 0) {
        return 5;
    }
    return func_ov175_021ddb38((Ov175Rec *)inner->unk_4, n);
}

u32 func_ov175_021ddfbc(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    func_ov175_021ddbe4(param_1, (Ov175Rec *)ptr[1], ptr[2]);
    return 0;
}

u32 func_ov175_021ddff8(Ov175Op *param_1) {
    u32 result;

    result = func_ov009_02183a24(param_1->unk_1c, "recordid",
                                                            param_1->unk_c);
    if (result == 0) {
        result = 5;
    }
    else {
        result = 0;
    }
    return result;
}

u32 func_ov175_021de024(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e39a0);
}

u32 func_ov175_021de038(Ov175Op *param_1) {
    Ov175_021dd1ecWork5 *ptr;
    ptr = param_1->unk_8;
    if (ptr->unk_00 == 0) {
        return 4;
    }
    if (0 >= ptr->unk_0c) {
        return 6;
    }
    if (ptr->unk_08 == 0) {
        return 5;
    }
    return func_ov175_021ddb38((Ov175Rec *)ptr->unk_08, ptr->unk_0c);
}

u32 func_ov175_021de078(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    func_ov009_02182cf0(param_1->unk_18, "ns1", "recordid", ptr[1]);
    func_ov175_021ddbe4(param_1, (Ov175Rec *)ptr[2], ptr[3]);
    return 0;
}

u32 func_ov175_021de0d4(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e3978);
}

u32 func_ov175_021de0e8(Ov175Op *param_1) {
    u32 val;
    if (*(s32 *)param_1->unk_8 != 0) {
        val = 0;
    } else {
        val = 4;
    }
    return val;
}

u32 func_ov175_021de100(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    func_ov009_02182cf0(param_1->unk_18, "ns1", "recordid", ptr[1]);
    return 0;
}

u32 sakeiStartDeleteRecordRequest(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e39c8);
}
