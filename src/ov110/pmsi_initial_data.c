// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov185/pmsi_initial_data.c, the same code and
// table in Black 2/White 2's overlay 185. The names are Black's dsd names; pokebw2's guesses are in the header and the
// comments.
#include "app/pmsi_initial_data.h"
#include "types.h"
#include "app/pms_input_data.h"
#include "gfl/str.h"

// The initials that the phrase input sorts words by, and where their buttons are on the lower screen. The ROM doesn't
// name this file; pmsi_initial_data.c is pokebw2's guess after its neighbors

typedef struct {
    u16 code;
    u8 x;
    u8 y;
    u8 up;
    u8 down;
    u8 left;
    u8 right;
    u8 column;
    u8 bottom;
} PMSIInitial;

// sPMSIInitials
static const PMSIInitial data_ov110_021c37a8[PMSI_INITIAL_COUNT] = {
    { 'A', 5, 24, PMSI_INITIAL_POS_BUTTON_0, 10, 9, 1, 0, 20 },
    { 'B', 29, 24, PMSI_INITIAL_POS_BUTTON_0, 11, 0, 2, 1, 21 },
    { 'C', 53, 24, PMSI_INITIAL_POS_BUTTON_0, 12, 1, 3, 2, 22 },
    { 'D', 77, 24, PMSI_INITIAL_POS_BUTTON_0, 13, 2, 4, 3, 23 },
    { 'E', 101, 24, PMSI_INITIAL_POS_BUTTON_1, 14, 3, 5, 4, 24 },
    { 'F', 125, 24, PMSI_INITIAL_POS_BUTTON_1, 15, 4, 6, 5, 25 },
    { 'G', 149, 24, PMSI_INITIAL_POS_BUTTON_1, 16, 5, 7, 6, 16 },
    { 'H', 173, 24, PMSI_INITIAL_POS_BUTTON_2, 17, 6, 8, 7, 17 },
    { 'I', 197, 24, PMSI_INITIAL_POS_BUTTON_2, 18, 7, 9, 8, 18 },
    { 'J', 221, 24, PMSI_INITIAL_POS_BUTTON_2, 19, 8, 0, 9, 26 },
    { 'K', 5, 48, 0, 20, 19, 11, 0, 20 },
    { 'L', 29, 48, 1, 21, 10, 12, 1, 21 },
    { 'M', 53, 48, 2, 22, 11, 13, 2, 22 },
    { 'N', 77, 48, 3, 23, 12, 14, 3, 23 },
    { 'O', 101, 48, 4, 24, 13, 15, 4, 24 },
    { 'P', 125, 48, 5, 25, 14, 16, 5, 25 },
    { 'Q', 149, 48, 6, PMSI_INITIAL_POS_BUTTON_1, 15, 17, 6, 16 },
    { 'R', 173, 48, 7, PMSI_INITIAL_POS_BUTTON_2, 16, 18, 7, 17 },
    { 'S', 197, 48, 8, PMSI_INITIAL_POS_BUTTON_2, 17, 19, 8, 18 },
    { 'T', 221, 48, 9, 26, 18, 10, 9, 26 },
    { 'U', 5, 72, 10, PMSI_INITIAL_POS_BUTTON_0, 26, 21, 0, 20 },
    { 'V', 29, 72, 11, PMSI_INITIAL_POS_BUTTON_0, 20, 22, 1, 21 },
    { 'W', 53, 72, 12, PMSI_INITIAL_POS_BUTTON_0, 21, 23, 2, 22 },
    { 'X', 77, 72, 13, PMSI_INITIAL_POS_BUTTON_0, 22, 24, 3, 23 },
    { 'Y', 101, 72, 14, PMSI_INITIAL_POS_BUTTON_1, 23, 25, 4, 24 },
    { 'Z', 125, 72, 15, PMSI_INITIAL_POS_BUTTON_1, 24, 26, 5, 25 },
    { '!', 221, 72, 19, PMSI_INITIAL_POS_BUTTON_2, 25, 20, 6, 26 },
};

u32 func_ov110_021bb1f4(void) {
    return PMSI_INITIAL_COUNT;
}

void func_ov110_021bb1f8(u32 initial, StrBuf *buf) {
    GFL_StrBufLoadFixedString(buf, &data_ov110_021c37a8[initial].code, 2);
}

u16 func_ov110_021bb214(u32 initial) {
    return data_ov110_021c37a8[initial].code;
}

void func_ov110_021bb224(u32 initial, u32 *x, u32 *y) {
    *x = data_ov110_021c37a8[initial].x;
    *y = data_ov110_021c37a8[initial].y;
}

u32 func_ov110_021bb240(u32 initial) {
    return data_ov110_021c37a8[initial].up;
}

u32 func_ov110_021bb250(u32 initial) {
    return data_ov110_021c37a8[initial].down;
}

u32 func_ov110_021bb260(u32 initial) {
    return data_ov110_021c37a8[initial].left;
}

u32 func_ov110_021bb270(u32 initial) {
    return data_ov110_021c37a8[initial].right;
}

u32 func_ov110_021bb280(u32 initial) {
    return data_ov110_021c37a8[initial].bottom;
}
