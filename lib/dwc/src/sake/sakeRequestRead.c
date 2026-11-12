// Black's sakeRequestRead.c (GameSpy's file, as pokebw2 names it), split from the glued ov175_021dd1ec.c.
// Black lacks sakeiStartSearchForRecordsRequest and sakeiStartGetRandomRecordRequest: the linker dead-stripped them
// with sakeMain.c's sakeSearchForRecords and sakeGetRandomRecord, and kept the descriptions of the two requests they
// pass (and the functions the descriptions name).

#include "dwc/webservices_internal.h"


u32 func_ov175_021de95c(Ov175Op *param_1);
u32 func_ov175_021de9bc(Ov175Op *param_1);
u32 func_ov175_021dead4(Ov175Op *param_1);
void func_ov175_021deaf8(Ov175Op *param_1);
u32 func_ov175_021deb1c(Ov175Op *param_1);
u32 func_ov175_021deb5c(Ov175Op *param_1);
u32 func_ov175_021deb98(Ov175Op *param_1);
void func_ov175_021debbc(Ov175Op *param_1);
u32 func_ov175_021debf4(Ov175Op *param_1);
u32 func_ov175_021dec54(Ov175Op *param_1);
u32 func_ov175_021deca0(Ov175Op *param_1);
void func_ov175_021decc4(Ov175Op *param_1);
u32 func_ov175_021decfc(Ov175Op *param_1);
u32 func_ov175_021ded3c(Ov175Op *param_1);
u32 func_ov175_021dedb4(Ov175Op *param_1);
void func_ov175_021dee18(Ov175Op *param_1);

// The descriptions of the four requests (GameSpy's SAKEIRequestInfo, pokebw2's sSearchRecordsType and so on)
static Ov175Desc data_ov175_021e3cf0 = {
    8,
    "SearchForRecords",
    "SOAPAction: \"http://gamespy.net/sake/SearchForRecords\"",
    "SearchForRecordsResponse",
    "SearchForRecordsResult",
    func_ov175_021de95c,
    func_ov175_021de9bc,
    func_ov175_021dead4,
    func_ov175_021deaf8,
    NULL,
};

static Ov175Desc data_ov175_021e3ca0 = {
    8,
    "GetMyRecords",
    "SOAPAction: \"http://gamespy.net/sake/GetMyRecords\"",
    "GetMyRecordsResponse",
    "GetMyRecordsResult",
    func_ov175_021deb1c,
    func_ov175_021deb5c,
    func_ov175_021deb98,
    func_ov175_021debbc,
    NULL,
};

static Ov175Desc data_ov175_021e3c78 = {
    8,
    "GetSpecificRecords",
    "SOAPAction: \"http://gamespy.net/sake/GetSpecificRecords\"",
    "GetSpecificRecordsResponse",
    "GetSpecificRecordsResult",
    func_ov175_021debf4,
    func_ov175_021dec54,
    func_ov175_021deca0,
    func_ov175_021decc4,
    NULL,
};

static Ov175Desc data_ov175_021e3cc8 = {
    4,
    "GetRandomRecords",
    "SOAPAction: \"http://gamespy.net/sake/GetRandomRecords\"",
    "GetRandomRecordsResponse",
    "GetRandomRecordsResult",
    func_ov175_021decfc,
    func_ov175_021ded3c,
    func_ov175_021dedb4,
    func_ov175_021dee18,
    NULL,
};

u32 func_ov175_021de14c(s8 **param_1, s32 param_2) {
    s32 i;
    for (i = 0; i < param_2; i++) {
        if (param_1[i] == NULL || param_1[i][0] == 0) {
            return 7;
        }
    }
    return 0;
}

void func_ov175_021de184(Ov175Op *param_1, s32 param_2, s32 param_3) {
    s32 i;

    func_ov009_021829ac(param_1->unk_18, "ns1", "fields");
    i = 0;
    if (0 < param_3) {
        do {
            func_ov009_02182ac0(param_1->unk_18, "ns1", "string",
                                                    *(s8 **)(param_2 + i * 4));
            i = i + 1;
        } while (i < param_3);
    }
    func_ov009_02182a2c(param_1->unk_18, "ns1", "fields");
}

