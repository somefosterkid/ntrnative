// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/union_view.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "system/union_view.h"

// The 16 looks a player can have in the Union Room, 8 for each gender, and their trainer types. The file's name is a
// guess: the ROM has no string for it

typedef struct {
    u16 trainerType;
    u8 unk2;
    u8 unk3;
} UnionView;

// pokebw2's sUnionViews
static const UnionView data_0209fb18[] = {
    { 11, 2, 2 },   { 30, 50, 50 },  { 36, 24, 24 }, { 34, 17, 17 },  { 73, 52, 52 }, { 64, 74, 74 },
    { 61, 57, 57 }, { 153, 15, 15 }, { 15, 3, 3 },   { 31, 49, 49 },  { 37, 25, 25 }, { 35, 18, 18 },
    { 74, 46, 46 }, { 84, 65, 65 },  { 51, 62, 62 }, { 154, 14, 14 },
};

// pokebw2's UnionView_GetTrainerType
u16 func_02025c74(u32 view) {
    if (view >= NELEMS(data_0209fb18)) {
        return 11;
    }
    return data_0209fb18[view].trainerType;
}

u8 func_02025c88(u32 view) {
    if (view >= NELEMS(data_0209fb18)) {
        return 2;
    }
    return data_0209fb18[view].unk3;
}

u8 func_02025c9c(u32 view) {
    if (view >= NELEMS(data_0209fb18)) {
        return 2;
    }
    return data_0209fb18[view].unk2;
}



u32 func_02025cb0(u32 value) {
    u32 i;

    for (i = 0; i < NELEMS(data_0209fb18); i++) {
        if (value == data_0209fb18[i].unk3) {
            return i;
        }
    }
    return 0;
}
