// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov169/btlv_scd.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "battle/btlv_scd.h"
#include "types.h"
#include "battle/btl_action.h"
#include "battle/btl_client.h"
#include "battle/btl_main.h"
#include "battle/btl_pokeparam.h"
#include "battle/btlv.h"
#include "battle/btlv_effect.h"
#include "battle/btlv_input.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/heap.h"
#include "gfl/str.h"
#include "gfl/touchpanel.h"
#include "nitro/mi.h"
#include "pml/poke_party.h"
#include "pml/waza.h"
#include "system/game_data.h"
#include "system/printsys.h"
#include "battle/btl_event.h"
#include "struct_decls.h"

// pokebw2's prototypes of what Black's headers don't declare, under pokebw2's names until tools/port_file.py
// gives them Black's from the relocations
BOOL func_ov094_022030f0(BtlvInput *work, u8 *slot, u32 *button);

// What BtlMain gets the positions of a group of targets by: a group, and the position of the user
#define TARGET_FLAGS(group, pos) (((group) << 8) | (pos))

// How the target screen offers its targets: the mons to choose among, or all of a group
enum {
    TARGETS_SELECT,
    TARGETS_GROUP,
    TARGETS_SIDE,
};

// The screens' buttons (BtlvInputButtonSet: the rectangles, their flags and the rows their flash covers), the cursor
// stops that go with them, and the tables of both by position and move range, as Black has them
static const u8 data_ov095_0689d828[4];
static const u8 data_ov095_0689d82c[6];
static const s32 data_ov095_0689d834[2];
static const s32 data_ov095_0689d83c[2];
static const BtlvInputButtonSet data_ov095_0689d844;
static const BtlvInputButtonSet data_ov095_0689d850;
static const BtlvInputButtonSet data_ov095_0689d85c;
static const BtlvInputButtonSet data_ov095_0689d868;
static const BtlvInputButtonSet data_ov095_0689d874;
static const BtlvInputButtonSet data_ov095_0689d880;
static const BtlvInputButtonSet data_ov095_0689d88c;
static const BtlvInputButtonSet data_ov095_0689d898;
static const BtlvInputButtonSet data_ov095_0689d8a4;
static const s32 data_ov095_0689d8b0[3];
static const BtlvInputButtonSet data_ov095_0689d8bc;
static const BtlvInputButtonSet data_ov095_0689d8c8;
static const BtlvInputButtonSet data_ov095_0689d8d4;
static const s32 data_ov095_0689d8e0[3];
static const BtlvInputButtonSet data_ov095_0689d8ec;
static const TouchRect data_ov095_0689d8f8[3];
static const BtlvInputButtonSet data_ov095_0689d904;
static const BtlvInputButtonSet data_ov095_0689d910;
static const BtlvInputButtonSet data_ov095_0689d91c;
static const BtlvInputButtonSet data_ov095_0689d928;
static const BtlvInputButtonSet data_ov095_0689d934;
static const BtlvInputButtonSet data_ov095_0689d940;
static const BtlvInputButtonSet data_ov095_0689d94c;
static const TouchRect data_ov095_0689d958[4];
static const s32 data_ov095_0689d968[4];
static const s32 data_ov095_0689d978[4];
static const s32 data_ov095_0689d988[5];
static const s32 data_ov095_0689d99c[5];
static const s32 data_ov095_0689d9b0[5];
static const s32 data_ov095_0689d9c4[5];
static const s32 data_ov095_0689d9d8[5];
static const s32 data_ov095_0689d9ec[5];
static const s32 data_ov095_0689da00[5];
static const s32 data_ov095_0689da14[5];
static const TouchRect data_ov095_0689da28[5];
static const s32 data_ov095_0689da3c[5];
static const BtlvInputKeyStop data_ov095_0689da50[2];
static const BtlvInputKeyStop data_ov095_0689da68[2];
static const BtlvInputKeyStop data_ov095_0689da80[2];
static const BtlvInputKeyStop data_ov095_0689da98[2];
static const BtlvInputKeyStop data_ov095_0689dab0[2];
static const BtlvInputKeyStop data_ov095_0689dac8[2];
static const BtlvInputKeyStop data_ov095_0689dae0[2];
static const BtlvInputKeyStop data_ov095_0689daf8[2];
static const s32 data_ov095_0689db10[6];
static const BtlvInputKeyStop data_ov095_0689db28[2];
static const BtlvInputKeyStop data_ov095_0689db40[2];
static const BtlvInputKeyStop data_ov095_0689db58[2];
static const TouchRect data_ov095_0689db70[6];
static const BtlvInputKeyStop data_ov095_0689db88[2];
static const TouchRect data_ov095_0689dba0[6];
static const BtlvInputKeyStop data_ov095_0689dbb8[2];
static const BtlvInputKeyStop data_ov095_0689dbd0[2];
static const BtlvInputKeyStop data_ov095_0689dbe8[2];
static const BtlvInputKeyStop data_ov095_0689dc00[2];
static const BtlvInputKeyStop data_ov095_0689dc18[2];
static const BtlvInputKeyStop data_ov095_0689dc30[2];
static const BtlvInputKeyStop data_ov095_0689dc48[2];
static const BtlvInputKeyStop data_ov095_0689dc60[2];
static const s32 data_ov095_0689dc78[6];
static const s32 data_ov095_0689dc90[7];
static const s32 data_ov095_0689dcac[7];
static const s32 data_ov095_0689dcc8[7];
static const s32 data_ov095_0689dce4[7];
static const s32 data_ov095_0689dd00[7];
static const s32 data_ov095_0689dd1c[7];
static const s32 data_ov095_0689dd38[7];
static const TouchRect data_ov095_0689dd54[7];
static const s32 data_ov095_0689dd70[7];
static const s32 data_ov095_0689dd8c[7];
static const s32 data_ov095_0689dda8[7];
static const TouchRect data_ov095_0689ddc4[8];
static const BtlvInputKeyStop data_ov095_0689dde4[3];
static const BtlvInputKeyStop data_ov095_0689de08[3];
static const BtlvInputKeyStop data_ov095_0689de2c[3];
static const BtlvInputKeyStop data_ov095_0689de50[3];
static const BtlvInputKeyStop data_ov095_0689de74[3];
static const BtlvInputKeyStop data_ov095_0689de98[3];
static const BtlvInputKeyStop data_ov095_0689debc[3];
static const BtlvInputKeyStop data_ov095_0689dee0[3];
static const BtlvInputKeyStop data_ov095_0689df04[4];
static const BtlvInputKeyStop data_ov095_0689df34[4];
static const BtlvInputKeyStop data_ov095_0689df64[4];
static const BtlvInputKeyStop data_ov095_0689df94[4];
static const BtlvInputKeyStop data_ov095_0689dfc4[4];
static const BtlvInputKeyStop data_ov095_0689dff4[4];
static const BtlvInputKeyStop data_ov095_0689e024[4];
static const BtlvInputKeyStop data_ov095_0689e054[5];
const u8 data_ov095_0689e090[2][15];
const u8 data_ov095_0689e0b0[3][15];
static const BtlvInputKeyStop data_ov095_0689e0e0[6];
static const BtlvInputKeyStop data_ov095_0689e128[6];
static const BtlvInputKeyStop data_ov095_0689e170[6];
static const BtlvInputKeyStop data_ov095_0689e1b8[6];
const BtlvInputKeyStop *data_ov095_0689e560[2][15];
static const BtlvInputButtonSet *data_ov095_0689e5d8[2][15];
static const BtlvInputButtonSet *data_ov095_0689e650[3][15];
const BtlvInputKeyStop *data_ov095_0689e704[3][15];

