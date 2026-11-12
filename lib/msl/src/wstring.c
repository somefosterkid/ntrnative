// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/wstring.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <wstring.h>

size_t wcslen(const wchar_t *str) {
    size_t len = -1;

    do {
        len++;
    } while (*str++ != 0);

    return len;
}

wchar_t *wcscpy(wchar_t *dest, const wchar_t *src) {
    const wchar_t *p = src;
    wchar_t *q       = dest;

    while ((*q++ = *p++) != L'\0')
        ;

    return dest;
}
