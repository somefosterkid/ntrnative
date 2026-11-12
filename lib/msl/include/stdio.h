// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/stdio.h, with Black's
// additions
#ifndef _C_STDIO_H
#define _C_STDIO_H

#include <stddef.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
#include <stdio_api.h>
#include <file_pos.h>
#include <file_io.h>
#include <printf.h>
#include <scanf.h>
// clang-format on

#ifdef __cplusplus
}
#endif

#endif
