// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_form_data.c.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"

// Three tables of the Pokédex detail screen's forms page. The link placed them ahead of zukan_detail_form.c's .rodata,
// before its local initializers, which no order of declarations in one file gives (MWCC's shared .rodata section sits
// where the file's sorted objects reach its smallest one, and an 8-byte local of zukan_detail_form.c sorts first), so
// they come from an object of their own linked just before it, as in pokebw2. The ROM names no such file; the name is
// pokebw2's guess after the page it serves

// The messages of the form count, the shiny count, the species, male, female and shiny
// pokebw2's ZUKAN_DETAIL_FORM_STRBUF_MESSAGES
const u16 data_ov208_021c8794[6] = { 0xba, 0xbb, 0x9f, 0x72, 0x73, 0xbc };

// The slider's bar and knob, and the color marks of the two entries shown
// pokebw2's ZUKAN_DETAIL_FORM_ACTORS
const ZukanDetailFormActorData data_ov208_021c87a0[4] = {
    { 0x90, 0x30, 0x15, 1, 3, ZUKAN_DETAIL_FORM_RES_MAIN_CHARS, ZUKAN_DETAIL_FORM_RES_MAIN_PALETTE,
      ZUKAN_DETAIL_FORM_RES_MAIN_CELL_ANIMS },
    { 0x9c, 0x30, 0x16, 0, 3, ZUKAN_DETAIL_FORM_RES_MAIN_CHARS, ZUKAN_DETAIL_FORM_RES_MAIN_PALETTE,
      ZUKAN_DETAIL_FORM_RES_MAIN_CELL_ANIMS },
    { 0, 0, 0, 0, 3, ZUKAN_DETAIL_FORM_RES_COLOR_CHARS, ZUKAN_DETAIL_FORM_RES_COLOR_PALETTE,
      ZUKAN_DETAIL_FORM_RES_COLOR_CELL_ANIMS },
    { 0x80, 0, 0, 0, 3, ZUKAN_DETAIL_FORM_RES_COLOR_CHARS, ZUKAN_DETAIL_FORM_RES_COLOR_PALETTE,
      ZUKAN_DETAIL_FORM_RES_COLOR_CELL_ANIMS },
};

// Where a sprite stands: in the middle, off the screen to the left and right, and on the left and right
// pokebw2's ZUKAN_DETAIL_FORM_DEFAULT_POSITIONS
const ZukanDetailFormPos data_ov208_021c87c0[5] = {
    { 0.0f, -13.9f, 0.0f },
    { -64.0f, -13.9f, 0.0f },
    { 64.0f, -13.9f, 0.0f },
    { -16.0f, -13.9f, 0.0f },
    { 16.0f, -13.9f, 0.0f },
};
