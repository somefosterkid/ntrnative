// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/std.h, the lines in use so far.
#ifndef BWPORT_GFL_STD_H
#define BWPORT_GFL_STD_H

#include "types.h"
#include "gfl/heap.h"
#include "nitro/mi.h"
#include "nitro/std.h"

u32 GFL_STD_StrLen(const char *str);
u16 getCRC16(const void *data, u32 size);
s32 GFL_STD_MemCmp(const void *a, const void *b, u32 size);

// Assertions (assert.c), and printing their messages (debug_print.c). A failed one calls the fail callback; debug
// builds show the message on the main screen through the handlers, which this build stores without calling
typedef void (*AssertInitFunc)(void);
typedef void (*AssertPrintFunc)(const char *message);
typedef void (*AssertFinishFunc)(void);
typedef void (*AssertFailCallback)(void);

void GFL_DebugSetAssertHandlers(AssertInitFunc init, AssertPrintFunc print, AssertFinishFunc finish);
void GFL_DebugSetAssertFailCallback(AssertFailCallback callback);
// Handlers that print to BG 0 of the main engine and stop the game
void GFL_DebugPrintCreateSurface(void);
void GFL_DebugPrintOutputMessage(const char *message);
void GFL_DebugPrintCommit(void);
void GFL_DebugSetVerboseAssertHandlers(void);

// A failed assertion, with its file, line and expression or printf-style message. Black's GFL is built with all three
// (the game's code passes "" and 0), so GFL_ASSERT_AT takes the original file's line, which this file's lines don't keep
void GFL_DebugAssertFail(const char *file, u32 line, const char *expression);
void GFL_DebugAssertFailEx(const char *file, u32 line, const char *format, ...);
#define GFL_ASSERT_AT(line, expression)                                                                                   \
    do {                                                                                                               \
        if (!(expression)) {                                                                                           \
            GFL_DebugAssertFail(__FILE__, line, #expression);                                                          \
        }                                                                                                              \
    } while (0)
#define GFL_ASSERT_MSG_AT(line, expression, ...)                                                                       \
    do {                                                                                                               \
        if (!(expression)) {                                                                                           \
            GFL_DebugAssertFailEx(__FILE__, line, __VA_ARGS__);                                                        \
        }                                                                                                              \
    } while (0)

// The game's code is built without the file, the line and the expression
#define GFL_ASSERT(expression)                                                                                         \
    do {                                                                                                               \
        if (!(expression)) {                                                                                           \
            GFL_DebugAssertFail("", 0, "");                                                                            \
        }                                                                                                              \
    } while (0)

#define GFL_ASSERT_MSG(expression, ...)                                                                                \
    do {                                                                                                               \
        if (!(expression)) {                                                                                           \
            GFL_DebugAssertFailEx("", 0, __VA_ARGS__);                                                                 \
        }                                                                                                              \
    } while (0)

u32 addUpDataStream(const u8 *data, u32 size);
// Encrypts or decrypts data by XOR with a stream from a linear congruential generator, two bytes at a time
void decryptData(void *data, u32 size, u32 seed);
// Steps the generator of decryptData and returns its next value
u16 crypt_SAV_BVideo_MG_misc_data(u32 *seed);

// Game Freak's standard library (gf_standard.c): its tables are allocated once, from a heap
void initTableArea(HeapID heapId);
// Seeds the Mersenne Twister of GFL_RandomMT
void GFL_RandomUpdateMT(u32 seed);
// pokebw2's crypt_SAV_BVideo_MG_misc_data
// Steps the generator of decryptData and returns its next value
u16 func_0203f388(u32 *seed);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void MI_CpuFill(void *dest, u8 data, u32 size);
void _decryptData(void *data, u32 size, u32 seed);
// Compares two strings with STD_CompareString, whatever swan's name says
int _STD_CompareNString(const char *a, const char *b);
// Copies size bytes even when the ranges overlap
void sys_memcpy_ex(const void *src, void *dest, u32 size);
void sys_memcpy_fast(const void *src, void *dest, u32 size);

#endif // BWPORT_GFL_STD_H
