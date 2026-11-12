// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov153/encounter_effect_columns.c, which is the
// same code in Black 2/White 2's overlay 153. Names are Black's dsd names.
#include "types.h"
#include "field/encounter_effect.h"
#include "field/field.h"

static GameEvent *func_ov080_021f5abc(GameSystem *gsys);
static void func_ov080_021f5ae8(EncEffGrid *grid);
static BOOL func_ov080_021f5b44(EncEffCell *cell);

GameEvent *func_ov080_021f5a80(GameSystem *gsys, Field *field) {
    VecFx32 pos = { 0, 0, FX32_CONST(264) };

    func_ov021_021cf580(Field_GetEncEff(field));
    return func_ov075_021f5260(gsys, &pos, func_ov080_021f5abc, func_ov077_021f582c);
}

static GameEvent *func_ov080_021f5abc(GameSystem *gsys) {
    EncEffGridParam param;

    param.cellWidth = 2;
    param.cellHeight = 2;
    param.init = NULL;
    param.update = func_ov080_021f5ae8;
    param.cellUpdate = func_ov080_021f5b44;
    return func_ov077_021f5620(gsys, &param, 1);
}

// Shows one column at a time
static void func_ov080_021f5ae8(EncEffGrid *grid) {
    s32 *work = grid->work;
    int row;

    if (work[0] < grid->columns) {
        if (work[1] == 0) {
            for (row = 0; row < grid->rows; row++) {
                grid->cells[work[0] + row * grid->columns].visible = TRUE;
            }
            work[0]++;
            work[1] = 1;
        } else {
            work[1]--;
        }
    }
}

static BOOL func_ov080_021f5b44(EncEffCell *cell) {
    if (cell->frame < 64) {
        cell->offset += FX32_CONST(0.125);
        cell->frame++;
    } else {
        cell->done = TRUE;
    }
    return cell->done;
}
