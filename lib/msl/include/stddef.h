// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/stddef.h, with NULL as the
// game's types.h has it, so that the two don't clash
#if !defined(__MWERKS__) && !defined(__INTELLISENSE__) && !defined(__arm__)
// A PC target (the port): the host's own size_t, ptrdiff_t, wchar_t, NULL and offsetof, not the SDK's 32-bit types
// (x64's size_t is 64 bits, and clang for Windows has it built in). MWCC and the tools' clang for ARM take the SDK's.
// Outside the guard, so that the other copy's #include_next (lib/nitro/sdk/stddef.h, lib/msl/include/stddef.h) goes
// on through this one to the host's.
#include_next <stddef.h>
#else
#ifndef _C_STDDEF_H
#define _C_STDDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef nullptr
    #define nullptr 0
#endif

#define offsetof(ST, M) ((size_t) &(((ST *) 0)->M))

typedef signed long intptr_t;
typedef unsigned long uintptr_t;
typedef intptr_t ptrdiff_t;

// gcc want size_t to be an unsigned int, and metrowerks want it to be an unsigned long (oh and vscode also doesn't
// agree....................)
#if defined(__MWERKS__) || defined(__INTELLISENSE__)
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
#endif
