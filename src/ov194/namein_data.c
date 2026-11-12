// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov280/namein_data.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/namein_data.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "system/str_tool.h"
#include "gfl/std.h"

// The name-entry screen's tables, named after the ROM's "namein_data.c". The name-entry screen is overlay 280

// The name-entry screen's archive
#define ARCID_NAMEIN 0x77
// Its members: the keyboards, then the word tables, then the conversion table
#define NARC_NAMEIN_KEYBOARD_0 0
#define NARC_NAMEIN_WORD_TABLE_0 5
#define NARC_NAMEIN_CONV_TABLE 8

static void *func_ov194_021f4d20(u32 index, HeapID heapId);
static void func_ov194_021f4d34(NameinKeyboard *keyboard);
static u16 func_ov194_021f4dd8(const NameinWordTable *table);
static BOOL func_ov194_021f4f10(const u16 *word, const u16 *str);
static u16 func_ov194_021f4f68(const NameinConvTable *table);
static u16 func_ov194_021f4f6c(const NameinConvTable *table, u16 row, u16 column);

// pokebw2's NameinData_LoadKeyboard
static void *func_ov194_021f4d20(u32 index, HeapID heapId) {
    return GFL_ArcSysReadHeapNewLZ(ARCID_NAMEIN, NARC_NAMEIN_KEYBOARD_0 + index, FALSE, heapId);
}

// pokebw2's NameinData_FreeKeyboard
static void func_ov194_021f4d34(NameinKeyboard *keyboard) {
    GFL_HeapFree(keyboard);
}

// pokebw2's NameinData_LoadKeyboards
NameinKeyboardSet *func_ov194_021f4d3c(HeapID heapId) {
    NameinKeyboardSet *set = GFL_HeapAllocate(heapId, sizeof(NameinKeyboardSet), FALSE, "namein_data.c", 147);
    int i;

    MI_CpuFill8(set, 0, sizeof(NameinKeyboardSet));
    for (i = 0; i < NAMEIN_KEYBOARD_COUNT; i++) {
        set->keyboards[i] = func_ov194_021f4d20(i, heapId);
    }
    return set;
}

// pokebw2's NameinData_FreeKeyboards
void func_ov194_021f4d78(NameinKeyboardSet *set) {
    int i;

    for (i = 0; i < NAMEIN_KEYBOARD_COUNT; i++) {
        func_ov194_021f4d34(set->keyboards[i]);
    }
    GFL_HeapFree(set);
}

// pokebw2's NameinData_GetKeyboard
NameinKeyboard *func_ov194_021f4d94(const NameinKeyboardSet *set, u32 index) {
    return set->keyboards[index];
}

// pokebw2's NameinKeyboard_GetWidth
u16 func_ov194_021f4d9c(const NameinKeyboard *keyboard) {
    return keyboard->width;
}

// pokebw2's NameinKeyboard_GetHeight
u16 func_ov194_021f4da0(const NameinKeyboard *keyboard) {
    return keyboard->height;
}

// pokebw2's NameinKeyboard_GetChar
u16 func_ov194_021f4da4(const NameinKeyboard *keyboard, u16 x, u16 y) {
    return keyboard->chars[x + y * func_ov194_021f4d9c(keyboard)];
}

// pokebw2's NameinData_LoadWordTable
NameinWordTable *func_ov194_021f4dbc(u32 index, HeapID heapId) {
    return GFL_ArcSysReadHeapNewLZ(ARCID_NAMEIN, NARC_NAMEIN_WORD_TABLE_0 + index, FALSE, heapId);
}

// pokebw2's NameinData_FreeWordTable
void func_ov194_021f4dd0(NameinWordTable *table) {
    GFL_HeapFree(table);
}

// pokebw2's NameinData_WordTableCount
static u16 func_ov194_021f4dd8(const NameinWordTable *table) {
    return table->count;
}

// pokebw2's NameinWordTable_GetForm0
void func_ov194_021f4ddc(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len) {
    wcharsncpy(table->words[index].forms[0], dest, n);
    if (len != NULL) {
        for (*len = 0; *len < NAMEIN_WORD_CHARS && dest[*len] != NAMEIN_EMPTY_CHAR; (*len)++) {
        }
    }
}

// pokebw2's NameinWordTable_GetForm1
BOOL func_ov194_021f4e18(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len) {
    u8 i;

    wcharsncpy(table->words[index].forms[1], dest, n);
    for (i = 0; i < NAMEIN_WORD_CHARS && dest[i] != NAMEIN_EMPTY_CHAR; i++) {
    }
    if (len != NULL) {
        *len = i;
    }
    return i != 0;
}

