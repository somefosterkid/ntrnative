// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov154/encounter_effect_spread.c, which is the
// same code in Black 2/White 2's overlay 154. Names are Black's dsd names.
#include "types.h"
#include "field/encounter_effect_spread.h"
#include "field/field.h"

static GameEvent *func_ov081_021f5abc(GameSystem *gsys);
static void func_ov081_021f5aec(EncEffGrid *grid);
static void func_ov081_021f5b30(EncEffGrid *grid);
static BOOL func_ov081_021f5b94(EncEffCell *cell);
static BOOL func_ov081_021f5bcc(int cell, EncEffSpread *spread);
static void func_ov081_021f5bfc(EncEffGrid *grid, int column, int row);
static void func_ov081_021f5c3c(EncEffGrid *grid, int cell);
static void func_ov081_021f5ca0(EncEffGrid *grid);

GameEvent *func_ov081_021f5a80(GameSystem *gsys, Field *field) {
    VecFx32 pos = { 0, 0, FX32_CONST(264) };

    func_ov021_021cf580(Field_GetEncEff(field));
    return func_ov075_021f5260(gsys, &pos, func_ov081_021f5abc, func_ov077_021f582c);
}

static GameEvent *func_ov081_021f5abc(GameSystem *gsys) {
    EncEffGridParam param;

    param.cellWidth = 2;
    param.cellHeight = 2;
    param.init = func_ov081_021f5aec;
    param.update = func_ov081_021f5b30;
    param.cellUpdate = func_ov081_021f5b94;
    return func_ov077_021f5620(gsys, &param, 1);
}

// Starts the waves at three cells
static void func_ov081_021f5aec(EncEffGrid *grid) {
    EncEffSpread *spread = (EncEffSpread *)grid->unk300C;

    spread->nextCount = 0;
    spread->waveCount = 0;
    spread->next[0] = 51;
    spread->nextCount++;
    spread->next[1] = 149;
    spread->nextCount++;
    spread->next[2] = 91;
    spread->nextCount++;
}

// Shows the next wave every other frame
static void func_ov081_021f5b30(EncEffGrid *grid) {
    s32 *work = grid->work;
    EncEffSpread *spread = (EncEffSpread *)grid->unk300C;
    int i;

    if (work[0] < grid->columns * grid->rows) {
        if (work[1] == 0) {
            for (i = 0; i < spread->waveCount; i++) {
                func_ov081_021f5c3c(grid, spread->wave[i]);
            }
            func_ov081_021f5ca0(grid);
            work[1] = 1;
        } else {
            work[1]--;
        }
    }
}

// Flies off to the right, faster each frame, while turning half a turn
static BOOL func_ov081_021f5b94(EncEffCell *cell) {
    if (cell->pos.x < FX32_CONST(256)) {
        cell->pos.z = FX32_ONE;
        cell->pos.x += cell->speed;
        cell->speed += FX32_ONE;
        if (cell->offset < FX32_CONST(8)) {
            cell->offset += FX32_CONST(0.125);
        } else {
            cell->offset = FX32_CONST(8);
        }
    } else {
        cell->done = TRUE;
    }
    return cell->done;
}

// Whether a cell is already in the next wave
static BOOL func_ov081_021f5bcc(int cell, EncEffSpread *spread) {
    int i;

    for (i = 0; i < spread->nextCount; i++) {
        if (cell == spread->next[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// Adds a cell to the next wave, unless it is shown or already there
static void func_ov081_021f5bfc(EncEffGrid *grid, int column, int row) {
    EncEffSpread *spread = (EncEffSpread *)grid->unk300C;
    int cell = column + row * grid->columns;

    if (!grid->cells[cell].visible && !func_ov081_021f5bcc(cell, spread)) {
        spread->next[spread->nextCount] = cell;
        spread->nextCount++;
    }
}

// Adds the cells above, right of, below and left of a cell to the next wave
static void func_ov081_021f5c3c(EncEffGrid *grid, int cell) {
    int column = cell % grid->columns;
    int row = cell / grid->columns;

    if (row - 1 >= 0) {
        func_ov081_021f5bfc(grid, column, row - 1);
    }
    if (column + 1 < grid->columns) {
        func_ov081_021f5bfc(grid, column + 1, row);
    }
    if (row + 1 < grid->rows) {
        func_ov081_021f5bfc(grid, column, row + 1);
    }
    if (column - 1 >= 0) {
        func_ov081_021f5bfc(grid, column - 1, row);
    }
}

// Shows the next wave's cells and makes it the last wave
static void func_ov081_021f5ca0(EncEffGrid *grid) {
    int i;
    s32 *work = grid->work;
    EncEffSpread *spread = (EncEffSpread *)grid->unk300C;

    for (i = 0; i < spread->nextCount; i++) {
        EncEffCell *cell = &grid->cells[spread->next[i]];

        if (!cell->visible) {
            cell->visible = TRUE;
            work[0]++;
            cell->speed = FX32_ONE;
            spread->wave[i] = spread->next[i];
        }
    }
    spread->waveCount = spread->nextCount;
    spread->nextCount = 0;
}
