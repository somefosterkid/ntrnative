// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/string.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). strcat, strchr, strcspn and strstr, which zeldaret-st lacks, are
// MSL's.
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define K1 0x80808080
#define K2 0xFEFEFEFF

size_t strlen(const char *str) {
    int length       = -1;
    unsigned char *p = (unsigned char *) str;

    do {
        length++;
    } while (*p++);

    return length;
}

char *strcpy(char *dest, const char *src) {
    register unsigned char *destb, *fromb;
    register unsigned int w, t, align;
    register unsigned int k1, k2;

    fromb = (unsigned char *) src;
    destb = (unsigned char *) dest;

    if ((align = ((unsigned int) fromb & 3)) != ((unsigned int) destb & 3)) {
        goto bytecopy;
    }

    if (align != 0) {
        if ((*destb = *fromb) == 0) {
            return dest;
        }

        for (align = 3 - align; align != 0; align--) {
            if ((*++destb = *++fromb) == 0) {
                return dest;
            }
        }
        ++destb;
        ++fromb;
    }

    k1 = K1;
    k2 = K2;

    w = *((int *) fromb);
    t = w + k2;
    t &= ~w;
    t &= k1;

    if (t != 0) {
        goto bytecopy;
    }

    --((int *) (destb));

    do {
        *(++((int *) destb)) = w;
        w                    = *(++((int *) fromb));
        t                    = w + k2;
        t &= ~w;
        t &= k1;
        if (t != 0) {
            goto adjust;
        }
    } while (true);

adjust:
    ++((int *) destb);
bytecopy:
    if ((*destb = *fromb) == 0) {
        return dest;
    }

    do {
        if ((*++destb = *++fromb) == 0) {
            return dest;
        }
    } while (true);

    return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
    const unsigned char *p = (const unsigned char *) src;
    unsigned char *q       = (unsigned char *) dest;

    n++;

    while (--n != 0) {
        if ((*q++ = *p++) == 0) {
            while (--n != 0) {
                *q++ = 0;
            }
            break;
        }
    }

    return dest;
}

char *strcat(char *dest, const char *src) {
    unsigned char *p       = (unsigned char *) dest;
    const unsigned char *s = (const unsigned char *) src;

    while (*p++ != 0)
        ;

    p--;

    while ((*p++ = *s++) != 0)
        ;

    return dest;
}

int strcmp(const char *str1, const char *str2) {
    register unsigned char *left  = (unsigned char *) str1;
    register unsigned char *right = (unsigned char *) str2;
    unsigned int k1, k2, align, l1, r1, x;

    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return (l1 - r1);
    }

    if ((align = ((unsigned int) left & 3)) != ((unsigned int) right & 3)) {
        goto bytecopy;
    }
    if (align) {
        if (l1 == 0) {
            return (0);
        }
        for (align = 3 - align; align; align--) {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1) {
                return (l1 - r1);
            }
            if (l1 == 0) {
                return (0);
            }
        }
        left++;
        right++;
    }

    k1 = K1;
    k2 = K2;

    l1 = *(unsigned int *) left;
    r1 = *(unsigned int *) right;
    x  = l1 + k2;
    x &= ~l1;
    if (x & k1) {
        goto adjust;
    }
    while (l1 == r1) {
        l1 = *(++((unsigned int *) (left)));
        r1 = *(++((unsigned int *) (right)));
        x  = l1 + k2;
        if (x & k1) {
            goto adjust;
        }
    }

    --left;
    --right;
    goto bytecopy;

adjust:
    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return (l1 - r1);
    }
bytecopy:
    if (l1 == 0) {
        return 0;
    }

    do {
        l1 = *(++left);
        r1 = *(++right);
        if (l1 - r1) {
            return (l1 - r1);
        }
        if (l1 == 0) {
            return 0;
        }
    } while (true);
}

int strncmp(const char *str1, const char *str2, size_t n) {
    const unsigned char *p1 = (unsigned char *) str1;
    const unsigned char *p2 = (unsigned char *) str2;
    unsigned int c1, c2;

    n++;

    while (--n) {
        if ((c1 = *p1++) != (c2 = *p2++)) {
            return (c1 - c2);
        } else if (!c1) {
            break;
        }
    }

    return 0;
}

char *strchr(const char *str, int chr) {
    const unsigned char *p = (const unsigned char *) str;
    unsigned char c        = chr;
    unsigned char ch;

    while ((ch = *p++) != 0) {
        if (ch == c) {
            return (char *) p - 1;
        }
    }

    return c ? NULL : (char *) p - 1;
}

size_t strcspn(const char *str, const char *set) {
    const unsigned char *p;
    int c;
    unsigned char map[32] = {0};

    p = (unsigned char *) set;
    while ((c = *p++) != 0) {
        map[(unsigned char) c >> 3] |= (unsigned char) (1 << (c & 7));
    }

    p = (unsigned char *) str;
    while ((c = *p++) != 0) {
        if (map[(unsigned char) c >> 3] & (unsigned char) (1 << (c & 7))) {
            break;
        }
    }

    return p - (unsigned char *) str - 1;
}

char *strstr(const char *str, const char *pat) {
    const unsigned char *s1 = (const unsigned char *) str;
    const unsigned char *p1 = (const unsigned char *) pat;
    unsigned char firstc, c1, c2;

    if ((pat == NULL) || (!(firstc = *p1++))) {
        return (char *) str;
    }

    while ((c1 = *s1++) != 0) {
        if (c1 == firstc) {
            const unsigned char *s2 = s1;
            const unsigned char *p2 = p1;

            while (((c1 = *s2++) == (c2 = *p2++)) && c1)
                ;

            if (!c2) {
                return (char *) s1 - 1;
            }
        }
    }

    return NULL;
}
