// TwlSDK's system call library for the DSi (libsyscall.twl's syscall_twl.o): Black's main 0x020911d8-0x0209128c, the
// wrappers of the DSi BIOS's SHA-1 and HMAC-SHA-1 calls, and its twl/ltdmain_begin.h block in the LTD autoload's
// .ltdmain 0x02731754-0x0273199c, which ltd_autoload_0's delinks.txt names syscall_twl@ltd.c (tools/two_modules.py: one
// object, two modules): the BIOS calls (Thumb `svc` stubs) and HMAC-SHA-1 built from them (ARM). The wrappers reach
// them through the linker's veneers, one after each wrapper. The whole object is built with a 2.0 compiler
// (lib/twl/library.toml). From pokebw2's lib/twl/src/syscall/syscall_twl.c and syscall_twl_ltd.c (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2), its two halves of the one object
#include "nitro/os.h"
#include "nitro/mi.h"
#include "twl/os/common/systemCall.h"

BOOL OSi_IsRunOnTwl(void);

#pragma thumb on
// TwlSDK's SVC_SHA1Init
void func_020911d8(SVCSHA1Context *context) {
    if (!OSi_IsRunOnTwl()) {
        OS_Terminate();
    }
    func_02731754(context);
}

// TwlSDK's SVC_SHA1Update
void func_02091200(SVCSHA1Context *context, const void *input, u32 len) {
    if (!OSi_IsRunOnTwl()) {
        OS_Terminate();
    }
    func_02731760(context, input, len);
}

// TwlSDK's SVC_SHA1GetHash
void func_02091230(SVCSHA1Context *context, void *digest) {
    if (!OSi_IsRunOnTwl()) {
        OS_Terminate();
    }
    func_02731764(context, digest);
}

void SVC_CalcHMACSHA1(void *digest, const void *bin_ptr, u32 bin_len, const void *key_ptr, u32 key_len) {
    if (!OSi_IsRunOnTwl()) {
        OS_Terminate();
    }
    func_02731920(digest, bin_ptr, bin_len, key_ptr, key_len);
}
#pragma thumb off

// The HMAC's context: a SHA-1 context with the key after it
typedef struct {
    SVCSHA1Context sha1;
    u8 key[64];
    u32 keyLength;
} SVCHMACSHA1Context;

static void func_02731770(void *digest, const void *input, u32 len);
static void func_02731774(SVCHMACSHA1Context *context, const void *key, u32 keyLength);
static void func_0273184c(SVCHMACSHA1Context *context, const void *input, u32 len);
static void func_02731868(SVCHMACSHA1Context *context, void *digest);

#include "twl/ltdmain_begin.h"

#pragma thumb on
// clang-format off
// The SHA-1 init SVC (0x24), after clearing the word after the BIOS's 0x60 bytes
asm void func_02731754(SVCSHA1Context *context) {
    mov r3, #0x60
    add r3, r0
    mov r2, #0
    str r2, [r3]
    swi 0x24
    bx lr
}

// The SHA-1 update SVC
asm void func_02731760(SVCSHA1Context *context, const void *input, u32 len) {
    swi 0x25
    bx lr
}

// The SHA-1 final SVC, which takes the digest first
asm void func_02731764(SVCSHA1Context *context, void *digest) {
    mov ip, r1
    mov r1, r0
    mov r0, ip
    swi 0x26
    bx lr
}

// The one-shot SHA-1 SVC
static asm void func_02731770(void *digest, const void *input, u32 len) {
    swi 0x27
    bx lr
}
// clang-format on
#pragma thumb off

static void func_02731774(SVCHMACSHA1Context *context, const void *key, u32 keyLength) {
    u8 pad[64];
    int i;

    if (context == NULL || (keyLength != 0 && key == NULL)) {
        return;
    }

    if (keyLength > sizeof(context->key)) {
        func_02731770(context->key, key, keyLength);
        context->keyLength = 20;
    } else {
        MI_CpuCopy8(key, context->key, keyLength);
        context->keyLength = keyLength;
    }

    for (i = 0; i < context->keyLength; i++) {
        pad[i] = context->key[i] ^ 0x36;
    }
    for (; i < 64; i++) {
        pad[i] = 0x36;
    }

    func_02731754(&context->sha1);
    func_02731760(&context->sha1, pad, sizeof(pad));
}

static void func_0273184c(SVCHMACSHA1Context *context, const void *input, u32 len) {
    if (context != NULL && len != 0 && input != NULL) {
        func_02731760(&context->sha1, input, len);
    }
}

static void func_02731868(SVCHMACSHA1Context *context, void *digest) {
    u8 pad[64];
    u8 innerHash[20];
    int i;

    if (context == NULL || digest == NULL) {
        return;
    }

    func_02731764(&context->sha1, innerHash);
    for (i = 0; i < context->keyLength; i++) {
        pad[i] = context->key[i] ^ 0x5c;
    }
    for (; i < 64; i++) {
        pad[i] = 0x5c;
    }

    func_02731754(&context->sha1);
    func_02731760(&context->sha1, pad, sizeof(pad));
    func_02731760(&context->sha1, innerHash, sizeof(innerHash));
    func_02731764(&context->sha1, digest);
}

void func_02731920(void *digest, const void *input, u32 len, const void *key, u32 keyLength) {
    SVCHMACSHA1Context context;

    if (digest == NULL) {
        return;
    }
    if (len != 0 && input == NULL) {
        return;
    }
    if (keyLength != 0 && key == NULL) {
        return;
    }

    func_02731774(&context, key, keyLength);
    func_0273184c(&context, input, len);
    func_02731868(&context, digest);
}

#include "twl/ltdmain_end.h"