void func_ov175_021de1f8(Ov175Op *param_1, s32 param_2, s32 param_3) {
    s32 i;

    func_ov009_021829ac(param_1->unk_18, "ns1", "recordids");
    i = 0;
    if (0 < param_3) {
        do {
            func_ov009_02182cf0(param_1->unk_18, "ns1", "int",
                                                    *(u32 *)(param_2 + i * 4));
            i = i + 1;
        } while (i < param_3);
    }
    func_ov009_02182a2c(param_1->unk_18, "ns1", "recordids");
}

void func_ov175_021de26c(Ov175Op *param_1, s32 param_2, s32 param_3) {
    s32 i;

    func_ov009_021829ac(param_1->unk_18, "ns1", "ownerids");
    i = 0;
    if (0 < param_3) {
        do {
            func_ov009_02182cf0(param_1->unk_18, "ns1", "int",
                                                    *(u32 *)(param_2 + i * 4));
            i = i + 1;
        } while (i < param_3);
    }
    func_ov009_02182a2c(param_1->unk_18, "ns1", "ownerids");
}

u32 func_ov175_021de2e0(Ov175Op *param_1, u32 *param_2, s32 *param_3, s32 param_4, s32 param_5) {
    Ov175Op *ctx = param_1;
    s32 *keys = (s32 *)param_5;
    u32 vByte;
    u32 vShort;
    u32 vInt;
    s64 vI64;
    u32 vFloat;
    char *str1;
    s32 len1;
    char *str2;
    s32 len2;
    char *str3;
    s32 len3;
    u16 *date;
    s32 binLen;
    Ov175Rec **list;
    s32 i;
    s32 size;
    Ov175Rec *recs;
    Ov175Rec *rec;
    s32 count;
    s32 j;
    u16 *wstr;
    u8 *bin;
    s32 flag;

    if (func_ov009_021835d0(ctx->unk_1c, "values") == 0) {
        return 5;
    }
    count = func_ov009_02183ee0(ctx->unk_1c, "ArrayOfRecordValue");
    *param_3 = count;
    if (count == 0) {
        *param_2 = 0;
        return 0;
    }
    list = gsimalloc(count * 4);
    if (list == NULL) {
        return 6;
    }
    i = 0;
    memset(list, 0, count * 4);
    *param_2 = (u32)list;
    if (*param_3 > 0) {
        size = param_4 * 0x10;
        do {
            if (func_ov009_021834e8(ctx->unk_1c, "ArrayOfRecordValue") == 0) {
                return 5;
            }
            recs = gsimalloc(size);
            if (recs == NULL) {
                return 6;
            }
            j = 0;
            memset(recs, 0, size);
            list[i] = recs;
            if (param_4 != func_ov009_02183ee0(ctx->unk_1c, "RecordValue")) {
                return 5;
            }
            if (param_4 > 0) {
                do {
                    rec = &recs[j];
                    recs[j].unk_0 = (s8 *)keys[j];
                    if (func_ov009_021834e8(ctx->unk_1c, "RecordValue") == 0) {
                        return 5;
                    }
                    if (func_ov009_021835d0(ctx->unk_1c, "byteValue") != 0) {
                        if (func_ov009_02183a24(ctx->unk_1c, "value", &vByte) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 0;
                        rec->v.b = vByte;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "shortValue") != 0) {
                        if (func_ov009_02183a24(ctx->unk_1c, "value", &vShort) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 1;
                        rec->v.h = vShort;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "intValue") != 0) {
                        if (func_ov009_02183a24(ctx->unk_1c, "value", &vInt) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 2;
                        rec->v.w = vInt;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "int64Value") != 0) {
                        if (func_ov009_02183ae4(ctx->unk_1c, "value", &vI64) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 9;
                        rec->v.i64 = vI64;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "floatValue") != 0) {
                        if (func_ov009_02183cd4(ctx->unk_1c, "value", (f32 *)&vFloat) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 3;
                        rec->v.w = vFloat;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "asciiStringValue") != 0) {
                        if (func_ov009_02183670(ctx->unk_1c, "value", (s32 *)&str1, &len1) == 0) {
                            return 5;
                        }
                        if (str1 != NULL) {
                            str1[len1] = 0;
                        } else {
                            str1 = "";
                        }
                        rec->unk_4 = 4;
                        rec->v.sp = (s8 *)str1;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "unicodeStringValue") != 0) {
                        if (func_ov009_02183670(ctx->unk_1c, "value", (s32 *)&str2, &len2) == 0) {
                            return 5;
                        }
                        if (str2 != NULL) {
                            str2[len2] = 0;
                        } else {
                            str2 = "";
                        }
                        wstr = UTF8ToUCS2StringAlloc((s8 *)str2);
                        if (wstr == NULL) {
                            return 6;
                        }
                        rec->unk_4 = 5;
                        rec->v.hp = wstr;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "booleanValue") != 0) {
                        if (func_ov009_02183670(ctx->unk_1c, "value", (s32 *)&str3, &len3) == 0) {
                            return 5;
                        }
                        if (str3 != NULL) {
                            str3[len3] = 0;
                            flag = strcmp(str3, "true") == 0 ? 1 : 0;
                        } else {
                            flag = 0;
                        }
                        rec->unk_4 = 6;
                        rec->v.w = flag;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "dateAndTimeValue") != 0) {
                        if (func_ov009_02183ba4(ctx->unk_1c, "value", (s32 *)&date) == 0) {
                            return 5;
                        }
                        rec->unk_4 = 7;
                        rec->v.hp = date;
                    } else if (func_ov009_021835d0(ctx->unk_1c, "binaryDataValue") != 0) {
                        if (func_ov009_02183918(ctx->unk_1c, "value", NULL, &binLen) == 0) {
                            return 5;
                        }
                        if (binLen > 0) {
                            bin = gsimalloc(binLen);
                            if (bin == NULL) {
                                return 6;
                            }
                            if (func_ov009_02183918(ctx->unk_1c, "value", bin, &binLen) == 0) {
                                gsifree(bin);
                                return 5;
                            }
                        } else {
                            binLen = 0;
                            bin = NULL;
                        }
                        rec->unk_4 = 8;
                        rec->v.bin.len = binLen;
                        rec->v.bin.data = (s8 *)bin;
                    } else {
                        return 0x16;
                    }
                    j++;
                } while (j < param_4);
            }
            i++;
        } while (i < *param_3);
    }
    return 0;
}

