// From zeldaret-st (CC0-1.0, https://github.com/zeldaret/st) at 4fca7f3, libs/c/src/mbstring.c: ported as it is, with credit
// (a project decision), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py). wctomb and wcstombs, which zeldaret-st lacks, are MSL's.
#include <locale.h>
#include <stddef.h>

char *strncpy(char *dest, const char *src, size_t n);
size_t strlen(const char *str);

int __mbtowc_noconv(unsigned short *pwc, const char *str, int n) {
    const unsigned char *s = (const unsigned char *) str;

    if (s == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (pwc != NULL) {
        *pwc = (unsigned char) *s;
    }

    if (*s == 0) {
        return 0;
    }

    return 1;
}

int __wctomb_noconv(char *s, unsigned short wchar) {
    if (s == NULL) {
        return 0;
    }
    *s = wchar;
    return 1;
}

int wctomb(char *s, wchar_t wchar) {
    return _current_locale.ctype_cmpt_ptr->encode_wc(s, wchar);
}

size_t wcstombs(char *s, const wchar_t *pwcs, size_t n) {
    int chars_written = 0;
    int result;
    char temp[3];
    const wchar_t *source;

    if (!s || !pwcs) {
        return 0;
    }

    source = pwcs;
    while (chars_written <= n) {
        if (!*source) {
            *(s + chars_written) = '\0';
            break;
        } else {
            result = wctomb(temp, *source++);
            if ((chars_written + result) <= n) {
                strncpy(s + chars_written, temp, result);
                chars_written += result;
            } else {
                break;
            }
        }
    }

    return chars_written;
}
