// Black's code at ov009 0x0217fd7c-0x021800cc. The web-request task between gsPlatformUtil.c and stringutil.c in the
// library's file order (header soap.h; Black's version runs a task whose answer is parsed by xml.c, so the name is a
// guess).

#include "types.h"
#include "gs/nonport.h"
#include "gfl/ghttpEncryption.h"
#include "dwc/soap.h"

typedef struct SoapWork4 {
    void *unk_00;
    // What adds more to the post before it is sent, or NULL
    void (*unk_04)(struct GHIPost *post, void *param);
    const char *unk_08;
    const char *unk_0c;
    // The message posted, and the reader of the answer
    SoapWriter *unk_10;
    u32 *unk_14;
    char *unk_18;
    struct GHIPost *unk_1c;
    void *unk_20;
    s32 unk_24;
    // The HTTP request
    int unk_28;
    u32 unk_2c;
    u32 unk_30;
} SoapWork4;

u32 *func_ov009_0217fda0(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
u32 *func_ov009_02181c2c(void);
u32 func_ov009_02181d10(u32 *param_1, char *param_2, s32 param_3);
void func_ov009_02181da0(u32 *param_1);

void *gsiCoreCreateTask(void);
void func_ov009_0218005c(u32 *param_1, u32 param_2, u32 param_3, u32 param_4);
void func_ov009_0217ff94(SoapWork4 *soap);
u32 func_ov009_0217ff70(SoapWork4 *soap);
u32 func_ov009_02180080(SoapWork4 *soap);
void func_ov009_0218002c(SoapWork4 *soap);
void gsiCoreExecuteTask(s32 param_1, u32 param_2);

#pragma thumb off
u32 *func_ov009_0217fd7c(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5) {
    return func_ov009_0217fda0(param_1, param_2, param_3, param_4, 0, param_5);
}
#pragma thumb on

#pragma thumb off
u32 *func_ov009_0217fda0(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6) {
    u32 *ptr;
    void *theTask;

    ptr = gsimalloc(0x34U);
    if (ptr == NULL) {
        return NULL;
    }
    *(u32 *)ptr = param_4;
    *(s32 *)((u8 *)ptr + 4) = 0;
    *(u32 *)((u8 *)ptr + 8) = param_1;
    *(u32 *)((u8 *)ptr + 0xC) = param_2;
    *(u32 *)((u8 *)ptr + 0x10) = param_3;
    *(s32 *)((u8 *)ptr + 0x1C) = 0;
    *(s32 *)((u8 *)ptr + 0x14) = 0;
    *(s32 *)((u8 *)ptr + 0x18) = 0;
    *(u32 *)((u8 *)ptr + 0x20) = param_6;
    *(s32 *)((u8 *)ptr + 0x2C) = 0;
    *(s32 *)((u8 *)ptr + 0x30) = 0;
    theTask = gsiCoreCreateTask();
    if (theTask == NULL) {
        gsifree(ptr);
        return NULL;
    }
    *(void (**)(u32 *, u32, u32, u32))((u8 *)(u8 *)theTask + 0x28) = func_ov009_0218005c;
    *(void (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x24) = func_ov009_0217ff94;
    *(u32 (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x34) = func_ov009_0217ff70;
    *(u32 (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x30) = func_ov009_02180080;
    *(void (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x2C) = func_ov009_0218002c;
    *(u32 **)((u8 *)(u8 *)theTask + 0x20) = ptr;
    *(void **)((u8 *)ptr + 0x24) = theTask;
    gsiCoreExecuteTask((s32) theTask, param_5);
    return ptr;
}
#pragma thumb on

#pragma thumb off
u32 *func_ov009_0217fe68(u32 param_1, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6) {
    u32 *ptr;
    void *theTask;

    ptr = gsimalloc(0x34U);
    *(u32 *)ptr = param_4;
    *(u32 *)((u8 *)ptr + 4) = param_5;
    *(u32 *)((u8 *)ptr + 8) = param_1;
    *(u32 *)((u8 *)ptr + 0xC) = param_2;
    *(u32 *)((u8 *)ptr + 0x10) = param_3;
    *(s32 *)((u8 *)ptr + 0x1C) = 0;
    *(s32 *)((u8 *)ptr + 0x14) = 0;
    *(s32 *)((u8 *)ptr + 0x18) = 0;
    *(u32 *)((u8 *)ptr + 0x20) = param_6;
    *(s32 *)((u8 *)ptr + 0x2C) = 0;
    *(s32 *)((u8 *)ptr + 0x30) = 0;
    theTask = gsiCoreCreateTask();
    *(void (**)(u32 *, u32, u32, u32))((u8 *)(u8 *)theTask + 0x28) = func_ov009_0218005c;
    *(void (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x24) = func_ov009_0217ff94;
    *(u32 (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x34) = func_ov009_0217ff70;
    *(u32 (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x30) = func_ov009_02180080;
    *(void (**)(SoapWork4 *))((u8 *)(u8 *)theTask + 0x2C) = func_ov009_0218002c;
    *(u32 **)((u8 *)(u8 *)theTask + 0x20) = ptr;
    *(void **)((u8 *)ptr + 0x24) = theTask;
    gsiCoreExecuteTask((s32) theTask, 0U);
    return ptr;
}
#pragma thumb on

#pragma thumb off

BOOL func_ov009_0217ff14(int request, int param_2, char *param_3, int param_4, void *param) {
    SoapWork4 *param_5 = param;
    u32 *ptr;

    *(s32 *)&param_5->unk_2c = param_2;
    param_5->unk_30 = 1;
    param_5->unk_18 = param_3;
    if (param_2 == 0) {
        ptr = func_ov009_02181c2c();
        param_5->unk_14 = ptr;
        if (ptr == (u32 *)0x0) {
            param_5->unk_2c = 1;
        }
        else {
            if (func_ov009_02181d10(ptr, param_3, param_4) == 0) {
                param_5->unk_2c = 7;
            }
        }
    }
    return 0;
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_0217ff70(SoapWork4 *soap) {
    if (soap->unk_30 == 0) {
        func_ov114_021c2284(soap->unk_28);
        return 1;
    }
    return 4;
}

void func_ov009_0217ff94(SoapWork4 *soap) {
    soap->unk_1c = func_ov114_021c2304();
    if (soap->unk_1c == NULL) {
        soap->unk_30 = 1;
        soap->unk_2c = 1;
        return;
    }
    func_ov114_021c2310(soap->unk_1c, 0);
    func_ov114_021c2338(soap->unk_1c, soap->unk_10);
    if (soap->unk_04 != NULL) {
        soap->unk_04(soap->unk_1c, soap->unk_20);
    }
    soap->unk_28 = func_ov114_021c20a4(soap->unk_08, soap->unk_0c, NULL, 0, soap->unk_1c, FALSE, FALSE, NULL,
                                       func_ov009_0217ff14, soap);
}

#pragma thumb on

#pragma thumb off
void func_ov009_0218002c(SoapWork4 *soap) {
    s32 val;

    if (soap->unk_30 != 0) {
        return;
    }
    val = soap->unk_28;
    if (val >= 0) {
        func_ov114_021c22a4(val);
    }
    soap->unk_2c = 0x12;
}
#pragma thumb on

#pragma thumb off
void func_ov009_0218005c(u32 *param_1, u32 param_2, u32 param_3, u32 param_4) {
    ((s32 (*)(s32, s32, s32, s32)) *(u32 *)param_1)(*(s32 *)((u8 *)param_1 + 0x2C), *(s32 *)((u8 *)param_1 + 0x10), *(s32 *)((u8 *)param_1 + 0x14), *(s32 *)((u8 *)param_1 + 0x20));
}
#pragma thumb on

#pragma thumb off
u32 func_ov009_02180080(SoapWork4 *soap) {
    if (soap->unk_14 != NULL) {
        func_ov009_02181da0(soap->unk_14);
    }
    if (soap->unk_18 != NULL) {
        gsifree(soap->unk_18);
    }
    if (soap->unk_1c != NULL) {
        func_ov114_021c2324(soap->unk_28, soap->unk_1c);
    }
    gsifree(soap);
    return 1;
}
#pragma thumb on