// pokebw2's NameinWordTable_GetForm2
BOOL func_ov194_021f4e58(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len) {
    u8 i;

    wcharsncpy(table->words[index].forms[2], dest, n);
    for (i = 0; i < NAMEIN_WORD_CHARS && dest[i] != NAMEIN_EMPTY_CHAR; i++) {
    }
    if (len != NULL) {
        *len = i;
    }
    return i != 0;
}

// pokebw2's NameinWordTable_GetForm3
BOOL func_ov194_021f4e98(const NameinWordTable *table, u32 index, u16 *dest, u32 n, u8 *len) {
    u8 i;

    wcharsncpy(table->words[index].forms[3], dest, n);
    for (i = 0; i < NAMEIN_WORD_CHARS && dest[i] != NAMEIN_EMPTY_CHAR; i++) {
    }
    if (len != NULL) {
        *len = i;
    }
    return i != 0;
}

// pokebw2's NameinWordTable_Find
u16 func_ov194_021f4ed8(const NameinWordTable *table, const u16 *str) {
    int count = func_ov194_021f4dd8(table);
    int i;

    for (i = 0; i < count; i++) {
        if (func_ov194_021f4f10(table->words[i].forms[0], str)) {
            return i;
        }
    }
    return 0xffff;
}

// pokebw2's NameinData_WordMatches
static BOOL func_ov194_021f4f10(const u16 *word, const u16 *str) {
    u16 terminator = GFL_StrBufGetTerminator();
    int i;

    for (i = 0; i < NAMEIN_WORD_CHARS; i++, word++, str++) {
        if (*word == NAMEIN_EMPTY_CHAR && *str == terminator) {
            return TRUE;
        }
        if (*word != *str) {
            return FALSE;
        }
        if (*str == terminator) {
            return FALSE;
        }
    }
    return TRUE;
}

// pokebw2's NameinData_LoadConvTable
NameinConvTable *func_ov194_021f4f50(HeapID heapId) {
    return GFL_ArcSysReadHeapNewLZ(ARCID_NAMEIN, NARC_NAMEIN_CONV_TABLE, FALSE, heapId);
}

// pokebw2's NameinData_FreeConvTable
void func_ov194_021f4f60(NameinConvTable *table) {
    GFL_HeapFree(table);
}

// pokebw2's NameinData_ConvTableCount
static u16 func_ov194_021f4f68(const NameinConvTable *table) {
    return table->count;
}

// pokebw2's NameinData_ConvTableGet
static u16 func_ov194_021f4f6c(const NameinConvTable *table, u16 row, u16 column) {
    return table->chars[row][column];
}

// pokebw2's NameinConvTable_GetNext
BOOL func_ov194_021f4f7c(const NameinConvTable *table, const u16 *in, u16 *out) {
    int i;
    int j;

    if (in[0] == NAMEIN_EMPTY_CHAR) {
        return FALSE;
    }
    for (i = 0; i < func_ov194_021f4f68(table); i++) {
        for (j = 0; j < NAMEIN_WORD_CHARS; j++) {
            if (in[0] == func_ov194_021f4f6c(table, i, j)) {
                do {
                    j = (j + 1) % NAMEIN_WORD_CHARS;
                    out[0] = table->chars[i][j];
                } while (out[0] == NAMEIN_EMPTY_CHAR);
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's NameinData_IsNameValid
BOOL func_ov194_021f5004(const u16 *name, HeapID heapId) {
    NameinKeyboard *keyboards[4];
    BOOL valid;
    NameinConvTable *conv;
    int y;
    int pos;
    u32 k;
    int x;
    int j;
    int i;
    BOOL found;

    for (i = 0; i < 4; i++) {
        keyboards[i] = func_ov194_021f4d20(i, HEAPID_TAIL(heapId));
    }
    conv = func_ov194_021f4f50(HEAPID_TAIL(heapId));
    valid = TRUE;
    pos = 0;
    while (name[pos] != GFL_StrBufGetTerminator()) {
        found = FALSE;
        for (k = 0; k < 4; k++) {
            for (y = 0; y < func_ov194_021f4da0(keyboards[k]) && !found; y++) {
                for (x = 0; x < func_ov194_021f4d9c(keyboards[k]) && !found; x++) {
                    if (name[pos] == func_ov194_021f4da4(keyboards[k], x, y)) {
                        found = TRUE;
                    }
                }
            }
        }
        for (i = 0; i < func_ov194_021f4f68(conv); i++) {
            for (j = 0; j < NAMEIN_WORD_CHARS; j++) {
                if (name[pos] == func_ov194_021f4f6c(conv, i, j)) {
                    found = TRUE;
                }
            }
        }
        if (!found) {
            valid = FALSE;
            break;
        }
        pos++;
    }
    func_ov194_021f4f60(conv);
    for (i = 0; i < 4; i++) {
        func_ov194_021f4d34(keyboards[i]);
    }
    return valid;
}