void func_ov175_021de840(s32 param_1, Ov175Rec *param_2) {
    s32 i;
    if (param_2 == NULL) {
        return;
    }
    for (i = 0; i < param_1; i++) {
        if (param_2[i].unk_4 == 8 && param_2[i].v.bin.data != NULL) {
            gsifree(param_2[i].v.bin.data);
        }
        if (param_2[i].unk_4 == 5) {
            gsifree(param_2[i].v.bin.data);
        }
    }
    gsifree(param_2);
}

void func_ov175_021de8ac(s32 param_1, s32 param_2, s32 param_3) {
    void *ptr;
    s32 val;
    s32 i;
    s32 j;

    if (param_3 != 0) {
        j = 0;
        if (0 < param_2) {
            do {
                if (*(s32 *)(param_3 + j * 4) != 0) {
                    i = 0;
                    if (0 < param_1) {
                        do {
                            val = *(s32 *)(param_3 + j * 4) + i * 0x10;
                            if ((*(s32 *)(val + 4) == 8) && (ptr = *(void **)(val + 8), ptr != (void *)0x0)) {
                                gsifree(ptr);
                            }
                            val = *(s32 *)(param_3 + j * 4) + i * 0x10;
                            if (*(s32 *)(val + 4) == 5) {
                                gsifree(*(void **)(val + 8));
                            }
                            i = i + 1;
                        } while (i < param_1);
                    }
                    gsifree(*(void **)(param_3 + j * 4));
                    *(u32 *)(param_3 + j * 4) = 0;
                }
                j = j + 1;
            } while (j < param_2);
        }
        gsifree((void *)param_3);
        return;
    }
}

