// MSL's string extras: the case-insensitive compares. The file's name is pokediamond's object's (MSL_string_extras.o),
// the functions' pokebw2's.
#include <ctype.h>
#include <file_io.h>
#include <string.h>

int stricmp(const char *s1, const char *s2) {
    unsigned char c1;
    unsigned char c2;

    do {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }
    } while (c1 != 0);
    return 0;
}

int strnicmp(const char *s1, const char *s2, int n) {
    return __msl_strnicmp(s1, s2, n);
}

int strcmpi(const char *s1, const char *s2) {
    return stricmp(s1, s2);
}
