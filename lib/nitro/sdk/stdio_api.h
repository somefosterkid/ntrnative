// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3: libs/c/include/stdio_api.h, as it is (tools/port_lib.py)
#ifndef _C_STDIO_API_H
#define _C_STDIO_API_H

#include <file_struc.h>
#include <stddef.h>
#include <wchar_io.h>

#ifdef __cplusplus
extern "C" {
#endif

enum __ReadProcActions {
    __GetAChar,
    __UngetAChar,
    __TestForError
};

enum __WReadProcActions {
    __GetAwChar,
    __UngetAwChar,
    __TestForwcsError
};

typedef struct {
    char *CharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __OutStrCtrl;

typedef struct {
    char *NextChar;
    int NullCharDetected;
} __InStrCtrl;

typedef struct {
    wchar_t *wCharStr;
    size_t MaxCharCount;
    size_t CharsWritten;
} __wOutStrCtrl;

typedef struct {
    wchar_t *wNextChar;
    int wNullCharDetected;
} __wInStrCtrl;

size_t __fwrite(const void *pPtr, size_t memb_size, size_t num_memb, FILE *file);
int __StringRead(void *, int, int);

#ifdef __cplusplus
}
#endif

#endif