u32 func_ov175_021de95c(Ov175Op *param_1) {
    Ov175Req *req;
    req = param_1->unk_8;
    if (req->unk_0 == 0) {
        return 4;
    }
    if (req->unk_8 <= 0) {
        return 6;
    }
    if (req->unk_4 == NULL) {
        return 5;
    }
    if (req->unk_14 < 0) {
        return 10;
    }
    if (req->unk_18 <= 0) {
        return 11;
    }
    return func_ov175_021de14c(req->unk_4, req->unk_8);
}

u32 func_ov175_021de9bc(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    if ((s8 *)ptr[3] != (s8 *)0x0) {
        func_ov009_02182ac0(param_1->unk_18, "ns1", "filter", (s8 *)ptr[3]);
    }
    if ((s8 *)ptr[4] != (s8 *)0x0) {
        func_ov009_02182ac0(param_1->unk_18, "ns1", "sort", (s8 *)ptr[4]);
    }
    func_ov009_02182cf0(param_1->unk_18, "ns1", "offset", ptr[5]);
    func_ov009_02182cf0(param_1->unk_18, "ns1", "max", ptr[6]);
    if ((s8 *)ptr[7] != (s8 *)0x0) {
        func_ov009_02182ac0(param_1->unk_18, "ns1", "targetfilter", (s8 *)ptr[7]);
    }
    func_ov009_02182cf0(param_1->unk_18, "ns1", "surrounding", ptr[8]);
    func_ov175_021de26c(param_1, ptr[9], ptr[10]);
    func_ov009_02182cf0(param_1->unk_18, "ns1", "cacheFlag", ptr[0xb]);
    func_ov175_021de184(param_1, ptr[1], ptr[2]);
    return 0;
}

u32 func_ov175_021dead4(Ov175Op *param_1) {
    Ov175Pair *out = param_1->unk_c;
    Ov175Inner *inner = param_1->unk_8;
    return func_ov175_021de2e0(param_1, (u32 *)&out->unk_4, &out->unk_0, inner->unk_8, inner->unk_4);
}

void func_ov175_021deaf8(Ov175Op *param_1) {
    Ov175Pair *pair;
    Ov175Inner *inner;
    pair = param_1->unk_c;
    inner = param_1->unk_8;
    if (pair == NULL) {
        return;
    }
    func_ov175_021de8ac(inner->unk_8, pair->unk_0, pair->unk_4);
}

u32 sakeiStartSearchForRecordsRequest(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e3cf0);
}

u32 func_ov175_021deb1c(Ov175Op *param_1) {
    s32 *ptr;
    ptr = param_1->unk_8;
    if (*ptr == 0) {
        return 4;
    }
    if (0 >= ptr[2]) {
        return 6;
    }
    if (ptr[1] == 0) {
        return 5;
    }
    return func_ov175_021de14c((s8 **)ptr[1], ptr[2]);
}

u32 func_ov175_021deb5c(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    func_ov175_021de184(param_1, ptr[1], ptr[2]);
    return 0;
}

u32 func_ov175_021deb98(Ov175Op *param_1) {
    Ov175Pair *out = param_1->unk_c;
    Ov175Inner *inner = param_1->unk_8;
    return func_ov175_021de2e0(param_1, (u32 *)&out->unk_4, &out->unk_0, inner->unk_8, inner->unk_4);
}

