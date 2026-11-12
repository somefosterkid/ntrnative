// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov151/encounter_effect_ripple.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// Overlay 151: the encounter effects that ripple the captured screen like water. The file's name is a guess; the ROM
// has no string for it

#include "field/encounter_effect_ripple.h"
#include "types.h"
#include "field/encounter_effect.h"
#include "field/encounter_effect_capture.h"
#include "field/field.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/gx.h"
#include "system/game_event.h"
#include "system/game_system.h"

// The middle vertex, where the drops land
#define CENTER_VERTEX (ENCEFF_RIPPLE_ROWS / 2 * ENCEFF_RIPPLE_COLUMNS + ENCEFF_RIPPLE_COLUMNS / 2)

static GameEvent *func_ov078_021f5698(GameSystem *gsys);
static GameEventReturnCode func_ov078_021f5748(GameEvent *event, u32 *state, void *data);
static void func_ov078_021f5800(EncEffRipple *ripple);
static BOOL func_ov078_021f5950(EncEffRipple *ripple);
static GameEvent *func_ov078_021f5afc(GameSystem *gsys);

// pokebw2's EncEffRipple_InitWhite

void func_ov021_021cf59c(EncEff *effect);

GameEvent *func_ov078_021f5620(GameSystem *gsys, Field *field) {
    VecFx32 pos = { 0, 0, FX32_CONST(33) };

    func_ov021_021cf59c(Field_GetEncEff(field));
    return func_ov075_021f5260(gsys, &pos, func_ov078_021f5698, (void (*)(EncEffGrid *))func_ov078_021f5800);
}

GameEvent *func_ov078_021f565c(GameSystem *gsys, Field *field) {
    VecFx32 pos = { 0, 0, FX32_CONST(33) };

    func_ov021_021cf59c(Field_GetEncEff(field));
    return func_ov075_021f529c(gsys, &pos, func_ov078_021f5afc, (void (*)(EncEffGrid *))func_ov078_021f5800);
}

static GameEvent *func_ov078_021f5698(GameSystem *gsys) {
    EncEffRipple *ripple = EncEff_AllocWorkArea(Field_GetEncEff(GSYS_GetField(gsys)), sizeof(EncEffRipple), 0x50);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov078_021f5748, 0);
    int i;

    ripple->drops = 7;
    ripple->timer = 0;
    ripple->dropInterval = 8;
    ripple->dropHeight = FX32_CONST(0.25);
    ripple->settleTime = 0;
    ripple->fadeDelay = 50;
    ripple->fadeStarted = FALSE;
    for (i = 0; i < ENCEFF_RIPPLE_VERTICES; i++) {
        ripple->vertices[i].x = i % ENCEFF_RIPPLE_COLUMNS * FX32_ONE / 16;
        ripple->vertices[i].y = i / ENCEFF_RIPPLE_COLUMNS * FX32_ONE / 16;
        ripple->vertices[i].z = 0;
    }
    ripple->fadeMode = FADE_ENGINE_A_WHITE | FADE_ENGINE_B_WHITE;
    return event;
}

