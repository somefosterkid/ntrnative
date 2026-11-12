// TwlSDK's twl/os/common/systemCall.h, the DSi's BIOS calls.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/syscall.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_OS_COMMON_SYSTEMCALL_H
#define BWPORT_TWL_OS_COMMON_SYSTEMCALL_H

#include "types.h"

// The state of a SHA-1 computation, of which the BIOS keeps the first 0x60 bytes. The library's init core clears the
// word after them, and the HMAC's context continues after it (the key at 0x64, its length at 0xa4)
typedef struct {
    u8 unk_00[0x60];
    u32 unk_60;
} SVCSHA1Context;

// syscall_twl.c: TwlSDK's SVC_SHA1Init, SVC_SHA1Update and SVC_SHA1GetHash, under Black's names
void func_020911d8(SVCSHA1Context *context);
void func_02091200(SVCSHA1Context *context, const void *data, u32 length);
void func_02091230(SVCSHA1Context *context, void *digest);
void SVC_CalcHMACSHA1(void *digest, const void *bin_ptr, u32 bin_len, const void *key_ptr, u32 key_len);

// syscall_twl.c's twl/ltdmain_begin.h block, in the LTD autoload, that the wrappers above call through the linker's
// veneers: the BIOS calls for SHA-1 (system calls 0x24, 0x25 and 0x26; the first clears unk_60, the last takes the
// digest first), and the core of SVC_CalcHMACSHA1, ARM code
void func_02731754(SVCSHA1Context *context);
void func_02731760(SVCSHA1Context *context, const void *input, u32 len);
void func_02731764(SVCSHA1Context *context, void *digest);
void func_02731920(void *digest, const void *input, u32 len, const void *key, u32 keyLength);

#endif // BWPORT_TWL_OS_COMMON_SYSTEMCALL_H
