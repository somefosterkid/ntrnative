// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/mem.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <mem_funcs.h>
#include <stddef.h>

void *memcpy(void *dst, const void *src, size_t n) {
    const unsigned char *p = (unsigned char *) src;
    unsigned char *q       = (unsigned char *) dst;

    for (n++; --n;) {
        *q++ = *p++;
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t len) {
    unsigned char *csrc;
    unsigned char *cdst;

    int reverse = (unsigned int) src < (unsigned int) dst;

    if (!reverse) {
        for (csrc = (const char *) src, cdst = (char *) dst, len++; --len;) {
            *cdst++ = *csrc++;
        }
    } else {
        for (csrc = (const char *) src + len, cdst = (char *) dst + len, len++; --len;) {
            *--cdst = *--csrc;
        }
    }

    return dst;
}

void *memset(void *dest, int val, size_t n) {
    __fill_mem(dest, val, n);
    return dest;
}

void *memchr(const void *src, int val, size_t n) {
    const unsigned char *p;

    unsigned int v = (val & 0xFF);

    for (p = (unsigned char *) src, n++; --n;) {
        if (*p++ == v) {
            return ((void *) (p - 1));
        }
    }

    return NULL;
}

int memcmp(const void *src1, const void *src2, size_t n) {
    const unsigned char *p1;
    const unsigned char *p2;

    for (p1 = (const unsigned char *) src1, p2 = (const unsigned char *) src2, n++; --n;) {
        if (*p1++ != *p2++) {
            return ((*--p1 < *--p2) ? -1 : +1);
        }
    }

    return 0;
}
