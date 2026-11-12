// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov150/encounter_effect_grid.c, which is the
// same code in Black 2/White 2's overlay 150. The names are Black's dsd names and swan's; pokebw2's guesses are in the
// comments.
#include "types.h"
#include "field/encounter_effect.h"
#include "field/field.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "system/game_event.h"
#include "system/game_system.h"

// Overlay 77: the encounter effects that split the screen into a grid of cells, each drawn as a textured quad that the
// effect's overlay (79 to 81) moves, turns and hides. Overlay 75 runs the event around them. The file's name is
// pokebw2's guess; the ROM has no string for it

static GameEventReturnCode func_ov077_021f5744(GameEvent *event, u32 *state, void *data);
static BOOL func_ov077_021f5798(EncEffGrid *grid);

// pokebw2's sCellScale
static const VecFx32 data_ov077_021f5a58 = { FX32_ONE, FX32_ONE, FX32_ONE };

// pokebw2's EncEffGrid_CreateEvent: mode picks the fade at the end, to white or to black
GameEvent *func_ov077_021f5620(GameSystem *gsys, const EncEffGridParam *param, u32 mode) {
    EncEffGrid *grid = EncEff_AllocWorkArea(Field_GetEncEff(GSYS_GetField(gsys)), sizeof(EncEffGrid), 0x50);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov077_021f5744, 0);
    s32 cellWidth = param->cellWidth;
    s32 cellHeight = param->cellHeight;
    s32 width = cellWidth * 8;
    s32 height = cellHeight * 8;
    s32 columns = 32 / cellWidth;
    s32 rows = 24 / cellHeight;
    s32 left, top, i;

    grid->columns = columns;
    grid->rows = rows;
    left = columns / 2 * width - width / 2;
    top = rows / 2 * height - height / 2;
    for (i = 0; i < columns * rows; i++) {
        EncEffCell *cell = &grid->cells[i];
        s32 column = i % columns;
        s32 row = i / columns;

        cell->pos.x = (column * width - left) * FX32_ONE;
        cell->pos.y = (top - row * height) * FX32_ONE;
        cell->pos.z = 0;
        cell->unk18 = 0;
        cell->offset = 0;
        cell->unk20 = 0;
        cell->visible = FALSE;
        cell->done = FALSE;
        cell->update = param->cellUpdate;
    }
    grid->update = param->update;
    if (param->init != NULL) {
        param->init(grid);
    }
    if (mode) {
        grid->unk3074 = 12;
        grid->unk3078 = GX_RGB(31, 31, 31);
    } else {
        grid->unk3074 = 3;
        grid->unk3078 = GX_RGB(0, 0, 0);
    }
    return event;
}

// pokebw2's EncEffGrid_EventCallback
static GameEventReturnCode func_ov077_021f5744(GameEvent *event, u32 *state, void *data) {
    EncEffGrid *grid = EncEff_GetWorkArea(Field_GetEncEff(GSYS_GetField(GameEvent_GetGameSystem(event))));

    switch (*state) {
    case 0:
        if (func_ov077_021f5798(grid)) {
            GFL_FadeSet(grid->unk3074, 0, 16, 0);
            (*state)++;
        }
        break;
    case 1:
        if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EncEffGrid_Update: updates the grid and its visible cells, and returns whether every cell is done
static BOOL func_ov077_021f5798(EncEffGrid *grid) {
    s32 i, count, doneCount;

    grid->update(grid);
    count = grid->columns * grid->rows;
    doneCount = 0;
    for (i = 0; i < count; i++) {
        EncEffCell *cell = &grid->cells[i];
        BOOL done = FALSE;

        if (!cell->done) {
            if (cell->visible) {
                if (cell->update == NULL) {
                    cell->done = TRUE;
                } else {
                    done = cell->update(cell);
                }
            }
        } else {
            done = TRUE;
        }
        if (done) {
            doneCount++;
        }
    }
    return doneCount >= count;
}

// pokebw2's EncEffGrid_ClearAndDraw
void func_ov077_021f57f8(EncEff *effect) {
    void *eventData = EncEff_GetEventData(effect);
    EncEffGrid *grid = EncEff_GetWorkArea(effect);

    G3X_SetClearColor(grid->unk3078, 31, 0x7fff, 0, FALSE);
    func_ov075_021f52d8(eventData);
}

// pokebw2's EncEffGrid_Draw: draws each cell as a quad that shows its part of the screen. It doesn't match, as in
// pokebw2: 0x234 bytes against 0x22c, the original keeping the grid's 0x300a offset in r7 into the loop, then the
// columns and t, where this keeps the address of reg_G3_MTX_TRANS there and spills one value more
void func_ov077_021f582c(EncEffGrid *grid) {
    G3_PushMtx();
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
    G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    G3_PolygonAttr(GX_LIGHTMASK_0, GX_POLYGONMODE_MODULATE, GX_CULL_BACK, 0, 31, 0);
    {
        VecFx32 trans = { 0, 0, 0 };
        VecFx32 scale = data_ov077_021f5a58;
        MtxFx33 rot;
        fx32 s, t;
        s32 count = grid->columns * grid->rows;
        s32 width = 32 / grid->columns * 8;
        s32 height = 24 / grid->rows * 8;
        s32 i;

        scale.x = (width / 8) * FX32_ONE;
        scale.y = (height / 8) * FX32_ONE;
        for (i = 0; i < count; i++) {
            EncEffCell *cell = &grid->cells[i];
            s32 column, row;
            fx32 s1, t1;

            G3_PushMtx();
            column = i % grid->columns;
            row = i / grid->columns;
            trans = cell->pos;
            s = column * width * FX32_ONE;
            t = row * height * FX32_ONE;
            s1 = s + width * FX32_ONE;
            t1 = t + height * FX32_ONE;
            G3_Translate(trans.x, trans.y, trans.z);
            G3_Scale(scale.x, scale.y, scale.z);
            MTX_RotY33_(&rot, FX_SinIdx(cell->offset), FX_CosIdx(cell->offset));
            G3_MultMtx33(&rot);
            MTX_RotZ33_(&rot, FX_SinIdx(cell->unk20), FX_CosIdx(cell->unk20));
            G3_MultMtx33(&rot);
            G3_Begin(GX_BEGIN_QUADS);
            G3_Normal(0, 0, FX16_ONE);
            G3_TexCoord(s, t);
            G3_Vtx(-FX16_CONST(4), FX16_CONST(4), 0);
            G3_TexCoord(s, t1);
            G3_Vtx(-FX16_CONST(4), -FX16_CONST(4), 0);
            G3_TexCoord(s1, t1);
            G3_Vtx(FX16_CONST(4), -FX16_CONST(4), 0);
            G3_TexCoord(s1, t);
            G3_Vtx(FX16_CONST(4), FX16_CONST(4), 0);
            G3_End();
            G3_PopMtx(1);
        }
    }
    G3_PopMtx(1);
}
