#include "types.h"
#include "gfl/str.h"
#include "gfl/str_sjis.h"

static BOOL CharUnicodeToSjis(const u16 *src, char *dest, BOOL *end);

void StrUnicodeToSjis(const u16 *src, char *dest, u32 length) {
    u32 i = 0;
    u32 j = 0;

    while (i < length) {
        BOOL end = FALSE;

        if (CharUnicodeToSjis(&src[i], &dest[j], &end) == TRUE) {
            i++;
            j += 2;
        } else {
            i++;
            j++;
        }
        if (end == TRUE) {
            break;
        }
    }
}

#define SET_SJIS(dest, sjis)                                                                                           \
    do {                                                                                                               \
        (dest)[0] = (char)(((sjis) & 0xff00) >> 8);                                                                    \
        (dest)[1] = (char)(sjis);                                                                                      \
    } while (0)

// Converts a character, and returns whether it took two bytes. A terminator ends the string with 0 and sets end, and
// characters without a Shift JIS form become '?'
static BOOL CharUnicodeToSjis(const u16 *src, char *dest, BOOL *end) {
    u16 c = *src;
    BOOL twoBytes = FALSE;
    u16 sjis;

    if (c == GFL_StrBufGetTerminator()) {
        *dest = '\0';
        if (end != NULL) {
            *end = TRUE;
        }
    } else if (c >= 0x20 && c <= 0x7e) {
        *dest = c;
    } else if (c >= 0x3041 && c <= 0x3093) {
        // Hiragana
        sjis = c - 0x3041 + 0x829f;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c >= 0x30a1 && c <= 0x30df) {
        // Katakana, which skip 0x837f
        sjis = c - 0x30a1 + 0x8340;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c >= 0x30e0 && c <= 0x30f3) {
        sjis = c - 0x30e0 + 0x8380;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c >= 0xff21 && c <= 0xff3a) {
        // Full width letters and digits
        sjis = c - 0xff21 + 0x8260;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c >= 0xff41 && c <= 0xff5a) {
        sjis = c - 0xff41 + 0x8281;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c >= 0xff10 && c <= 0xff19) {
        sjis = c - 0xff10 + 0x824f;
        SET_SJIS(dest, sjis);
        twoBytes = TRUE;
    } else if (c == 0x30fc) {
        // ー
        SET_SJIS(dest, 0x815b);
        twoBytes = TRUE;
    } else if (c == 0x8140) {
        // Shift JIS's own full width space, kept as it is
        SET_SJIS(dest, 0x8140);
        twoBytes = TRUE;
    } else if (c == 0x2642) {
        // ♂
        SET_SJIS(dest, 0x8189);
        twoBytes = TRUE;
    } else if (c == 0x2640) {
        // ♀
        SET_SJIS(dest, 0x818a);
        twoBytes = TRUE;
    } else {
        *dest = '?';
    }
    return twoBytes;
}
