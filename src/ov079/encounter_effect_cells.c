// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov152/encounter_effect_cells.c, which is the
// same code in Black 2/White 2's overlay 152. The names are Black's dsd names.
#include "types.h"
#include "field/encounter_effect.h"
#include "field/field.h"

// Overlay 79: the encounter effect that sets the screen's cells of 4 by 4 tiles turning, one at a time. The
// file's name is pokebw2's guess; the ROM has no string for it

static GameEvent *func_ov079_021f5abc(GameSystem *gsys);
static void func_ov079_021f5ae4(EncEffGrid *grid);
static BOOL func_ov079_021f5b24(EncEffCell *cell);

GameEvent *func_ov079_021f5a80(GameSystem *gsys, Field *field) {
    VecFx32 pos = { 0, 0, FX32_CONST(264) };

    func_ov021_021cf580(Field_GetEncEff(field));
    return func_ov075_021f5260(gsys, &pos, func_ov079_021f5abc, func_ov077_021f582c);
}

static GameEvent *func_ov079_021f5abc(GameSystem *gsys) {
    EncEffGridParam param;

    param.cellWidth = 4;
    param.cellHeight = 4;
    param.init = NULL;
    param.update = func_ov079_021f5ae4;
    param.cellUpdate = func_ov079_021f5b24;
    return func_ov077_021f5620(gsys, &param, 0);
}

// Shows one cell at a time
static void func_ov079_021f5ae4(EncEffGrid *grid) {
    s32 *work = grid->work;

    if (work[0] < grid->columns * grid->rows) {
        if (work[1] == 0) {
            grid->cells[work[0]].visible = TRUE;
            work[0]++;
            work[1] = 0;
        } else {
            work[1]--;
        }
    }
}

static BOOL func_ov079_021f5b24(EncEffCell *cell) {
    if (cell->frame < 1) {
        cell->offset += FX32_ONE;
        if (cell->offset >= FX32_CONST(16)) {
            cell->offset -= FX32_CONST(16);
            cell->frame++;
        }
    } else if (cell->frame == 1) {
        cell->offset += FX32_ONE;
        if (cell->offset >= FX32_CONST(8)) {
            cell->offset = FX32_CONST(8);
            cell->frame++;
        }
    } else {
        cell->done = TRUE;
    }
    return cell->done;
}