void func_ov175_021debbc(Ov175Op *param_1) {
    Ov175Pair *pair;
    Ov175Inner *inner;
    pair = param_1->unk_c;
    inner = param_1->unk_8;
    if (pair == NULL) {
        return;
    }
    func_ov175_021de8ac(inner->unk_8, pair->unk_0, pair->unk_4);
}

u32 func_ov175_021debe0(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e3ca0);
}

u32 func_ov175_021debf4(Ov175Op *param_1) {
    Ov175Req *req;
    req = param_1->unk_8;
    if (req->unk_0 == 0) {
        return 4;
    }
    if (req->unk_8 <= 0) {
        return 0xd;
    }
    if (req->unk_4 == NULL) {
        return 0xc;
    }
    if (req->unk_10 <= 0) {
        return 6;
    }
    if (req->unk_c == NULL) {
        return 5;
    }
    return func_ov175_021de14c(req->unk_c, req->unk_10);
}

u32 func_ov175_021dec54(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    func_ov175_021de1f8(param_1, ptr[1], ptr[2]);
    func_ov175_021de184(param_1, ptr[3], ptr[4]);
    return 0;
}

u32 func_ov175_021deca0(Ov175Op *param_1) {
    Ov175Pair *out = param_1->unk_c;
    Ov175Inner *inner = param_1->unk_8;
    return func_ov175_021de2e0(param_1, (u32 *)&out->unk_4, &out->unk_0, inner->unk_10, inner->unk_c);
}

void func_ov175_021decc4(Ov175Op *param_1) {
    Ov175Pair *pair;
    Ov175Inner *inner;

    pair = param_1->unk_c;
    inner = param_1->unk_8;
    if (pair == NULL) {
        return;
    }
    func_ov175_021de8ac(inner->unk_10, pair->unk_0, pair->unk_4);
}

u32 func_ov175_021dece8(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e3c78);
}

u32 func_ov175_021decfc(Ov175Op *param_1) {
    s32 *ptr;
    ptr = param_1->unk_8;
    if (*ptr == 0) {
        return 4;
    }
    if (0 >= ptr[2]) {
        return 6;
    }
    if (ptr[1] == 0) {
        return 5;
    }
    return func_ov175_021de14c((s8 **)ptr[1], ptr[2]);
}

u32 func_ov175_021ded3c(Ov175Op *param_1) {
    u32 *ptr;

    ptr = param_1->unk_8;
    func_ov009_02182ac0(param_1->unk_18, "ns1", "tableid", (s8 *)*ptr);
    if ((s8 *)ptr[3] != (s8 *)0x0) {
        func_ov009_02182ac0(param_1->unk_18, "ns1", "filter", (s8 *)ptr[3]);
    }
    func_ov009_02182cf0(param_1->unk_18, "ns1", "max", 1);
    func_ov175_021de184(param_1, ptr[1], ptr[2]);
    return 0;
}

u32 func_ov175_021dedb4(Ov175Op *param_1) {
    Ov175Pair *out;
    Ov175Inner *inner;
    s32 *ptr;
    s32 count;
    s32 result;
    out = param_1->unk_c;
    inner = param_1->unk_8;
    result = func_ov175_021de2e0(param_1, (u32 *)&ptr, &count, inner->unk_8, inner->unk_4);
    if (result == 0) {
        if (ptr != NULL && count > 0) {
            out->unk_0 = *ptr;
        } else {
            out->unk_0 = 0;
        }
    }
    gsifree(ptr);
    return result;
}

void func_ov175_021dee18(Ov175Op *param_1) {
    Ov175Pair *pair;
    Ov175Inner *inner;

    pair = param_1->unk_c;
    inner = param_1->unk_8;
    if (pair == NULL) {
        return;
    }
    func_ov175_021de840(inner->unk_8, (Ov175Rec *)pair->unk_0);
}

u32 sakeiStartGetRandomRecordRequest(Ov175Op *param_1) {
    return func_ov175_021ddb10(param_1, &data_ov175_021e3cc8);
}
