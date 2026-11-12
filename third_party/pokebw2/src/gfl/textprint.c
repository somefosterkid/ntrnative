#include "types.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "gfl/systemfont.h"
#include "gfl/textprint.h"

// The glyph being drawn, as a 16 color tile followed by its width, and the position to draw the next one at
typedef struct {
    u8 glyph[0x20];
    u8 width;
    u16 x;
    u16 y;
} TextPrint;

// ASCII characters other than letters, digits and !"#$%&'()*+,-./
typedef struct {
    u8 c;
    u16 index;
} SpecialChar;

static TextPrint *sTextPrint;

static const SpecialChar sSpecialChars[] = {
    { ' ', TEXTPRINT_HALF_TAB },
    { '\n', TEXTPRINT_NEWLINE },
    { '?', 0xf6 },
    { '=', 0xf7 },
    { '_', 0x103 },
    { '[', 0xa7 },
    { ']', 0xa8 },
    { '@', 0x96 },
    { ':', 0x53 },
    { ';', 0x54 },
    { '<', 0x104 },
    { '>', 0x105 },
    { '^', 0x106 },
    { '~', 0x107 },
    { '\'', 0xa5 },
    { '\r', TEXTPRINT_SKIP },
};

void GFL_TextPrintInit(const char *path) {
    if (sTextPrint == NULL) {
        GFL_SystemFontInit(path);
        sTextPrint = GFL_HeapAllocate(HEAPID_SYSTEM, sizeof(TextPrint), FALSE, "textprint.c", 51);
    }
}

void GFL_TextPrintDrawIndices(const u16 *indices, TextPrintParam *param) {
    u16 terminator;
    GFLBitmap *glyph;
    u8 height = GFL_SystemFontGetHeight();
    u16 tabSize = GFL_SystemFontGetTabSize();
    u16 c;

    sTextPrint->x = param->x;
    sTextPrint->y = param->y;
    GFL_SystemFontSetColorIndices(param->fgColor, param->bgColor);
    glyph = GFL_BitmapWrap(sTextPrint->glyph, 1, 1, 0x20, HEAPID_SYSTEM);
    terminator = GFL_StrBufGetTerminator();
    while ((c = *indices) != terminator) {
        indices++;
        switch (c) {
        case TEXTPRINT_NEWLINE:
            sTextPrint->x = param->x;
            sTextPrint->y += (u16)(height + param->lineSpacing);
            break;
        case TEXTPRINT_TAB:
            sTextPrint->x += tabSize;
            break;
        case TEXTPRINT_HALF_TAB:
            sTextPrint->x += (u16)(tabSize / 2);
            break;
        default:
            GFL_SystemFontGetGlyph((u16)(c - TEXTPRINT_GLYPH_BASE), sTextPrint->glyph);
            GFL_BitmapCopyArea(glyph, param->dest, 0, 0, sTextPrint->x, sTextPrint->y, sTextPrint->width, height, 0);
            sTextPrint->x += (u16)(sTextPrint->width + param->letterSpacing);
            break;
        }
    }
    param->x = sTextPrint->x;
    param->y = sTextPrint->y;
    GFL_BitmapFree(glyph);
}

#define STRING_BUF_SIZE 0x200

void GFL_TextPrintDrawString(const char *str, TextPrintParam *param) {
    u16 *indices = GFL_HeapAllocate(HEAPID_SYSTEM, STRING_BUF_SIZE, FALSE, "textprint.c", 174);

    GFL_TextPrintStringToIndices(str, indices, STRING_BUF_SIZE / sizeof(u16));
    GFL_TextPrintDrawIndices(indices, param);
    GFL_HeapFree(indices);
}

void GFL_TextPrintStringToIndices(const char *str, u16 *dest, int max) {
    int count = 0;

    while (*str != '\0' && count < max - 1) {
        u8 c = *str;
        u16 index;

        str++;

        // The first byte of a two byte Shift JIS character
        if ((c >= 0x81 && c <= 0x9f) || c >= 0xe0) {
            index = GFL_TextPrintSJISToIndex((c << 8) + (u8)*str);
            str++;
        } else {
            index = GFL_TextPrintASCIIToIndex(c);
        }
        if (index != TEXTPRINT_SKIP) {
            *dest++ = index;
            count++;
        }
    }
    *dest = GFL_StrBufGetTerminator();
}

u16 GFL_TextPrintASCIIToIndex(u16 c) {
    u32 i;

    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 0xda;
    }
    if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 0xc0;
    }
    if (c >= '0' && c <= '9') {
        return c - '0' + 0xb6;
    }
    if (c >= '!' && c <= '/') {
        return c - '!' + 0xf4;
    }
    for (i = 0; i < NELEMS(sSpecialChars); i++) {
        if (c == sSpecialChars[i].c) {
            return sSpecialChars[i].index;
        }
    }
    return TEXTPRINT_UNKNOWN;
}

u16 GFL_TextPrintSJISToIndex(u16 c) {
    // Hiragana, katakana, and full width letters and digits
    if (c >= 0x829f && c <= 0x82f1) {
        return c - 0x829f + TEXTPRINT_GLYPH_BASE;
    }
    if (c >= 0x8340 && c <= 0x8393) {
        return c - 0x8340 + 0x57;
    }
    if (c >= 0x8281 && c <= 0x829a) {
        return c - 0x8281 + 0xda;
    }
    if (c >= 0x8260 && c <= 0x8279) {
        return c - 0x8260 + 0xc0;
    }
    if (c >= 0x824f && c <= 0x8258) {
        return c - 0x824f + 0xac;
    }
    // ♂, ♀, the full width space and ー
    if (c == 0x8189) {
        return 0xcc;
    }
    if (c == 0x818a) {
        return 0xc5;
    }
    if (c == 0x8140) {
        return 2;
    }
    if (c == 0x815b) {
        return 0x51;
    }
    return TEXTPRINT_UNKNOWN;
}