// pokebw2's EncEffRipple_EventCallback
static GameEventReturnCode func_ov078_021f5748(GameEvent *event, u32 *state, void *data) {
    EncEffRipple *ripple = EncEff_GetWorkArea(Field_GetEncEff(GSYS_GetField(GameEvent_GetGameSystem(event))));

    switch (*state) {
    case 0:
        if (ripple->fadeDelay != 0) {
            ripple->fadeDelay--;
        } else if (!ripple->fadeStarted) {
            GFL_FadeSet(ripple->fadeMode, 0, 16, 0);
            ripple->fadeStarted = TRUE;
        }
        if (func_ov078_021f5950(ripple)) {
            ripple->timer = 6;
            (*state)++;
        }
        break;
    case 1:
        if (ripple->timer != 0) {
            ripple->timer--;
        } else if (!GFL_FadeIsRunning()) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

// pokebw2's EncEffRipple_ClearAndDraw
void func_ov078_021f57d4(EncEff *effect) {
    void *eventData = EncEff_GetEventData(effect);

    G3X_SetClearColor(GX_RGB(16, 16, 16), 31, 0x7fff, 0, FALSE);
    func_ov075_021f52d8(eventData);
}

// Draws the mesh as a quad strip per row, each vertex showing its part of the screen
// pokebw2's EncEffRipple_Draw
static void func_ov078_021f5800(EncEffRipple *ripple) {
    int x, y;

    G3_PushMtx();
    G3_MaterialColorDiffAmb(GX_RGB(31, 31, 31), GX_RGB(31, 31, 31), FALSE);
    G3_MaterialColorSpecEmi(GX_RGB(16, 16, 16), GX_RGB(0, 0, 0), FALSE);
    G3_LightColor(GX_LIGHTID_0, GX_RGB(31, 31, 31));
    G3_PolygonAttr(GX_LIGHTMASK_0, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 0, 31, 0);
    {
        VecFx32 trans = { -FX32_CONST(16), FX32_CONST(12), 0 };
        VecFx32 scale = { FX32_CONST(16), FX32_CONST(16), FX32_CONST(64) };

        G3_Translate(trans.x, trans.y, trans.z);
        G3_Scale(scale.x, scale.y, scale.z);
    }
    for (y = 0; y < ENCEFF_RIPPLE_ROWS - 1; y++) {
        // The row's t texture coordinate
        fx32 t;

        G3_Begin(GX_BEGIN_QUAD_STRIP);
        t = y * 8 * FX32_ONE;
        for (x = 0; x < ENCEFF_RIPPLE_COLUMNS; x++) {
            fx16 x0 = ripple->vertices[y * ENCEFF_RIPPLE_COLUMNS + x].x;
            fx16 x1 = ripple->vertices[(y + 1) * ENCEFF_RIPPLE_COLUMNS + x].x;
            fx16 y0 = ripple->vertices[y * ENCEFF_RIPPLE_COLUMNS + x].y;
            fx16 y1 = ripple->vertices[(y + 1) * ENCEFF_RIPPLE_COLUMNS + x].y;
            fx16 z0 = ripple->vertices[y * ENCEFF_RIPPLE_COLUMNS + x].z;
            fx16 z1 = ripple->vertices[(y + 1) * ENCEFF_RIPPLE_COLUMNS + x].z;

            G3_Normal(0, 0, -FX16_ONE);
            G3_TexCoord(x * 8 * FX32_ONE, t);
            G3_Vtx(x0, -y0, z0);
            G3_TexCoord(x * 8 * FX32_ONE, t + 8 * FX32_ONE);
            G3_Vtx(x1, -y1, z1);
        }
        G3_End();
    }
    G3_PopMtx(1);
}

// Drops the next drop when it is time, then moves each vertex toward the average of its neighbors. Returns TRUE once
// the drops are over and the water has had its time to settle
// pokebw2's EncEffRipple_Update
static BOOL func_ov078_021f5950(EncEffRipple *ripple) {
    s16 heights[ENCEFF_RIPPLE_VERTICES];
    int i;

    for (i = 0; i < ENCEFF_RIPPLE_VERTICES; i++) {
        heights[i] = ripple->vertices[i].z;
    }
    if (ripple->drops > 0) {
        if (ripple->timer == 0) {
            ripple->vertices[CENTER_VERTEX].z = ripple->dropHeight;
            ripple->timer = ripple->dropInterval;
            ripple->drops--;
        } else {
            ripple->timer--;
        }
        if (ripple->drops <= 0) {
            if (ripple->settleTime == 0) {
                ripple->timer = 6;
                return TRUE;
            }
            ripple->timer = ripple->settleTime;
        }
    } else {
        ripple->timer--;
        if (ripple->timer <= 0) {
            ripple->timer = 6;
            return TRUE;
        }
    }
    for (i = 0; i < ENCEFF_RIPPLE_VERTICES; i++) {
        VecFx16 *vtx;
        int x = i % ENCEFF_RIPPLE_COLUMNS;
        int y = i / ENCEFF_RIPPLE_COLUMNS;
        int left, right, up, down;
        // The neighbor's column or row
        int n;

        n = x - 1;
        left = n >= 0 ? heights[y * ENCEFF_RIPPLE_COLUMNS + n] : 0;
        n = x + 1;
        right = n < ENCEFF_RIPPLE_COLUMNS ? heights[y * ENCEFF_RIPPLE_COLUMNS + n] : 0;
        n = y - 1;
        up = n >= 0 ? heights[n * ENCEFF_RIPPLE_COLUMNS + x] : 0;
        n = y + 1;
        down = n < ENCEFF_RIPPLE_ROWS ? heights[n * ENCEFF_RIPPLE_COLUMNS + x] : 0;

        vtx = &ripple->vertices[i];
        ripple->speeds[i] += (left + right + up + down) / 4 - vtx->z;
        vtx->z += ripple->speeds[i];
        if (vtx->z > FX16_ONE) {
            vtx->z = FX16_ONE;
        }
        if (vtx->z < -FX16_ONE) {
            vtx->z = -FX16_ONE;
        }
        // The edges stay flat
        if (x == 0 || x == ENCEFF_RIPPLE_COLUMNS - 1 || y == 0 || y == ENCEFF_RIPPLE_ROWS - 1) {
            vtx->z = 0;
        }
    }
    return FALSE;
}

// pokebw2's EncEffRipple_InitBlack
static GameEvent *func_ov078_021f5afc(GameSystem *gsys) {
    EncEffRipple *ripple = EncEff_AllocWorkArea(Field_GetEncEff(GSYS_GetField(gsys)), sizeof(EncEffRipple), 0x50);
    GameEvent *event = GameEvent_Create(gsys, NULL, func_ov078_021f5748, 0);
    int i;

    ripple->drops = 7;
    ripple->timer = 0;
    ripple->dropInterval = 2;
    ripple->dropHeight = FX32_CONST(1) / 3;
    ripple->settleTime = 45;
    ripple->fadeDelay = 55;
    ripple->fadeStarted = FALSE;
    for (i = 0; i < ENCEFF_RIPPLE_VERTICES; i++) {
        ripple->vertices[i].x = i % ENCEFF_RIPPLE_COLUMNS * FX32_ONE / 16;
        ripple->vertices[i].y = i / ENCEFF_RIPPLE_COLUMNS * FX32_ONE / 16;
        ripple->vertices[i].z = 0;
    }
    ripple->fadeMode = FADE_ENGINE_A_BLACK | FADE_ENGINE_B_BLACK;
    return event;
}
