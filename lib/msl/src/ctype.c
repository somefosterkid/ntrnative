// MSL's ctype.c: the C locale's tables, which ctype.h's inlines read. Its functions are inline, so only its .rodata is
// in Black. The table names are zeldaret-st's ctype.h's (CC0-1.0, https://github.com/zeldaret/st).
#include <ctype.h>

#define ctrl __msl_cntrl
#define motn (__msl_cntrl | __msl_space)
#define tabc (__msl_cntrl | __msl_blank | __msl_space)
#define spac (__msl_blank | __msl_print | __msl_space)
#define punc (__msl_graph | __msl_print | __msl_punct)
#define digi (__msl_digit | __msl_graph | __msl_print | __msl_xdigit)
#define hexl (__msl_alpha | __msl_graph | __msl_lower | __msl_print | __msl_xdigit)
#define lowc (__msl_alpha | __msl_graph | __msl_lower | __msl_print)
#define hexu (__msl_alpha | __msl_graph | __msl_print | __msl_upper | __msl_xdigit)
#define uppc (__msl_alpha | __msl_graph | __msl_print | __msl_upper)

const unsigned char __upper_mapC[__msl_cmap_size] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
    '`', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '{', '|', '}', '~', 0x7F,
};

const unsigned char __lower_mapC[__msl_cmap_size] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', '\\', ']', '^', '_',
    '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', 0x7F,
};

const unsigned short __ctype_mapC[__msl_cmap_size] = {
    ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, tabc, motn, motn, motn, motn, ctrl, ctrl,
    ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
    spac, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
    digi, digi, digi, digi, digi, digi, digi, digi, digi, digi, punc, punc, punc, punc, punc, punc,
    punc, hexu, hexu, hexu, hexu, hexu, hexu, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc,
    uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, punc, punc, punc, punc, punc,
    punc, hexl, hexl, hexl, hexl, hexl, hexl, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
    lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, punc, punc, punc, punc, ctrl,
};
