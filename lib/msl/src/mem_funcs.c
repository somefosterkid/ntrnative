// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/mem_funcs.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <mem_funcs.h>

#define cps ((unsigned char *) src)
#define cpd ((unsigned char *) dst)
#define lps ((unsigned int *) src)
#define lpd ((unsigned int *) dst)

#define deref_auto_inc(p) *(p)++

void __fill_mem(void *dst, int val, unsigned int n) {
    unsigned int v = (unsigned char) val;
    unsigned int i;

    if (n >= 32) {
        i = (-(unsigned int) dst) & 3;

        if (i) {
            n -= i;
            do {
                deref_auto_inc(cpd) = v;
            } while (--i);
        }

        if (v) {
            v |= v << 24 | v << 16 | v << 8;
        }

        i = n >> 5;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        i = (n & 31) >> 2;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        n &= 3;
    }

    if (n) {
        do {
            deref_auto_inc(cpd) = v;
        } while (--n);
    }
}