static const u8 data_ov095_0689d828[4] = { 1, 2, 3, 4 };
static const u8 data_ov095_0689d82c[6] = { 2, 3, 4, 5, 6, 7 };
static const s32 data_ov095_0689d83c[2] = { 0, 0 };
static const s32 data_ov095_0689d834[2] = { 4, 8 };
static const TouchRect data_ov095_0689d8f8[3] = {
    { 32, 80, 0, 255 },
    { 80, 128, 0, 255 },
    { 255, 0, 0, 0 },
};
static const BtlvInputButtonSet data_ov095_0689d874 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dcc8 };
static const BtlvInputButtonSet data_ov095_0689d88c = { data_ov095_0689d8f8, data_ov095_0689d83c, data_ov095_0689d834 };
static const BtlvInputButtonSet data_ov095_0689d940 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dda8 };
static const s32 data_ov095_0689d8e0[3] = { 0, 0, 0 };
static const BtlvInputButtonSet data_ov095_0689d8d4 = { data_ov095_0689da28, data_ov095_0689d978, data_ov095_0689d968 };
static const BtlvInputButtonSet data_ov095_0689d8bc = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dd70 };
static const BtlvInputButtonSet data_ov095_0689d898 = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689d988 };
static const BtlvInputButtonSet data_ov095_0689d910 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dd1c };
static const BtlvInputButtonSet data_ov095_0689d880 = { data_ov095_0689dd54, data_ov095_0689dc78, data_ov095_0689db10 };
static const s32 data_ov095_0689d8b0[3] = { 4, 8, 16 };
static const BtlvInputButtonSet data_ov095_0689d850 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dd38 };
static const BtlvInputButtonSet data_ov095_0689d91c = { data_ov095_0689db70, data_ov095_0689d9b0, data_ov095_0689da3c };
static const BtlvInputButtonSet data_ov095_0689d94c = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dd8c };
static const BtlvInputButtonSet data_ov095_0689d8c8 = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689da00 };
static const BtlvInputButtonSet data_ov095_0689d934 = { data_ov095_0689d958, data_ov095_0689d8e0, data_ov095_0689d8b0 };
static const BtlvInputButtonSet data_ov095_0689d8ec = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689d99c };
static const BtlvInputButtonSet data_ov095_0689d928 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dce4 };
static const BtlvInputButtonSet data_ov095_0689d85c = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689d9c4 };
static const BtlvInputButtonSet data_ov095_0689d844 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dcac };
static const BtlvInputButtonSet data_ov095_0689d868 = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689d9ec };
static const BtlvInputButtonSet data_ov095_0689d8a4 = { data_ov095_0689ddc4, data_ov095_0689dc90, data_ov095_0689dd00 };
static const BtlvInputButtonSet data_ov095_0689d904 = { data_ov095_0689dba0, data_ov095_0689d9d8, data_ov095_0689da14 };
static const s32 data_ov095_0689d968[4] = { 65539, 4, 8, 16 };
static const s32 data_ov095_0689d978[4] = { 0, 0, 0, 2 };
static const TouchRect data_ov095_0689d958[4] = {
    { 144, 192, 0, 80 },
    { 144, 192, 176, 255 },
    { 152, 192, 88, 168 },
    { 255, 0, 0, 0 },
};
static const s32 data_ov095_0689d99c[5] = { 896, 896, 896, 896, 16 };
static const s32 data_ov095_0689d9b0[5] = { 0, 0, 0, 0, 1 };
static const s32 data_ov095_0689d9c4[5] = { 1536, 1536, 1536, 1536, 16 };
static const s32 data_ov095_0689d9d8[5] = { 0, 0, 0, 0, 1 };
static const s32 data_ov095_0689d9ec[5] = { 1408, 1408, 1408, 1408, 16 };
static const s32 data_ov095_0689da00[5] = { 384, 384, 384, 384, 16 };
static const s32 data_ov095_0689da14[5] = { 1920, 1920, 1920, 1920, 16 };
static const TouchRect data_ov095_0689da28[5] = {
    { 24, 144, 0, 255 },
    { 144, 192, 0, 80 },
    { 144, 192, 176, 255 },
    { 152, 192, 88, 168 },
    { 255, 0, 0, 0 },
};
static const s32 data_ov095_0689da3c[5] = { 512, 1024, 2048, 4096, 16 };
static const s32 data_ov095_0689d988[5] = { 512, 256, 1024, 128, 16 };
static const BtlvInputKeyStop data_ov095_0689da50[2] = {
    { { 3, 1, 0, 1, -1, 0 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689da68[2] = {
    { { 3, 1, 3, 1, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689da80[2] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689dbe8[2] = {
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689da98[2] = {
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dab0[2] = {
    { { 0, 2, 0, 2, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689dac8[2] = {
    { { 5, 1, 0, 4, 4, 0 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 5, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689db88[2] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dae0[2] = {
    { { 5, 1, 5, 1, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 5, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689daf8[2] = {
    { { 3, 1, 2, 2, -1, 1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const s32 data_ov095_0689db10[6] = { 512, 1024, 2048, 4096, 16, 4 };
static const BtlvInputKeyStop data_ov095_0689db28[2] = {
    { { 5, 1, 0, 4, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 5, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689db40[2] = {
    { { 0, 4, 0, 4, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689db58[2] = {
    { { 3, 1, 3, 1, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const TouchRect data_ov095_0689db70[6] = {
    { 32, 80, 0, 128 },
    { 32, 80, 128, 255 },
    { 80, 128, 0, 128 },
    { 80, 128, 128, 255 },
    { 144, 192, 176, 255 },
    { 255, 0, 0, 0 },
};
static const TouchRect data_ov095_0689dba0[6] = {
    { 88, 128, 0, 128 },
    { 32, 88, 128, 255 },
    { 88, 128, 128, 255 },
    { 32, 88, 0, 128 },
    { 144, 192, 176, 255 },
    { 255, 0, 0, 0 },
};
static const BtlvInputKeyStop data_ov095_0689dbb8[2] = {
    { { 3, 1, 0, 2, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689dbd0[2] = {
    { { 5, 3, 5, 2, 2, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 5, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dc00[2] = {
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dc18[2] = {
    { { 4, 4, 4, 4, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dc30[2] = {
    { { 3, 1, 3, 2, 2, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689dc48[2] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 1 },
    { { 1, 1, 1, 1, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 1, 1 },
};
static const BtlvInputKeyStop data_ov095_0689dc60[2] = {
    { { 5, 3, 5, 3, -1, -1 }, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 5, 6 },
    { { 6, 6, 6, 6, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const s32 data_ov095_0689dc78[6] = { 0, 0, 0, 0, 1, 0 };
static const s32 data_ov095_0689dd00[7] = { 2816, 2816, 2816, 2816, 2816, 2816, 16 };
static const s32 data_ov095_0689dd1c[7] = { 384, 384, 384, 384, 384, 384, 16 };
static const s32 data_ov095_0689dd38[7] = { 896, 896, 896, 896, 896, 896, 16 };
static const TouchRect data_ov095_0689dd54[7] = {
    { 32, 80, 0, 128 },
    { 32, 80, 128, 255 },
    { 80, 128, 0, 128 },
    { 80, 128, 128, 255 },
    { 144, 192, 176, 255 },
    { 144, 192, 0, 80 },
    { 255, 0, 0, 0 },
};
static const s32 data_ov095_0689dd70[7] = { 768, 768, 768, 768, 768, 768, 16 };
static const s32 data_ov095_0689dd8c[7] = { 7168, 7168, 7168, 7168, 7168, 7168, 16 };
static const s32 data_ov095_0689dda8[7] = { 8064, 8064, 8064, 8064, 8064, 8064, 16 };
static const s32 data_ov095_0689dc90[7] = { 0, 0, 0, 0, 0, 0, 3 };
static const s32 data_ov095_0689dcac[7] = { 1024, 512, 2048, 256, 4096, 128, 16 };
static const s32 data_ov095_0689dcc8[7] = { 2432, 2432, 2432, 2432, 2432, 2432, 16 };
static const s32 data_ov095_0689dce4[7] = { 6016, 6016, 6016, 6016, 6016, 6016, 16 };
static const TouchRect data_ov095_0689ddc4[8] = {
    { 88, 128, 8, 88 },
    { 32, 88, 168, 248 },
    { 88, 128, 88, 168 },
    { 32, 88, 88, 168 },
    { 88, 128, 168, 248 },
    { 32, 88, 8, 88 },
    { 144, 192, 176, 255 },
    { 255, 0, 0, 0 },
};
static const BtlvInputKeyStop data_ov095_0689dde4[3] = {
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 3, 4 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 1, 4 },
    { { 4, 4, 4, 4, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689de08[3] = {
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 2, 6 },
    { { 4, 4, 4, 4, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689de2c[3] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 0, 4 },
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 2, 4 },
    { { 4, 4, 4, 4, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689de50[3] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 0, 6 },
    { { 4, 4, 4, 4, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689de74[3] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 3, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689de98[3] = {
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 1, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689debc[3] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, KEY_CURSOR_NONE },
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 1, 2, KEY_CURSOR_NONE },
};
static const BtlvInputKeyStop data_ov095_0689dee0[3] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 0, 6 },
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689dfc4[4] = {
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 3, 4 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 1, 4 },
    { { 2, 2, 2, 2, -1, -1 }, 1, 3, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 4 },
    { { 4, 4, 4, 4, -1, -1 }, -2, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689dff4[4] = {
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 3, 4 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 3, 0, KEY_CURSOR_NONE, 1, 4 },
    { { 0, 0, 0, 0, -1, -1 }, 0, 3, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 0, 4 },
    { { 4, 4, 4, 4, -1, -1 }, -2, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
static const BtlvInputKeyStop data_ov095_0689e024[4] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, 0, KEY_CURSOR_NONE, 3, 6 },
    { { 2, 2, 2, 2, -1, -1 }, 1, 3, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -2, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689df04[4] = {
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 3, 0, KEY_CURSOR_NONE, 1, 6 },
    { { 2, 2, 2, 2, -1, -1 }, 0, 3, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689df34[4] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 3, 0, 2, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 3, 1, KEY_CURSOR_NONE, 1, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689df64[4] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, -1, 1, 2, 0, -3 },
    { { 1, 1, 1, 1, -1, -1 }, 0, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 3, 1, -3 },
    { { 2, 2, 2, 2, -1, -1 }, 0, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 2, -3 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 1, 2, 3, -3 },
};
static const BtlvInputKeyStop data_ov095_0689df94[4] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 0, 6 },
    { { 2, 2, 2, 2, -1, -1 }, KEY_CURSOR_NONE, 3, 0, 2, 2, 6 },
    { { 4, 4, 4, 4, -1, -1 }, KEY_CURSOR_NONE, 3, 1, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689e054[5] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 0, 4 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 3, 0, KEY_CURSOR_NONE, 1, 4 },
    { { 2, 2, 2, 2, -1, -1 }, 0, 4, KEY_CURSOR_NONE, 3, 2, 4 },
    { { 3, 3, 3, 3, -1, -1 }, 1, 4, 2, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, -2, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 4 },
};
const u8 data_ov095_0689e090[2][15] = {
    { 4, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 },
    { 4, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4 },
};
const u8 data_ov095_0689e0b0[3][15] = {
    { 4, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6 },
    { 6, 4, 3, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6 },
    { 4, 3, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6 },
};
static const BtlvInputKeyStop data_ov095_0689e1b8[6] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 4, 0, 2, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 5, 1, KEY_CURSOR_NONE, 1, 6 },
    { { 0, 0, 0, 0, -1, -1 }, 0, 5, KEY_CURSOR_NONE, 4, 0, 6 },
    { { 2, 2, 2, 2, -1, -1 }, 2, 5, 3, KEY_CURSOR_NONE, 2, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689e0e0[6] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 3, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 5, 0, 2, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 4, 1, KEY_CURSOR_NONE, 1, 6 },
    { { 0, 0, 0, 0, -1, -1 }, 0, 5, KEY_CURSOR_NONE, 4, 0, 6 },
    { { 4, 4, 4, 4, -1, -1 }, 2, 5, 3, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputKeyStop data_ov095_0689e128[6] = {
    { { 0, 0, 0, 0, -1, -1 }, KEY_CURSOR_NONE, 2, KEY_CURSOR_NONE, 1, 0, 4 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 3, 0, KEY_CURSOR_NONE, 1, 4 },
    { { 2, 2, 2, 2, -1, -1 }, 0, 5, KEY_CURSOR_NONE, 3, 2, 4 },
    { { 3, 3, 3, 3, -1, -1 }, 1, 4, 2, KEY_CURSOR_NONE, 3, 4 },
    { { 4, 4, 4, 4, -1, -1 }, 3, KEY_CURSOR_NONE, 5, KEY_CURSOR_NONE, 4, 4 },
    { { 5, 5, 5, 5, -1, -1 }, 2, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 4, 5, 4 },
};
static const BtlvInputKeyStop data_ov095_0689e170[6] = {
    { { 5, 5, 5, 5, -1, -1 }, KEY_CURSOR_NONE, 5, KEY_CURSOR_NONE, 1, 5, 6 },
    { { 3, 3, 3, 3, -1, -1 }, KEY_CURSOR_NONE, 3, 0, 2, 3, 6 },
    { { 1, 1, 1, 1, -1, -1 }, KEY_CURSOR_NONE, 4, 1, KEY_CURSOR_NONE, 1, 6 },
    { { 2, 2, 2, 2, -1, -1 }, 1, 5, KEY_CURSOR_NONE, 4, 2, 6 },
    { { 4, 4, 4, 4, -1, -1 }, 2, 5, 3, KEY_CURSOR_NONE, 4, 6 },
    { { 6, 6, 6, 6, -1, -1 }, -1, KEY_CURSOR_NONE, KEY_CURSOR_NONE, KEY_CURSOR_NONE, 6, 6 },
};
static const BtlvInputButtonSet *data_ov095_0689e5d8[2][15] = {
    { &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d868,
      &data_ov095_0689d8c8, &data_ov095_0689d85c, &data_ov095_0689d898, &data_ov095_0689d904, &data_ov095_0689d898,
      &data_ov095_0689d904, &data_ov095_0689d8c8, &data_ov095_0689d85c, &data_ov095_0689d898, &data_ov095_0689d898 },
    { &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d898, &data_ov095_0689d8ec,
      &data_ov095_0689d8c8, &data_ov095_0689d85c, &data_ov095_0689d898, &data_ov095_0689d904, &data_ov095_0689d898,
      &data_ov095_0689d904, &data_ov095_0689d8c8, &data_ov095_0689d85c, &data_ov095_0689d898, &data_ov095_0689d898 },
};
const BtlvInputKeyStop *data_ov095_0689e560[2][15] = {
    { data_ov095_0689dfc4, data_ov095_0689de2c, data_ov095_0689dbe8, data_ov095_0689dde4, data_ov095_0689dc30,
      data_ov095_0689da68, data_ov095_0689dab0, data_ov095_0689da80, data_ov095_0689dbb8, data_ov095_0689da80,
      data_ov095_0689dbb8, data_ov095_0689da68, data_ov095_0689dab0, data_ov095_0689da80, data_ov095_0689dfc4 },
    { data_ov095_0689dff4, data_ov095_0689de2c, data_ov095_0689da80, data_ov095_0689dde4, data_ov095_0689da50,
      data_ov095_0689da68, data_ov095_0689dab0, data_ov095_0689dbe8, data_ov095_0689dbb8, data_ov095_0689dbe8,
      data_ov095_0689dbb8, data_ov095_0689da68, data_ov095_0689dab0, data_ov095_0689dbe8, data_ov095_0689dff4 },
};
static const BtlvInputButtonSet *data_ov095_0689e650[3][15] = {
    { &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d874,
      &data_ov095_0689d910, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d940, &data_ov095_0689d844,
      &data_ov095_0689d940, &data_ov095_0689d850, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d844 },
    { &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d928,
      &data_ov095_0689d850, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d940, &data_ov095_0689d844,
      &data_ov095_0689d940, &data_ov095_0689d850, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d844 },
    { &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d844, &data_ov095_0689d8a4,
      &data_ov095_0689d8bc, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d940, &data_ov095_0689d844,
      &data_ov095_0689d940, &data_ov095_0689d850, &data_ov095_0689d94c, &data_ov095_0689d844, &data_ov095_0689d844 },
};
const BtlvInputKeyStop *data_ov095_0689e704[3][15] = {
    { data_ov095_0689e024, data_ov095_0689dee0, data_ov095_0689da98, data_ov095_0689de74, data_ov095_0689dbd0,
      data_ov095_0689dc60, data_ov095_0689db40, data_ov095_0689db88, data_ov095_0689db28, data_ov095_0689db88,
      data_ov095_0689db28, data_ov095_0689dae0, data_ov095_0689db40, data_ov095_0689db88, data_ov095_0689e170 },
    { data_ov095_0689e0e0, data_ov095_0689df94, data_ov095_0689de50, data_ov095_0689df34, data_ov095_0689dac8,
      data_ov095_0689dae0, data_ov095_0689db40, data_ov095_0689dc00, data_ov095_0689db28, data_ov095_0689dc00,
      data_ov095_0689db28, data_ov095_0689dae0, data_ov095_0689db40, data_ov095_0689dc00, data_ov095_0689e0e0 },
    { data_ov095_0689df04, data_ov095_0689de08, data_ov095_0689da98, data_ov095_0689de98, data_ov095_0689daf8,
      data_ov095_0689db58, data_ov095_0689db40, data_ov095_0689dc18, data_ov095_0689db28, data_ov095_0689dc18,
      data_ov095_0689db28, data_ov095_0689dae0, data_ov095_0689db40, data_ov095_0689dc18, data_ov095_0689e1b8 },
};

u32 func_ov094_022033f0(BtlvInput *work);
void data_ov095_06899d39(BtlvScd *scd);
static void data_ov095_06899e2d(BtlvScd *scd);
static void data_ov095_06899e49(BtlvScd *scd, BtlvSubProcFn init, BtlvSubProcFn main);
static BOOL data_ov095_06899e69(BtlvScd *scd);
BOOL data_ov095_0689a031(s32 *seq, void *arg);
BOOL data_ov095_0689a76d(s32 *seq, void *arg);
BOOL data_ov095_0689a84d(s32 *seq, void *arg);
BOOL data_ov095_0689a879(s32 *seq, void *arg);
BOOL data_ov095_0689a989(s32 *seq, void *arg);
BOOL data_ov095_0689ab49(s32 *seq, void *arg);
BOOL data_ov095_0689aba1(s32 *seq, void *arg);
static void func_ov095_0689accc(BtlvScdTargetList *list);
static void func_ov095_0689acd8(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags);
static void func_ov095_0689acfc(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags);
static void func_ov095_0689ad24(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags);
static void func_ov095_0689ad4c(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 move, u8 pos);
static BOOL func_ov095_0689addc(BtlvScdTargetList *list, u32 button, BtlMainModule *mainModule, u8 *outPos);
static void BtlvScd_SetTargetScreen(const u8 *positions, u8 count, u32 kind, BtlvScd *scd);
static void func_ov095_0689b098(BtlvScdTargetList *list, BtlvScd *scd);
static BOOL data_ov095_0689b0d5(s32 *seq, void *arg);
static BOOL data_ov095_0689b1e5(s32 *seq, void *arg);
static void func_ov095_0689b354(BtlvScdTargetList *list, BtlvScd *scd);

BtlvScd *data_ov095_06899af1(BtlvCore *core, BtlMainModule *mainModule, BtlPokeCon *pokeCon, TCBExManager *tcbManager, Font *font, BtlClient *client, u32 scdState, HeapID heapId) {
    BtlvScd *scd;
    BattleParty *party;
    StrBuf *strBuf;
    PrintQueue *printQueue;

    scd = GFL_HeapAllocate(heapId, sizeof(BtlvScd), TRUE, "btlv_scd.c", 200);
    scd->core = core;
    scd->mainModule = mainModule;
    scd->pokeCon = pokeCon;
    scd->heapId = heapId;
    scd->font = font;
    scd->client = client;
    party = func_ov093_021b9af8(pokeCon, func_ov093_021d3d70(client));
    scd->party = party;
    strBuf = GFL_StrBufCreate(0x80, heapId);
    scd->strBuf = strBuf;
    scd->unk_74 = 0;
    scd->unk_90 = (u8)scdState;
    scd->unk_91 = 0;
    scd->unk_92_low = 0;
    scd->unk_92_high = 0;
    scd->unk_8f = 0;
    printQueue = func_0201c440(scd->heapId);
    scd->printQueue = printQueue;
    data_ov095_06899e2d(scd);
    return scd;
}

void data_ov095_06899be1(BtlvScd *scd) {
    GameData *gameData;
    u32 rule;
    u32 unk54;
    PaletteFade *paletteFade;
    BtlvInput *input;

    gameData = func_ov093_021b8748(scd->mainModule);
    rule = BtlSetup_GetBattleStyle(scd->mainModule);
    unk54 = BtlSetup_GetBattleType(scd->mainModule);
    paletteFade = func_ov094_021f86e0();
    input = func_ov094_0220207c(gameData, rule, unk54, paletteFade, scd->font, &scd->unk_8f, scd->unk_90, scd->heapId);
    scd->input = input;
    GFL_BGSysSetBGEnabledB(0x10, 1);
    data_ov095_06899d39(scd);
}

void data_ov095_06899ca5(BtlvScd *scd, u32 mode) {
    scd->unk_8c = 0;
    scd->unk_8d = -16;
    scd->unk_8e = -1;
    func_ov094_022025d8(scd->input, mode);
}

void data_ov095_06899cdd(BtlvScd *scd) {
    scd->unk_8c = -16;
    scd->unk_8d = 0;
    scd->unk_8e = 1;
    func_ov094_0220266c(scd->input);
}

BOOL data_ov095_06899d05(BtlvScd *scd) {
    BOOL BVar1;

    BVar1 = func_ov094_022026fc(scd->input);
    if (BVar1 == 0) {
        return 1;
    }
    return 0;
}

void data_ov095_06899d2d(BtlvScd *scd) {
}

void data_ov095_06899d31(BtlvScd *scd) {
    data_ov095_06899d39(scd);
}

void data_ov095_06899d39(BtlvScd *scd) {
}

void data_ov095_06899d3d(BtlvScd *scd) {
    if (func_ov094_022033f0(scd->input) != 0) {
        data_ov095_06899d65(scd);
    }
}

void data_ov095_06899d65(BtlvScd *scd) {
    func_ov094_0220270c(scd->input, 0, NULL);
    func_ov094_021f81ec(8);
    func_ov094_021f8a78();
}

BOOL data_ov095_06899db1(BtlvScd *scd) {
    BOOL BVar1;

    BVar1 = func_ov094_022033cc(scd->input);
    if (BVar1 == 0) {
        return 1;
    }
    return 0;
}

void data_ov095_06899dd9(BtlvScd *scd) {
    func_0201c4c0(scd->printQueue);
    GFL_StrBufFree(scd->strBuf);
    func_ov094_022021f4(scd->input);
    GFL_HeapFree(scd);
}

#pragma thumb on

// pokebw2's BtlvScd_ClearProcs
static void data_ov095_06899e2d(BtlvScd *scd) {
    int i;

    for (i = 0; i < NUM_PROCS; i++) {
        scd->procs[i].init = NULL;
        scd->procs[i].main = NULL;
        scd->procs[i].arg = NULL;
        scd->procs[i].seq = 0;
    }
    scd->numProcs = 0;
}

// pokebw2's BtlvScd_PushProc
static void data_ov095_06899e49(BtlvScd *scd, BtlvSubProcFn init, BtlvSubProcFn main) {
    BtlvSubProc *proc = &scd->procs[scd->numProcs++];

    proc->init = init;
    proc->main = main;
    proc->arg = scd;
    proc->seq = 0;
}

// Runs the procedure on top of the stack, and returns TRUE only when there is none
// pokebw2's BtlvScd_RunProcs
static BOOL data_ov095_06899e69(BtlvScd *scd) {
    BtlvSubProc *proc;
    BOOL done;

    if (scd->numProcs != 0) {
        proc = &scd->procs[scd->numProcs - 1];
        if (proc->init != NULL) {
            if (proc->init(&proc->seq, proc->arg)) {
                proc->init = NULL;
                proc->seq = 0;
            }
            done = FALSE;
        } else if (proc->main != NULL) {
            if (proc->main(&proc->seq, proc->arg)) {
                proc->main = NULL;
                done = TRUE;
            } else {
                done = FALSE;
            }
        } else {
            done = TRUE;
        }
        if (done) {
            proc->init = NULL;
            proc->main = NULL;
            proc->arg = NULL;
            proc->seq = 0;
            scd->numProcs--;
        }
        return FALSE;
    }
    return TRUE;
}

void data_ov095_06899ed1(BtlvScd *scd, BattleMon *mon, u8 slot, u8 shooterEnergy, BattleAction *action) {
    scd->mon = mon;
    scd->action = action;
    scd->typeMask = 0;
    scd->slot = slot;
    scd->unk_91 = shooterEnergy;
    data_ov095_06899e49(scd, (void *)data_ov095_0689a031, (void *)data_ov095_0689a76d);
}

void data_ov095_06899f01(BtlvScd *scd, BattleMon *mon, u8 slot, BattleAction *action) {
    scd->mon = mon;
    scd->action = action;
    scd->typeMask = 0;
    scd->slot = slot;
    data_ov095_06899e49(scd, (void *)data_ov095_0689a031, (void *)data_ov095_0689a84d);
}

u32 data_ov095_06899f29(BtlvScd *scd) {
    BOOL BVar1;

    BVar1 = data_ov095_06899e69(scd);
    if (BVar1 != 0) {
        return scd->typeMask;
    }
    return 0;
}

void data_ov095_06899f41(BtlvScd *scd, BattleMon *mon, BattleAction *action) {
    scd->mon = mon;
    scd->action = action;
    scd->typeMask = 0;
    scd->moveSelectMain = (BtlvSubProcFn)data_ov095_0689a989;
    data_ov095_06899e49(scd, (void *)data_ov095_0689a879, (void *)data_ov095_0689a989);
}

void data_ov095_06899f65(BtlvScd *scd, BtlvSelectTargetParam *param, BattleAction *action) {
    scd->typeMask = 0;
    scd->targetParam = param;
    scd->action = action;
    scd->moveSelectMain = data_ov095_0689aba1;
    BattleAction_SetNull(action);
    BattleAction_SetNull(&param->rotateAction);
    BattleAction_SetNull(&param->action);
    data_ov095_06899e49(scd, (void *)data_ov095_0689ab49, (void *)data_ov095_0689aba1);
}


// pokebw2's BtlvScd_IsDone
BOOL data_ov095_06899fd5(BtlvScd *scd) {
    return data_ov095_06899e69(scd);
}

// pokebw2's BtlvScd_ResumeMoveSelect
void data_ov095_06899fdd(BtlvScd *scd) {
    data_ov095_06899e49(scd, NULL, scd->moveSelectMain);
}

void data_ov095_06899fe9(BtlvScd *scd, BattleMon *mon, BattleAction *action) {
    scd->mon = mon;
    scd->action = action;
    scd->typeMask = 0;
    data_ov095_06899e49(scd, (void *)data_ov095_0689a879, (void *)data_ov095_0689a84d);
}

// pokebw2's BtlvScd_StartTargetSelect
void data_ov095_0689a009(BtlvScd *scd, BattleMon *mon, BattleAction *action) {
    scd->mon = mon;
    scd->action = action;
    data_ov095_06899e49(scd, data_ov095_0689b0d5, data_ov095_0689b1e5);
}

// pokebw2's BtlvScd_IsTargetSelectDone
BOOL data_ov095_0689a021(BtlvScd *scd) {
    return data_ov095_06899e69(scd);
}

// pokebw2's BtlvScd_CancelProcs
void data_ov095_0689a029(BtlvScd *scd) {
    data_ov095_06899e2d(scd);
}

BOOL data_ov095_0689a031(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    BtlvInputCommandParam param;
    BattleParty *parties[2] = { NULL, NULL };
    u16 counts[2] = { 0, 0 };
    BattleMon *mon;
    u16 total;
    u16 side = 0;
    u32 style;

    MI_CpuClear16(&param, sizeof(param));
    param.scdState = scd->unk_90;
    param.launcherPoints = scd->unk_91;
    param.pokePos = func_ov093_021b8e70(scd->mainModule, MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)));
    func_ov094_021f81ec(param.pokePos);
    style = BtlSetup_GetBattleStyle(scd->mainModule);
    for (; side < 2; side++) {
        u8 playerClient;
        u8 otherClient;
        u16 i;

        parties[0] = NULL;
        parties[1] = NULL;
        if (side == 0) {
            playerClient = GetPlayerClientID(scd->mainModule);
            otherClient = func_ov093_021b9000(scd->mainModule);
            if (style == BTL_STYLE_DOUBLE && otherClient != 4) {
                if (playerClient < otherClient) {
                    parties[0] = func_ov093_021b9af8(scd->pokeCon, playerClient);
                    counts[0] = GetNumMonsInParty(parties[0]);
                    parties[1] = func_ov093_021b9af8(scd->pokeCon, otherClient);
                    counts[1] = GetNumMonsInParty(parties[1]);
                } else {
                    parties[0] = func_ov093_021b9af8(scd->pokeCon, otherClient);
                    counts[0] = GetNumMonsInParty(parties[0]);
                    parties[1] = func_ov093_021b9af8(scd->pokeCon, playerClient);
                    counts[1] = GetNumMonsInParty(parties[1]);
                }
            } else {
                parties[0] = scd->party;
                counts[0] = GetNumMonsInParty(parties[0]);
            }
            switch (style) {
            case BTL_STYLE_SINGLE:
                mon = func_ov093_021b98ac(scd->pokeCon, func_ov093_021b8ed8(scd->mainModule, 0));
                if (!IsFainted(mon)) {
                    param.species[0] = GetBattleMonSpecies(mon);
                    param.forms[0] = PokeParty_GetParam(func_ov093_021d5864(mon), PKM_PARAM_FORM, NULL);
                    param.sexes[0] = GetBattleMonStat(mon, BATTLEMON_GENDER);
                }
                break;
            case BTL_STYLE_TRIPLE:
                mon = func_ov093_021b98ac(scd->pokeCon, func_ov093_021b8ed8(scd->mainModule, 6));
                if (!IsFainted(mon)) {
                    param.species[2] = GetBattleMonSpecies(mon);
                    param.forms[2] = PokeParty_GetParam(func_ov093_021d5864(mon), PKM_PARAM_FORM, NULL);
                    param.sexes[2] = GetBattleMonStat(mon, BATTLEMON_GENDER);
                }
                /* fall through */
            case BTL_STYLE_DOUBLE:
            case BTL_STYLE_ROTATION:
                mon = func_ov093_021b98ac(scd->pokeCon, func_ov093_021b8ed8(scd->mainModule, 2));
                if (!IsFainted(mon)) {
                    param.species[0] = GetBattleMonSpecies(mon);
                    param.forms[0] = PokeParty_GetParam(func_ov093_021d5864(mon), PKM_PARAM_FORM, NULL);
                    param.sexes[0] = GetBattleMonStat(mon, BATTLEMON_GENDER);
                }
                mon = func_ov093_021b98ac(scd->pokeCon, func_ov093_021b8ed8(scd->mainModule, 4));
                if (!IsFainted(mon)) {
                    param.species[1] = GetBattleMonSpecies(mon);
                    param.forms[1] = PokeParty_GetParam(func_ov093_021d5864(mon), PKM_PARAM_FORM, NULL);
                    param.sexes[1] = GetBattleMonStat(mon, BATTLEMON_GENDER);
                }
                break;
            }
        } else {
            u8 firstClient;
            u8 secondClient;

            if (BtlSetup_GetBattleType(scd->mainModule) == 0 || BtlSetup_GetBattleType(scd->mainModule) == 4) {
                break;
            }
            firstClient = func_ov093_021b904c(scd->mainModule, 0);
            secondClient = func_ov093_021b904c(scd->mainModule, 1);
            param.secondRow = TRUE;
            parties[0] = func_ov093_021b9af8(scd->pokeCon, firstClient);
            counts[0] = GetNumMonsInParty(parties[0]);
            if (style == BTL_STYLE_DOUBLE && firstClient != secondClient) {
                parties[1] = func_ov093_021b9af8(scd->pokeCon, secondClient);
                counts[1] = GetNumMonsInParty(parties[1]);
            }
        }
        if (parties[1] == NULL) {
            total = counts[0];
        } else {
            if (counts[0] > 3) {
                counts[0] = 3;
            }
            if (counts[1] > 3) {
                counts[1] = 3;
            }
            total = counts[0] + counts[1];
        }
        for (i = 0; i < total; i++) {
            u32 party;
            u32 slot;

            if (i < counts[0]) {
                party = 0;
                slot = i;
            } else {
                party = 1;
                slot = i - counts[0];
            }
            mon = GetBattleMonFromParty(parties[party], slot);
            if (!IsFainted(mon)) {
                if (GetBattleMonStatus(mon) != 0) {
                    param.entries[side][party * 3 + slot].iconAnim = 3;
                } else if (!CanPokemonBattle(mon)) {
                    param.entries[side][party * 3 + slot].iconAnim = 0;
                } else {
                    param.entries[side][party * 3 + slot].iconAnim = 1;
                }
            } else {
                param.entries[side][party * 3 + slot].iconAnim = 2;
            }
        }
    }
    param.laterPoke = scd->slot;
    param.ballSlide = func_ov093_021d3d84(scd->client);
    func_ov094_0220270c(scd->input, 1, &param);
    return TRUE;
}


BOOL data_ov095_0689a76d(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    s32 input;

    switch (*seq) {
    case 0:
        input = func_ov094_02202dc4(scd->input, &data_ov095_0689d8d4, data_ov095_0689df64);
        if (input != TOUCH_RECT_NONE) {
            scd->typeMask = data_ov095_0689d828[input];
            if (scd->typeMask == 4) {
                func_ov093_021d8414(scd->action);
                func_ov094_0220270c(scd->input, 0, NULL);
                func_ov094_021f81ec(8);
                func_ov094_021f8a78();
                (*seq)++;
            } else {
                return TRUE;
            }
        }
        break;
    case 1:
        if (!func_ov094_022033cc(scd->input)) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}


BOOL data_ov095_0689a84d(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    BOOL done = func_ov094_02203040(scd->input);

    if (done == TRUE) {
        scd->typeMask = 1;
    }
    return done;
}


// pokebw2's BtlvScd_MoveInit
BOOL data_ov095_0689a879(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    BtlvInputMoveParam param;
    u16 count;
    u16 i;
    u8 pp;
    u8 maxPP;

    count = GetBattleMonMoveCount(scd->mon);
    param.noMoveInfo = TransformCheck(scd->mon);
    for (i = 0; i < count; i++) {
        param.moves[i] = func_ov093_021d5528(scd->mon, i, &pp, &maxPP);
        param.pp[i] = pp;
        param.maxPP[i] = maxPP;
    }
    for (; i < 4; i++) {
        param.moves[i] = 0;
        param.pp[i] = 0;
        param.maxPP[i] = 0;
    }
    param.pos =
        func_ov093_021b8e70(scd->mainModule, MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)));
    func_ov094_0220270c(scd->input, 2, &param);
    return TRUE;
}

// pokebw2's BtlvScd_MoveMain
BOOL data_ov095_0689a989(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    const BtlvInputButtonSet *set;
    const BtlvInputKeyStop *stops;
    s32 input;
    u8 pos;
    BOOL flagged;

    pos = func_ov093_021b8e70(scd->mainModule, MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)));
    if (BtlSetup_GetBattleStyle(scd->mainModule) == BTL_STYLE_TRIPLE && pos != 4) {
        set = &data_ov095_0689d880;
    } else {
        set = &data_ov095_0689d91c;
    }
    if (BtlSetup_GetBattleStyle(scd->mainModule) == BTL_STYLE_TRIPLE && pos != 4) {
        stops = data_ov095_0689e128;
    } else {
        stops = data_ov095_0689e054;
    }
    input = func_ov094_02202dc4(scd->input, set, stops);
    if (input != TOUCH_RECT_NONE) {
        flagged = (input & 0x8000) != 0;
        input &= ~0x8000;
        if (input == 4) {
            BattleAction_SetNull(scd->action);
            return TRUE;
        }
        if (input < GetBattleMonMoveCount(scd->mon)) {
            BattleAction_SetFightParam(scd->action, MoveGetID(scd->mon, input), 6);
            if (flagged) {
                func_ov093_021d8318(scd->action);
            }
            return TRUE;
        }
        if (BtlSetup_GetBattleStyle(scd->mainModule) == BTL_STYLE_TRIPLE && input == 5) {
            func_ov093_021d8424(scd->action);
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's BtlvScd_RotationInit
BOOL data_ov095_0689ab49(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    BtlvInputRotationParam param;
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        param.mons[i] = scd->targetParam->mons[i].mon;
        for (j = 0; j < 4; j++) {
            param.moveUsable[i][j] = scd->targetParam->mons[i].selectable[j];
        }
    }
    func_ov094_0220270c(scd->input, 5, &param);
    return TRUE;
}

// pokebw2's BtlvScd_RotationMain
BOOL data_ov095_0689aba1(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    u8 slot;
    s32 button;
    u16 move;
    BOOL flagged;
    BattleMon *mon;

    if (func_ov094_022030f0(scd->input, &slot, &button)) {
        flagged = (button & 0x8000) != 0;
        button &= ~0x8000;
        if (button != 4) {
            if (slot == 0) {
                slot = 1;
            }
            if (button != 7) {
                mon = scd->targetParam->mons[func_ov093_021b9ab0(slot)].mon;
                move = MoveGetID(mon, button);
            } else {
                move = MOVE_STRUGGLE;
            }
            func_ov093_021d83fc(&scd->targetParam->rotateAction, slot);
            BattleAction_SetFightParam(&scd->targetParam->action, move, 6);
            BattleAction_SetFightParam(scd->action, move, 6);
            if (flagged) {
                func_ov093_021d8318(scd->action);
            }
        } else if (button == 4) { // tested again, as the original does
            BattleAction_SetNull(scd->action);
        }
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvScd_ClearTargetList
static void func_ov095_0689accc(BtlvScdTargetList *list) {
    MI_CpuFill8(list, 0, sizeof(*list));
}

// pokebw2's BtlvScd_FillSelectTargets
static void func_ov095_0689acd8(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags) {
    list->selectCount = func_ov093_021b8778(mainModule, posFlags, list->pos);
}

// pokebw2's BtlvScd_FillGroupTargets
static void func_ov095_0689acfc(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags) {
    list->count = func_ov093_021b8778(mainModule, posFlags, list->pos);
    list->kind = TARGETS_GROUP;
}

// pokebw2's BtlvScd_FillSideTargets
static void func_ov095_0689ad24(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 posFlags) {
    list->count = func_ov093_021b8778(mainModule, posFlags, list->pos);
    list->kind = TARGETS_SIDE;
}

// Adds the far positions that a move of a triple battle reaches
// pokebw2's BtlvScd_AddFarTargets
static void func_ov095_0689ad4c(BtlvScdTargetList *list, BtlMainModule *mainModule, u16 move, u8 pos) {
    u8 far;

    if (BtlSetup_GetBattleStyle(mainModule) == BTL_STYLE_TRIPLE && getMoveFlag(move, 0xb) &&
        func_ov093_021b9a00(pos, &far)) {
        list->pos[list->selectCount++] = far;
        list->pos[list->selectCount++] = func_ov093_021b8c5c(BTL_STYLE_TRIPLE, far);
    }
}

// Whether the target screen's button is one of the targets, and its position
// pokebw2's BtlvScd_GetTargetPos
static BOOL func_ov095_0689addc(BtlvScdTargetList *list, u32 button, BtlMainModule *mainModule, u8 *outPos) {
    u8 i;

    if (list->selectCount != 0) {
        *outPos = func_ov093_021b8ed8(mainModule, data_ov095_0689d82c[button]);
        for (i = 0; i < list->selectCount; i++) {
            if (*outPos == list->pos[i]) {
                return TRUE;
            }
        }
        return FALSE;
    }
    *outPos = 6;
    return TRUE;
}

void BtlvScd_SetTargetScreen(const u8 *positions, u8 count, u32 kind, BtlvScd *scd) {
    BtlvInputTargetParam param;
    u16 move = scd->action->bits.move;

    MI_CpuClear16(&param, sizeof(param));
    param.attackerPos = func_ov093_021b8e70(scd->mainModule, MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)));
    param.range = PML_MoveGetParam(move, MOVE_PARAM_TARGET);
    if (move == MOVE_CURSE) {
        param.range = func_ov093_021d80a8(scd->mon);
    }
    if (param.range == 0 && getMoveFlag(move, 0xb)) {
        param.range = 14;
    }
    while (count-- != 0) {
        BattleMon *mon = func_ov093_021b98ac(scd->pokeCon, *positions);
        PartyPkm *pkm = func_ov093_021d5864(mon);
        u8 index = func_ov093_021b8e70(scd->mainModule, *positions) - 2;
        BtlvInputTargetEntry *entry = &param.mons[index];

        entry->hp = GetBattleMonStat(mon, BATTLEMON_HP);
        entry->selectable = TRUE;
        if (entry->hp != 0) {
            u16 species = GetBattleMonSpecies(mon);
            entry->pkm = pkm;
            entry->maxHp = GetBattleMonStat(mon, BATTLEMON_HP + 1);
            if (species == SPECIES_NIDORAN_M || species == SPECIES_NIDORAN_F) {
                entry->msgForm = 2;
            } else {
                entry->msgForm = PokeParty_GetSex(func_ov093_021d5864(mon));
            }
            if (GetBattleMonStatus(mon) != 0) {
                entry->unk1 = 3;
            } else {
                entry->unk1 = 1;
            }
        } else {
            entry->unk1 = 2;
        }
        positions++;
    }
    func_ov094_0220270c(scd->input, 3, &param);
}


// pokebw2's BtlvScd_ShowTargetScreen
static void func_ov095_0689b098(BtlvScdTargetList *list, BtlvScd *scd) {
    if (list->selectCount != 0) {
        BtlvScd_SetTargetScreen(list->pos, list->selectCount, TARGETS_SELECT, scd);
    } else if (list->kind == TARGETS_GROUP) {
        BtlvScd_SetTargetScreen(list->pos, list->count, TARGETS_GROUP, scd);
    } else {
        BtlvScd_SetTargetScreen(list->pos, list->count, TARGETS_SIDE, scd);
    }
}

// pokebw2's BtlvScd_IsTargetChosen
BOOL data_ov095_0689b0cd(BtlvScd *scd) {
    return scd->targetChosen;
}

// pokebw2's BtlvScd_TargetInit
static BOOL data_ov095_0689b0d5(s32 *seq, void *arg) {
    BtlvScd *scd = arg;

    func_ov095_0689b354(&scd->targets, scd);
    if (BtlSetup_GetBattleStyle(scd->mainModule) == BTL_STYLE_TRIPLE) {
        u8 target = 6;

        if (scd->targets.selectCount != 0) {
            target = func_ov093_021b99e0(MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)))->list1[0];
        }
        BattleAction_SetFightParam(scd->action, scd->action->bits.move, target);
        scd->targetChosen = TRUE;
    }
    func_ov095_0689b098(&scd->targets, scd);
    scd->targetChosen = FALSE;
    scd->moveRange = PML_MoveGetParam(scd->action->bits.move, MOVE_PARAM_TARGET);
    if (scd->action->bits.move == MOVE_CURSE) {
        scd->moveRange = func_ov093_021d80a8(scd->mon);
    }
    return TRUE;
}

// pokebw2's BtlvScd_TargetMain
static BOOL data_ov095_0689b1e5(s32 *seq, void *arg) {
    BtlvScd *scd = arg;
    const BtlvInputButtonSet *set;
    const BtlvInputKeyStop *stops;
    u8 pos;
    u8 index;
    u8 range;
    u8 cancel;
    u8 target;
    s32 input;

    if (*seq == 0) {
        pos = func_ov093_021b8e70(scd->mainModule, MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon)));
        range = scd->moveRange;
        index = (pos - 2) / 2;
        if (BtlSetup_GetBattleStyle(scd->mainModule) != BTL_STYLE_TRIPLE) {
            set = data_ov095_0689e5d8[index][range];
            stops = data_ov095_0689e560[index][range];
            cancel = 4;
        } else {
            if (range == 0 && getMoveFlag(scd->action->bits.move, 0xb)) {
                range = 14;
            }
            set = data_ov095_0689e650[index][range];
            stops = data_ov095_0689e704[index][range];
            cancel = 6;
        }
        input = func_ov094_02202dc4(scd->input, set, stops);
        if (input != -1) {
            if (input != cancel) {
                if (func_ov095_0689addc(&scd->targets, input, scd->mainModule, &target)) {
                    BattleAction_SetFightParam(scd->action, scd->action->bits.move, target);
                    scd->targetChosen = TRUE;
                    return TRUE;
                }
            } else {
                scd->targetChosen = FALSE;
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's BtlvScd_BuildTargetList
static void func_ov095_0689b354(BtlvScdTargetList *list, BtlvScd *scd) {
    u16 move = scd->action->bits.move;
    u32 range = PML_MoveGetParam(move, MOVE_PARAM_TARGET);
    u8 pos = MonIDToBattlePos(scd->mainModule, scd->pokeCon, GetMonID(scd->mon));

    func_ov095_0689accc(list);
    if (move == MOVE_CURSE) {
        range = func_ov093_021d80a8(scd->mon);
    }
    switch (range) {
    case 0:
        func_ov095_0689acd8(list, scd->mainModule, TARGET_FLAGS(2, pos));
        func_ov095_0689ad4c(list, scd->mainModule, move, pos);
        break;
    case 3:
        func_ov095_0689acd8(list, scd->mainModule, TARGET_FLAGS(1, pos));
        func_ov095_0689ad4c(list, scd->mainModule, move, pos);
        break;
    case 1:
        func_ov095_0689acd8(list, scd->mainModule, TARGET_FLAGS(3, pos));
        break;
    case 2:
        func_ov095_0689acd8(list, scd->mainModule, TARGET_FLAGS(4, pos));
        break;
    case 5:
        func_ov095_0689acfc(list, scd->mainModule, TARGET_FLAGS(1, pos));
        break;
    case 4:
        func_ov095_0689acfc(list, scd->mainModule, TARGET_FLAGS(2, pos));
        break;
    case 7:
    case 9:
        func_ov095_0689acfc(list, scd->mainModule, TARGET_FLAGS(0, pos));
        break;
    case 6:
    case 12:
        func_ov095_0689ad24(list, scd->mainModule, TARGET_FLAGS(7, pos));
        break;
    case 11:
        func_ov095_0689ad24(list, scd->mainModule, TARGET_FLAGS(6, pos));
        break;
    case 8:
    case 10:
        func_ov095_0689ad24(list, scd->mainModule, TARGET_FLAGS(8, pos));
        break;
    case 13:
        func_ov095_0689acfc(list, scd->mainModule, TARGET_FLAGS(0, pos));
        break;
    }
}

// pokebw2's BtlvScd_ShowRotationScreen
void data_ov095_0689b515(BtlvScd *scd, void *param) {
    func_ov094_0220270c(scd->input, 5, param);
}

// pokebw2's BtlvScd_PollRotationInput
BOOL data_ov095_0689b525(BtlvScd *scd, u8 *button) {
    s32 input = func_ov094_02202dc4(scd->input, NULL, NULL);

    if (input != -1) {
        *button = input;
        return TRUE;
    }
    return FALSE;
}

// pokebw2's BtlvScd_HideRotationScreen
void data_ov095_0689b559(BtlvScd *scd) {
    func_ov094_0220270c(scd->input, 0, NULL);
    func_ov094_021f81ec(8);
    func_ov094_021f8a78();
}

// pokebw2's BtlvScd_ShowYesNo
void data_ov095_0689b5a5(BtlvScd *scd, void *param) {
    func_ov094_0220270c(scd->input, 4, param);
    scd->unk_93 = 0;
}

BOOL data_ov095_0689b5cd(BtlvScd *scd, u32 *answer) {
    s32 input;

    switch (scd->unk_93) {
    case 0:
        input = func_ov094_02202dc4(scd->input, &data_ov095_0689d88c, data_ov095_0689dc48);
        if (input != TOUCH_RECT_NONE) {
            scd->typeCount = input;
            func_ov094_0220270c(scd->input, 0, NULL);
            scd->unk_93++;
        }
        break;
    case 1:
        if (!func_ov094_022033cc(scd->input)) {
            *answer = scd->typeCount;
            return TRUE;
        }
        break;
    }
    return FALSE;
}


// pokebw2's BtlvScd_GetKeyCursorFlag
u8 *data_ov095_0689b66d(BtlvScd *scd) {
    return &scd->unk_8f;
}

// pokebw2's BtlvScd_ForceQuitInput
void data_ov095_0689b671(BtlvScd *scd) {
    if (!scd->unk_92_high) {
        scd->unk_92_low = 0;
        scd->unk_92_high = TRUE;
    }
}

BOOL data_ov095_0689b69d(BtlvScd *scd) {
    switch (scd->unk_92_low) {
    case 0:
        if (func_ov094_022033cc(scd->input) == 0) {
            scd->unk_92_low++;
        }
        break;
    case 1:
        func_ov094_0220270c(scd->input, 0, NULL);
        func_ov094_021f81ec(8);
        func_ov094_021f8a78();
        scd->unk_92_low++;
        break;
    case 2:
        if (func_ov094_022033cc(scd->input) == 0) {
            scd->unk_92_low++;
        }
        break;
    case 3:
        scd->unk_92_high = 0;
        return TRUE;
    }
    return FALSE;
}

void data_ov095_0689b781(BtlvScd *scd, void *param) {
    func_ov094_0220270c(scd->input, 6, param);
}

u32 data_ov095_0689b791(BtlvScd *scd) {
    return func_ov094_02202dc4(scd->input, &data_ov095_0689d934, data_ov095_0689debc);
}
