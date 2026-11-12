// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dsprot/include/dsprot/dsprot.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DSPROT_DSPROT_H
#define BWPORT_DSPROT_DSPROT_H

#include "types.h"
#include "gfl/overlay.h"
#include "nitro/memorymap.h"

// MWCC's asm functions (the ones below written in assembly): a PC target's compiler reads plain prototypes, for
// the C the port writes in their place
#ifdef __MWERKS__
#define DSPROT_ASM asm
#else
#define DSPROT_ASM
#endif

#define OVERLAY_DSPROT OVERLAY_ID(337)
#define DSPROT_CHECKSUM 0x9f75a8d6
typedef void *(*DSProtCallback)(void *arg0, void *arg1);

// Calls a DS Protect function after verifying its code. If the checksum does not match, `tamper` is called instead.
// `nop` and `tamper` are put in a table in a random order, picked with the VBlank counter.
#define DSPROT_CHECKED_CALL(func, nop, tamper, arg0, arg1)                                                             \
    {                                                                                                                  \
        u32 index = *(u32 *)HW_VBLANK_COUNT_BUF & 1;                                                                   \
        u32 tamperIndex;                                                                                               \
        u32 i;                                                                                                         \
        u32 checksum;                                                                                                  \
        u32 *code;                                                                                                     \
        data_ov337_02182440 = index;                                                                                   \
        tamperIndex = index ^ 1;                                                                                       \
        data_ov337_02182444[index] = nop;                                                                              \
        data_ov337_02182444[tamperIndex] = tamper;                                                                     \
        code = (u32 *)func;                                                                                            \
        for (i = 0x25, checksum = 0; i != 0; i--) {                                                                    \
            checksum ^= (*code >> i) | (*code << (32 - i));                                                            \
            code++;                                                                                                    \
        }                                                                                                              \
        if (checksum == DSPROT_CHECKSUM) {                                                                             \
            func(arg0, arg1);                                                                                          \
        } else {                                                                                                       \
            data_ov337_02182444[tamperIndex](arg0, arg1);                                                              \
        }                                                                                                              \
    }

// Calls callback(arg0, arg1) through the DS Protect function func, which returns what the callback returns, after
// verifying func's code. If the checksum does not match, tamper(NULL, NULL) is called instead
#define DSPROT_CHECKED_RUN(ret, func, tamper, callback, arg0, arg1)                                                    \
    {                                                                                                                  \
        u32 i;                                                                                                         \
        u32 checksum;                                                                                                  \
        u32 *code = (u32 *)func;                                                                                       \
        for (i = 0x25, checksum = 0; i != 0; i--) {                                                                    \
            checksum ^= (*code >> i) | (*code << (32 - i));                                                            \
            code++;                                                                                                    \
        }                                                                                                              \
        if (checksum == DSPROT_CHECKSUM) {                                                                             \
            ret = func(callback, arg0, arg1);                                                                          \
        } else {                                                                                                       \
            ret = tamper(NULL, NULL);                                                                                  \
        }                                                                                                              \
    }

// dsprot_main.c's decryption wrappers (the reference's __DSProt_DetectFlashcart, _DetectNotFlashcart, _DetectEmulator,
// _DetectNotEmulator, _DetectDummy and _DetectNotDummy, in this order): each decrypts its check, runs it with the
// callback and param and encrypts it again, returning the check's answer
DSPROT_ASM u32 func_ov230_021882a0(void *callback, void *param, u32 unused);
DSPROT_ASM u32 func_ov230_021882dc(void *callback, void *param, u32 unused);
DSPROT_ASM u32 func_ov230_02188318(void *callback, void *param, u32 unused);
DSPROT_ASM u32 func_ov230_02188354(void *callback, void *param, u32 unused);
DSPROT_ASM u32 func_ov230_02188390(void *callback, void *param, u32 unused);
DSPROT_ASM u32 func_ov230_021883cc(void *callback, void *param, u32 unused);

#endif // BWPORT_DSPROT_DSPROT_H
