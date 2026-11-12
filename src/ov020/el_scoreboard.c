// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov035/el_scoreboard.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "field/el_scoreboard.h"
#include "field/field.h"
#include "gfl/graphics.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "nnsys/gfd.h"

struct ElScoreboard {
    u32 unk0;
    u16 unk4;
    u16 unk6;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 frame;
};

struct ElScoreboardPaletteTarget {
    u32 palette;
    u32 vramOffset;
};

// Declared in reverse, as the compiler emits them in reverse order
// pokebw2's sScoreboardPalette3
static const u16 data_ov020_02187e20[4] = { 0x0000, 0x18c6, 0x0d73, 0x021f };
// pokebw2's sScoreboardPalette2
static const u16 data_ov020_02187e18[4] = { 0x0000, 0x2108, 0x0d73, 0x021f };
// pokebw2's sScoreboardPalette1
static const u16 data_ov020_02187e10[4] = { 0x0000, 0x18c6, 0x0df3, 0x031f };
// pokebw2's sScoreboardPalette0
static const u16 data_ov020_02187e08[4] = { 0x0000, 0x2108, 0x0df3, 0x031f };

static const u16 *sScoreboardPalettes[4] = { data_ov020_02187e20, data_ov020_02187e18, data_ov020_02187e10,
                                             data_ov020_02187e08 };

void func_ov020_021876b0(ElScoreboardPaletteTarget *target, s32 frame);

ElScoreboard *func_ov020_021875f4(void *texture, const char *texName, const char *plName, const StrBuf *text, u16 a4,
                                  u16 a5, HeapID heapId) {
    G3DTextDrawResource resource;
    ElScoreboard *board = GFL_HeapAllocate(heapId, sizeof(ElScoreboard), TRUE, "el_scoreboard.c", 412);

    board->unk0 = 0;
    board->unk4 = func_ov010_0216fc4c() - 1;
    board->unk6 = func_ov010_0216fc4c() - 1;
    board->unk10 = -1;
    board->unk14 = -1;
    if (G3DTextDraw_CreateResource(texture, texName, 0, plName, text, a4, a5, 0xc40, heapId, &resource)) {
        board->unk8 = resource.unk4;
        board->unkC = resource.unk8;
        board->unk10 = resource.unkC;
        board->unk4 = resource.unk0;
        board->unk6 = resource.unk2;
        board->unk14 = resource.unk10;
        board->frame = 0;
    }
    return board;
}

// pokebw2's ElScoreboard_Free
void func_ov020_02187684(ElScoreboard *board) {
    GFL_HeapFree(board);
}

// pokebw2's ElScoreboard_Update
void func_ov020_0218768c(ElScoreboard *board) {
    ElScoreboardPaletteTarget target;

    board->frame++;
    if (board->unk14 != -1) {
        target.palette = board->unkC;
        target.vramOffset = board->unk14;
        func_ov020_021876b0(&target, board->frame);
    }
}

// pokebw2's ElScoreboard_UploadPalette
void func_ov020_021876b0(ElScoreboardPaletteTarget *target, s32 frame) {
    NNS_GfdRegisterNewVramTransferTask(1, target->vramOffset + (u16)target->palette * 8,
                                       sScoreboardPalettes[(frame & 0x1f) / 8], 8);
}
