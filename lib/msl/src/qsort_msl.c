// MSL's qsort.c, a heap sort: kh-recoded-decomp's eu/libs/msl/c/calls/qsort.c (CC0-1.0,
// https://github.com/kh-recoded-decomp/kh-recoded-decomp) at 96de2ec, ported with MSL's types (the maintainer's decision,
// the maintainers' notes, 2026-10-13). MSL's qsort.c: NitroSDK has a qsort.c too, and the link wants each name once.
#include <stdlib.h>

static inline void SwapElements(unsigned char *right, unsigned char *left, size_t size) {
    while (size != 0) {
        unsigned char leftByte  = *left;
        unsigned char rightByte = *right;
        size--;
        *right++ = leftByte;
        *left++  = rightByte;
    }
}

/* The runtime qsort uses an in-place binary heap, with one-based indices. */
void qsort(void *base, size_t count, size_t size, int (*compare)(const void *, const void *)) {
    unsigned char *array = base;
    size_t root;
    unsigned char *first;
    unsigned char *last;
    if (count < 2) {
        return;
    }
    root  = count / 2 + 1;
    first = array + size * (root - 1);
    last  = array + size * (count - 1);
    for (;;) {
        if (root > 1) {
            root--;
            first -= size;
        } else {
            SwapElements(first, last, size);
            count--;
            if (count == 1) {
                return;
            }
            last -= size;
        }
        {
            size_t index         = root;
            unsigned char *child = array + size * (root - 1);
            while (count >= index * 2) {
                unsigned char *parent = child;
                index *= 2;
                child = array + size * (index - 1);
                if (count > index && compare(child, child + size) < 0) {
                    index++;
                    child += size;
                }
                if (compare(parent, child) >= 0) {
                    break;
                }
                SwapElements(parent, child, size);
            }
        }
    }
}
